#include "UndoModel.h"

UndoModel::UndoModel()
{
}

UndoModel::~UndoModel()
{
}

void UndoModel::pushRecord(const UndoRecord& record)
{
    _records.push_back(record);
}

UndoRecord UndoModel::popRecord()
{
    UndoRecord record = _records.back();
    _records.pop_back();
    return record;
}

bool UndoModel::hasRecords() const
{
    return !_records.empty();
}

void UndoModel::clear()
{
    _records.clear();
}

const std::vector<UndoRecord>& UndoModel::getRecords() const
{
    return _records;
}
