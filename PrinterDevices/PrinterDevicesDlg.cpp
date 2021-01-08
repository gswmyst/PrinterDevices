
// PrinterDevicesDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "PrinterDevices.h"
#include "PrinterDevicesDlg.h"
#include "afxdialogex.h"
#include "CAttributesDialog.h"
#include "JobsDialog.h"
#include "Printer.h"
#include "PrinterList.h"

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


// CPrinterDevicesDlg 对话框



CPrinterDevicesDlg::CPrinterDevicesDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PRINTERDEVICES_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	//PrinterCounts = 0;
}

void CPrinterDevicesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ctlList);
}

BEGIN_MESSAGE_MAP(CPrinterDevicesDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SIZE()
	ON_COMMAND(ID_ADD_PRINTER, &CPrinterDevicesDlg::OnAddPrinter)
	ON_COMMAND(ID_32773, &CPrinterDevicesDlg::OnAttribute)
	ON_NOTIFY(NM_RCLICK, IDC_LIST1, &CPrinterDevicesDlg::OnRclickList1)
	ON_NOTIFY(NM_CLICK, IDC_LIST1, &CPrinterDevicesDlg::OnClickList1)
	ON_COMMAND(ID_32774, &CPrinterDevicesDlg::OnDelete)
	ON_COMMAND(ID_32775, &CPrinterDevicesDlg::OnEnumJobs)
	ON_COMMAND(ID_32776, &CPrinterDevicesDlg::OnProperties)
	ON_COMMAND(ID_32777, &CPrinterDevicesDlg::OnSetDefault)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &CPrinterDevicesDlg::OnDblclkList1)
END_MESSAGE_MAP()


// CPrinterDevicesDlg 消息处理程序

BOOL CPrinterDevicesDlg::OnInitDialog()
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

	int flag = EnumPrinters(PRINTER_ENUM_LOCAL, NULL, 2, NULL, 0, &dwSize, &dwPrinters);//获取dwSize大小
	pPrintInfo = (PRINTER_INFO_2*)malloc(dwSize);//给pPrintInfo数组分配内存空间
	EnumPrinters(PRINTER_ENUM_LOCAL, NULL, 2, (PBYTE)pPrintInfo, dwSize, &dwSize, &dwPrinters);//获取本地打印机设备
	if (dwPrinters != 0)//添加打印机设备和图标到list control
	{
		static CImageList ImageList;
		ImageList.Create(92, 59, ILC_COLOR32, 1, 1);
		CBitmap bm;
		bm.LoadBitmapW(IDB_BITMAP1);
		ImageList.Add(&bm, RGB(0, 0, 0));
		bm.DeleteObject();
		m_ctlList.SetImageList(&ImageList, LVSIL_NORMAL);
		for (int i = 0; i < dwPrinters; i++)
		{
			Printer printer(pPrintInfo[i].pPrinterName, pPrintInfo[i].pPortName, pPrintInfo[i].pDriverName, L"", 0);
			InsertPrinter(printer);
		}
	}
	free(pPrintInfo);
	pPrintInfo = nullptr;
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CPrinterDevicesDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CPrinterDevicesDlg::OnPaint()
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
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CPrinterDevicesDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}




void CPrinterDevicesDlg::OnAddPrinter()
{
	// TODO: 在此添加命令处理程序代码
	AddDialog* addDialog = new AddDialog();
	if (addDialog)
	{
		BOOL ret = addDialog->Create(IDD_ADD_DIALOG, this);
		CRect clientRect, dlgRect;
		GetClientRect(&clientRect);
		ClientToScreen(clientRect);
		addDialog->GetWindowRect(&dlgRect);
		addDialog->MoveWindow(clientRect.left + 30, clientRect.top + 50, dlgRect.Width(), dlgRect.Height());
		addDialog->ShowWindow(SW_SHOW);
		Invalidate(true);
	}
}




void CPrinterDevicesDlg::InsertPrinter(Printer printer)
{
	PrinterList& printerList = PrinterList::getInstance();
	printerList.push_back(printer);
	m_ctlList.InsertItem(printerList.printerCounts, printer.name);
}


void CPrinterDevicesDlg::OnAttribute()
{
	// TODO: 在此添加命令处理程序代码
	CAttributesDialog attrDlg;
	CString name = m_ctlList.GetItemText(nItem, 0);
	PrinterList& printerList = PrinterList::getInstance();
	Printer printer = printerList.findPrinterByName(name);
	attrDlg.m_device = printer.drivername;
	attrDlg.m_port = printer.port;
	attrDlg.m_IP = printer.ip;
	if (printer.type == 0)
		attrDlg.m_type = _T("本地打印机");
	else
		attrDlg.m_type = _T("远程打印机");
	attrDlg.m_name = name;
	attrDlg.DoModal();
}


void CPrinterDevicesDlg::OnRclickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	
	//获取鼠标坐标
	CPoint ptCurSel(0, 0);
	GetCursorPos(&ptCurSel);
	//转换为当前控件坐标系的坐标
	m_ctlList.ScreenToClient(&ptCurSel);
	LVHITTESTINFO HitTestInfo;
	HitTestInfo.pt = ptCurSel;
	//判断坐标命中第几项
	nItem = m_ctlList.HitTest(&HitTestInfo);
	CWnd* pWnd = AfxGetApp()->GetMainWnd();
	CMenu* pMenu = pWnd->GetMenu();
	pMenu->RemoveMenu(1, MF_BYPOSITION);
	if (nItem != -1)
	{
		pMenu->AppendMenuW(MF_STRING, ID_32774, L"删除打印机");
		DrawMenuBar();
		//弹出菜单
		CMenu menu;
		menu.LoadMenu(IDR_MENU2);
		CPoint point;
		GetCursorPos(&point);
		CMenu* pSubMenu = menu.GetSubMenu(0);
		pSubMenu->TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, this);
	}
	*pResult = 0;
}



void CPrinterDevicesDlg::OnClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	//获取鼠标坐标
	CPoint ptCurSel(0, 0);
	GetCursorPos(&ptCurSel);
	//转换为当前控件坐标系的坐标
	m_ctlList.ScreenToClient(&ptCurSel);
	LVHITTESTINFO HitTestInfo;
	HitTestInfo.pt = ptCurSel;
	//判断坐标命中第几项
	nItem = m_ctlList.HitTest(&HitTestInfo);
	CWnd* pWnd = AfxGetApp()->GetMainWnd();
	CMenu* pMenu = pWnd->GetMenu();
	pMenu->RemoveMenu(1, MF_BYPOSITION);
	if (nItem != -1)
	{
		pMenu->AppendMenuW(MF_STRING, ID_32774, L"删除打印机");
	}
	DrawMenuBar();
	*pResult = 0;
}


void CPrinterDevicesDlg::OnDelete()
{
	// TODO: 在此添加命令处理程序代码
	PrinterList& printerList = PrinterList::getInstance();
	printerList.deletePrinter(m_ctlList.GetItemText(nItem, 0));
	m_ctlList.DeleteAllItems();
	int i = 0;
	for (std::list<Printer>::const_iterator iter = printerList.printerList.begin(); iter != printerList.printerList.end(); iter++)
	{
		m_ctlList.InsertItem(i, iter->name);
		i++;
	}
	CWnd* pWnd = AfxGetApp()->GetMainWnd();
	CMenu* pMenu = pWnd->GetMenu();
	pMenu->RemoveMenu(1, MF_BYPOSITION);
	DrawMenuBar();
}




void CPrinterDevicesDlg::OnEnumJobs()
{
	CString name = m_ctlList.GetItemText(nItem, 0);
	HANDLE hPrinter;
	OpenPrinterW(name.GetBuffer(), &hPrinter, NULL);
	DWORD dwNeed, dwItems;
	BOOL bRet = EnumJobs(hPrinter, 0, -1, 2, NULL, 0, &dwNeed, &dwItems);
	JOB_INFO_2* pJob2 = (JOB_INFO_2*)malloc(dwNeed);
	bRet = EnumJobs(hPrinter, 0, -1, 2, (LPBYTE)pJob2, dwNeed, &dwNeed, &dwItems);

	if (bRet)
	{
		JobsDialog* jobsdialog=new JobsDialog();
		jobsdialog->Create(IDD_JOBS, this);
		for (int i = 0; i < dwItems; i++)
		{
			SYSTEMTIME t_UCT, t_Local;
			TIME_ZONE_INFORMATION timeinfo;
			CString strTime, strUser, strDocName, strPage, strSize;
			t_UCT = pJob2[i].Submitted;
			::GetTimeZoneInformation(&timeinfo);
			::SystemTimeToTzSpecificLocalTime(&timeinfo, &t_UCT, &t_Local);
			strTime.Format(_T("%d:%d:%d"), t_Local.wHour, t_Local.wMinute, t_Local.wSecond);
			strDocName = pJob2[i].pDocument;
			strUser = pJob2[i].pUserName;
			strPage.Format(L"%d", pJob2[i].TotalPages);
			if (pJob2[i].Size / 1024 > 1024)
			{
				strSize.Format(L"%dMB", pJob2[i].Size / 1024 / 1024);
			}
			else
			{
				strSize.Format(L"%dKB", pJob2[i].Size / 1024);
			}
			jobsdialog->m_ctlList.InsertItem(i, strDocName);
			jobsdialog->m_ctlList.SetItemText(i, 1, strUser);
			jobsdialog->m_ctlList.SetItemText(i, 2, strPage);
			jobsdialog->m_ctlList.SetItemText(i, 3, strSize);
			jobsdialog->m_ctlList.SetItemText(i, 4, strTime);
		}
		jobsdialog->ShowWindow(SW_SHOW);
	}
	free(pJob2);
}


void CPrinterDevicesDlg::OnProperties()
{
	// TODO: 在此添加命令处理程序代码
	CString name = m_ctlList.GetItemText(nItem, 0);
	HANDLE hPrinter;
	OpenPrinterW(name.GetBuffer(), &hPrinter, NULL);
	PrinterProperties(0, hPrinter);
}


void CPrinterDevicesDlg::OnSetDefault()
{
	// TODO: 在此添加命令处理程序代码
	CString name = m_ctlList.GetItemText(nItem, 0);
	if (SetDefaultPrinter(name))
	{
		AfxMessageBox(L"设置成功");
	}
}


void CPrinterDevicesDlg::OnDblclkList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	OnEnumJobs();
	*pResult = 0;
}
