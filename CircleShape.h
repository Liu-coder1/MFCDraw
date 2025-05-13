#pragma once
#include "Shape.h"

class CircleShape : public Shape {
public:
    CircleShape() = default;

    static constexpr int TYPE_ID = 1;

    void Serialize(std::ostream& out) const override {
        out.write(reinterpret_cast<const char*>(&TYPE_ID), sizeof(TYPE_ID));
        out.write(reinterpret_cast<const char*>(&m_color), sizeof(m_color));
        out.write(reinterpret_cast<const char*>(&m_center), sizeof(m_center));
        out.write(reinterpret_cast<const char*>(&m_radius), sizeof(m_radius));
    }

    void Deserialize(std::istream& in) override {
        in.read(reinterpret_cast<char*>(&m_color), sizeof(m_color));
        in.read(reinterpret_cast<char*>(&m_center), sizeof(m_center));
        in.read(reinterpret_cast<char*>(&m_radius), sizeof(m_radius));
    }

    CircleShape(const CPoint& center, int radius, COLORREF color = RGB(0, 0, 0))
        : m_center(center), m_radius(radius) {
        m_color = color;
    }

    void Draw(CDC* pDC) const override {
        CPen pen(PS_SOLID, 2, m_color);
        CBrush brush(m_color);

        CPen* pOldPen = pDC->SelectObject(&pen);
        CBrush* pOldBrush = pDC->SelectObject(&brush);

        pDC->Ellipse(m_center.x - m_radius, m_center.y - m_radius,
            m_center.x + m_radius, m_center.y + m_radius);

        pDC->SelectObject(pOldPen);
        pDC->SelectObject(pOldBrush);
    }

    std::unique_ptr<Shape> Clone() const override {
        return std::make_unique<CircleShape>(m_center, m_radius, m_color);
    }

    void SetCenter(const CPoint& center) { m_center = center; }
    CPoint GetCenter() const { return m_center; }
    void SetRadius(int radius) { m_radius = radius; }
    int GetRadius() const { return m_radius; }

private:
    CPoint m_center;
    int m_radius;
};