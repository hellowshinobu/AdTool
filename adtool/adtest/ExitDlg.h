#pragma once


// CExitDlg 对话框

class CExitDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CExitDlg)

public:
	CExitDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CExitDlg();

// 对话框数据
	enum { IDD = IDD_EXITDIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
};
