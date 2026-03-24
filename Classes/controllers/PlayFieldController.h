#pragma once

#include "models/GameModel.h"
#include "views/PlayFieldView.h"
#include <functional>

// 主牌区控制器，处理卡牌点击交互
class PlayFieldController {
public:
    PlayFieldController();
    ~PlayFieldController();

    void init(GameModel* model, PlayFieldView* view);
    void setOnCardMatchCallback(std::function<void(int)> callback);

    // 处理点击，验证卡牌状态后触发回调
    bool handleCardClick(int cardId);

private:
    GameModel* _gameModel;
    PlayFieldView* _playFieldView;
    std::function<void(int)> _onCardMatchCallback;
};
