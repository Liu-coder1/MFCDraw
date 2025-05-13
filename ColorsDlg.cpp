#include "pch.h"
#include "MFCDraw.h"
#include "ColorsDlg.h"

// 预定义颜色网格
static COLORREF s_standardColors[] = {
    RGB(0, 0, 0),       RGB(128, 0, 0),     RGB(0, 128, 0),     RGB(128, 128, 0),
    RGB(0, 0, 128),     RGB(128, 0, 128),   RGB(0, 128, 128),   RGB(192, 192, 192),
    RGB(128, 128, 128), RGB(255, 0, 0),     RGB(0, 255, 0),     RGB(255, 255, 0),
    RGB(0, 0, 255),     RGB(255, 0, 255),   RGB(0, 255, 255),   RGB(255, 255, 255)
};

IMPLEMENT_DYNAMIC(CColorsDlg, CDialogEx)

CColorsDlg::CColorsDlg(COLORREF initialColor, CWnd* pParent /*=nullptr*/)
    : CDialogEx(IDD_COLOR_PICKER_DLG, pParent)
    , m_initialColor(initialColor)
    , m_selectedColor(initialColor)
{
}

CColorsDlg::~CColorsDlg()
{
}

void CColorsDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_COLOR_PREVIEW, m_colorPreview);
}

BEGIN_MESSAGE_MAP(CColorsDlg, CDialogEx)
    ON_WM_PAINT()
    ON_WM_LBUTTONDOWN()
    ON_BN_CLICKED(IDOK, &CColorsDlg::OnBnClickedOk)
    ON_BN_CLICKED(IDCANCEL, &CColorsDlg::OnBnClickedCancel)
END_MESSAGE_MAP()

BOOL CColorsDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // 初始化颜色预览区域
    CRect rect;
    m_colorPreview.GetWindowRect(&rect);
    ScreenToClient(&rect);
    
    m_colorPreview.ModifyStyle(0, SS_NOTIFY);
    m_colorPreview.ShowWindow(SW_HIDE);
    return TRUE;
}

void CColorsDlg::OnPaint()
{
    CPaintDC dc(this); // 用于绘制的设备上下文

    // 绘制颜色预览
    CRect rect;
    m_colorPreview.GetWindowRect(&rect);
    ScreenToClient(&rect);

    CBrush brush(m_selectedColor);
    dc.FillRect(rect, &brush);

    // 绘制颜色网格
    CRect gridRect(40, 60, 370, 270);
    int rows = 4;
    int cols = 4;
    int cellWidth = (gridRect.Width() - 10) / cols;   
    int cellHeight = (gridRect.Height() - 10) / rows;  
    dc.Rectangle(gridRect);

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            int index = i * cols + j;
            if (index < 16)
            {
                CRect cellRect(
                    gridRect.left + j * cellWidth + 5,
                    gridRect.top + i * cellHeight + 5,
                    gridRect.left + (j + 1) * cellWidth,
                    gridRect.top + (i + 1) * cellHeight
                );

                CBrush colorBrush(s_standardColors[index]);
                dc.FillRect(cellRect, &colorBrush);
                dc.FrameRect(cellRect, CBrush::FromHandle((HBRUSH)GetStockObject(BLACK_BRUSH)));
            }
        }
    }
}

void CColorsDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
    // 检查是否点击了颜色网格
    CRect gridRect(40, 60, 370, 270);
    if (gridRect.PtInRect(point))
    {
        int rows = 4;
        int cols = 4;
        int cellWidth = (gridRect.Width() - 10) / cols;
        int cellHeight = (gridRect.Height() - 10) / rows;

        int col = (point.x - gridRect.left - 5) / cellWidth;
        int row = (point.y - gridRect.top - 5) / cellHeight;

        if (col >= 0 && col < cols && row >= 0 && row < rows)
        {
            int index = row * cols + col;
            if (index < 16)
            {
                m_selectedColor = s_standardColors[index];
                Invalidate(FALSE); // 重绘预览区域
            }
        }
    }

    CDialogEx::OnLButtonDown(nFlags, point);
}

void CColorsDlg::OnBnClickedOk()
{
    // 用户点击确定，使用当前选择的颜色
    CDialogEx::OnOK();
}

void CColorsDlg::OnBnClickedCancel()
{
    // 用户点击取消，恢复原始颜色
    m_selectedColor = m_initialColor;
    CDialogEx::OnCancel();
}