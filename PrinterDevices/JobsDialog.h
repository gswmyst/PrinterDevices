#pragma once


// JobsDialog 对话框

class JobsDialog : public CDialogEx
{
	DECLARE_DYNAMIC(JobsDialog)

public:
	JobsDialog(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~JobsDialog();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_JOBS };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_ctlList;
	virtual BOOL OnInitDialog();
};
