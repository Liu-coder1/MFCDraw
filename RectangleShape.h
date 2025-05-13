#pragma once
#include "Shape.h"

class RectangleShape : public Shape {
public:
	RectangleShape() = default;

	static constexpr int TYPE_ID = 0;

	void Serialize(std::ostream& out) const override {
		// д�����ͱ�ʶ
		out.write(reinterpret_cast<const char*>(&TYPE_ID), sizeof(TYPE_ID));
		// д����ɫ
		out.write(reinterpret_cast<const char*>(&m_color), sizeof(m_color));
		// д���������
		out.write(reinterpret_cast<const char*>(&m_rect), sizeof(m_rect));
	}

	void Deserialize(std::istream& in) override {
		// ��ȡ��ɫ
		in.read(reinterpret_cast<char*>(&m_color), sizeof(m_color));
		// ��ȡ��������
		in.read(reinterpret_cast<char*>(&m_rect), sizeof(m_rect));
	}

	RectangleShape(const CRect& rect, COLORREF color = RGB(0, 0, 0))
		: m_rect(rect) {
		m_color = color;
	}

	void Draw(CDC* pDC) const override {


		CPen pen(PS_SOLID, 1, m_color);
		CBrush brush(m_color);

		CPen* pOldPen = pDC->SelectObject(&pen);
		CBrush* pOldBrush = pDC->SelectObject(&brush);

		pDC->Rectangle(m_rect);

		pDC->SelectObject(pOldPen);
		pDC->SelectObject(pOldBrush);
	}

	std::unique_ptr<Shape> Clone() const override {
		return std::make_unique<RectangleShape>(m_rect, m_color);
	}

	void SetRect(const CRect& rect) { m_rect = rect; }
	CRect GetRect() const { return m_rect; }

private:
	CRect m_rect;
};