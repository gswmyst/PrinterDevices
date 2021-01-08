// AddDialog.cpp: 实现文件
//

#include "pch.h"
#include "PrinterDevices.h"
#include "AddDialog.h"
#include "afxdialogex.h"
#include <WS2tcpip.h>
#include "UtilPrinterStatus.h"
#include "GlobalDef.h"
#include "UtilCommonFunction.h"
#include "PrinterDevicesDlg.h"
#include "PrinterList.h"
#define RECV_SIZE_MAX 65538
CImageList ImageList;
// AddDialog 对话框

IMPLEMENT_DYNAMIC(AddDialog, CDialogEx)

AddDialog::AddDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ADD_DIALOG, pParent)
	, m_strStaticIP(_T(""))
{
	crColor = RGB(255, 255, 255);
	m_BKBrush.CreateSolidBrush(crColor);
}

AddDialog::~AddDialog()
{
}

void AddDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ctlList);
	DDX_Text(pDX, IDC_STATIC_SEARCHIP, m_strStaticIP);
	DDX_Control(pDX, IDC_BUTTON3, m_ctlCancel);
	DDX_Control(pDX, IDC_BUTTON2, m_ctlAdd);
	DDX_Control(pDX, IDC_BUTTON1, m_ctlRefresh);
}


BEGIN_MESSAGE_MAP(AddDialog, CDialogEx)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON1, &AddDialog::OnRefresh)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON3, &AddDialog::OnCancel)
	ON_BN_CLICKED(IDC_BUTTON2, &AddDialog::OnAdd)
	ON_WM_DESTROY()
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &AddDialog::OnDblclkList1)
END_MESSAGE_MAP()


// AddDialog 消息处理程序

BOOL AddDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	ImageList.Create(92, 59, ILC_COLOR32, 1, 1);
	CBitmap bm;
	bm.LoadBitmapW(IDB_BITMAP1);
	ImageList.Add(&bm, RGB(0, 0, 0));
	bm.DeleteObject();
	m_ctlList.SetImageList(&ImageList, LVSIL_NORMAL);
	OnRefresh();//打开时搜索打印机
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


HBRUSH AddDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性
	if (nCtlColor == CTLCOLOR_DLG)
		return (HBRUSH)m_BKBrush.GetSafeHandle(); 
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}


bool AddDialog::CheckIfPrinter(CString strIPAddress)
{
	WSADATA wsData;
	::WSAStartup(MAKEWORD(2, 2), &wsData);
	StPrinterBasicInfo stInfo;
	PrinterList& printerList = PrinterList::getInstance();
	if (ConnectToPrinter(strIPAddress, 1, stInfo))
	{
		EnPrinterType type;
		if (GetPrinterType(stInfo.strModelName, type))
		{
			m_stPrinterInfo[m_iPrinterNum] = stInfo;
			Printer printer(stInfo.strModelName + _T("\n") + stInfo.strIPAddress, L"", L"", m_stPrinterInfo[m_iPrinterNum].strIPAddress, 0);
			bool flag = printerList.checkIfRepeat(printer);
			if (!flag)
			{
				m_ctlList.InsertItem(m_iPrinterNum, m_stPrinterInfo[m_iPrinterNum].strModelName + _T("\n") + m_stPrinterInfo[m_iPrinterNum].strIPAddress);
				m_iPrinterNum++;
			}
		}
		return true;
	}
	return false;
}


bool AddDialog::ConnectToPrinter(CString strIPAddress, int iSec, StPrinterBasicInfo &stInfo)
{
	struct  sockaddr_in to_addr;
	DWORD     scount, rcount;
	BYTE    sbuf[] = { 0xA5, 0x00, 0x0C, 0x50, 0xE0, 0x74, 0x08, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

	int     fromlen = sizeof(sockaddr_in);

	static const unsigned short port = 9300;

	if (strIPAddress.IsEmpty())
	{
		return false;
	}

	/* 必要なバッファサイズを取得 */
	int iBufferSize;
	iBufferSize = WideCharToMultiByte(CP_UTF8, 0, strIPAddress, -1, (char*)NULL, 0, NULL, NULL);

	/* バッファを取得 */
	//char* cpMultiByte = new char[iBufferSize];
	char cpMultiByte[64] = { 0 };
	/* マルチバイトに変換 */
	iBufferSize = WideCharToMultiByte(CP_UTF8, 0, strIPAddress, -1, cpMultiByte, iBufferSize, NULL, NULL);

	const char* IPAddress = cpMultiByte;

	DWORD errCode = static_cast<DWORD>(ERROR_SUCCESS);
	SOCKET sockfd;

	/* 接続するDeviceのアドレス・ポート番号を設定 */
	ZeroMemory((char *)&to_addr, sizeof(to_addr));
	to_addr.sin_family = AF_INET;
	//to_addr.sin_addr.s_addr = inet_addr(IPAddress);
	inet_pton(AF_INET, IPAddress, &(to_addr.sin_addr));
	to_addr.sin_port = htons(port);

	/* ソケットの生成 */
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sockfd == INVALID_SOCKET)
	{
		return false;
	}

	/* 送信 */
	scount = sendto(sockfd, (char*)sbuf, sizeof(sbuf), 0, (struct sockaddr *)&to_addr, sizeof(to_addr));
	if (scount != sizeof(sbuf))
	{
		errCode = WSAGetLastError();
		closesocket(sockfd);
		return false;
	}

	fd_set read_fd;
	FD_ZERO(&read_fd);
	FD_SET(sockfd, &read_fd);
	struct timeval timeout;
	timeout.tv_sec = iSec;
	timeout.tv_usec = 0;
	rcount = select((int)sockfd, &read_fd, NULL, NULL, &timeout);

	BYTE pRecvBuf[RECV_SIZE_MAX] = { 0 };
	if (rcount < 0 || !(FD_ISSET(sockfd, &read_fd)))
	{
		errCode = WSAGetLastError();
		closesocket(sockfd);
		return false;
	}
	else
	{
		/* 受信 */
		rcount = recvfrom(sockfd, (char*)pRecvBuf, RECV_SIZE_MAX, 0, (struct sockaddr *)&to_addr, &fromlen);
		if (rcount < 8)
		{
			errCode = WSAGetLastError();
			closesocket(sockfd);
			return false;
		}
		//WriteBinaryLog((char*)pRecvBuf, _T(".\\test\\") + strIPAddress + _T(".dat"), sizeof(BYTE) * rcount);
		if (pRecvBuf[0] == 0xA5 && pRecvBuf[4] == 0xE0 && pRecvBuf[5] == 0x74)
		{

			CUtilPrinterStatus cPrinterStatus;
			cPrinterStatus.parseRecvBuf((BYTE*)pRecvBuf, rcount);

			stInfo.strIPAddress = strIPAddress;
			stInfo.strModelName = cPrinterStatus.m_pVenderNameValue;
			stInfo.strPrinterSerialNumber = cPrinterStatus.m_pSerialNumberValue;
			stInfo.strFWVersion = cPrinterStatus.m_pFWVersion;
			stInfo.dwRemainingToner = cPrinterStatus.m_dwRemainingToner;
			stInfo.dwNumberOfPrint = cPrinterStatus.m_dwNumberOfPrint;
			closesocket(sockfd);
			return true;
		}
	}
	closesocket(sockfd);
	WSACleanup();
	return false;
}


void AddDialog::OnRefresh()
{
	// TODO: 在此添加控件通知处理程序代码
	m_ctlCancel.EnableWindow(false);
	m_ctlAdd.EnableWindow(false);
	m_ctlRefresh.EnableWindow(false);
	m_ctlList.DeleteAllItems();
	m_SearchThreadIndex = 0;
	m_iPrinterNum = 0;
	SearchLocalPrinter();
	m_ThreadCount = 0;
	WSADATA wsData;
	::WSAStartup(MAKEWORD(2, 2), &wsData);
	char hostname[100] = { 0 };
	gethostname(hostname, sizeof(hostname));
	addrinfo hints, *res, *curr;
	struct sockaddr_in *sa;
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_flags = AI_CANONNAME;
	hints.ai_family = AF_INET;
	getaddrinfo(hostname, NULL, &hints, &res);
	curr = res;
	sa = (struct sockaddr_in *)curr->ai_addr;
	char *ip = (char*)inet_ntop(AF_INET, &sa->sin_addr.s_addr, hostname, sizeof(hostname));
	freeaddrinfo(res);
	WSACleanup();
	int iplen = strlen(ip);
	int count = 0;
	int i;
	for (i = 0; i < iplen; i++)
	{
		if (ip[i] == '.')
			count++;
		if (count == 3)
			break;
	}
	ip1 = CA2CT(ip);
	ip1 = ip1.Left(i + 1);
	DWORD dwThreadID[15];
	m_strStaticIP = _T("正在搜索局域网中的打印机");
	UpdateData(false);
	SetTimer(1,1000,NULL);
	for (int i = 0; i < 15; i++)
	{
		dwThreadID[i] = 0;
		CloseHandle(CreateThread(NULL, 0, CallSearchPrinterThread, this, 0, &dwThreadID[i]));
	}
}

DWORD AddDialog::CallSearchPrinterThread(LPVOID lpParameter)
{
	return ((AddDialog *)lpParameter)->SearchPrinterThread();
}

DWORD AddDialog::SearchPrinterThread()
{
	int index = m_SearchThreadIndex++;
	m_ThreadCount++;
	CString ip2;
	for (int i = index * 17 + 1; i < (index + 1) * 17; i++)
	{
		ip2.Format(L"%d", i);
		ip2 = ip1 + ip2;
		CheckIfPrinter(ip2);
	}
	m_ThreadCount--;
	return 0;
}

void AddDialog::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (0 == m_ThreadCount)
	{
		m_strStaticIP = _T("搜索完成");
		m_ctlCancel.EnableWindow(true);
		m_ctlAdd.EnableWindow(true);
		m_ctlRefresh.EnableWindow(true);
		UpdateData(false);
		KillTimer(1);
	}
	CDialogEx::OnTimer(nIDEvent);
}


void AddDialog::OnCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	ImageList.DeleteImageList();
	DestroyWindow();
}

void AddDialog::OnAdd()
{
	// TODO: 在此添加控件通知处理程序代码
	POSITION ps;
	int nIndex;
	ps = m_ctlList.GetFirstSelectedItemPosition();
	nIndex = m_ctlList.GetNextSelectedItem(ps);
	PrinterList& printerList = PrinterList::getInstance();
	if (-1 != nIndex && 0 == m_ThreadCount) 
	{
		CString str = m_ctlList.GetItemText(nIndex,0);
		int n = str.Find(L"\n");
		CPrinterDevicesDlg* cWnd = (CPrinterDevicesDlg*)this->GetParent();
		if (-1 == n)
		{
			for (int i = 0; i < m_iPrinterNum; i++)
			{
				if (str.Compare(PrinterArray[i].name) == 0)
				{
					cWnd->InsertPrinter(PrinterArray[i]);
					break;
				}
			}
		}
		else
		{
			CString printerip = str.Mid(n + 1);
			Printer printer(str, L"", L"", printerip, 1);
			cWnd->InsertPrinter(printer);
		}
		ImageList.DeleteImageList();
		DestroyWindow();
	}	
}


void AddDialog::OnDestroy()
{
	CDialogEx::OnDestroy();
	delete this;
	// TODO: 在此处添加消息处理程序代码
}


void AddDialog::OnDblclkList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	OnAdd();
	*pResult = 0;
}


void AddDialog::SearchLocalPrinter()
{
	// TODO: 在此处添加实现代码.
	int flag = EnumPrinters(PRINTER_ENUM_LOCAL, NULL, 2, NULL, 0, &dwSize, &dwPrinters);//获取dwSize大小
	pPrintInfo = (PRINTER_INFO_2*)malloc(dwSize);//给pPrintInfo数组分配内存空间
	EnumPrinters(PRINTER_ENUM_LOCAL, NULL, 2, (PBYTE)pPrintInfo, dwSize, &dwSize, &dwPrinters);//获取本地打印机设备
	PrinterList& printerList = PrinterList::getInstance();
	if (dwPrinters != 0)//添加打印机设备到list control
	{
		for (int i = 0; i < dwPrinters; i++)
		{
			Printer printer(pPrintInfo[i].pPrinterName, pPrintInfo[i].pPortName, pPrintInfo[i].pDriverName, L"", 0);
			if (!printerList.checkIfRepeat(printer))
			{
				m_ctlList.InsertItem(m_iPrinterNum, printer.name);
				PrinterArray[m_iPrinterNum] = printer;
				m_iPrinterNum++;
			}
		}
	}
	free(pPrintInfo);
	pPrintInfo = nullptr;
}
