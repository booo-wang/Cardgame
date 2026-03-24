#pragma once

#include "cocos2d.h"
#include "CardModel.h"
#include "json/document.h"
#include <vector>

// 游戏状态数据，管理三个牌堆
class GameModel {
public:
    GameModel();
    ~GameModel();

    const std::vector<CardModel*>& getPlayfieldCards() const;
    const std::vector<CardModel*>& getStackCards() const;
    const std::vector<CardModel*>& getTrayCards() const;

    // tray操作
    CardModel* getTrayTopCard() const;
    void addTrayCard(CardModel* card);
    CardModel* popTrayTopCard();

    CardModel* findPlayfieldCard(int id) const;
    void removePlayfieldCard(int id);
    bool isPlayfieldEmpty() const;

    CardModel* popStackTopCard();
    int getStackSize() const;

    // 初始化用，直接拿引用
    std::vector<CardModel*>& playfieldCards();
    std::vector<CardModel*>& stackCards();
    std::vector<CardModel*>& trayCards();

    // 序列化
    rapidjson::Value serialize(rapidjson::Document::AllocatorType& allocator) const;
    static GameModel* deserialize(rapidjson::Value& value);

private:
    std::vector<CardModel*> _playfieldCards;
    std::vector<CardModel*> _stackCards;
    std::vector<CardModel*> _trayCards;
};
