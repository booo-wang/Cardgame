#pragma once

#include "cocos2d.h"
#include "../../models/CardModel.h"
#include <vector>

// 单张牌的配置
struct CardConfig {
    CardFaceType face;
    CardSuitType suit;
    cocos2d::Vec2 position;
};

// 关卡配置，包含牌面和牌堆的卡牌数据
struct LevelConfig {
    std::vector<CardConfig> playfieldCards;
    std::vector<CardConfig> stackCards;
};
