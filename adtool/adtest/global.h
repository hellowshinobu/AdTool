#pragma  once
#include <string>

using namespace std;

extern BOOL g_bCheckState;
extern ULONG_PTR g_ulToken;



typedef  struct _stAdInfo
{
	int	  id;
	BOOL bKillProcess;
	BOOL bWndStatus;
	string wndName;
	string className;
	string proName;
	string proCompanyName;
	string proPath;

	_stAdInfo()
		:	id(-1)
		,	bKillProcess(FALSE)
		,   bWndStatus(TRUE)
		,	wndName("")
		,	className("")
		,	proName("")
		,	proCompanyName("")
		,	proPath("")
	{
	}
}stAdInfo;

typedef  struct _stSysInfo
{
	BOOL bWndStatus;
	string wndName;

	_stSysInfo()
		:	 bWndStatus(TRUE)
		,	 wndName("")

	{
	}
}stSysInfo;

typedef enum _ButtonPos
{
	POS_LEFTTOP = 30,
	POS_RIGHTTOP,
	POS_LEFTDOWN,
	POS_RIGHTDOWN,
	POS_FULLSCREEN
}ButtonPos;

struct IMG_BTN
{
	Bitmap* pImg;
	int nStatus;
	int nWidth;
	int nHeight;
	CRect rtArea;
	IMG_BTN()
	{
		pImg = NULL;
		nStatus = 0;
		nWidth = 0;
		nHeight = 0;
		rtArea = CRect(0,0,0,0);
	}
};

#define  SQLITEDB						L"\\addata.db"


#define TIPSTIMER						  WM_USER + 501
#define LITSENWND                      WM_USER +  502
#define WM_SHOWADLIST            WM_USER +  503
#define WM_ISADDTOLIST				WM_USER + 504
#define WM_ISAUTORUN				 WM_USER +	505
#define WM_EXIT							WM_USER + 	506
#define WM_FEEDBACK				WM_USER + 	507

#define  WM_SHOWTASK					WM_USER + 1000
#define  WM_TIPMOUSEMOVE			WM_USER + 1001
#define  WM_TIPCLICKOK					WM_USER + 1002
#define  WM_TIPCLICKCANCEL			WM_USER + 1003

#define	  EXENAME							L"广告神器"
#define  BALLONTIP							L"广告神器已启动。\n单击托盘图标可以捕获并屏蔽桌面窗口！"
