#pragma once
class Utilities
{
public:
	Utilities(void);
	~Utilities(void);
private:
	static vector<CString> m_vecProtectedPro;				//受保护的数组
public:
	
	/*!
	function:KillProcessFromHWnd
	describe:通过窗口句柄结束进程
	para:hwnd 窗口句柄
	return: null
	*/
	static void KillProcessFromHWnd(HWND hwnd);
	
	/*!
	function:KillProcessFromName
	describe:通过进程名结束进程
	para:proName 进程名
	return: null
	*/
	static void KillProcessFromName(CString proName);

	/*!
	function:FindProcess
	describe:通过进程名查找进程ID
	para:strProcessName 进程名
	return: 成功,返回进程ID；
			   失败,返回0;
	*/
	static DWORD FindProcess(CString strProcessName);

	/*!
	function:GetProcessNameAndPath
	describe:通过窗口句柄获取进程路径和进程名
	para:hWnd 访问的窗口句柄
			path[out] 进程路径
			proName[out] 进程名
	return:	成功,TRUE;
				失败,FALSE;
	*/
	static BOOL GetProcessNameAndPath(HWND hWnd, CString& path, CString& proName);

	/*!
	function:PaternMatch
	describe:匹配进程字符串是否相等
	para:	   pat 进程名
			   str	 快照中的进程名
	return: 成功,TRUE；
			   失败,FALSE;
	*/
	static BOOL PaternMatch(const TCHAR *pat, const TCHAR *str);

	/*!
	function:AutoRun
	describe:开机启动
	return: null
	*/
	static void AutoRun();
	
	/*!
	function:AutoRun
	describe:关闭开机启动
	return: null
	*/
	static void NoRun();
	
	/*!
	function:IsSysWindow
	describe:判断是否是系统窗口（附加一些浏览器）
	para:	 proName 窗口名称
	return: 成功返回TRUE
			   失败返回FALSE
	*/
	static BOOL IsSysWindow(CString& proName);

	/*!
	function:InitProtectExe
	describe:初始化系统窗口（附加一些浏览器）
	return: null
	*/
	static void InitProtectExe();
};

