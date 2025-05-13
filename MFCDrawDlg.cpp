
// MFCDrawDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "MFCDraw.h"
#include "MFCDrawDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMFCDrawDlg 对话框



CMFCDrawDlg::CMFCDrawDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFCDRAW_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCDrawDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IMAGE1, m_colorSelect);
}

BEGIN_MESSAGE_MAP(CMFCDrawDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CTLCOLOR()
	ON_COMMAND_RANGE(ID_SHAPE_RECT, ID_SHAPE_ELLIPSE, OnShapeTypeChanged)
	ON_BN_CLICKED(IDC_BTN_COLOR, OnColorChanged)
	ON_BN_CLICKED(IDC_BTN_DRAW2, &CMFCDrawDlg::OnBnClickedBtnDraw2)
	ON_BN_CLICKED(IDC_BTN_SAVE, &CMFCDrawDlg::OnBnClickedBtnSave)
	ON_BN_CLICKED(IDC_BTN_OPENFILE, &CMFCDrawDlg::OnBnClickedBtnOpenfile)
	ON_BN_CLICKED(IDC_BTN_UNDO, &CMFCDrawDlg::OnBnClickedBtnUndo)
END_MESSAGE_MAP()


// CMFCDrawDlg 消息处理程序

BOOL CMFCDrawDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	
	CWnd* pPlaceholder = GetDlgItem(IDC_CUSTOM_VIEW_PLACEHOLDER);
	CRect rect;
	pPlaceholder->GetWindowRect(&rect);
	ScreenToClient(&rect);
	pPlaceholder->DestroyWindow(); // 删除占位控件

	// 创建自定义视图
	m_paintView = new CPaintView;
	m_paintView->Create(nullptr, nullptr, WS_VISIBLE | WS_CHILD, rect, this, IDC_CUSTOM_VIEW);
	if (!m_paintView->m_shapeManager) { // 首次使用检查
		m_paintView->m_shapeManager = std::make_unique<ShapeManager>();
	}

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMFCDrawDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMFCDrawDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CPaintDC dc(this);
		CDialogEx::OnPaint();
		
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMFCDrawDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


HBRUSH CMFCDrawDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	// 安全获取句柄 先检查不为空
	if (pWnd == nullptr || !::IsWindow(pWnd->m_hWnd))
		return CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	if (pWnd->m_hWnd == GetDlgItem(IDC_IMAGE1)->m_hWnd) {
		pDC->SetBkColor(m_currentColor);      // 设置背景色
		return m_bkBrush;                // 返回新画刷
	}
	return CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
}

// 切换图形类型
void CMFCDrawDlg::OnShapeTypeChanged(UINT nID) {
	m_currentShapeType = nID - ID_SHAPE_RECT;
	m_paintView->SetCurrentShapeType(m_currentShapeType);
}

// 改变颜色
void CMFCDrawDlg::OnColorChanged() {
	CColorDialog dlg(m_currentColor);
	if (dlg.DoModal() == IDOK) {
		m_currentColor = dlg.GetColor();
		// 销毁旧画刷并创建新画刷
		if (m_bkBrush.GetSafeHandle()) {
			m_bkBrush.DeleteObject();
		}
		m_bkBrush.CreateSolidBrush(m_currentColor);
		m_paintView->SetCurrentColor(m_currentColor, true);
		m_colorSelect.Invalidate(FALSE);  
		m_colorSelect.UpdateWindow();    
	}
}


void CMFCDrawDlg::OnBnClickedBtnDraw2()
{
	// 检查视图有效性
	if (!m_paintView || !m_paintView->GetSafeHwnd())
		return;

	m_paintView->Invalidate(FALSE); // 触发重绘窗口
	m_paintView->UpdateWindow();
}


void CMFCDrawDlg::OnBnClickedBtnSave()
{
	CFileDialog dlg(FALSE, _T(".shape"), NULL,
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		_T("Shape Files (*.shape)|*.shape||"));

	if (dlg.DoModal() == IDOK) {
		CString path = dlg.GetPathName();
		m_paintView->m_shapeManager->SaveToFile(path);
	}
}


void CMFCDrawDlg::OnBnClickedBtnOpenfile()
{
	CFileDialog dlg(TRUE, _T(".shape"), NULL,
		OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST,
		_T("Shape Files (*.shape)|*.shape||"));

	if (dlg.DoModal() == IDOK) {
		CString path = dlg.GetPathName();
		m_paintView->m_shapeManager->LoadFromFile(path);
		if (m_paintView) {
			m_paintView->OnInitialUpdate();  // 自定义重置方法
		}
		//m_paintView->Invalidate(FALSE);
		m_paintView->UpdateWindow();
	}
}


void CMFCDrawDlg::OnBnClickedBtnUndo()
{
	auto record = m_paintView->GetHistory()->Pop();
	if (!record) return;

	switch (record->type) {
	case OpType::CHANGE_COLOR:
		// 回滚到上一个颜色
		m_currentColor = GetLastValidColor();
		m_paintView->SetCurrentColor(m_currentColor, false);
		if (m_bkBrush.GetSafeHandle()) {
			m_bkBrush.DeleteObject();
		}
		m_bkBrush.CreateSolidBrush(m_currentColor);
		m_colorSelect.Invalidate(FALSE);
		m_colorSelect.UpdateWindow();
		break;

	case OpType::ADD_SHAPE:
		// 删除最后添加的图形
		if (m_paintView->m_shapeManager->GetShapeCount() != 0) {
			m_paintView->m_shapeManager->DeleteShape();
		}
		break;
	}

	Invalidate(FALSE); // 触发重绘
}

COLORREF CMFCDrawDlg::GetLastValidColor() {
	for (const auto& rec : m_paintView->GetHistory()->GetRecords()) {
		if (rec->type == OpType::CHANGE_COLOR) {
			return rec->color;
		}
	}
	return RGB(255, 255, 255); // 默认颜色
}
