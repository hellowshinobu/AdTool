// WarnDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "adtest.h"
#include "WarnDlg.h"
#include "afxdialogex.h"


// CWarnDlg 对话框

IMPLEMENT_DYNAMIC(CWarnDlg, CDialogEx)

CWarnDlg::CWarnDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CWarnDlg::IDD, pParent)
{
	m_bCheckState = TRUE;
}

CWarnDlg::~CWarnDlg()
{
}

void CWarnDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CWarnDlg, CDialogEx)
END_MESSAGE_MAP()


// CWarnDlg 消息处理程序


BOOL CWarnDlg::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// TODO: 在此添加专用代码和/或调用基类
	UINT nID = LOWORD(wParam);  
	HWND hWndCtrl = (HWND)lParam;  
	int nCode = HIWORD(wParam);

	switch(nID)
	{
	case WARN_BTN_CHECK:
		{
			CButton *checkBtn = (CButton *)(GetDlgItem(WARN_BTN_CHECK));
			if (checkBtn->GetCheck())
			{
				checkBtn->SetCheck(1);
				m_bCheckState = TRUE;
			}
			else
			{
				checkBtn->SetCheck(0);
				m_bCheckState = FALSE;
			}

			checkBtn->SetCheck(m_bCheckState);
		}
		break;
	case WARN_BTN_OK:
		{
			g_bCheckState = m_bCheckState;
			OnOK();
		}
		break;
	default:
		break;
	}

	return CDialogEx::OnCommand(wParam, lParam);
}


BOOL CWarnDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	SetWindowPos(&wndTop,0,0,300,140,SWP_SHOWWINDOW); 
	CenterWindow();

	if (::IsWindow((GetDlgItem(WARN_STATIC_DESC))->GetSafeHwnd()))
	{
		(GetDlgItem(WARN_STATIC_DESC))->MoveWindow(20,10,260,40);
		(GetDlgItem(WARN_STATIC_DESC))->SetWindowText(L"该窗口是浏览器或系统窗口，\n请不要屏蔽它！");
	}

	if (::IsWindow((GetDlgItem(WARN_BTN_CHECK))->GetSafeHwnd()))
	{
		(GetDlgItem(WARN_BTN_CHECK))->MoveWindow(20,80,150,20);
		((CButton *)(GetDlgItem(WARN_BTN_CHECK)))->SetCheck(m_bCheckState);
	}

	if (::IsWindow((GetDlgItem(WARN_BTN_OK))->GetSafeHwnd()))
	{
		(GetDlgItem(WARN_BTN_OK))->MoveWindow(200,80,60,20);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
