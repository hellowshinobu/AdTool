
// AdTipsDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "adtest.h"
#include "AdTipsDlg.h"
#include <windows.h>
#include "afxdialogex.h"
#include "CharTransform.h"
#include "WarnDlg.h"
#include "Utilities.h"
#include <WindowsX.h>

// CAdTipsDlg 对话框

IMPLEMENT_DYNAMIC(CAdTipsDlg, CDialogEx)

	CAdTipsDlg::CAdTipsDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CAdTipsDlg::IDD, pParent)
{
	// GDI初始化
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	Gdiplus::Status sResult = Gdiplus::GdiplusStartup(&g_ulToken, &gdiplusStartupInput, NULL);

	//字体初始化
	m_pFont = NULL;
	TCHAR* pFontName = NULL;
	LOGFONTW lf;
	SystemParametersInfo(SPI_GETICONTITLELOGFONT,sizeof(LOGFONT),&lf,0);
	pFontName = lf.lfFaceName;
	FontFamily fontF(pFontName);
	m_pFont = new Gdiplus::Font(&fontF,12,0,Gdiplus::UnitPixel);

	m_sFormat.SetAlignment(Gdiplus::StringAlignmentCenter);
	m_sFormat.SetLineAlignment(Gdiplus::StringAlignmentCenter);

	m_pMainWnd = NULL;
	m_rtWindowRect = 0;
	m_hCurrentWnd = NULL;
	m_bShowToolBar = FALSE;
	m_bSysProcess = FALSE;
	m_bZoomed = FALSE;
}

CAdTipsDlg::~CAdTipsDlg()
{

}

void CAdTipsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CAdTipsDlg, CDialogEx)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_MOUSEMOVE()
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_PAINT()
	ON_WM_PAINT()
	ON_WM_SETCURSOR()
END_MESSAGE_MAP()


// CAdTipsDlg 消息处理程序


int CAdTipsDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_stBack.pImg = LoadBitmaps(IDB_PNG6);
	m_stBack.nWidth = m_stBack.pImg->GetWidth();
	m_stBack.nHeight = m_stBack.pImg->GetHeight();
	

	m_stOK.nWidth = 56;
	m_stOK.nHeight = 24;


	m_stKill.nWidth = 82;
	m_stKill.nHeight = 24;


	m_stOpen.nWidth = 82;
	m_stOpen.nHeight = 24;


	m_stCancel.nWidth = 34;
	m_stCancel.nHeight = 24;

	return 0;
}


void CAdTipsDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

}


BOOL CAdTipsDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();


	SetWindowLong(this->GetSafeHwnd(),
		GWL_EXSTYLE, 
		GetWindowLong(this->GetSafeHwnd(),GWL_EXSTYLE)| 0x80000);  //设定窗体使用扩展模式 
	//SetLayeredWindowAttributes(0,0,LWA_ALPHA);
	GetDestopWnd();


	int width = GetSystemMetrics ( SM_CXSCREEN );
	int height= GetSystemMetrics ( SM_CYSCREEN );
	SetWindowPos(&wndTopMost,0,0,width,height,SWP_SHOWWINDOW);

	CRect windowRt;
	GetWindowRect(&windowRt);	
	InitDC(windowRt);

	GetDestopBit();


	//SetCursor(LoadCursor(NULL,MAKEINTRESOURCE(IDC_COLOR)));

	PostMessage(WM_PAINT);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CAdTipsDlg::SetWnd(CWnd* wnd)
{
	m_pMainWnd = wnd ;
}

BOOL CAdTipsDlg::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// TODO: 在此添加专用代码和/或调用基类
	UINT nID = LOWORD(wParam);  
	HWND hWndCtrl = (HWND)lParam;  
	int nCode = HIWORD(wParam);

	switch( nID )
	{
	case Tips_BTN_OK:
		{
			if (m_hCurrentWnd == GetParent()->m_hWnd)
			{
				m_bShowToolBar = FALSE;
				PostMessage(WM_PAINT);
				break;
			}
			RecordInfo();
			if (m_bSysProcess)
			{
				if (!g_bCheckState)
				{
					CWarnDlg dlg;
					if (IDOK == dlg.DoModal()){}
				}
				m_bShowToolBar = FALSE;
				PostMessage(WM_PAINT);
				break;
			}
			::ShowWindow(m_hCurrentWnd,SW_HIDE);
			::PostMessage(m_hCurrentWnd, WM_CLOSE, 0, 0);
			m_pMainWnd->PostMessage(WM_TIPCLICKOK,(WPARAM)&m_stMudleInfo);
			HideWindow();
		}
		break;
	case Tips_BTN_CANCEL:
		{
			m_pMainWnd->PostMessage(WM_TIPCLICKCANCEL);
			HideWindow();
		}
		break;
	case Tips_BTN_OPEN:
		{
			OpenCurrentFile();
			m_pMainWnd->PostMessage(WM_TIPCLICKCANCEL);
			HideWindow();
		}
		break;
	case  Tips_BTN_KILL:
		{
			if (m_hCurrentWnd == GetParent()->m_hWnd)
			{
				m_bShowToolBar = FALSE;
				PostMessage(WM_PAINT);
				break;
			}
			RecordInfo();
			if (m_bSysProcess)
			{
				if (!g_bCheckState)
				{
					CWarnDlg dlg;
					if (IDOK == dlg.DoModal()){}
				}
				m_bShowToolBar = FALSE;
				PostMessage(WM_PAINT);
				break;
			}
			m_stMudleInfo.bKillProcess = TRUE;
			::ShowWindow(m_hCurrentWnd,SW_HIDE);
			Utilities::KillProcessFromHWnd(m_hCurrentWnd);
			m_pMainWnd->PostMessage(WM_TIPCLICKOK,(WPARAM)&m_stMudleInfo);
			HideWindow();
		}
		break;
	default:
		break;
	}
	return CDialogEx::OnCommand(wParam, lParam);
}

void CAdTipsDlg::GetProcessName()
{
	CString path = L"";
	CString proName = L"";
	Utilities::GetProcessNameAndPath(m_hCurrentWnd,path,proName);
	m_stMudleInfo.proPath = CharTransform::WideByte2Acsi(path.GetString());
	m_stMudleInfo.proName = CharTransform::WideByte2Acsi(proName.GetString());
	if(Utilities::IsSysWindow(proName))
		m_bSysProcess = TRUE;
}

void CAdTipsDlg::RecordInfo()
{
	m_bSysProcess = FALSE;

	GetProcessName();

	TCHAR winText[1000] = L"";
	::GetWindowText(m_hCurrentWnd, winText, ARRAYSIZE(winText));
	CString wsText = winText;
	wsText.MakeLower();
	m_stMudleInfo.wndName = CharTransform::WideByte2Acsi(wsText.GetString());

	TCHAR className[1000]= L"";
	::GetClassName(m_hCurrentWnd, className, ARRAYSIZE(className));
	CString wsClassName = className;
	wsClassName.MakeLower();
	m_stMudleInfo.className = CharTransform::WideByte2Acsi(wsClassName.GetString());
}


BOOL CAdTipsDlg::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此添加专用代码和/或调用基类

	return CDialogEx::PreCreateWindow(cs);
}

void CAdTipsDlg::OpenCurrentFile()
{

	ULONG nProcessId = 0;
	HANDLE	hProcessHandle = NULL;
	if (m_hCurrentWnd == NULL)
		return;

	::GetWindowThreadProcessId(m_hCurrentWnd,&nProcessId);

	hProcessHandle = ::OpenProcess(PROCESS_ALL_ACCESS,FALSE,(DWORD)nProcessId);
	if(hProcessHandle!=0)
	{ 
		TCHAR szPath[MAX_PATH];
		memset(szPath,0,MAX_PATH);
		GetModuleFileNameEx(hProcessHandle, NULL, szPath, MAX_PATH);

		CString strFilePath = szPath;
		CString OpenParam = _T("/select,");
		OpenParam += strFilePath;
		ShellExecute( NULL, _T("open"), _T("explorer.exe"), OpenParam, NULL, SW_SHOWNORMAL ); 
	}	
}

void CAdTipsDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	if (!m_bShowToolBar)
	{
		BOOL bRet = FALSE;
		bRet = SearchWindow();
		if (bRet)
			return;	
	}

	Resetstatus();

	if (PtInRect(&m_stOK.rtArea,point))
	{
		m_stOK.nStatus =1;
		PostMessage(WM_PAINT);
		return;
	}

	if (PtInRect(&m_stKill.rtArea,point))
	{
		m_stKill.nStatus =1;
		PostMessage(WM_PAINT);
		return;
	}

	if (PtInRect(&m_stOpen.rtArea,point))
	{
		m_stOpen.nStatus =1;
		PostMessage(WM_PAINT);
		return;
	}

	if (PtInRect(&m_stCancel.rtArea,point))
	{
		m_stCancel.nStatus =1;
		PostMessage(WM_PAINT);
		return;
	}

	PostMessage(WM_PAINT);
}

void CAdTipsDlg::OnRButtonUp(UINT nFlags, CPoint point)
{
	m_pMainWnd->PostMessage(WM_TIPCLICKCANCEL);
	HideWindow();
}


void CAdTipsDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	OnDraw();

	CDialogEx::OnPaint();
}

void CAdTipsDlg::InitDC(CRect &rect)
{
	m_hMemDC = ::CreateCompatibleDC(NULL);
	BITMAPINFOHEADER bmih;
	int nSize=sizeof(BITMAPINFOHEADER);
	ZeroMemory(&bmih,nSize);
	bmih.biSize=nSize;
	bmih.biWidth=rect.Width();
	bmih.biHeight=-rect.Height();
	bmih.biPlanes=1;
	bmih.biBitCount=32;
	HBITMAP hBmp=::CreateDIBSection(nullptr,(BITMAPINFO*)&bmih,DIB_RGB_COLORS,nullptr,nullptr,0);
	::SelectObject(m_hMemDC,hBmp);
	DeleteObject(hBmp);
}

void CAdTipsDlg::OnDraw()
{
	CRect rt;
	GetWindowRect(&rt);
	int width = GetSystemMetrics ( SM_CXSCREEN );
	int height= GetSystemMetrics ( SM_CYSCREEN );

	HDC hDc = ::GetWindowDC(m_hWnd);
	Graphics gra(m_hMemDC);
	gra.Clear(Color(255,0,0,0));
	//gra.DrawImage(m_ImgBack,Rect(0,0,rt.Width(),rt.Height()),0,0,m_ImgBack->GetWidth(),m_ImgBack->GetHeight(),UnitPixel);  // 绘制背景
	::BitBlt(m_hMemDC, 0, 0,width, height,m_hTmpMemDC, 0, 0, SRCCOPY);

	Pen pen(Color(255,255,0,0),5);
	if (m_bZoomed)
		gra.DrawRectangle(&pen,3,1,width-5,m_rtWindowRect.Height()-8);
	else
		gra.DrawRectangle(&pen,m_rtWindowRect.left,m_rtWindowRect.top,m_rtWindowRect.Width(),m_rtWindowRect.Height());

	if (m_bShowToolBar)
	{
		DrawImgBtn(gra,m_stBack);
		DrawStringBtn(gra,L"屏蔽窗口",m_stOK);
		DrawStringBtn(gra,L"屏蔽窗口进程",m_stKill);
		DrawStringBtn(gra,L"打开进程位置",m_stOpen);
		DrawStringBtn(gra,L"取消",m_stCancel);
	}

	BLENDFUNCTION Blend;
	Blend.BlendOp = 0;
	Blend.BlendFlags = 0;    
	Blend.AlphaFormat = AC_SRC_ALPHA;    
	Blend.SourceConstantAlpha = 255;
	::UpdateLayeredWindow(m_hWnd,hDc,&CPoint(rt.left,rt.top),&CSize(rt.Width(),rt.Height()),m_hMemDC,&CPoint(0,0),0,&Blend,ULW_ALPHA);
	::ReleaseDC(m_hWnd,hDc);
}

Bitmap* CAdTipsDlg::LoadBitmaps(DWORD dwResource)
{
	Bitmap* pImg = nullptr;
	HRSRC hRsrc =  ::FindResource(NULL,MAKEINTRESOURCE(dwResource),L"PNG");
	HGLOBAL hRscData = LoadResource(NULL,hRsrc);

	size_t nDataSize = SizeofResource(NULL,hRsrc);
	HGLOBAL hPngData = GlobalAlloc(GHND,nDataSize);
	void* pData = LockResource(hRscData);
	void* pPng = GlobalLock(hPngData);
	memcpy(pPng,pData,nDataSize);
	IStream* pStream;
	CreateStreamOnHGlobal(hPngData,TRUE,&pStream);
	pImg = Gdiplus::Bitmap::FromStream(pStream);
	FreeResource(hPngData);
	return pImg;
}

void CAdTipsDlg::DrawImgBtn(Graphics& gra,IMG_BTN stImgbtn)
{
	gra.DrawImage(stImgbtn.pImg,Gdiplus::Rect(stImgbtn.rtArea.left,stImgbtn.rtArea.top,stImgbtn.nWidth,stImgbtn.nHeight)
		,stImgbtn.nWidth*stImgbtn.nStatus,0,stImgbtn.nWidth,stImgbtn.nHeight,UnitPixel);
}

void CAdTipsDlg::Resetstatus()
{
	m_stOK.nStatus = 0;
	m_stKill.nStatus = 0;
	m_stOpen.nStatus = 0;
	m_stCancel.nStatus = 0;
}

void CAdTipsDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (m_hCurrentWnd == NULL)
	{
		return;
	}
	int nId = 0;
	if (!m_bShowToolBar)
		ShowToolBar();

	if (PtInRect(&m_stOK.rtArea,point))
	{
		m_stOK.nStatus = 2;
		nId = Tips_BTN_OK;
		PostMessage(WM_PAINT);
	}

	if (PtInRect(&m_stKill.rtArea,point))
	{
		nId = Tips_BTN_KILL;
		m_stKill.nStatus = 2;
		PostMessage(WM_PAINT);
	}

	if (PtInRect(&m_stOpen.rtArea,point))
	{
		nId = Tips_BTN_OPEN;
		m_stOpen.nStatus = 2;
		PostMessage(WM_PAINT);
	}

	if (PtInRect(&m_stCancel.rtArea,point))
	{
		nId = Tips_BTN_CANCEL;
		m_stCancel.nStatus = 2;
		PostMessage(WM_PAINT);
	}

	PostMessage(WM_COMMAND,nId);
}

void CAdTipsDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (PtInRect(&m_stOK.rtArea,point))
	{
		m_stOK.nStatus = 1;
		PostMessage(WM_PAINT);
	}

	if (PtInRect(&m_stKill.rtArea,point))
	{
		m_stKill.nStatus = 1;
		PostMessage(WM_PAINT);
	}

	if (PtInRect(&m_stOpen.rtArea,point))
	{
		m_stOpen.nStatus = 1;
		PostMessage(WM_PAINT);
	}

	if (PtInRect(&m_stCancel.rtArea,point))
	{
		m_stCancel.nStatus = 1;
		PostMessage(WM_PAINT);
	}
}

void CAdTipsDlg::AdjustButton(ButtonPos buttonpos)
{
	CRect rt;
	rt = m_rtWindowRect;
	int width = GetSystemMetrics ( SM_CXSCREEN );
	int height= GetSystemMetrics ( SM_CYSCREEN );
	int topPos = rt.top - 30;
	int downPos = rt.bottom + 5;
	int leftPos = rt.left;
	int rightPos = rt.right;
	if (leftPos < 0)
	{
		leftPos = 0;
	}
	if (rightPos > width)
	{
		rightPos = width;
	}

	switch(buttonpos)
	{
	case POS_LEFTTOP:
		{
			m_stBack.rtArea = CRect(leftPos,topPos,leftPos+m_stBack.nWidth,topPos+m_stBack.nHeight);
			m_stOK.rtArea = CRect(leftPos,topPos,leftPos+m_stOK.nWidth,topPos+m_stOK.nHeight);
			m_stKill.rtArea = CRect(leftPos+56,topPos,leftPos+56+m_stKill.nWidth,topPos+m_stKill.nHeight);
			m_stOpen.rtArea = CRect(leftPos+138,topPos,leftPos+138+m_stOpen.nWidth,topPos+m_stOpen.nHeight);
			m_stCancel.rtArea = CRect(leftPos+220,topPos,leftPos+220+m_stCancel.nWidth,topPos+m_stCancel.nHeight);
		}
		break;
	case POS_RIGHTTOP:
		{
			m_stCancel.rtArea = CRect(rightPos-30,topPos,rightPos-30+m_stCancel.nWidth,topPos+m_stCancel.nHeight);
			m_stOpen.rtArea = CRect(rightPos-112,topPos,rightPos-112+m_stOpen.nWidth,topPos+m_stOpen.nHeight);
			m_stKill.rtArea = CRect(rightPos-194,topPos,rightPos-194+m_stKill.nWidth,topPos+m_stKill.nHeight);
			m_stOK.rtArea = CRect(rightPos-249,topPos,rightPos-249+m_stOK.nWidth,topPos+m_stOK.nHeight);
			m_stBack.rtArea = CRect(rightPos-249,topPos,rightPos-249+m_stBack.nWidth,topPos+m_stBack.nHeight);
		}
		break;
	case POS_LEFTDOWN:
		{	
			m_stBack.rtArea = CRect(leftPos,downPos,leftPos+m_stBack.nWidth,downPos+m_stBack.nHeight);
			m_stOK.rtArea = CRect(leftPos,downPos,leftPos+m_stOK.nWidth,downPos+m_stOK.nHeight);
			m_stKill.rtArea = CRect(leftPos+56,downPos,56+leftPos+m_stKill.nWidth,downPos+m_stKill.nHeight);
			m_stOpen.rtArea = CRect(leftPos+138,downPos,138+leftPos+m_stOpen.nWidth,downPos+m_stOpen.nHeight);
			m_stCancel.rtArea = CRect(leftPos+220,downPos,220+leftPos+m_stCancel.nWidth,downPos+m_stCancel.nHeight);
		}
		break;
	case POS_RIGHTDOWN:
		{
			m_stCancel.rtArea = CRect(rightPos-30,downPos,rightPos-30+m_stCancel.nWidth,downPos+m_stCancel.nHeight);
			m_stOpen.rtArea = CRect(rightPos-112,downPos,rightPos-112+m_stOpen.nWidth,downPos+m_stOpen.nHeight);
			m_stKill.rtArea = CRect(rightPos-194,downPos,rightPos-194+m_stKill.nWidth,downPos+m_stKill.nHeight);
			m_stOK.rtArea = CRect(rightPos-249,downPos,rightPos-249+m_stOK.nWidth,downPos+m_stOK.nHeight);
			m_stBack.rtArea = CRect(rightPos-249,downPos,rightPos-249+m_stBack.nWidth,downPos+m_stBack.nHeight);
		}
		break;
	case POS_FULLSCREEN:
		{
			m_stBack.rtArea = CRect(5,5,5+m_stBack.nWidth,5+m_stBack.nHeight);
			m_stOK.rtArea = CRect(5,5,5+m_stOK.nWidth,5+m_stOK.nHeight);
			m_stKill.rtArea = CRect(57,5,57+m_stKill.nWidth,5+m_stKill.nHeight);
			m_stOpen.rtArea = CRect(139,5,139+m_stOpen.nWidth,5+m_stOpen.nHeight);
			m_stCancel.rtArea = CRect(221,5,221+m_stCancel.nWidth,5+m_stCancel.nHeight);
		}
		break;
	default:
		break;
	}
	// 	char cBuff[MAX_PATH]={0};
	// 	sprintf_s(cBuff,"%d,%d,%d,%d",m_stCancel.rtArea.left,m_stCancel.rtArea.top,m_stCancel.rtArea.right,m_stCancel.rtArea.bottom);
	// 	MessageBoxA(0,cBuff,0,0);
}

void CAdTipsDlg::RecordWnd(HWND wnd)
{
	do 
	{
		stWndInfo wndInfo;
		wndInfo.m_hWnd = wnd;
		CRect rt;
		::GetWindowRect(wnd,&rt);
		if (rt.Width() <= 0 || rt.Height() <= 0)
			break;

		TCHAR className[1000]= L"";
		GetClassName(wnd, className, ARRAYSIZE(className));
		CString wsClassName = className;

		if (wsClassName.CompareNoCase(L"Shell_TrayWnd") == 0
			|| wsClassName.CompareNoCase(L"Progman") == 0)
			break;

		wndInfo.m_bIsZoomed = ::IsZoomed(wnd);

		wndInfo.m_rtRect = rt;
		m_vecWndInfo.push_back(wndInfo);
	}while (0);
}

void CAdTipsDlg::ShowToolBar()
{
	m_bShowToolBar = TRUE;

	int width = GetSystemMetrics ( SM_CXSCREEN );
	int height= GetSystemMetrics ( SM_CYSCREEN );
	if (m_rtWindowRect.Width() == width 
		&& m_rtWindowRect.Height() == height 
		&& m_rtWindowRect.left == 0
		&& m_rtWindowRect.top == 0)
	{
		AdjustButton(POS_FULLSCREEN);
	}else if (m_rtWindowRect.top > 30)
	{
		if (abs(m_rtWindowRect.left-width)<261)
			AdjustButton(POS_RIGHTTOP);
		else
			AdjustButton(POS_LEFTTOP);	
	}else
	{
		if (abs(m_rtWindowRect.left-width)<261)
			AdjustButton(POS_RIGHTDOWN);
		else
			AdjustButton(POS_LEFTDOWN);	
	}
}

void CAdTipsDlg::ShowWndRt()
{
	POINT pCursorPos;
	GetCursorPos(&pCursorPos);// 获取鼠标当前位置
	for (vector<stWndInfo>::const_iterator wnd_iter = m_vecWndInfo.begin();
		wnd_iter != m_vecWndInfo.end(); ++wnd_iter)
	{
		if (PtInRect(&(wnd_iter->m_rtRect),pCursorPos))
		{
			m_rtWindowRect = wnd_iter->m_rtRect;
			PostMessage(WM_PAINT);
			break;
		}	
	}
}

BOOL CALLBACK CAdTipsDlg::lpEnumWndFunc(HWND hwnd, LPARAM lParam)
{
	DWORD style = GetWindowStyle(hwnd);
	DWORD styleex = GetWindowExStyle(hwnd);

	if (style & WS_VISIBLE)
	{
		HWND hCurrentWnd = NULL;
		hCurrentWnd=::FindWindow(NULL,_T("AdToolsScreen")); 
		if (hCurrentWnd != NULL)
		{
			CAdTipsDlg* dlg= (CAdTipsDlg*)CWnd::FromHandle(hCurrentWnd);
			dlg->RecordWnd(hwnd);
		}
	}
	return TRUE;
}


BOOL CAdTipsDlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (m_bShowToolBar)
	{
		return CDialogEx::OnSetCursor(pWnd, nHitTest, message);
	}
	::SetCursor(::LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_COLOR)));	
	return TRUE;
	//return CDialogEx::OnSetCursor(pWnd, nHitTest, message);
}

void CAdTipsDlg::GetDestopBit()
{
	int width = GetSystemMetrics ( SM_CXSCREEN );
	int height= GetSystemMetrics ( SM_CYSCREEN );

	HWND hDeskTopWnd = ::GetDesktopWindow();
	m_hFullDC = ::GetWindowDC(hDeskTopWnd);
	m_hTmpMemDC = CreateCompatibleDC(NULL);

	BYTE  *pMemBuff = 0;

	BITMAPINFOHEADER bmih;
	int nSize=sizeof(BITMAPINFOHEADER);
	ZeroMemory(&bmih,nSize);
	bmih.biSize=nSize;
	bmih.biWidth=width;
	bmih.biHeight=-height;
	bmih.biPlanes=1;
	bmih.biBitCount=32;

	HBITMAP hBmp=::CreateDIBSection(nullptr,(BITMAPINFO*)&bmih,DIB_RGB_COLORS,(void **)&pMemBuff,nullptr,0);
	SelectObject(m_hTmpMemDC,hBmp);
	::DeleteObject(hBmp);

	::BitBlt(m_hTmpMemDC, 0, 0, width, height, m_hFullDC, 0, 0, SRCCOPY);

	::ReleaseDC(hDeskTopWnd,m_hFullDC);
	DWORD* pData = (DWORD*)pMemBuff;
	for (int i=0;i<height;i++)
	{
		for (int j=0;j<width;j++)
		{
			*((BYTE*)(pData+j)+3) = 255;
		}
		(DWORD*)pData+=width;
	}
}

void CAdTipsDlg::HideWindow()
{
	if (!m_vecWndInfo.empty())
	{
		m_vecWndInfo.clear();
	}
	m_rtWindowRect = 0;
	m_hCurrentWnd = NULL;
	m_bShowToolBar = FALSE;
	m_bSysProcess = FALSE;
	m_bZoomed = FALSE;

	ShowWindow(SW_HIDE);
	::PostMessage(m_hWnd, WM_CLOSE, 0, 0);
	DestroyWindow();
}

void CAdTipsDlg::GetDestopWnd()
{
	SetWindowText(L"AdToolsScreen");

	EnumWindows(lpEnumWndFunc, NULL);
}


BOOL CAdTipsDlg::DestroyWindow()
{
	// TODO: 在此添加专用代码和/或调用基类

	if (m_pFont != NULL)
	{
		delete m_pFont;
		m_pFont = NULL;
	}

	DeleteDC(m_hTmpMemDC);
	DeleteDC(m_hMemDC);
	Gdiplus::GdiplusShutdown(g_ulToken);
	g_ulToken = 0;

	return CDialogEx::DestroyWindow();
}

BOOL CAdTipsDlg::SearchWindow()
{
	BOOL bRet = FALSE;
	do 
	{
		POINT pCursorPos;
		GetCursorPos(&pCursorPos);// 获取鼠标当前位置
		for (vector<stWndInfo>::const_iterator wnd_iter = m_vecWndInfo.begin();
			wnd_iter != m_vecWndInfo.end(); ++wnd_iter)
		{
			if (PtInRect(&(wnd_iter->m_rtRect),pCursorPos))
			{
				if (m_hCurrentWnd == wnd_iter->m_hWnd )
				{
					bRet = TRUE;
					break;
				}
				m_rtWindowRect = wnd_iter->m_rtRect;
				m_hCurrentWnd = wnd_iter->m_hWnd;
				m_bZoomed = wnd_iter->m_bIsZoomed;
				PostMessage(WM_PAINT);
				break;
			}	
		}
	} while (0);

	return bRet;
}

void CAdTipsDlg::DrawStringBtn(Graphics& gra,wstring wsWord,IMG_BTN stImgbtn)
{

	SolidBrush* pBrush = NULL;

	switch(stImgbtn.nStatus)
	{
	case 0:
		pBrush = new SolidBrush(Color(255,255,255,255));
		break;
	case 1:
		pBrush = new SolidBrush(Color(255,108,220,90));
		break;
	case 2:
		pBrush = new SolidBrush(Color(255,108,220,90));
		break;
	default:
		break;
	}

	RectF rf(stImgbtn.rtArea.left,stImgbtn.rtArea.top+4,stImgbtn.rtArea.Width(),stImgbtn.rtArea.Height());
	gra.DrawString(wsWord.c_str(),wsWord.length(),m_pFont,rf,&m_sFormat,pBrush);
	DeleteObject(pBrush);
	delete pBrush;
	pBrush = NULL;
}
