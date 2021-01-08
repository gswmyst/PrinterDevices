
// PrinterDevicesDlg.h: 头文件
//

#pragma once
#include <winspool.h>
#pragma comment(lib,"Winspool.lib")
#pragma comment(lib,"shell32.lib")
//#include <list>
#include "AddDialog.h"

// CPrinterDevicesDlg 对话框
class CPrinterDevicesDlg : public CDialogEx
{
// 构造
public:
	CPrinterDevicesDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PRINTERDEVICES_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;
	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_ctlList;
	DWORD dwSize, dwPrinters;//dwSize为PintInfo结构体数组需要的大小,dwPrinters为打印机数量
	PRINTER_INFO_2* pPrintInfo;//pPrinInfo数组存放打印机信息
	afx_msg void OnAddPrinter();
	void InsertPrinter(Printer pPrinter);
	afx_msg void OnAttribute();
	afx_msg void OnRclickList1(NMHDR *pNMHDR, LRESULT *pResult);
private:
	int nItem;
public:
	afx_msg void OnClickList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDelete();
	afx_msg void OnEnumJobs();
	afx_msg void OnProperties();
	afx_msg void OnSetDefault();
	afx_msg void OnDblclkList1(NMHDR *pNMHDR, LRESULT *pResult);
};
