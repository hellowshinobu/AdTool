#pragma once


// CSettingDlg �Ի���

class CSettingDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSettingDlg)

public:
	CSettingDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSettingDlg();

	BOOL GetAuto() const;
	BOOL GetAdd() const;
	void SetAuto(BOOL bAuto);
	void SetAdd(BOOL bAdd);

// �Ի�������
	enum { IDD = IDD_SETTINGDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);


private:
	BOOL m_bAuto;
	BOOL m_bAdd;
};
