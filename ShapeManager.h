#pragma once
#include <vector>
#include <memory>
#include "Shape.h"
#include "RectangleShape.h"
#include "CircleShape.h"
#include "EllipseShape.h"
class ShapeManager {
public:
    std::vector<Shape*> GetShapes() const {
        std::vector<Shape*> views;
        for (const auto& shape : m_shapes) {
            views.push_back(shape.get());
        }
        return views;
    }

    // 添加图形
    void AddShape(std::unique_ptr<Shape> shape) {
        m_shapes.push_back(std::move(shape));
    }
    void DeleteShape() {
        m_shapes.pop_back();
    }

    // 绘制所有图形
    void DrawAll(CDC* pDC) const {
        for (const auto& shape : m_shapes) {
            shape->Draw(pDC);
        }
    }

    // 清空所有图形
    void Clear() {
        m_shapes.clear();
    }

    // 获取图形数量
    size_t GetShapeCount() const {
        return m_shapes.size();
    }

    // 获取指定图形
    Shape* GetShape(size_t index) {
        if (index < m_shapes.size()) {
            return m_shapes[index].get();
        }
        return nullptr;
    }

    void SaveToFile(const CString& path) {
        std::ofstream out(CT2A(path), std::ios::binary);
        if (!out) return;

        // 写入图形数量
        size_t count = m_shapes.size();
        out.write(reinterpret_cast<const char*>(&count), sizeof(count));

        // 写入每个图形
        for (const auto& shape : m_shapes) {
            shape->Serialize(out);
        }
    }

    void LoadFromFile(const CString& path) {
        std::ifstream in(CT2A(path), std::ios::binary);
        if (!in) return;

        // 清空当前图形
        m_shapes.clear();

        // 读取图形数量
        size_t count;
        in.read(reinterpret_cast<char*>(&count), sizeof(count));

        // 读取每个图形
        for (size_t i = 0; i < count; ++i) {
            int typeId;
            in.read(reinterpret_cast<char*>(&typeId), sizeof(typeId));

            std::unique_ptr<Shape> shape;
            switch (typeId) {
            case RectangleShape::TYPE_ID:
                shape = std::make_unique<RectangleShape>();
                break;
            case CircleShape::TYPE_ID:
                shape = std::make_unique<CircleShape>();
                break;
            case EllipseShape::TYPE_ID:
                shape = std::make_unique<EllipseShape>();
                break;
            default:
                throw std::runtime_error("Unknown shape type");
            }

            shape->Deserialize(in);
            m_shapes.push_back(std::move(shape));
        }
    }

private:
    std::vector<std::unique_ptr<Shape>> m_shapes;
};