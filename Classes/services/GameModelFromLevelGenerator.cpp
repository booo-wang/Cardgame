#include "GameModelFromLevelGenerator.h"
#include <cmath>

GameModel* GameModelFromLevelGenerator::generate(const LevelConfig* config) {
    if (config == nullptr) {
        return nullptr;
    }

    // 重置ID计数器
    CardModel::resetIdCounter();

    // 创建牌区卡牌（先全部正面朝上）
    std::vector<CardModel*> playfieldCards;
    for (const auto& cfg : config->playfieldCards) {
        auto* card = new CardModel(cfg.face, cfg.suit);
        card->setPosition(cfg.position);
        card->setFaceUp(true);
        playfieldCards.push_back(card);
    }

    // 创建牌堆卡牌
    std::vector<CardModel*> stackCards;
    for (const auto& cfg : config->stackCards) {
        auto* card = new CardModel(cfg.face, cfg.suit);
        card->setPosition(cfg.position);
        card->setFaceUp(false);
        stackCards.push_back(card);
    }

    // 根据位置关系建立遮盖
    // A在B上方且水平距离<200、垂直距离<300时，A遮盖B
    for (size_t i = 0; i < playfieldCards.size(); ++i) {
        for (size_t j = 0; j < playfieldCards.size(); ++j) {
            if (i == j) continue;

            CardModel* cardA = playfieldCards[i];
            CardModel* cardB = playfieldCards[j];

            float dx = std::fabs(cardA->getPosition().x - cardB->getPosition().x);
            float dy = cardA->getPosition().y - cardB->getPosition().y;

            if (dy > 0 && dx < 200.0f && dy < 300.0f) {
                cardB->addCoveredBy(cardA->getId());
            }
        }
    }

    // 被遮盖的牌翻成背面
    for (auto* card : playfieldCards) {
        if (card->isCovered()) {
            card->setFaceUp(false);
        } else {
            card->setFaceUp(true);
        }
    }

    // 组装GameModel
    auto* model = new GameModel();
    model->playfieldCards() = playfieldCards;
    model->stackCards() = stackCards;

    // 从牌堆抽一张作为初始tray牌
    if (model->getStackSize() > 0) {
        CardModel* firstTrayCard = model->popStackTopCard();
        firstTrayCard->setFaceUp(true);
        model->addTrayCard(firstTrayCard);
    }

    return model;
}
