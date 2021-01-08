#pragma once


// CAttributesDialog 对话框

class CAttributesDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CAttributesDialog)

public:
	CAttributesDialog(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CAttributesDialog();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ATTRIBUTE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_name;
	CString m_device;
	CString m_port;
	CString m_IP;
	CString m_type;
};
