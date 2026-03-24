#pragma once

#include "cocos2d.h"
#include "models/GameModel.h"
#include "views/GameView.h"
#include "controllers/PlayFieldController.h"
#include "controllers/StackController.h"
#include "managers/UndoManager.h"
#include "configs/models/LevelConfig.h"

// 游戏主控制器，管理整个游戏流程
class GameController {
public:
    GameController();
    ~GameController();

    // 初始化游戏，加载关卡配置并创建视图
    bool init(cocos2d::Node* parentNode);

    void startGame();
    void onPlayfieldCardClick(int cardId);
    void onStackClick();
    void onUndoClick();
    void checkWinCondition();

private:
    // 移除卡牌后更新遮盖关系，返回被翻面的卡牌ID
    std::vector<int> updateCoveringAfterRemove(int removedCardId);

    GameModel* _gameModel;
    GameView* _gameView;
    PlayFieldController* _playFieldController;
    StackController* _stackController;
    UndoManager* _undoManager;
    LevelConfig* _levelConfig;
    bool _isAnimating;
};
