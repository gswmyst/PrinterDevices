#pragma once
#include <winnetwk.h>
#include <winspool.h>
#pragma comment(lib,"Winspool.lib")
#pragma comment(lib,"shell32.lib")
#pragma comment (lib,"Mpr.lib")
#pragma comment (lib,"Ws2_32.lib")
#include "Printer.h"
#define WM_ADDPRINTER WM_USER+100

// AddDialog 对话框

typedef struct stPrinterBasicInfo
{
	CString strPrinterSerialNumber;
	CString strModelName;
	CString strIPAddress;
	CString strFWVersion;
	DWORD dwNumberOfPrint;
	DWORD dwRemainingToner;
}StPrinterBasicInfo;

class AddDialog : public CDialogEx
{
	DECLARE_DYNAMIC(AddDialog)

public:
	AddDialog(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~AddDialog();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ADD_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	COLORREF crColor;
	CBrush m_BKBrush;
	int m_iPrinterNum;
	StPrinterBasicInfo m_stPrinterInfo[256];
	CString ip1;
	int m_SearchThreadIndex;
	int m_ThreadCount;
	DECLARE_MESSAGE_MAP()
public:
	DWORD dwSize, dwPrinters;//dwSize为PintInfo结构体数组需要的大小,dwPrinters为打印机数量
	PRINTER_INFO_2* pPrintInfo;//pPrinInfo数组存放打印机信息
	CListCtrl m_ctlList;
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	bool CheckIfPrinter(CString strIPAddress);
	bool ConnectToPrinter(CString strIPAddress, int iSec, StPrinterBasicInfo &stInfo);
	afx_msg void OnRefresh();
	CString m_strStaticIP;
	DWORD Info[15];
	DWORD SearchPrinterThread();
	static DWORD WINAPI CallSearchPrinterThread(LPVOID lpParameter);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnCancel();
	CButton m_ctlCancel;
	afx_msg void OnAdd();
	CButton m_ctlAdd;
	CButton m_ctlRefresh;
	afx_msg void OnDestroy();
	afx_msg void OnDblclkList1(NMHDR *pNMHDR, LRESULT *pResult);
	void SearchLocalPrinter();
	Printer PrinterArray[100];
};
