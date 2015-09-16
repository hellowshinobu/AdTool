#pragma once

#include "global.h"
// CAdTipsDlg 对话框

typedef  struct _stWndInfo
{
	BOOL m_bIsZoomed;
	HWND m_hWnd;
	CRect m_rtRect;

	_stWndInfo()
		:	m_hWnd(NULL)
		,	m_rtRect(CRect(0,0,0,0))
		,	m_bIsZoomed(FALSE)
	{
	}
}stWndInfo;


class CAdTipsDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CAdTipsDlg)

public:
	CAdTipsDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CAdTipsDlg();
	void SetWnd(CWnd* wnd);
	void GetProcessName();
	void RecordInfo();
	void OpenCurrentFile();
	void InitDC(CRect &rect);
	void InitProVec(vector<CString> vecPro);
	void OnDraw();
	void DrawImgBtn(Graphics& gra,IMG_BTN stImgbtn);
	void DrawStringBtn(Graphics& gra,wstring wsWord,IMG_BTN stImgbtn);
	BOOL SearchWindow();
	void HideWindow();
	void Resetstatus();
	void AdjustButton(ButtonPos buttonpos);
	void RecordWnd(HWND wnd);
	void ShowToolBar();
	void ShowWndRt();
	void GetDestopBit();
	void GetDestopWnd();

//枚举窗口
	static BOOL CALLBACK lpEnumWndFunc(HWND hwnd, LPARAM lParam);
// 对话框数据
	enum { IDD = IDD_AdTipsDlg };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	virtual BOOL OnInitDialog();

private:
	BOOL m_bSysProcess;
	BOOL m_bShowToolBar;
	BOOL m_bZoomed;
	vector<stWndInfo> m_vecWndInfo;

	IMG_BTN m_stBack;
	IMG_BTN m_stOK;
	IMG_BTN m_stCancel;
	IMG_BTN m_stOpen;
	IMG_BTN m_stKill;

	CWnd*	m_pMainWnd;
	HWND m_hCurrentWnd;
	CRect	m_rtWindowRect;
	CRect	m_rtLastRect;
	stAdInfo m_stMudleInfo;
	HDC m_hMemDC;
	HDC m_hFullDC;
	HDC m_hTmpMemDC;
	Bitmap* m_ImgBack;
	Bitmap* m_ImgOk;

	Gdiplus::Font* m_pFont;
	StringFormat m_sFormat;
private:
	Bitmap* LoadBitmaps( DWORD dwResource );
public:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	afx_msg void OnPaint();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	virtual BOOL DestroyWindow();
};
