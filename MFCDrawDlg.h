
// MFCDrawDlg.h: 头文件
//

#pragma once
#include "ShapeManager.h"
#include "PaintView.h"

// CMFCDrawDlg 对话框
class CMFCDrawDlg : public CDialogEx
{
// 构造
public:
	CMFCDrawDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFCDRAW_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	CStatic m_colorSelect;  
	CBrush m_bkBrush; // 画刷对象

	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();


	afx_msg void OnShapeTypeChanged(UINT nID);
	afx_msg void OnColorChanged();

	afx_msg HCURSOR OnQueryDragIcon();

	COLORREF GetLastValidColor();
	DECLARE_MESSAGE_MAP()


private:
	int m_currentShapeType = 0; // 0:矩形, 1:圆形, 2:椭圆
	COLORREF m_currentColor = RGB(0, 0, 0); // 默认蓝色
	CPaintView* m_paintView;

public:
	afx_msg void OnBnClickedBtnDraw2();
	afx_msg void OnBnClickedBtnSave();
	afx_msg void OnBnClickedBtnOpenfile();
	afx_msg void OnBnClickedBtnUndo();
};
