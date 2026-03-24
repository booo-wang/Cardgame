#pragma once

#include "cocos2d.h"
#include "models/CardModel.h"
#include <functional>

// 卡牌视图，显示单张牌
class CardView : public cocos2d::Node {
public:
    static CardView* create(CardModel* model);
    bool init(CardModel* model);

    void updateDisplay();
    void showFront();
    void showBack();

    void playFlipAnimation(std::function<void()> callback = nullptr);

    void setClickable(bool clickable);
    bool isClickable() const;
    void setOnClickCallback(std::function<void(int cardId)> callback);

    CardModel* getCardModel() const;

private:
    CardModel* _cardModel;
    cocos2d::Sprite* _cardBase;
    cocos2d::Sprite* _bigNumber;
    cocos2d::Sprite* _smallNumber;
    cocos2d::Sprite* _suitIcon;
    cocos2d::LayerColor* _cardBack;
    bool _isClickable;
    std::function<void(int cardId)> _onClickCallback;
};
