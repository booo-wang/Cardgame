#include "UndoManager.h"

UndoManager::UndoManager() {
    _undoModel = new UndoModel();
}

UndoManager::~UndoManager() {
    delete _undoModel;
    _undoModel = nullptr;
}

void UndoManager::recordPlayfieldToTray(int cardId, const cocos2d::Vec2& originalPos,
                                        const std::vector<int>& uncoveredIds) {
    UndoRecord record;
    record.type = UNDO_PLAYFIELD_TO_TRAY;
    record.cardId = cardId;
    record.originalPosition = originalPos;
    record.uncoveredCardIds = uncoveredIds;
    _undoModel->pushRecord(record);
}

void UndoManager::recordStackToTray(int cardId) {
    UndoRecord record;
    record.type = UNDO_STACK_TO_TRAY;
    record.cardId = cardId;
    record.originalPosition = cocos2d::Vec2::ZERO;
    _undoModel->pushRecord(record);
}

bool UndoManager::canUndo() const {
    return _undoModel->hasRecords();
}

UndoRecord UndoManager::undo() {
    return _undoModel->popRecord();
}

void UndoManager::clear() {
    _undoModel->clear();
}
