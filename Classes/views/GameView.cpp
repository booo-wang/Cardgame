#include "views/GameView.h"
#include "models/CardModel.h"

USING_NS_CC;

GameView* GameView::create()
{
    GameView* ret = new (std::nothrow) GameView();
    if (ret && ret->init())
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool GameView::init()
{
    if (!Node::init())
    {
        return false;
    }

    this->setContentSize(Size(1080, 2080));

    _playFieldView = nullptr;
    _trayView = nullptr;
    _stackView = nullptr;
    _undoButton = nullptr;
    _onUndoClickCallback = nullptr;

    // 回退按钮
    _undoButton = Node::create();
    if (_undoButton)
    {
        auto bg = LayerColor::create(Color4B(160, 82, 45, 230), 130, 60);
        bg->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        bg->setPosition(Vec2(-65, -30));
        _undoButton->addChild(bg, 0);

        auto label = Label::createWithSystemFont("\xe5\x9b\x9e\xe9\x80\x80", "Arial", 34);
        if (label)
        {
            label->setPosition(Vec2::ZERO);
            label->setTextColor(Color4B(255, 240, 200, 255));
            label->enableOutline(Color4B(80, 40, 10, 200), 2);
            _undoButton->addChild(label, 1);
        }

        _undoButton->setContentSize(Size(120, 60));
        _undoButton->setPosition(Vec2(800, 290));
        this->addChild(_undoButton, 10);

        auto listener = EventListenerTouchOneByOne::create();
        listener->setSwallowTouches(true);

        listener->onTouchBegan = [this](Touch* touch, Event* event) -> bool {
            if (!_undoButton)
            {
                return false;
            }

            Vec2 locationInNode = _undoButton->convertToNodeSpace(touch->getLocation());
            Size size = _undoButton->getContentSize();
            // 按钮居中的所以要从-half开始算
            Rect rect(-size.width * 0.5f, -size.height * 0.5f, size.width, size.height);

            if (rect.containsPoint(locationInNode))
            {
                if (_onUndoClickCallback)
                {
                    _onUndoClickCallback();
                }
                return true;
            }
            return false;
        };

        _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, _undoButton);
    }

    return true;
}

PlayFieldView* GameView::getPlayFieldView() const
{
    return _playFieldView;
}

TrayView* GameView::getTrayView() const
{
    return _trayView;
}

StackView* GameView::getStackView() const
{
    return _stackView;
}

void GameView::initPlayField(const std::vector<CardModel*>& cards)
{
    if (_playFieldView)
    {
        _playFieldView->removeFromParent();
        _playFieldView = nullptr;
    }

    _playFieldView = PlayFieldView::create(cards);
    if (_playFieldView)
    {
        // 主牌区放在上面，y=580开始
        _playFieldView->setPosition(Vec2(0, 580));
        _playFieldView->setContentSize(Size(1080, 1500));
        this->addChild(_playFieldView, 1);
    }
}

void GameView::initStack(int count)
{
    if (_stackView)
    {
        _stackView->removeFromParent();
        _stackView = nullptr;
    }

    _stackView = StackView::create(count);
    if (_stackView)
    {
        _stackView->setPosition(Vec2(150, 290));
        this->addChild(_stackView, 1);
    }
}

void GameView::initTray(CardModel* topCard)
{
    if (_trayView)
    {
        _trayView->removeFromParent();
        _trayView = nullptr;
    }

    _trayView = TrayView::create();
    if (_trayView)
    {
        _trayView->setPosition(Vec2(540, 290));
        this->addChild(_trayView, 1);

        if (topCard)
        {
            _trayView->setTopCard(topCard);
        }
    }
}

void GameView::setOnUndoClickCallback(std::function<void()> callback)
{
    _onUndoClickCallback = callback;
}
