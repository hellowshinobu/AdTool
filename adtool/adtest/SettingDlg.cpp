// SettingDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "adtest.h"
#include "SettingDlg.h"
#include "afxdialogex.h"


// CSettingDlg �Ի���

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


// CSettingDlg ��Ϣ�������


BOOL CSettingDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	SetWindowPos(&wndTop,0,0,450,250,SWP_HIDEWINDOW);
	CenterWindow();

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


BOOL CSettingDlg::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// TODO: �ڴ����ר�ô����/����û���

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
