#include "GameController.h"
#include "services/CardMatchService.h"
#include "services/GameModelFromLevelGenerator.h"
#include "configs/loaders/LevelConfigLoader.h"
#include "utils/CardUtils.h"
#include <cmath>

USING_NS_CC;

GameController::GameController()
    : _gameModel(nullptr)
    , _gameView(nullptr)
    , _playFieldController(nullptr)
    , _stackController(nullptr)
    , _undoManager(nullptr)
    , _levelConfig(nullptr)
    , _isAnimating(false) {
}

GameController::~GameController() {
    delete _playFieldController;
    _playFieldController = nullptr;

    delete _stackController;
    _stackController = nullptr;

    delete _undoManager;
    _undoManager = nullptr;

    delete _gameModel;
    _gameModel = nullptr;

    delete _levelConfig;
    _levelConfig = nullptr;

    // _gameView由场景树管理，不需要手动delete
}

bool GameController::init(Node* parentNode) {
    // 加载关卡
    _levelConfig = LevelConfigLoader::loadFromFile("levels/level_1.json");
    if (_levelConfig == nullptr) {
        CCLOG("GameController::init - failed to load level config");
        return false;
    }

    // 生成运行时model
    _gameModel = GameModelFromLevelGenerator::generate(_levelConfig);
    if (_gameModel == nullptr) {
        CCLOG("GameController::init - failed to generate game model");
        return false;
    }

    // 创建view
    _gameView = GameView::create();
    if (_gameView == nullptr) {
        CCLOG("GameController::init - failed to create game view");
        return false;
    }
    parentNode->addChild(_gameView);

    // 初始化各个子视图
    _gameView->initPlayField(_gameModel->getPlayfieldCards());
    _gameView->initStack(_gameModel->getStackSize());
    _gameView->initTray(_gameModel->getTrayTopCard());

    _undoManager = new UndoManager();

    // 创建子控制器
    _playFieldController = new PlayFieldController();
    _playFieldController->init(_gameModel, _gameView->getPlayFieldView());
    _playFieldController->setOnCardMatchCallback([this](int cardId) {
        this->onPlayfieldCardClick(cardId);
    });

    _stackController = new StackController();
    _stackController->init(_gameModel, _gameView->getStackView());
    _stackController->setOnStackDrawCallback([this]() {
        this->onStackClick();
    });

    // 绑定view的回调
    _gameView->getPlayFieldView()->setOnCardClickCallback([this](int cardId) {
        if (!_isAnimating) {
            _playFieldController->handleCardClick(cardId);
        }
    });

    _gameView->getStackView()->setOnClickCallback([this]() {
        if (!_isAnimating) {
            _stackController->handleStackClick();
        }
    });

    _gameView->setOnUndoClickCallback([this]() {
        if (!_isAnimating) {
            this->onUndoClick();
        }
    });

    return true;
}

void GameController::startGame() {
    CCLOG("GameController::startGame - game started");
}

void GameController::onPlayfieldCardClick(int cardId) {
    if (_isAnimating) return;

    CardModel* card = _gameModel->findPlayfieldCard(cardId);
    if (card == nullptr) return;
    if (!card->isFaceUp()) return;
    if (card->isCovered()) return;

    // 检查是否跟tray顶牌匹配
    CardModel* trayTop = _gameModel->getTrayTopCard();
    if (!CardMatchService::canMatchCards(card, trayTop)) {
        CCLOG("GameController - card %s cannot match tray top %s",
              CardUtils::cardToString(card).c_str(),
              CardUtils::cardToString(trayTop).c_str());
        return;
    }

    Vec2 originalPos = card->getPosition();

    // 从牌区移到tray
    _gameModel->removePlayfieldCard(cardId);
    card->setFaceUp(true);
    _gameModel->addTrayCard(card);

    // 更新遮盖关系
    std::vector<int> uncoveredIds = updateCoveringAfterRemove(cardId);

    _undoManager->recordPlayfieldToTray(cardId, originalPos, uncoveredIds);

    // 播放移动动画
    _isAnimating = true;

    CardView* cardView = _gameView->getPlayFieldView()->getCardView(cardId);
    Size cardSize = cardView->getContentSize();

    // 将卡牌从PlayFieldView重新挂到GameView上，实现跨区域平滑动画
    Vec2 worldPos = cardView->getParent()->convertToWorldSpace(cardView->getPosition());
    Vec2 gameViewPos = _gameView->convertToNodeSpace(worldPos);

    cardView->retain();
    _gameView->getPlayFieldView()->detachCardView(cardId);
    cardView->removeFromParent();
    cardView->setPosition(gameViewPos);
    _gameView->addChild(cardView, 50);
    cardView->release();

    // 目标位置：牌库中心（GameView坐标系）
    Vec2 trayPos = _gameView->getTrayView()->getPosition();
    Vec2 trayTarget = Vec2(trayPos.x - cardSize.width * 0.5f,
                           trayPos.y - cardSize.height * 0.5f);

    auto moveTo = EaseInOut::create(MoveTo::create(0.3f, trayTarget), 2.0f);

    auto onComplete = CallFunc::create([this, cardView, uncoveredIds]() {
        cardView->removeFromParent();
        _gameView->getTrayView()->setTopCard(_gameModel->getTrayTopCard());

        // 翻开被解除遮盖的牌
        for (int uncoveredId : uncoveredIds) {
            _gameView->getPlayFieldView()->flipCard(uncoveredId);
        }

        _isAnimating = false;
        checkWinCondition();
    });

    cardView->runAction(Sequence::create(moveTo, onComplete, nullptr));
}

void GameController::onStackClick() {
    if (_isAnimating) return;

    if (_gameModel->getStackSize() <= 0) {
        return;
    }

    _isAnimating = true;

    // 从牌堆抽一张到tray
    CardModel* card = _gameModel->popStackTopCard();
    card->setFaceUp(true);
    _gameModel->addTrayCard(card);

    _undoManager->recordStackToTray(card->getId());

    _gameView->getStackView()->updateCount(_gameModel->getStackSize());

    // 创建临时卡牌视图做动画（从牌堆飞到牌库）
    CardView* animCard = CardView::create(card);
    Size cardSize = animCard->getContentSize();

    // 起始位置：牌堆中心（GameView坐标）
    Vec2 stackPos = _gameView->getStackView()->getPosition();
    animCard->setPosition(Vec2(stackPos.x - cardSize.width * 0.5f,
                               stackPos.y - cardSize.height * 0.5f));
    animCard->showBack();
    _gameView->addChild(animCard, 50);

    // 目标位置：牌库中心
    Vec2 trayPos = _gameView->getTrayView()->getPosition();
    Vec2 trayTarget = Vec2(trayPos.x - cardSize.width * 0.5f,
                           trayPos.y - cardSize.height * 0.5f);

    auto moveTo = EaseInOut::create(MoveTo::create(0.25f, trayTarget), 2.0f);

    // 移动中途翻转：scaleX缩到0 → 切换正面 → scaleX恢复
    auto delayFlip = DelayTime::create(0.06f);
    auto flipShrink = ScaleTo::create(0.1f, 0.0f, 1.0f);
    auto showFront = CallFunc::create([animCard]() {
        animCard->showFront();
    });
    auto flipGrow = ScaleTo::create(0.1f, 1.0f, 1.0f);

    auto onComplete = CallFunc::create([this, animCard]() {
        animCard->removeFromParent();
        _gameView->getTrayView()->setTopCard(_gameModel->getTrayTopCard());
        _isAnimating = false;
    });

    animCard->runAction(Sequence::create(
        Spawn::create(moveTo,
            Sequence::create(delayFlip, flipShrink, showFront, flipGrow, nullptr),
            nullptr),
        onComplete, nullptr));

    CCLOG("GameController::onStackClick - drew %s from stack",
          CardUtils::cardToString(card).c_str());
}

void GameController::onUndoClick() {
    if (_isAnimating) return;

    if (!_undoManager->canUndo()) {
        CCLOG("GameController::onUndoClick - nothing to undo");
        return;
    }

    UndoRecord record = _undoManager->undo();

    _isAnimating = true;

    if (record.type == UNDO_PLAYFIELD_TO_TRAY) {
        // 把牌从tray拿回来
        CardModel* card = _gameModel->popTrayTopCard();
        if (card == nullptr) { _isAnimating = false; return; }

        // 恢复到原来的位置
        card->setPosition(record.originalPosition);
        card->setFaceUp(true);
        _gameModel->playfieldCards().push_back(card);

        // 重新遮盖之前被翻开的牌
        for (int uncoveredId : record.uncoveredCardIds) {
            CardModel* uncovered = _gameModel->findPlayfieldCard(uncoveredId);
            if (uncovered != nullptr) {
                uncovered->setFaceUp(false);
                uncovered->addCoveredBy(card->getId());
            }
        }

        // 恢复遮盖关系，被盖住的牌不能点
        for (auto* other : _gameModel->getPlayfieldCards()) {
            if (other->getId() == card->getId()) continue;
            float dx = std::fabs(card->getPosition().x - other->getPosition().x);
            float dy = card->getPosition().y - other->getPosition().y;
            if (dy > 0 && dx < 200.0f && dy < 300.0f) {
                other->addCoveredBy(card->getId());
                CardView* otherView = _gameView->getPlayFieldView()->getCardView(other->getId());
                if (otherView) {
                    otherView->setClickable(false);
                }
            }
        }

        _gameView->getTrayView()->setTopCard(_gameModel->getTrayTopCard());

        // 创建动画卡牌：从牌库位置飞回牌区
        CardView* animCard = CardView::create(card);
        Size cardSize = animCard->getContentSize();
        animCard->showFront();
        _gameView->addChild(animCard, 50);

        // 起始位置：牌库中心（GameView坐标）
        Vec2 trayPos = _gameView->getTrayView()->getPosition();
        animCard->setPosition(Vec2(trayPos.x - cardSize.width * 0.5f,
                                   trayPos.y - cardSize.height * 0.5f));

        // 目标位置：牌区原始位置（需要加上PlayFieldView的偏移）
        Vec2 playfieldOffset = _gameView->getPlayFieldView()->getPosition();
        Vec2 targetInGameView = Vec2(record.originalPosition.x + playfieldOffset.x,
                                     record.originalPosition.y + playfieldOffset.y);

        auto moveTo = EaseOut::create(MoveTo::create(0.3f, targetInGameView), 2.0f);

        auto onComplete = CallFunc::create([this, animCard, card, record]() {
            animCard->removeFromParent();

            // 在牌区创建最终视图
            CardView* restoredView = CardView::create(card);
            restoredView->setPosition(record.originalPosition);
            restoredView->showFront();
            restoredView->setClickable(true);
            _gameView->getPlayFieldView()->addChild(restoredView);
            _gameView->getPlayFieldView()->addCardView(card->getId(), restoredView);

            restoredView->setOnClickCallback([this](int clickedId) {
                if (!_isAnimating) {
                    _playFieldController->handleCardClick(clickedId);
                }
            });

            // 翻转被重新遮盖的牌（动画卡到位后再翻）
            for (int uncoveredId : record.uncoveredCardIds) {
                _gameView->getPlayFieldView()->flipCard(uncoveredId);
            }

            _isAnimating = false;
        });

        animCard->runAction(Sequence::create(moveTo, onComplete, nullptr));

        CCLOG("GameController::onUndoClick - restored card %s to playfield",
              CardUtils::cardToString(card).c_str());

    } else if (record.type == UNDO_STACK_TO_TRAY) {
        // 把牌退回牌堆
        CardModel* card = _gameModel->popTrayTopCard();
        if (card == nullptr) { _isAnimating = false; return; }

        // 先创建动画卡牌（此时card还是faceUp=true）
        CardView* animCard = CardView::create(card);
        animCard->showFront();
        Size cardSize = animCard->getContentSize();
        _gameView->addChild(animCard, 50);

        // 改变model状态
        card->setFaceUp(false);
        _gameModel->stackCards().push_back(card);

        // 起始位置：牌库中心
        Vec2 trayPos = _gameView->getTrayView()->getPosition();
        animCard->setPosition(Vec2(trayPos.x - cardSize.width * 0.5f,
                                   trayPos.y - cardSize.height * 0.5f));

        // 更新tray显示（露出下面的牌）
        _gameView->getTrayView()->setTopCard(_gameModel->getTrayTopCard());

        // 目标位置：牌堆中心
        Vec2 stackPos = _gameView->getStackView()->getPosition();
        Vec2 stackTarget = Vec2(stackPos.x - cardSize.width * 0.5f,
                                stackPos.y - cardSize.height * 0.5f);

        auto moveTo = EaseInOut::create(MoveTo::create(0.25f, stackTarget), 2.0f);

        // 移动中途翻回背面
        auto delayFlip = DelayTime::create(0.1f);
        auto doFlip = CallFunc::create([animCard]() {
            animCard->showBack();
        });
        auto flipAnim = ScaleTo::create(0.12f, 0.0f, 1.0f);
        auto flipBack = ScaleTo::create(0.12f, 1.0f, 1.0f);

        auto onComplete = CallFunc::create([this, animCard]() {
            animCard->removeFromParent();
            _gameView->getStackView()->updateCount(_gameModel->getStackSize());
            _isAnimating = false;
        });

        animCard->runAction(Sequence::create(
            Spawn::create(moveTo,
                Sequence::create(delayFlip, flipAnim, doFlip, flipBack, nullptr),
                nullptr),
            onComplete, nullptr));

        CCLOG("GameController::onUndoClick - returned card %s to stack",
              CardUtils::cardToString(card).c_str());
    } else {
        _isAnimating = false;
    }
}

void GameController::checkWinCondition() {
    if (_gameModel->isPlayfieldEmpty()) {
        CCLOG("GameController::checkWinCondition - YOU WIN!");

        auto winLabel = Label::createWithSystemFont("YOU WIN!", "Arial", 72);
        winLabel->setPosition(Vec2(540, 1040));
        winLabel->setTextColor(Color4B::YELLOW);
        _gameView->addChild(winLabel, 100);
    }
}

std::vector<int> GameController::updateCoveringAfterRemove(int removedCardId) {
    std::vector<int> flippedIds;

    for (auto* card : _gameModel->getPlayfieldCards()) {
        card->removeCoveredBy(removedCardId);

        // 没人遮盖了就翻面
        if (!card->isCovered() && !card->isFaceUp()) {
            card->setFaceUp(true);
            flippedIds.push_back(card->getId());
        }
    }

    return flippedIds;
}
