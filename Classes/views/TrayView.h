#pragma once

#include "cocos2d.h"
#include "models/CardModel.h"
#include "views/CardView.h"

// 底牌区，显示当前手牌顶部的牌
class TrayView : public cocos2d::Node {
public:
    static TrayView* create();
    bool init() override;

    void setTopCard(CardModel* card);
    CardView* getTopCardView() const;
    cocos2d::Vec2 getTopCardPosition() const;

private:
    CardView* _topCardView;
    cocos2d::Sprite* _placeholder;
};
