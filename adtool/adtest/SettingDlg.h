#pragma once


// CSettingDlg 对话框

class CSettingDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSettingDlg)

public:
	CSettingDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSettingDlg();

	BOOL GetAuto() const;
	BOOL GetAdd() const;
	void SetAuto(BOOL bAuto);
	void SetAdd(BOOL bAdd);

// 对话框数据
	enum { IDD = IDD_SETTINGDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);


private:
	BOOL m_bAuto;
	BOOL m_bAdd;
};
