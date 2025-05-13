#pragma once
#include "afxwin.h"

class CColorsDlg : public CDialogEx
{
    DECLARE_DYNAMIC(CColorsDlg)

public:
    CColorsDlg(COLORREF initialColor = RGB(0, 0, 0), CWnd* pParent = nullptr);
    virtual ~CColorsDlg();

    COLORREF GetSelectedColor() const { return m_selectedColor; }

    // 对话框数据
    enum { IDD = IDD_COLOR_PICKER_DLG };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);
    virtual BOOL OnInitDialog();

    COLORREF m_selectedColor;
    COLORREF m_initialColor;
    CStatic m_colorPreview;

    DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnPaint();
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnBnClickedOk();
    afx_msg void OnBnClickedCancel();
};