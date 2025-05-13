#pragma once
#include "OperationType.h"

struct OperationRecord {
    OpType type;
    COLORREF color;          // ����type==CHANGE_COLORʱ��Ч
    std::unique_ptr<Shape> shape; // ����type==ADD_SHAPEʱ��Ч

    OperationRecord(COLORREF c)
        : type(OpType::CHANGE_COLOR), color(c), shape(nullptr) {}

    OperationRecord(std::unique_ptr<Shape> s)
        : type(OpType::ADD_SHAPE), color(CLR_NONE), shape(std::move(s)) {}
};