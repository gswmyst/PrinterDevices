// JobsDialog.cpp: 实现文件
//

#include "pch.h"
#include "PrinterDevices.h"
#include "JobsDialog.h"
#include "afxdialogex.h"


// JobsDialog 对话框

IMPLEMENT_DYNAMIC(JobsDialog, CDialogEx)

JobsDialog::JobsDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_JOBS, pParent)
{
}

JobsDialog::~JobsDialog()
{
}

void JobsDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ctlList);
}


BEGIN_MESSAGE_MAP(JobsDialog, CDialogEx)
END_MESSAGE_MAP()


// JobsDialog 消息处理程序


BOOL JobsDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	
	// TODO:  在此添加额外的初始化
	CString Field[] = { L"文档名",L"所有者",L"页数",L"大小",L"提交时间" };
	for (int j = 0; j < 5; j++)
	{
		m_ctlList.InsertColumn(j, Field[j],LVCFMT_LEFT,195);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}
