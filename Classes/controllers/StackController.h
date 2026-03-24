#pragma once

#include "models/GameModel.h"
#include "views/StackView.h"
#include <functional>

// 备用牌堆控制器
class StackController {
public:
    StackController();
    ~StackController();

    void init(GameModel* model, StackView* view);
    void setOnStackDrawCallback(std::function<void()> callback);

    // 点击牌堆抽牌，返回是否成功
    bool handleStackClick();

private:
    GameModel* _gameModel;
    StackView* _stackView;
    std::function<void()> _onStackDrawCallback;
};
