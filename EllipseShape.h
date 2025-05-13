#pragma once
#include "Shape.h"

class EllipseShape : public Shape {
public:
    EllipseShape() = default;

    static constexpr int TYPE_ID = 2;

    void Serialize(std::ostream& out) const override {
        out.write(reinterpret_cast<const char*>(&TYPE_ID), sizeof(TYPE_ID));
        out.write(reinterpret_cast<const char*>(&m_color), sizeof(m_color));
        out.write(reinterpret_cast<const char*>(&m_bounds), sizeof(m_bounds));
    }

    void Deserialize(std::istream& in) override {
        in.read(reinterpret_cast<char*>(&m_color), sizeof(m_color));
        in.read(reinterpret_cast<char*>(&m_bounds), sizeof(m_bounds));
    }

    EllipseShape(const CRect& bounds, COLORREF color = RGB(0, 0, 0))
        : m_bounds(bounds) {
        m_color = color;
    }

    void Draw(CDC* pDC) const override {
        CPen pen(PS_SOLID, 1, m_color);
        CBrush brush(m_color);

        CPen* pOldPen = pDC->SelectObject(&pen);
        CBrush* pOldBrush = pDC->SelectObject(&brush);

        pDC->Ellipse(m_bounds);

        pDC->SelectObject(pOldPen);
        pDC->SelectObject(pOldBrush);
    }

    std::unique_ptr<Shape> Clone() const override {
        return std::make_unique<EllipseShape>(m_bounds, m_color);
    }

    void SetBounds(const CRect& bounds) { m_bounds = bounds; }
    CRect GetBounds() const { return m_bounds; }

private:
    CRect m_bounds;
};