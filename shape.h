#pragma once
#include <afxwin.h>
#include <fstream>

class Shape {
public:
	virtual ~Shape() = default;

	//����ͼ�δ��麯��
	virtual void Draw(CDC* pDC) const = 0;
	//���л��麯��
	virtual void Serialize(std::ostream& out) const = 0;
	virtual void Deserialize(std::istream& in) = 0;

	//�麯��������ɫ
	virtual void SetColor(COLORREF color) {
		m_color = color;
	}
	virtual COLORREF GetColor() const {
		return m_color;
	}

	//���
	virtual std::unique_ptr<Shape> Clone() const = 0;

protected:
	COLORREF m_color = RGB(0, 0, 0);
};