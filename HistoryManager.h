#pragma once
#include <memory>
#include "OperationRecord.h"
#include <deque>

class HistoryManager {
public:
    const std::deque<std::unique_ptr<OperationRecord>>& GetRecords() const {
        return records; // ����const���ã����⿽��
    }

    void Push(std::unique_ptr<OperationRecord> record) {
        records.push_front(std::move(record));
        if (records.size() > 10) {
            records.pop_back();
        }
    }

    std::unique_ptr<OperationRecord> Pop() {
        if (records.empty()) return nullptr;
        auto record = std::move(records.front());
        records.pop_front();
        return record;
    }

    bool IsEmpty() const { return records.empty(); }

private:
    std::deque<std::unique_ptr<OperationRecord>> records; // ˫�˶���ʵ��
};