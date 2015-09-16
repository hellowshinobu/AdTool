
// adtestDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "adtest.h"
#include "adtestDlg.h"
#include "afxdialogex.h" 
#include "AdTipsDlg.h"
#include "windowsx.h"
#include "CharTransform.h"
#include "ExitDlg.h"
#include "Utilities.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//http加密链接


typedef void(*_Run)(const char* ,BOOL);

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// CadtestDlg 对话框

BOOL CALLBACK CadtestDlg::lpEnumFunc(HWND hwnd, LPARAM lParam)
{
	DWORD style = GetWindowStyle(hwnd);
	DWORD styleex = GetWindowExStyle(hwnd);

	int width = GetSystemMetrics ( SM_CXSCREEN );
	int height= GetSystemMetrics ( SM_CYSCREEN );

	CRect rect;
	::GetWindowRect(hwnd, &rect);

	TCHAR winText[1000] = L"";
	::GetWindowText(hwnd, winText, ARRAYSIZE(winText));
	CString wsText = winText;
	wsText.MakeLower();

	TCHAR className[1000]= L"";
	GetClassName(hwnd, className, ARRAYSIZE(className));
	CString wsClassName = className;
	wsClassName.MakeLower();

	CadtestDlg* dlg = (CadtestDlg*)AfxGetMainWnd();

	if (dlg->m_bDown)
	{
		string strWndName;
		strWndName = CharTransform::WideByte2Acsi(wsText.GetString());
		BOOL result1  = dlg->CompareWndName(strWndName);

		if (result1)
		{
			::ShowWindow(hwnd,SW_HIDE);
			::PostMessage(hwnd, WM_CLOSE, 0, 0);
			// 			::PostMessage(hwnd, WM_DESTROY, 0, 0);
			return TRUE;
		}
	}

	if ( (style&WS_VISIBLE ))
	{
		if (rect.Height() < 400 && rect.Width() < 400 && rect.Height() > 100 && rect.Width() > 100)
		{
			if (abs(width - rect.right) < 50
				&& abs(height - rect.bottom) >30 
				&& abs(height - rect.bottom) < 80)
			{
				if (hwnd == AfxGetMainWnd()->m_hWnd)
					return TRUE;
				if (rect.Width() == 270)
					return TRUE;
				CString proName = L"";
				CString path = L"";
				Utilities::GetProcessNameAndPath(hwnd,path,proName);
				if (Utilities::IsSysWindow(proName))
					return TRUE;

				::ShowWindow(hwnd,SW_HIDE);
				::PostMessage(hwnd, WM_CLOSE, 0, 0);
				return TRUE;
			}
		}

		string strWinText;
		string strClassName;
		strWinText = CharTransform::WideByte2Acsi(wsText.GetString());
		strClassName = CharTransform::WideByte2Acsi(wsClassName.GetString());
		BOOL result1 = dlg->CompareWinTextAndClassName(strWinText,strClassName);

		if (result1)
		{
			::ShowWindow(hwnd,SW_HIDE);
			::PostMessage(hwnd, WM_CLOSE, 0, 0);
			::PostMessage(hwnd, WM_DESTROY, 0, 0);
		}
	}

	return TRUE;
}

CadtestDlg::CadtestDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CadtestDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pAdDb = new AdToolDb();
	m_hwndPointOld = NULL;
	m_dlgTips = NULL;
	m_bOpenInfo = FALSE;
	m_bAddToList = TRUE;
	m_bAutoRun = FALSE;
	m_bRunLua = FALSE;
	m_bDown = FALSE;
	m_strFBUrl = L"";
	m_strSysUrl = L"";
	m_nCurUserItem = -1;
	m_nCurSysItem = -1;
}

CadtestDlg::~CadtestDlg()
{
}

void CadtestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_list);
	DDX_Control(pDX, IDC_LIST2, m_listSys);
	DDX_Control(pDX, MAIN_TAB_LIST, m_tabList);
}

BEGIN_MESSAGE_MAP(CadtestDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SHOWWINDOW()
	ON_WM_SIZE()
	ON_MESSAGE(WM_SHOWTASK,OnShowTask)
	ON_MESSAGE(WM_TIPMOUSEMOVE,OnTipsMoveLeave)
	ON_MESSAGE(WM_TIPCLICKOK,OnTipsClickOk)
	ON_MESSAGE(WM_TIPCLICKCANCEL,OnTipsClickCancel)
	ON_WM_TIMER()
	ON_WM_CREATE()
	ON_NOTIFY(NM_RCLICK, IDC_LIST1, &CadtestDlg::OnNMRClickList1)
	ON_NOTIFY(NM_RCLICK, IDC_LIST2, &CadtestDlg::OnNMRClickList2)
	ON_COMMAND(IDM_DELETE, &CadtestDlg::OnDelete)
	ON_WM_CLOSE()
	ON_NOTIFY(TCN_SELCHANGE, MAIN_TAB_LIST, &CadtestDlg::OnTcnSelchangeTabList)
END_MESSAGE_MAP()


// CadtestDlg 消息处理程序

BOOL CadtestDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标
	// TODO: 在此添加额外的初始化代码

	::CreateMutex(NULL,TRUE,_T("adtools"));
	if(GetLastError()==ERROR_ALREADY_EXISTS)
	{
		//dup
		exit(0);
	}

	SetWindowPos(&wndNoTopMost,0,0,0,0,SWP_HIDEWINDOW); 
	ModifyStyleEx(WS_EX_APPWINDOW,WS_EX_TOOLWINDOW);
	//读取配置
	ReadConfig();
	//创建托盘
	CreateTray();
	//初始化屏蔽列表
	InitMyList();
	//初始化受保护的窗口数组
	Utilities::InitProtectExe();

	CString des=L"";
	GetCurrentPath(des);
	des += SQLITEDB;

	m_bOpenInfo = m_pAdDb->InitDb(des);

	if (m_bOpenInfo){
		m_pAdDb->SelectUser(m_vecAdInfo);
		if (!m_vecAdInfo.empty()){
			for ( unsigned i = 0; i < m_vecAdInfo.size(); ++i)
				AddData(m_vecAdInfo.at(i));
		}

		m_pAdDb->SelectSys(m_vecAdSysInfo);
		if (!m_vecAdSysInfo.empty()){
			for ( unsigned i = 0; i < m_vecAdSysInfo.size(); ++i)
				AddSysData(m_vecAdSysInfo.at(i));
		}
	}	

	SetTimer(LITSENWND, 500, NULL);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CadtestDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CadtestDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CadtestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CadtestDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	// TODO: 在此处添加消息处理程序代码
	// 	if( GetStyle() & WS_VISIBLE )
	// 	{
	// 		CDialog::OnShowWindow(bShow, nStatus);
	// 	}
	// 	else
	// 	{
	// 		long Style = ::GetWindowLong(*this,GWL_STYLE);
	// 		::SetWindowLong(*this,GWL_STYLE,Style | WS_VISIBLE);
	// 		CDialog::OnShowWindow(SW_HIDE,nStatus);
	// 	}
}

void CadtestDlg::CreateTray()
{
	CString cmd = L"";
	m_nid.cbSize  = (DWORD)sizeof(NOTIFYICONDATA);
	m_nid.hWnd    = this->m_hWnd;
	m_nid.uID     = IDR_MAINFRAME;
	//m_nid.uFlags  = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	m_nid.uFlags=NIF_ICON|NIF_MESSAGE|NIF_TIP|NIF_INFO | NIF_SHOWTIP;    
	wcscpy_s(m_nid.szTip, EXENAME);  
	cmd = AfxGetApp()->m_lpCmdLine;
	if (cmd != L"")
		wcscpy_s(m_nid.szInfo,BALLONTIP);   
	else
		wcscpy_s(m_nid.szInfo,L"");   

	wcscpy_s(m_nid.szInfoTitle,EXENAME);  
	m_nid.dwInfoFlags= NIIF_USER | NIIF_LARGE_ICON ;    //打开自定义及大图标提示    
	m_nid.hBalloonIcon=LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME));   

	m_nid.uCallbackMessage = WM_SHOWTASK;             // 自定义的消息名称
	m_nid.hIcon   = LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME));

	Shell_NotifyIcon(NIM_ADD, &m_nid);                // 在托盘区添加图标
}

LRESULT CadtestDlg::OnShowTask(WPARAM wParam, LPARAM lParam)
{
	if(wParam != IDR_MAINFRAME)
		return 1;
	switch(lParam)
	{
	case WM_RBUTTONUP:                                        // 右键起来时弹出菜单
		{
			LPPOINT lpoint = new tagPOINT;
			::GetCursorPos(lpoint);                    // 得到鼠标位置
			CMenu menu;
			menu.CreatePopupMenu();                    // 声明一个弹出式菜单
			if (m_bAutoRun)
				menu.AppendMenu(MF_STRING | MF_CHECKED, WM_ISAUTORUN, L"随系统启动");
			else
				menu.AppendMenu(MF_STRING , WM_ISAUTORUN, L"随系统启动");

			if (m_bAddToList)
				menu.AppendMenu(MF_STRING | MF_CHECKED, WM_ISADDTOLIST, L"加入屏蔽列表");
			else
				menu.AppendMenu(MF_STRING , WM_ISADDTOLIST, L"加入屏蔽列表");
			menu.AppendMenu(MF_STRING, WM_SHOWADLIST, L"显示屏蔽列表");
			menu.AppendMenu(MF_STRING, WM_FEEDBACK, L"问题反馈");
			menu.AppendMenu(MF_STRING, WM_EXIT, L"关闭");		
			::SetForegroundWindow(m_hWnd); 

			menu.TrackPopupMenu(TPM_LEFTALIGN, lpoint->x ,lpoint->y, this);
			HMENU hmenu = menu.Detach();
			menu.DestroyMenu();
			delete lpoint;
		}
		break;
	case WM_LBUTTONDOWN:                                 // 左键的处理
	case WM_LBUTTONDBLCLK:
		{
			FindAdWindow();
		}
		break;
	default:
		break;
	}

	return 0;
}

void CadtestDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	if(nType == SIZE_MINIMIZED)  
	{  
		ShowWindow(SW_HIDE); // 当最小化市，隐藏主窗口              
	}  

	if(::IsWindow(m_tabList.GetSafeHwnd()))
	{
		m_tabList.MoveWindow(1,1,300,25);
	}
}

BOOL CadtestDlg::DestroyWindow()
{
	// TODO: 在此添加专用代码和/或调用基类
	Shell_NotifyIcon(NIM_DELETE, &m_nid);  
	WriteData();
	WriteConfig();
	if ( m_dlgTips != NULL)
	{
		m_dlgTips->DestroyWindow();
		delete m_dlgTips;
		m_dlgTips = NULL;
	}

	if (m_pAdDb !=  NULL)
	{
		m_pAdDb->CloseDb();
		delete m_pAdDb;
		m_pAdDb = NULL;
	}

	return CDialogEx::DestroyWindow();
}

void CadtestDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	switch(nIDEvent)
	{
	case LITSENWND:
		{

			KillProcessFromVector();

			EnumWindows(lpEnumFunc, NULL);
		}
		break;
	default:
		break;
	}
	CDialogEx::OnTimer(nIDEvent);
}

void CadtestDlg::FindAdWindow()
{
	if ( m_dlgTips != NULL)
	{
		delete m_dlgTips;
		m_dlgTips = NULL;
	}
	DrawAdRect();
}

void CadtestDlg::DrawAdRect()
{
	int width = GetSystemMetrics ( SM_CXSCREEN );
	int height= GetSystemMetrics ( SM_CYSCREEN );
	if (m_dlgTips == NULL)
	{
		m_dlgTips = new CAdTipsDlg();
		m_dlgTips->Create(IDD_AdTipsDlg);
		m_dlgTips->SetWnd(this);
	}
}

LRESULT CadtestDlg::OnTipsMoveLeave(WPARAM wParam, LPARAM lParam)
{
	m_hwndPointOld = NULL;
	return 0;
}

LRESULT CadtestDlg::OnTipsClickOk(WPARAM wParam, LPARAM lParam)
{
	m_hwndPointOld = NULL;
	stAdInfo info = *((stAdInfo *)wParam);

	if (m_bOpenInfo && m_bAddToList)
	{
		m_pAdDb->AddUser(info);
		if (m_pAdDb->SelectLastUser(m_vecAdInfo))
		{
			if (!m_vecAdInfo.empty())
			{
				AddData(m_vecAdInfo.back());
			}		
		}	
	}
	SetTimer(LITSENWND, 10, NULL);

	return 0;
}

LRESULT CadtestDlg::OnTipsClickCancel(WPARAM wParam, LPARAM lParam)
{
	SetTimer(LITSENWND, 10, NULL);
	return 0;
}

BOOL CadtestDlg::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// TODO: 在此添加专用代码和/或调用基类
	UINT nID = LOWORD(wParam);  
	HWND hWndCtrl = (HWND)lParam;  
	int nCode = HIWORD(wParam);

	switch( nID )
	{
	case WM_SHOWADLIST:
		{
			ShowMainWnd();
		}
		break;
	case Test_BTN_HIDE:
		{
			HideMainWnd();
		}
		break;
	case Test_BTN_DELETE:
		{
			OnDelete();
		}
		break;
	case	Test_BTN_DELETEALL:
		{
			DeleteAllItem();
		}
		break;
	case WM_ISADDTOLIST:
		{
			m_bAddToList = !m_bAddToList;
		}
		break;
	case WM_ISAUTORUN:
		{
			m_bAutoRun = !m_bAutoRun;
			if (m_bAutoRun)
				Utilities::AutoRun();
			else
				Utilities::NoRun();
		}
		break;
	case WM_EXIT:
		{
			CExitDlg dlg;
			if (IDOK == dlg.DoModal())
				PostMessage(WM_QUIT);
		}
		break;
	case WM_FEEDBACK:
		ShellExecute(NULL,NULL,L"iexplore.exe"
			,m_strFBUrl,NULL,SW_SHOW);
		break;
	case USRLIST_MENU_BAN:
	case USRLIST_MENU_START:
		ModifyUsrWndStatus();
		break;
	case SYSLIST_MENU_BAN:
	case SYSLIST_MENU_START:
		ModifySysWndStatus();
		break;
	default:
		break;
	}
	return CDialogEx::OnCommand(wParam, lParam);
}

void CadtestDlg::InitMyList()
{
	if (::IsWindow(m_tabList.GetSafeHwnd()))
	{
		m_tabList.InsertItem(0,L"用户规则"); 
		m_tabList.InsertItem(1,L"系统规则"); 
	}

	if (::IsWindow(m_list.GetSafeHwnd()))
	{
		m_list.SetExtendedStyle(LVS_EX_FULLROWSELECT );
		m_list.InsertColumn(0, _T("目标位置"), LVCFMT_LEFT, 220);
		m_list.InsertColumn(0, _T("状态"), LVCFMT_LEFT, 50);
		m_list.InsertColumn(0, _T("窗口标题"), LVCFMT_LEFT, 100);
		m_list.InsertColumn(0, _T("进程名"), LVCFMT_LEFT, 100);
	}

	if (::IsWindow(m_listSys.GetSafeHwnd()))
	{
		m_listSys.SetExtendedStyle(LVS_EX_FULLROWSELECT );
		m_listSys.InsertColumn(0, _T("窗口状态"), LVCFMT_LEFT, 220);
		m_listSys.InsertColumn(0, _T("窗口标题"), LVCFMT_LEFT, 250);
	}
}

void CadtestDlg::ShowMainWnd()
{

	SetWindowPos(&wndTop,0,0,520,300,SWP_SHOWWINDOW); 

	if(::IsWindow(m_tabList.GetSafeHwnd()))
	{
		m_tabList.MoveWindow(1,1,300,25);
	}

	if(::IsWindow(m_list.GetSafeHwnd()))
	{
		m_list.MoveWindow(1,27,518,200);
	}

	if(::IsWindow(m_listSys.GetSafeHwnd()))
	{
		m_listSys.MoveWindow(1,27,518,200);
		m_listSys.ShowWindow(SW_HIDE);
	}

	if (::IsWindow((GetDlgItem(Test_BTN_HIDE))->GetSafeHwnd()))
	{
		(GetDlgItem(Test_BTN_HIDE))->MoveWindow(395,235,100,30);
	}

	if (::IsWindow((GetDlgItem(Test_BTN_DELETE))->GetSafeHwnd()))
	{
		(GetDlgItem(Test_BTN_DELETE))->MoveWindow(1,235,100,30);
	}

	if (::IsWindow(GetDlgItem(Test_BTN_UPDATE)->GetSafeHwnd()))
	{
		(GetDlgItem(Test_BTN_UPDATE))->MoveWindow(1,235,100,30);
		(GetDlgItem(Test_BTN_UPDATE))->ShowWindow(SW_HIDE);
	}

	if (::IsWindow((GetDlgItem(Test_BTN_DELETEALL))->GetSafeHwnd()))
	{
		(GetDlgItem(Test_BTN_DELETEALL))->MoveWindow(116,235,100,30);
	}

	CenterWindow();
	ModifyStyleEx(WS_EX_TOOLWINDOW,WS_EX_APPWINDOW);
}

int CadtestDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码

	return 0;
}

void CadtestDlg::HideMainWnd()
{
	ShowWindow(SW_HIDE);
}

void CadtestDlg::AddData(stAdInfo& info)
{
	if (info.wndName.empty() && info.className.empty())
		return;

	CString proName;
	CString wndTxt;
	CString proPath;
	CString wndStatus = L"";

	proName = info.proName.c_str();
	wndTxt =info.wndName.c_str();
	proPath = info.proPath.c_str();

	if (info.bWndStatus)
		wndStatus = L"启用";
	else
		wndStatus = L"禁用";
	int count = m_list.GetItemCount();

	int idx =m_list.InsertItem(count, proName);
	m_list.SetItemText(idx, 1, wndTxt);
	m_list.SetItemText(idx, 2, wndStatus);
	m_list.SetItemText(idx,3,proPath);

	m_list.EnsureVisible(idx, FALSE);
}

void CadtestDlg::AddSysData(stSysInfo& info)
{
	if (info.wndName.empty())
		return;

	CString wndTxt;
	CString wndStatus = L"";

	wndTxt =info.wndName.c_str();


	if (info.bWndStatus)
		wndStatus = L"启用";
	else
		wndStatus = L"禁用";
	int count = m_listSys.GetItemCount();

	int idx =m_listSys.InsertItem(count, wndTxt);
	m_listSys.SetItemText(idx, 1, wndStatus);

	m_listSys.EnsureVisible(idx, FALSE);
}

void CadtestDlg::OnNMRClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	if (m_list.GetSelectedCount() <= 0)  
		return;

	if(pNMItemActivate->iItem != -1){
		DWORD dwPos = GetMessagePos();
		CPoint point( LOWORD(dwPos), HIWORD(dwPos) );

		CMenu menu;
		VERIFY( menu.LoadMenu( IDR_MENU1 ) );
		CMenu* popup = menu.GetSubMenu(0);

		stAdInfo info;
		m_nCurUserItem = pNMItemActivate->iItem;
		GetInfoByPos(m_nCurUserItem,info);
		if (info.bWndStatus)
			popup->AppendMenu(MF_STRING,USRLIST_MENU_BAN,L"禁用");
		else	
			popup->AppendMenu(MF_STRING,USRLIST_MENU_START,L"启用");

		ASSERT( popup != NULL );
		popup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this );
	} 

	*pResult = 0;
}

void CadtestDlg::OnNMRClickList2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	if (m_listSys.GetSelectedCount() <= 0)  
		return;  

	if(pNMItemActivate->iItem != -1){
		DWORD dwPos = GetMessagePos();
		CPoint point( LOWORD(dwPos), HIWORD(dwPos) );

		CMenu menu;
		menu.CreatePopupMenu();

		stSysInfo info;
		m_nCurSysItem = pNMItemActivate->iItem;
		GetSysInfoByPos(m_nCurSysItem,info);
		if (info.bWndStatus)
			menu.AppendMenu(MF_STRING,SYSLIST_MENU_BAN,L"禁用");
		else	
			menu.AppendMenu(MF_STRING,SYSLIST_MENU_START,L"启用");

		::SetForegroundWindow(m_hWnd); 

		menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this );

		HMENU hmenu = menu.Detach();
		menu.DestroyMenu();
	} 

	*pResult = 0;
}

void CadtestDlg::OnDelete()
{
	if (m_list.GetSelectedCount() <= 0) 
		return;  
	int index = (int)m_list.GetFirstSelectedItemPosition() - 1;
	if (index < 0)
		return;

	stAdInfo info;
	if (!GetInfoByPos(index,info))					//获取删除数据
		return;

	if (m_bOpenInfo && m_pAdDb->DeleteUser(info)){	
		for (vector<stAdInfo>::iterator ad_iter = m_vecAdInfo.begin(); ad_iter != m_vecAdInfo.end();)
		{
			vector<stAdInfo>::difference_type dist = ad_iter - m_vecAdInfo.begin();
			if ( index == dist){
				ad_iter = m_vecAdInfo.erase(ad_iter);
				break;
			}
			else
				++ad_iter;
		}
	}
	m_list.DeleteItem(index);
}

BOOL CadtestDlg::GetInfoByPos(unsigned index,stAdInfo& info)
{
	BOOL bRet = FALSE;
	do 
	{
		bRet = TRUE;

		if (m_vecAdInfo.empty()){
			bRet = FALSE;
			break;
		}	

		if (index > m_vecAdInfo.size() || index == m_vecAdInfo.size()){
			bRet = FALSE;
			break;
		}	

		info = m_vecAdInfo.at(index);	
	} while (0);

	return bRet;
}

BOOL CadtestDlg::GetSysInfoByPos(unsigned index,stSysInfo& info)
{
	BOOL bRet = FALSE;
	do 
	{
		bRet = TRUE;

		if (m_vecAdSysInfo.empty())
		{
			bRet = FALSE;
			break;
		}	

		if (index > m_vecAdSysInfo.size() || index == m_vecAdSysInfo.size())
		{
			bRet = FALSE;
			break;
		}	

		info = m_vecAdSysInfo.at(index);	
	} while (0);

	return bRet;
}

void CadtestDlg::DeleteAllItem()
{
	BOOL bRet = FALSE;
	do 
	{
		bRet = TRUE;
		if (m_vecAdInfo.empty() || m_list.GetItemCount() == 0){
			bRet = FALSE;
			break;
		}

		for(unsigned i = 0; i < m_vecAdInfo.size(); i ++)
		{
			stAdInfo info;
			if (!GetInfoByPos(i,info)){
				bRet = FALSE;
				break;
			}

			if (!m_pAdDb->DeleteUser(info)){
				bRet = FALSE;
				break;
			}
		}

	} while (0);

	if (bRet){
		m_vecAdInfo.clear();
		while ( m_list.DeleteItem (0));
	}
}

void CadtestDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	HideMainWnd();
}

void CadtestDlg::ReadConfig()
{
	CString startString=L"";
	CString addString=L"";
	CString fbUrlString = L"";
	CString sysUrlString = L"";
	CString runString = L"";
	CString warnString = L"";
	CString des=L"";
	GetCurrentPath(des);
	des+="\\config.ini";

	GetPrivateProfileString(L"set",L"warn",L"0",warnString.GetBuffer(MAX_PATH),MAX_PATH,des);
	warnString.ReleaseBuffer();

	if (warnString == "0"){
		g_bCheckState = FALSE;
	}else{
		g_bCheckState = TRUE;
	}


	GetPrivateProfileString(L"set",L"auto",L"",startString.GetBuffer(MAX_PATH),MAX_PATH,des);
	startString.ReleaseBuffer();

	if (startString == "yes"){
		m_bAutoRun = TRUE;
		Utilities::AutoRun();
	}else{
		m_bAutoRun = FALSE;
		Utilities::NoRun();
	}

	GetPrivateProfileString(L"set",L"run",L"",runString.GetBuffer(MAX_PATH),MAX_PATH,des);
	runString.ReleaseBuffer();

	if (runString == "1"){
		m_bRunLua = TRUE;
	}else{
		m_bRunLua = FALSE;
	}


	GetPrivateProfileString(L"set",L"add",L"",addString.GetBuffer(MAX_PATH),MAX_PATH,des);
	addString.ReleaseBuffer();

	if (addString == "no"){
		m_bAddToList = FALSE;
	}else{
		m_bAddToList = TRUE;
	}

	GetPrivateProfileString(L"set",L"sysrule",L"",sysUrlString.GetBuffer(MAX_PATH),MAX_PATH,des);
	sysUrlString.ReleaseBuffer();
	if (sysUrlString != L"")
		m_strSysUrl = sysUrlString;
	else
		m_strSysUrl = L"";

	CString strUrl = L"http://www.yn1819.com/ggsq_feedback/feedback.html?sc=";

	GetPrivateProfileString(L"set",L"feedback",strUrl,fbUrlString.GetBuffer(MAX_PATH),MAX_PATH,des);
	fbUrlString.ReleaseBuffer();
	if (fbUrlString != L"")
		m_strFBUrl = fbUrlString;
	else
		m_strFBUrl = L"";

	CString strId = L"";
	GetPrivateProfileString(L"base",L"id",L"",strId.GetBuffer(MAX_PATH),MAX_PATH,des);
	strId.ReleaseBuffer();
	m_strFBUrl += strId;



}

void CadtestDlg::WriteConfig()
{
	CString startString=L"";
	CString addString=L"";
	CString warnString = L"";
	CString des=L"";
	GetCurrentPath(des);
	des+="\\config.ini";

	if (m_bAutoRun)
		startString = L"yes";
	else
		startString = L"no";

	WritePrivateProfileString(L"set",L"auto",startString,des);

	if (m_bAddToList)
		addString = L"yes";
	else
		addString = L"no";

	WritePrivateProfileString(L"set",L"add",addString,des);

	if (g_bCheckState)
		warnString = L"1";
	else
		warnString = L"0";

	WritePrivateProfileString(L"set",L"warn",warnString,des);
}

void CadtestDlg::IsPauseKill(BOOL bPause)
{
	if (bPause)
		KillTimer(LITSENWND);
	else
		SetTimer(LITSENWND,10,0);
}

BOOL CadtestDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (pMsg->message == WM_KEYDOWN)  
	{  
		switch(pMsg->wParam)  
		{  
		case VK_ESCAPE: //Esc按键事件  
		case VK_RETURN: //Enter按键事件  
			return true;  
		default:  
			;  
		}  
	}  
	return CDialogEx::PreTranslateMessage(pMsg);
}

void CadtestDlg::OnTcnSelchangeTabList(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	int CurSel = m_tabList.GetCurSel(); 

	switch(CurSel)
	{
	case 0:
		{
			if (::IsWindow(m_list.GetSafeHwnd()))
			{
				m_list.ShowWindow(SW_SHOW);
			}

			if (::IsWindow(m_listSys.GetSafeHwnd()))
			{
				m_listSys.ShowWindow(SW_HIDE);
			}

			if (::IsWindow((GetDlgItem(Test_BTN_DELETE))->GetSafeHwnd()))
			{
				(GetDlgItem(Test_BTN_DELETE))->ShowWindow(SW_SHOW);
			}

			if (::IsWindow((GetDlgItem(Test_BTN_DELETEALL))->GetSafeHwnd()))
			{
				(GetDlgItem(Test_BTN_DELETEALL))->ShowWindow(SW_SHOW);
			}

			if (::IsWindow(GetDlgItem(Test_BTN_UPDATE)->GetSafeHwnd()))
			{
				(GetDlgItem(Test_BTN_UPDATE))->ShowWindow(SW_HIDE);
			}

		}
		break;
	case 1:
		{
			if (::IsWindow(m_list.GetSafeHwnd()))
			{
				m_list.ShowWindow(SW_HIDE);
			}

			if (::IsWindow(m_listSys.GetSafeHwnd()))
			{
				m_listSys.ShowWindow(SW_SHOW);
			}

			if (::IsWindow((GetDlgItem(Test_BTN_DELETE))->GetSafeHwnd()))
			{
				(GetDlgItem(Test_BTN_DELETE))->ShowWindow(SW_HIDE);
			}

			if (::IsWindow((GetDlgItem(Test_BTN_DELETEALL))->GetSafeHwnd()))
			{
				(GetDlgItem(Test_BTN_DELETEALL))->ShowWindow(SW_HIDE);
			}

			if (::IsWindow(GetDlgItem(Test_BTN_UPDATE)->GetSafeHwnd()))
			{
				(GetDlgItem(Test_BTN_UPDATE))->ShowWindow(SW_HIDE);
			}
		}
		break;
	default:
		break;
	}
	*pResult = 0;
}

BOOL CadtestDlg::ModifyUsrWndStatus()
{
	BOOL bRet = FALSE;
	do 
	{
		bRet = TRUE;
		if ( m_nCurUserItem == -1 || m_nCurUserItem >= m_vecAdInfo.size()){
			bRet = FALSE;
			break;
		}
		CString wndStatus = L"";

		m_vecAdInfo.at(m_nCurUserItem).bWndStatus = 
			!m_vecAdInfo.at(m_nCurUserItem).bWndStatus;

		if (m_bOpenInfo)
			m_pAdDb->ModifyUser(m_vecAdInfo.at(m_nCurUserItem));

		if (m_vecAdInfo.at(m_nCurUserItem).bWndStatus)
			wndStatus = L"启用";
		else
			wndStatus = L"禁用";
		m_list.SetItemText(m_nCurUserItem, 2, wndStatus);
	} while (0);

	return bRet;
}

BOOL CadtestDlg::ModifySysWndStatus()
{
	BOOL bRet = FALSE;
	do 
	{
		bRet = TRUE;
		if ( m_nCurSysItem == -1 || m_nCurSysItem >= m_vecAdSysInfo.size()){
			bRet = FALSE;
			break;
		}
		CString wndStatus = L"";

		m_vecAdSysInfo.at(m_nCurSysItem).bWndStatus = 
			!m_vecAdSysInfo.at(m_nCurSysItem).bWndStatus;

		if (m_bOpenInfo)
			m_pAdDb->ModifySys(m_vecAdSysInfo.at(m_nCurSysItem));

		if (m_vecAdSysInfo.at(m_nCurSysItem).bWndStatus)
			wndStatus = L"启用";
		else
			wndStatus = L"禁用";
		m_listSys.SetItemText(m_nCurSysItem, 1, wndStatus);
	} while (0);

	return bRet;
}

void CadtestDlg::WriteData()
{
	do 
	{
		if (m_vecAdSysInfo.empty() || !m_bOpenInfo)
			break;

		for (vector<stSysInfo>::const_iterator cntIter = m_vecAdSysInfo.begin();
			cntIter != m_vecAdSysInfo.end();
			++cntIter)
			m_pAdDb->AddSys(*cntIter);
	} while (0);
}

void CadtestDlg::ParseSysData(const char *msg,CWnd * wnd)
{
	Json::Reader reader;
	Json::Value root;
	if (!reader.parse(msg, root, false))
	{
		return ;
	}

	Json::Value data = root["data"];
	std::string name;
	int size = data.size();
	for (int i=0; i<size; ++i)
	{
		stSysInfo info;
		info.wndName = data[i]["WND"].asString() == "" ? "":data[i]["WND"].asString();
		info.bWndStatus = data[i]["status"].asString() == "1"?TRUE:FALSE;
		if (!((CadtestDlg *)wnd)->CheckSameRule(info))
			((CadtestDlg *)wnd)->AddSysData(info);	
	}

	((CadtestDlg *)wnd)->m_bDown = TRUE;
}

void CadtestDlg::GetCurrentPath(CString& path)
{
	TCHAR szPath[MAX_PATH];
	memset(szPath,0,MAX_PATH);
	GetModuleFileName(NULL, szPath, MAX_PATH);
	path = szPath;
	path = path.Left(path.ReverseFind('\\'));
}

BOOL CadtestDlg::CheckSameRule(stSysInfo& info)
{
	for (vector<stSysInfo>::iterator iter = m_vecAdSysInfo.begin();
		iter != m_vecAdSysInfo.end();
		++iter)
	{
		if (info.wndName.compare(iter->wndName) == 0)
			return TRUE;
	}
	m_vecAdSysInfo.push_back(info);
	return FALSE;
}

BOOL CadtestDlg::CompareWndName(string& wndName)
{
	BOOL bRet = FALSE;
	do 
	{
		if (m_vecAdSysInfo.empty()){
			bRet = FALSE;
			break;
		}

		for (vector<stSysInfo>::iterator ad_iter =m_vecAdSysInfo.begin();
			ad_iter != m_vecAdSysInfo.end();
			++ad_iter )
		{
			if (wndName.find(ad_iter->wndName) != wndName.npos){
				bRet = TRUE;
				break;
			}
		}	
	} while (0);

	return bRet;
}

void CadtestDlg::KillProcessFromVector()
{
	do 
	{
		if (m_vecAdInfo.empty())
			break;

		for (vector<stAdInfo>::iterator ad_iter = m_vecAdInfo.begin();
			ad_iter != m_vecAdInfo.end(); ++ ad_iter)
		{
			if (ad_iter->proName == "" || !ad_iter->bKillProcess)
				continue;
			CString proName((ad_iter->proName).c_str());
			Utilities::KillProcessFromName(proName);
		}

	} while (0);
}

BOOL CadtestDlg::CompareWinTextAndClassName(string& winText,string & className)
{
	if (m_vecAdInfo.empty())
		return FALSE;

	for (vector<stAdInfo>::iterator ad_iter =m_vecAdInfo.begin();
		ad_iter != m_vecAdInfo.end();
		++ad_iter )
	{
		if (!ad_iter->bWndStatus)
			continue;

		if (winText.compare(ad_iter->wndName)==0)	{
			if (className.compare(ad_iter->className)==0)
				return TRUE;
		}
	}	
	return FALSE;
}
