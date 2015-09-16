// ExitDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "adtest.h"
#include "ExitDlg.h"
#include "afxdialogex.h"


// CExitDlg 对话框

IMPLEMENT_DYNAMIC(CExitDlg, CDialogEx)

CExitDlg::CExitDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CExitDlg::IDD, pParent)
{

}

CExitDlg::~CExitDlg()
{
}

void CExitDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CExitDlg, CDialogEx)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CExitDlg 消息处理程序


void CExitDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
}


BOOL CExitDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	SetWindowPos(&CWnd::wndTopMost,0,0,350,150,SWP_NOSIZE|SWP_NOMOVE); 
	BringWindowToTop();
	CenterWindow();
	//BringWindowToTop();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


BOOL CExitDlg::OnCommand(WPARAM wParam, LPARAM lParam)
{
	UINT nID = LOWORD(wParam);  
	HWND hWndCtrl = (HWND)lParam;  
	int nCode = HIWORD(wParam);

	switch( nID )
	{
	case EXIT_BTN_QUIT:
		OnOK();
		break;
	case EXIT_BTN_CANCEL:
		OnCancel();
		break;
	default:
		break;
	}
	return CDialogEx::OnCommand(wParam, lParam);
}
