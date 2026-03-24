#pragma once

#include "cocos2d.h"
#include "json/document.h"
#include <vector>
#include <string>

// 花色
enum CardSuitType {
    CST_NONE = -1,
    CST_CLUBS = 0,     // 梅花 (black)
    CST_DIAMONDS = 1,  // 方块 (red)
    CST_HEARTS = 2,    // 红桃 (red)
    CST_SPADES = 3,    // 黑桃 (black)
    CST_NUM_CARD_SUIT_TYPES = 4
};

// 牌面值 A~K
enum CardFaceType {
    CFT_NONE = -1,
    CFT_ACE = 0, CFT_TWO, CFT_THREE, CFT_FOUR, CFT_FIVE,
    CFT_SIX, CFT_SEVEN, CFT_EIGHT, CFT_NINE, CFT_TEN,
    CFT_JACK, CFT_QUEEN, CFT_KING,
    CFT_NUM_CARD_FACE_TYPES
};

// 单张卡牌的数据模型
class CardModel {
public:
    CardModel();
    CardModel(CardFaceType face, CardSuitType suit);
    ~CardModel();

    int getId() const;
    CardFaceType getFace() const;
    CardSuitType getSuit() const;
    bool isFaceUp() const;
    cocos2d::Vec2 getPosition() const;
    const std::vector<int>& getCoveredByIds() const;

    void setFace(CardFaceType face);
    void setSuit(CardSuitType suit);
    void setFaceUp(bool faceUp);
    void setPosition(const cocos2d::Vec2& position);

    // 覆盖关系
    void addCoveredBy(int id);
    void removeCoveredBy(int id);
    bool isCovered() const;

    // 序列化
    rapidjson::Value serialize(rapidjson::Document::AllocatorType& allocator) const;
    static CardModel* deserialize(rapidjson::Value& value);

    static void resetIdCounter();

private:
    int _id;
    CardFaceType _face;
    CardSuitType _suit;
    bool _faceUp;
    cocos2d::Vec2 _position;
    std::vector<int> _coveredByIds;

    static int _nextId;
};
