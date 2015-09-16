#pragma once


// CWarnDlg 对话框

class CWarnDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CWarnDlg)

public:
	CWarnDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CWarnDlg();

// 对话框数据
	enum { IDD = IDD_WarnDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
public:
	virtual BOOL OnInitDialog();

private:
	BOOL m_bCheckState;
};
