#include "pch.h"
#include "PaintView.h"


IMPLEMENT_DYNCREATE(CPaintView, CView)

BEGIN_MESSAGE_MAP(CPaintView, CView)
    ON_WM_PAINT() 
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()


void CPaintView::OnDraw(CDC* pDC) {

    // ������ʵ�ֻ�ͼ�߼���֧�ִ�ӡ/Ԥ����
    if (this->m_shapeManager != nullptr  && this->m_shapeManager->GetShapeCount() != 0) {
        this->m_shapeManager->DrawAll(pDC);
    }
}

void CPaintView::SetCurrentColor(COLORREF color, bool add) {
	this->m_currentColor = color;
	if (add) {
		m_history.Push(std::make_unique<OperationRecord>(color));
	}
}

void CPaintView::SetCurrentShapeType(int type) {
	this->m_currentShapeType = type;
}

// ��갴�¿�ʼ����
void CPaintView::OnLButtonDown(UINT nFlags, CPoint point) {
	m_ptStart = m_ptEnd = m_ptOldEnd = point; // ��ʼ�����е�
	SetCapture();
	CView::OnLButtonDown(nFlags, point);
}

// ����ƶ�ʵʱ����Ԥ��
void CPaintView::OnMouseMove(UINT nFlags, CPoint point) {
	if (GetCapture() == this) {
		CClientDC dc(this);
		dc.SetROP2(R2_NOTXORPEN); // ʹ��NOTXORģʽ

		// ������һ֡
		if (m_ptOldEnd != m_ptStart) {
			DrawPreview(&dc, m_ptStart, m_ptOldEnd);
		}

		// ������һ֡
		DrawPreview(&dc, m_ptStart, point);

		m_ptOldEnd = point;
	}
	CView::OnMouseMove(nFlags, point);
}

// ����ͷ�������
void CPaintView::OnLButtonUp(UINT nFlags, CPoint point) {
	if (GetCapture() == this) {
		ReleaseCapture();
		m_ptEnd = point;

		// ��������ͼ�β���ӵ�������
		int m_ptStart_xcorrect = m_ptStart.x;
		int m_ptEnd_xcorrect = m_ptEnd.x;
		CRect rect(m_ptStart_xcorrect, m_ptStart.y, m_ptEnd_xcorrect, m_ptEnd.y);
		rect.NormalizeRect();

		switch (m_currentShapeType) {
		case 0: // ����
		{
			m_shapeManager->AddShape(std::make_unique<RectangleShape>(rect, m_currentColor));
			m_history.Push(std::make_unique<OperationRecord>(std::make_unique<RectangleShape>(rect, m_currentColor)));
		}
		break;
		case 1: // Բ��
		{
			int radius = static_cast<int>(sqrt(pow(rect.Width(), 2) + pow(rect.Height(), 2)) / 2);
			CPoint center((rect.left + rect.right) / 2, (rect.top + rect.bottom) / 2);
			m_shapeManager->AddShape(std::make_unique<CircleShape>(center, radius, m_currentColor));
			m_history.Push(std::make_unique<OperationRecord>(std::make_unique<CircleShape>(center, radius, m_currentColor)));
		}
		break;
		case 2: // ��Բ
		{
			m_shapeManager->AddShape(std::make_unique<EllipseShape>(rect, m_currentColor));
			m_history.Push(std::make_unique<OperationRecord>(std::make_unique<EllipseShape>(rect, m_currentColor)));
		}
			break;
		}


	}
	CView::OnLButtonUp(nFlags, point);
}

// ����Ԥ��
void CPaintView::DrawPreview(CDC* pDC, CPoint pt1, CPoint pt2) {
	CRect rect(pt1.x, pt1.y, pt2.x, pt2.y);
	rect.NormalizeRect();

	switch (m_currentShapeType) {
	case 0: // ����Ԥ��
		pDC->Rectangle(rect);
		break;
	case 1: // Բ��Ԥ��
	{
		int radius = static_cast<int>(sqrt(pow(rect.Width(), 2) + pow(rect.Height(), 2)) / 2);
		CPoint center((rect.left + rect.right) / 2, (rect.top + rect.bottom) / 2);
		pDC->Ellipse(center.x - radius, center.y - radius,
			center.x + radius, center.y + radius);
	}
	break;
	case 2: // ��ԲԤ��
		pDC->Ellipse(rect);
		break;
	}
}