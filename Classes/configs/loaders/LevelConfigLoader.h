#pragma once

#include "cocos2d.h"
#include "../models/LevelConfig.h"
#include <string>

// 从json文件加载关卡配置
class LevelConfigLoader {
public:
    // 返回nullptr表示读取失败，调用方负责释放
    static LevelConfig* loadFromFile(const std::string& filePath);
};
