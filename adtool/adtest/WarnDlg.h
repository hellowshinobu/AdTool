#pragma once


// CWarnDlg �Ի���

class CWarnDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CWarnDlg)

public:
	CWarnDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CWarnDlg();

// �Ի�������
	enum { IDD = IDD_WarnDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
public:
	virtual BOOL OnInitDialog();

private:
	BOOL m_bCheckState;
};
