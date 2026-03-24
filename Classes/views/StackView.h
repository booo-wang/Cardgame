#pragma once

#include "cocos2d.h"
#include <functional>

// 备用牌堆，点击抽牌
class StackView : public cocos2d::Node {
public:
    static StackView* create(int count);
    bool init(int count);

    void updateCount(int count);
    void setOnClickCallback(std::function<void()> callback);

private:
    int _remainingCount;
    cocos2d::Sprite* _stackSprite;
    cocos2d::Label* _countLabel;
    std::function<void()> _onClickCallback;
};
