#pragma once

#include "models/UndoModel.h"
#include "cocos2d.h"
#include <vector>

// 撤销管理器，记录操作历史用于undo
class UndoManager {
public:
    UndoManager();
    ~UndoManager();

    // 记录牌区->tray的操作
    void recordPlayfieldToTray(int cardId, const cocos2d::Vec2& originalPos,
                               const std::vector<int>& uncoveredIds);
    // 记录牌堆->tray的操作
    void recordStackToTray(int cardId);

    bool canUndo() const;
    UndoRecord undo();
    void clear();

private:
    UndoModel* _undoModel;
};
