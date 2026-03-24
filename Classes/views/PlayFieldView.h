#pragma once

#include "cocos2d.h"
#include "models/CardModel.h"
#include "views/CardView.h"
#include <map>
#include <vector>
#include <functional>

// 主牌区，显示桌面上所有的牌
class PlayFieldView : public cocos2d::Node {
public:
    static PlayFieldView* create(const std::vector<CardModel*>& cards);
    bool init(const std::vector<CardModel*>& cards);

    void setOnCardClickCallback(std::function<void(int cardId)> callback);
    CardView* getCardView(int cardId);
    void removeCardView(int cardId);
    void detachCardView(int cardId);  // 只从map移除，不删除节点
    void flipCard(int cardId);
    void addCardView(int cardId, CardView* cardView);

private:
    std::map<int, CardView*> _cardViews;
    std::function<void(int cardId)> _onCardClickCallback;
};
