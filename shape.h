#pragma once
#include <afxwin.h>
#include <fstream>

class Shape {
public:
	virtual ~Shape() = default;

	//绘制图形纯虚函数
	virtual void Draw(CDC* pDC) const = 0;
	//序列化虚函数
	virtual void Serialize(std::ostream& out) const = 0;
	virtual void Deserialize(std::istream& in) = 0;

	//虚函数设置颜色
	virtual void SetColor(COLORREF color) {
		m_color = color;
	}
	virtual COLORREF GetColor() const {
		return m_color;
	}

	//深拷贝
	virtual std::unique_ptr<Shape> Clone() const = 0;

protected:
	COLORREF m_color = RGB(0, 0, 0);
};