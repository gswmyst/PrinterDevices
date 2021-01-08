// CAttributesDialog.cpp: 实现文件
//

#include "pch.h"
#include "PrinterDevices.h"
#include "CAttributesDialog.h"
#include "afxdialogex.h"


// CAttributesDialog 对话框

IMPLEMENT_DYNAMIC(CAttributesDialog, CDialogEx)

CAttributesDialog::CAttributesDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ATTRIBUTE, pParent)
	, m_name(_T(""))
	, m_port(_T(""))
	, m_IP(_T(""))
	, m_type(_T(""))
{

}

CAttributesDialog::~CAttributesDialog()
{
}

void CAttributesDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STATIC_NAME, m_name);
	DDX_Text(pDX, IDC_STATIC_DEVICE, m_device);
	DDX_Text(pDX, IDC_STATIC_PORT, m_port);
	DDX_Text(pDX, IDC_STATIC_IP, m_IP);
	DDX_Text(pDX, IDC_STATIC_TYPE, m_type);
}


BEGIN_MESSAGE_MAP(CAttributesDialog, CDialogEx)
END_MESSAGE_MAP()


// CAttributesDialog 消息处理程序
