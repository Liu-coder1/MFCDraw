#pragma once
#include <afxwin.h>
#include "ShapeManager.h"
#include "RectangleShape.h"
#include "CircleShape.h"
#include "EllipseShape.h"
#include "HistoryManager.h"

class CPaintView : public CView {
public:
    CPaintView() = default;

    DECLARE_DYNCREATE(CPaintView) // 支持动态创建
    std::unique_ptr<ShapeManager> m_shapeManager;

    void SetCurrentColor(COLORREF color, bool add);
    void SetCurrentShapeType(int type);

    HistoryManager* GetHistory(){
        return &m_history; 
    }

    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    void DrawPreview(CDC* pDC, CPoint pt1, CPoint pt2);
protected:
    afx_msg void OnDraw(CDC* pDC) override; // 重写绘制函数
    DECLARE_MESSAGE_MAP()
private:
    CPoint m_ptStart;
    CPoint m_ptEnd;
    CPoint m_ptOldEnd;
    int m_currentShapeType = 0; // 0:矩形, 1:圆形, 2:椭圆
    COLORREF m_currentColor = RGB(0, 0, 0); // 默认蓝色
    HistoryManager m_history;

};
