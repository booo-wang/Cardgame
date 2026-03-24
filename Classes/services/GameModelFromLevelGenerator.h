#pragma once

#include "models/GameModel.h"
#include "configs/models/LevelConfig.h"

// 根据关卡配置生成运行时GameModel
class GameModelFromLevelGenerator {
public:
    // caller负责释放返回的指针
    static GameModel* generate(const LevelConfig* config);
};
