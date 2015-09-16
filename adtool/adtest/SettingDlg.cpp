// SettingDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "adtest.h"
#include "SettingDlg.h"
#include "afxdialogex.h"


// CSettingDlg 对话框

IMPLEMENT_DYNAMIC(CSettingDlg, CDialogEx)

CSettingDlg::CSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSettingDlg::IDD, pParent)
{

}

CSettingDlg::~CSettingDlg()
{
}

void CSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSettingDlg, CDialogEx)
END_MESSAGE_MAP()


// CSettingDlg 消息处理程序


BOOL CSettingDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	SetWindowPos(&wndTop,0,0,450,250,SWP_HIDEWINDOW);
	CenterWindow();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


BOOL CSettingDlg::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// TODO: 在此添加专用代码和/或调用基类

	return CDialogEx::OnCommand(wParam, lParam);
}

BOOL CSettingDlg::GetAuto() const
{
	return m_bAuto;
}

BOOL CSettingDlg::GetAdd() const
{
	return m_bAdd;
}

void CSettingDlg::SetAuto(BOOL bAuto)
{
	m_bAuto = bAuto;
}

void CSettingDlg::SetAdd(BOOL bAdd)
{
	m_bAdd = bAdd;
}
