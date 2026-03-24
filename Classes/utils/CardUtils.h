#pragma once

#include "models/CardModel.h"
#include <string>

// 卡牌工具类，格式化输出用
class CardUtils {
public:
    static std::string faceToString(CardFaceType face);
    static std::string suitToString(CardSuitType suit);
    // 返回类似"♥A"、"♣10"的字符串，null返回"?"
    static std::string cardToString(const CardModel* card);
};
