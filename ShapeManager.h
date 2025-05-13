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

    // ���ͼ��
    void AddShape(std::unique_ptr<Shape> shape) {
        m_shapes.push_back(std::move(shape));
    }
    void DeleteShape() {
        m_shapes.pop_back();
    }

    // ��������ͼ��
    void DrawAll(CDC* pDC) const {
        for (const auto& shape : m_shapes) {
            shape->Draw(pDC);
        }
    }

    // �������ͼ��
    void Clear() {
        m_shapes.clear();
    }

    // ��ȡͼ������
    size_t GetShapeCount() const {
        return m_shapes.size();
    }

    // ��ȡָ��ͼ��
    Shape* GetShape(size_t index) {
        if (index < m_shapes.size()) {
            return m_shapes[index].get();
        }
        return nullptr;
    }

    void SaveToFile(const CString& path) {
        std::ofstream out(CT2A(path), std::ios::binary);
        if (!out) return;

        // д��ͼ������
        size_t count = m_shapes.size();
        out.write(reinterpret_cast<const char*>(&count), sizeof(count));

        // д��ÿ��ͼ��
        for (const auto& shape : m_shapes) {
            shape->Serialize(out);
        }
    }

    void LoadFromFile(const CString& path) {
        std::ifstream in(CT2A(path), std::ios::binary);
        if (!in) return;

        // ��յ�ǰͼ��
        m_shapes.clear();

        // ��ȡͼ������
        size_t count;
        in.read(reinterpret_cast<char*>(&count), sizeof(count));

        // ��ȡÿ��ͼ��
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