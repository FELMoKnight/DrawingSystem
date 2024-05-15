#include "pch.h"
#include "framework.h"
#include "DrawingSystem.h"
#include "DrawingSystemDlg.h"
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



// CDrawingSystemDlg 对话框


CDrawingSystemDlg::CDrawingSystemDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DRAWINGSYSTEM_DIALOG, pParent)
	, LineWidth(0)
	, m_LineType(0)
	, m_text(_T(""))
	, m_bTextPromptShown(false)
{
	BackgroundColor = RGB(255, 255, 255);
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	Mode = LineMode; // 默认模式
	startdrawing = false;
}

void CDrawingSystemDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, LineWidth);
	DDV_MinMaxInt(pDX, LineWidth, 0, 20);
	DDX_Radio(pDX, IDC_RADIO1, m_LineType);
	DDX_Control(pDX, IDC_COMBO1, m_fill);
	DDX_Control(pDX, IDC_COMBO2, m_Mode);
	DDX_Text(pDX, IDC_EDIT2, m_text);
}

BEGIN_MESSAGE_MAP(CDrawingSystemDlg, CDialogEx)
	ON_WM_CTLCOLOR()
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CDrawingSystemDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CDrawingSystemDlg::OnBnClickedButton2)
	ON_EN_CHANGE(IDC_EDIT1, &CDrawingSystemDlg::OnEnChangeEdit1)
	ON_BN_CLICKED(IDC_RADIO1, &CDrawingSystemDlg::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &CDrawingSystemDlg::OnBnClickedRadio2)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CDrawingSystemDlg::OnCbnSelchangeCombo1)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_CBN_SELCHANGE(IDC_COMBO2, &CDrawingSystemDlg::OnCbnSelchangeCombo2)
	ON_BN_CLICKED(IDC_BUTTON3, &CDrawingSystemDlg::OnBnClickedButton3)
	ON_WM_MOUSEMOVE()
	ON_BN_CLICKED(IDC_BUTTON4, &CDrawingSystemDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CDrawingSystemDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, &CDrawingSystemDlg::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON_FILL_BACKGROUND, &CDrawingSystemDlg::OnBnClickedButtonFillBackground)
END_MESSAGE_MAP()


// CDrawingSystemDlg 消息处理程序

BOOL CDrawingSystemDlg::OnInitDialog()
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

	LineColor = RGB(0, 0, 255);
	ShapeColor = RGB(255, 0, 0);

	LineWidth = 5;

	m_fill.AddString(TEXT("有填充"));
	m_fill.AddString(TEXT("无填充"));
	m_fill.SetCurSel(1);

	m_Mode.AddString(TEXT("画点"));
	m_Mode.AddString(TEXT("画线"));
	m_Mode.AddString(TEXT("画椭圆"));
	m_Mode.AddString(TEXT("画矩形"));
	m_Mode.AddString(TEXT("自由绘制"));
	m_Mode.SetCurSel(1);

	UpdateData(FALSE);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CDrawingSystemDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

HBRUSH CDrawingSystemDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	if (nCtlColor == CTLCOLOR_STATIC)
	{
		pDC->SetBkMode(OPAQUE); // 设置背景不透明
		pDC->SetBkColor(RGB(255, 255, 255)); // 设置背景颜色为白色
		return (HBRUSH)GetStockObject(WHITE_BRUSH); // 返回白色刷子
	}

	return hbr;
}



void CDrawingSystemDlg::OnPaint()
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
		CPaintDC dc(this); // 用于绘制的设备上下文

		if (Mode == FillBackgroundMode)
		{
			CRect rect;
			GetClientRect(&rect);
			CBrush brush(BackgroundColor); // 使用选择的背景颜色
			dc.FillRect(&rect, &brush);
			RedrawControls();
		}
		else if (startdrawing)
		{
			CPen pen(LineType, LineWidth, LineColor);
			CPen* pOldPen = dc.SelectObject(&pen);
			CBrush* pOldBrush = nullptr;

			if (Mode == EllipseMode || Mode == RectangleMode)
			{
				CBrush transparentBrush;
				transparentBrush.CreateStockObject(NULL_BRUSH);
				pOldBrush = dc.SelectObject(&transparentBrush);
			}

			if (Mode == LineMode)
			{
				dc.MoveTo(DownPoint.x, DownPoint.y);
				dc.LineTo(lastpoint.x, lastpoint.y);
			}
			else if (Mode == EllipseMode)
			{
				dc.Ellipse(DownPoint.x, DownPoint.y, lastpoint.x, lastpoint.y);
			}
			else if (Mode == RectangleMode)
			{
				dc.Rectangle(DownPoint.x, DownPoint.y, lastpoint.x, lastpoint.y);
			}
			else if (Mode == TextMode)
			{
				CClientDC dc(this); // 用于绘制的设备上下文
				dc.TextOut(DownPoint.x, DownPoint.y, m_text);
				OnCbnSelchangeCombo2();	//切换回原来的模式
			}
			dc.SelectObject(pOldPen);
			if (Mode == EllipseMode || Mode == RectangleMode)
			{
				dc.SelectObject(pOldBrush);
			}
			
		}
		for (const auto& text : m_texts)
		{
			dc.TextOut(text.position.x, text.position.y, text.text);
		}

		if (!m_ImagePath.IsEmpty())
		{
			CImage image;
			if (image.Load(m_ImagePath) == S_OK)
			{
				// 根据需要调整插入图片的位置
				int imageX = 200; // 例如，设置图片左上角的 x 坐标
				int imageY = 200; // 例如，设置图片左上角的 y 坐标
				image.BitBlt(dc, imageX, imageY, image.GetWidth(), image.GetHeight(), 0, 0);
			}
		}
	}
}


void CDrawingSystemDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	if (startdrawing)
	{
		CClientDC dc(this); // 用于绘制的设备上下文
		CRect rect;
		GetClientRect(&rect);
		dc.SaveDC();
		dc.SetROP2(R2_NOTXORPEN);

		CPen pen(LineType, LineWidth, LineColor);
		CPen* pOldPen = dc.SelectObject(&pen);
		CBrush* pOldBrush = nullptr;

		if (Mode == EllipseMode || Mode == RectangleMode)
		{
			CBrush transparentBrush;
			transparentBrush.CreateStockObject(NULL_BRUSH);
			pOldBrush = dc.SelectObject(&transparentBrush);
		}

		// 擦除上次绘制的图形
		if (Mode == LineMode)
		{
			dc.MoveTo(DownPoint.x, DownPoint.y);
			dc.LineTo(lastpoint.x, lastpoint.y);
		}
		else if (Mode == EllipseMode)
		{
			dc.Ellipse(DownPoint.x, DownPoint.y, lastpoint.x, lastpoint.y);
		}
		else if (Mode == RectangleMode)
		{
			dc.Rectangle(DownPoint.x, DownPoint.y, lastpoint.x, lastpoint.y);
		}

		// 绘制新的图形
		if (Mode == LineMode)
		{
			dc.MoveTo(DownPoint.x, DownPoint.y);
			dc.LineTo(point.x, point.y);
		}
		else if (Mode == EllipseMode)
		{
			dc.Ellipse(DownPoint.x, DownPoint.y, point.x, point.y);
		}
		else if (Mode == RectangleMode)
		{
			dc.Rectangle(DownPoint.x, DownPoint.y, point.x, point.y);
		}
		else if (Mode == OwnerDrawingMode)
		{
			//point：当前时刻的点（直线的终点）
			//lastpoint：上一时刻的点（直线的起点）

			CClientDC dc(this); // 用于绘制的设备上下文

			// 创建红色的画笔
			CPen pen(LineType, LineWidth, LineColor);

			// 选择画笔到设备上下文
			CPen* pOldPen = dc.SelectObject(&pen);

			// 绘制直线
			dc.MoveTo(lastpoint.x, lastpoint.y); // 起始点坐标
			dc.LineTo(point.x, point.y); // 终点坐标

			// 恢复原始的画笔
			dc.SelectObject(pOldPen);

			lastpoint = point;	//画完以后，当前时刻的起点就会变为下一时刻的终点
		}

		dc.SelectObject(pOldPen);
		if (Mode == EllipseMode || Mode == RectangleMode)
		{
			dc.SelectObject(pOldBrush);
		}

		dc.RestoreDC(-1);

		lastpoint = point; // 更新最后一个点
	}
	CDialogEx::OnMouseMove(nFlags, point);
}



//当用户拖动最小化窗口时系统调用此函数取得光标显示。
HCURSOR CDrawingSystemDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CDrawingSystemDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	
	CColorDialog colorDlg(LineColor);         // 构造对话框，传入初始颜色值   
	if (IDOK == colorDlg.DoModal()) // 显示对话框并判断是否点击“确定”   
	{
		LineColor = colorDlg.GetColor();    // 获取颜色对话框中选择的颜色值   

	}

}


void CDrawingSystemDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	CColorDialog colorDlg(ShapeColor);         // 构造对话框，传入初始颜色值   
	if (IDOK == colorDlg.DoModal()) // 显示对话框并判断是否点击“确定”   
	{
		ShapeColor = colorDlg.GetColor();    // 获取颜色对话框中选择的颜色值   

	}
}


void CDrawingSystemDlg::OnEnChangeEdit1()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);
	//CString str;
	//str.Format(TEXT("%d"), LineWidth);
	//MessageBox(str);
}


void CDrawingSystemDlg::OnBnClickedRadio1()
{
	// TODO: 在此添加控件通知处理程序代码
	LineType = false;
}


void CDrawingSystemDlg::OnBnClickedRadio2()
{
	// TODO: 在此添加控件通知处理程序代码
	LineType = true;
}


void CDrawingSystemDlg::OnCbnSelchangeCombo1()
{
	// TODO: 在此添加控件通知处理程序代码
	//判断当前选择的选项
	int index = m_fill.GetCurSel();
	//当下拉框选择改变时，改变填充值
	if(index == 0) isfilled = true;
	else isfilled = false;

	//CString str;
	//str.Format(TEXT("%d"), index);
	//MessageBox(str);
}

void CDrawingSystemDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	DownPoint = point;
	lastpoint = point;
	startdrawing = true; 
	m_ImagePath.Empty();
	CDialogEx::OnLButtonDown(nFlags, point);
}


void CDrawingSystemDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (startdrawing)
	{
		UpPoint = point;
		startdrawing = false; // 结束绘图标志

		// 最终绘制
		if (Mode == LineMode)
		{
			DrawLine();
		}
		else if (Mode == EllipseMode)
		{
			DrawEllipse();
		}
		else if (Mode == RectangleMode)
		{
			DrawRectangle();
		}
	}
	if (Mode == TextMode)
	{
		TextInfo info;
		info.position = point;
		info.text = m_text;
		m_texts.push_back(info);
		m_text.Empty();
		Invalidate(); // 触发重绘以显示新添加的文本
	}
	CDialogEx::OnLButtonUp(nFlags, point);
}


void CDrawingSystemDlg::DrawLine()
{
	CClientDC dc(this); // 用于绘制的设备上下文
	
	// 创建红色的画笔
	CPen pen(LineType, LineWidth, LineColor);

	// 选择画笔到设备上下文
	CPen* pOldPen = dc.SelectObject(&pen);

	// 绘制直线
	dc.MoveTo(DownPoint.x, DownPoint.y); // 起始点坐标
	dc.LineTo(UpPoint.x, UpPoint.y); // 终点坐标

	// 恢复原始的画笔
	dc.SelectObject(pOldPen);
}

void CDrawingSystemDlg::DrawEllipse()
{
	CClientDC dc(this); // 用于绘制的设备上下文

	// 创建红色的轮廓画笔
	CPen outlinePen(LineType, LineWidth, LineColor);

	// 选择轮廓画笔到设备上下文
	CPen* pOldPen = dc.SelectObject(&outlinePen);
	CBrush* pOldBrush;

	// 创建蓝色的填充画刷
	CBrush fillBrush(ShapeColor);
	// 创建透明的画刷
	CBrush transparentBrush;
	transparentBrush.CreateStockObject(NULL_BRUSH);

	if (isfilled)
	{
		// 选择填充画刷到设备上下文
		pOldBrush = dc.SelectObject(&fillBrush);
	}
	else
	{
		// 选择透明画刷到设备上下文
		pOldBrush = dc.SelectObject(&transparentBrush);
	}


	// 绘制椭圆，左上角和右下角的矩形坐标确定椭圆的边界
	dc.Ellipse(DownPoint.x, DownPoint.y, UpPoint.x, UpPoint.y);

	// 恢复原始的画笔和画刷
	dc.SelectObject(pOldPen);
	dc.SelectObject(pOldBrush);
}

void CDrawingSystemDlg::OnCbnSelchangeCombo2()
{
	// TODO: 在此添加控件通知处理程序代码
	int index = m_Mode.GetCurSel();

	if (index == 0) Mode = PointMode;
	else if (index == 1) Mode = LineMode;
	else if (index == 2) Mode = EllipseMode;
	else if (index == 3) Mode = RectangleMode;
	else if (index == 4) Mode = OwnerDrawingMode;
	else if (index == 5) Mode = FillBackgroundMode;
	else Mode = LineMode;
}

void CDrawingSystemDlg::DrawRectangle()
{
	CClientDC dc(this); // 用于绘制的设备上下文

	// 创建红色的轮廓画笔
	CPen outlinePen(LineType, LineWidth, LineColor);

	// 选择轮廓画笔到设备上下文
	CPen* pOldPen = dc.SelectObject(&outlinePen);
	CBrush* pOldBrush;

	// 创建蓝色的填充画刷
	CBrush fillBrush(ShapeColor);
	// 创建透明的画刷
	CBrush transparentBrush;
	transparentBrush.CreateStockObject(NULL_BRUSH);

	if (isfilled)
	{
		// 选择填充画刷到设备上下文
		pOldBrush = dc.SelectObject(&fillBrush);
	}
	else
	{
		// 选择透明画刷到设备上下文
		pOldBrush = dc.SelectObject(&transparentBrush);
	}


	// 绘制矩形
	dc.Rectangle(DownPoint.x, DownPoint.y, UpPoint.x, UpPoint.y);

	// 恢复原始的画笔和画刷
	dc.SelectObject(pOldPen);
	dc.SelectObject(pOldBrush);
}



void CDrawingSystemDlg::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	BackgroundColor = RGB(255, 255, 255);
	m_texts.clear();
	m_ImagePath.Empty();
	Invalidate();
}


void CDrawingSystemDlg::OnBnClickedButton4()
{
	// TODO: 在此添加控件通知处理程序代码
	Mode = TextMode;
	UpdateData(TRUE);
	if (!m_bTextPromptShown) {
        MessageBox(TEXT("请左键点击对话框选择插入文字的位置"), TEXT("提示"));
        m_bTextPromptShown = true; // 更新标志变量，避免重复提示
    }
}


void CDrawingSystemDlg::OnBnClickedButton5()
{
	// TODO: 在此添加控件通知处理程序代码
	
	Mode = ImageMode;

	// TODO: Add your control notification handler code here   
	CFileDialog fileDlg(TRUE, _T("png"), NULL, 0, _T("图片文件 (*.png;*.jpg;*.gif;*.bmp)|*.png;*.jpg;*.gif;*.bmp||"), this);

	if (IDOK == fileDlg.DoModal())
	{
		m_ImagePath = fileDlg.GetPathName();
		Mode = ImageMode;
		Invalidate(); // 重绘窗口以显示选择的图片
	}

}


void CDrawingSystemDlg::OnBnClickedButton6()
{
	// 指定保存的区域大小
	CRect rectToSave(0, 0, 850, 850); // 示例：左上角坐标为(100, 100)，右下角坐标为(300, 300)

	// 创建兼容位图和设备上下文
	CClientDC dc(this);
	CBitmap bmp;
	bmp.CreateCompatibleBitmap(&dc, rectToSave.Width(), rectToSave.Height());
	CDC memDC;
	memDC.CreateCompatibleDC(&dc);
	CBitmap* pOldBmp = memDC.SelectObject(&bmp);

	// 将指定区域的绘图内容复制到兼容位图中
	memDC.BitBlt(0, 0, rectToSave.Width(), rectToSave.Height(), &dc, rectToSave.left, rectToSave.top, SRCCOPY);

	// 保存兼容位图为图片文件
	CImage image;
	image.Attach((HBITMAP)bmp.Detach());

	CString strFilter = _T("位图文件(*.bmp)|*.bmp|JPEG 图像文件|*.jpg|GIF图像文件 | *.gif | PNG图像文件 | *.png |其他格式(*.*) | *.* || ");
	CFileDialog dlg(FALSE, _T("bmp"), _T("iPaint1.bmp"), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, strFilter);
	if (dlg.DoModal() != IDOK)
		return;

	CString strFileName = dlg.GetPathName();
	HRESULT hResult = image.Save(strFileName); // 保存图像
	if (FAILED(hResult))
	{
		MessageBox(_T("保存图像文件失败！"));
	}
	else
	{
		MessageBox(_T("文件保存成功！"));
	}

	memDC.SelectObject(pOldBmp);
}




void CDrawingSystemDlg::OnBnClickedButtonFillBackground()
{
	CColorDialog colorDlg(BackgroundColor); // 选择背景颜色

	if (IDOK == colorDlg.DoModal()) {
		Mode = FillBackgroundMode;
		BackgroundColor = colorDlg.GetColor(); // 获取选择的背景颜色
		Invalidate(); // 触发重绘以应用新的背景颜色
		UpdateWindow(); // 确保立即重绘
		RedrawControls(); // 重绘所有控件
	}
}

void CDrawingSystemDlg::RedrawControls()
{
	CWnd* pChildWnd = GetWindow(GW_CHILD);
	while (pChildWnd != nullptr)
	{
		pChildWnd->Invalidate(); // 使控件无效
		pChildWnd->UpdateWindow(); // 确保控件立即重绘
		pChildWnd = pChildWnd->GetNextWindow();
	}
}

