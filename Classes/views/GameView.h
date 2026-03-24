#pragma once

#include "cocos2d.h"
#include "views/PlayFieldView.h"
#include "views/TrayView.h"
#include "views/StackView.h"
#include <functional>
#include <vector>

class CardModel;

// 游戏主视图 1080x2080
// 上面1500是主牌区，下面580放底牌/牌堆/回退
class GameView : public cocos2d::Node {
public:
    static GameView* create();
    bool init() override;

    PlayFieldView* getPlayFieldView() const;
    TrayView* getTrayView() const;
    StackView* getStackView() const;

    void initPlayField(const std::vector<CardModel*>& cards);
    void initStack(int count);
    void initTray(CardModel* topCard);

    void setOnUndoClickCallback(std::function<void()> callback);

private:
    PlayFieldView* _playFieldView;
    TrayView* _trayView;
    StackView* _stackView;
    cocos2d::Node* _undoButton;
    std::function<void()> _onUndoClickCallback;
};
