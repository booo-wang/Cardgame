#pragma once
#include "cocos2d.h"

class GameController;

// 游戏主场景
class GameScene : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init() override;
    CREATE_FUNC(GameScene);

    virtual ~GameScene();

private:
    GameController* _gameController;
};
