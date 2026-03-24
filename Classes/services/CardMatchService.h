#pragma once

#include "models/CardModel.h"

// 匹配规则：点数差1即可匹配，不限花色
class CardMatchService {
public:
    static bool canMatch(CardFaceType face1, CardFaceType face2);
    // null safe版本
    static bool canMatchCards(const CardModel* card1, const CardModel* card2);
};
