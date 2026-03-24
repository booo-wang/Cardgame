#pragma once

#include "cocos2d.h"
#include <vector>

// 撤销操作类型
enum UndoActionType {
    UNDO_PLAYFIELD_TO_TRAY,  // 从牌面移到tray
    UNDO_STACK_TO_TRAY       // 从牌堆翻到tray
};

// 撤销记录
struct UndoRecord {
    UndoActionType type;
    int cardId;
    cocos2d::Vec2 originalPosition;       // 移动前的位置
    std::vector<int> uncoveredCardIds;    // 移走后翻开的牌
};

// 撤销栈，管理操作历史
class UndoModel {
public:
    UndoModel();
    ~UndoModel();

    void pushRecord(const UndoRecord& record);
    UndoRecord popRecord();
    bool hasRecords() const;
    void clear();
    const std::vector<UndoRecord>& getRecords() const;

private:
    std::vector<UndoRecord> _records;
};
