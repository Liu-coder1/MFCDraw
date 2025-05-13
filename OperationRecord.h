#pragma once
#include "OperationType.h"

struct OperationRecord {
    OpType type;
    COLORREF color;          // 仅当type==CHANGE_COLOR时有效
    std::unique_ptr<Shape> shape; // 仅当type==ADD_SHAPE时有效

    OperationRecord(COLORREF c)
        : type(OpType::CHANGE_COLOR), color(c), shape(nullptr) {}

    OperationRecord(std::unique_ptr<Shape> s)
        : type(OpType::ADD_SHAPE), color(CLR_NONE), shape(std::move(s)) {}
};