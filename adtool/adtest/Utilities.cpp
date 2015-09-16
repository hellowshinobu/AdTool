#include "StdAfx.h"
#include "Utilities.h"
#include <TlHelp32.h>

vector<CString> Utilities::m_vecProtectedPro(0);

Utilities::Utilities(void)
{
}


Utilities::~Utilities(void)
{
}

void Utilities::KillProcessFromName(CString proName)
{
	DWORD dwProcessID = FindProcess(proName);
	if (dwProcessID > 0)
	{
		HANDLE hProcess = ::OpenProcess(PROCESS_TERMINATE, FALSE, dwProcessID);
		::TerminateProcess(hProcess, 0);
		CloseHandle(hProcess);
	}
}

DWORD Utilities::FindProcess(CString strProcessName)
{
	HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	PROCESSENTRY32 pe;
	pe.dwSize = sizeof(PROCESSENTRY32);

	if (!Process32First(hSnapShot, &pe))
	{
		return 0;
	}

	strProcessName.MakeLower();

	while (Process32Next(hSnapShot, &pe))
	{
		CString scTmp = pe.szExeFile;
		scTmp.MakeLower();

		if (PaternMatch(strProcessName.GetString(), scTmp.GetString()))
		{
			DWORD dwProcessID = pe.th32ProcessID;
			CloseHandle(hSnapShot);
			return dwProcessID;
		}
		scTmp.ReleaseBuffer();
	}
	strProcessName.ReleaseBuffer();

	CloseHandle(hSnapShot);

	return 0;
}

BOOL Utilities::PaternMatch(const TCHAR *pat, const TCHAR *str)
{
	const TCHAR *s = NULL;
	const TCHAR *p = NULL;
	bool star = FALSE;
	bool bBreak = FALSE;
	do
	{
		bBreak = FALSE;
		for (s = str, p = pat; *s; ++s, ++p)
		{
			switch (*p)
			{
			case '?':
				break;
			case '*':
				star = TRUE; //出现*匹配符
				str = s;
				pat = p;
				if (!*++pat)
					return TRUE;
				bBreak = TRUE; //退出循环
				break;
			default:
				if (*s != *p)
				{
					if (!star)
						return FALSE;
					str++;
					bBreak = TRUE;
				}
				break;
			}
			if (bBreak) //退出循环 重新开始循环
				break;
		}
		if (bBreak == FALSE)
		{
			if (*p == '*')
				++p;
			return (!*p);
		}
	} while (TRUE);
}

void Utilities::AutoRun()
{
	CString keyStr;
	CString fileName;
	HKEY hRegKey;
	keyStr =_T("Software\\Microsoft\\Windows\\CurrentVersion\\Run");
	if(::RegOpenKey(HKEY_LOCAL_MACHINE,keyStr,&hRegKey) !=  ERROR_SUCCESS)
		return; 
	else
	{
		//获得fileName
		TCHAR pFileName[MAX_PATH] = {0};   
		//得到程序自身的全路径   
		DWORD dwRet = GetModuleFileName(NULL, pFileName, MAX_PATH);   

		if(::RegSetValueEx(hRegKey,
			EXENAME,
			0,
			REG_SZ,
			(CONST BYTE *)pFileName,
			MAX_PATH) != ERROR_SUCCESS)
		{
			RegCloseKey(hRegKey); 
			return;
		}
		RegCloseKey(hRegKey); 
	}
}

void Utilities::NoRun()
{
	CString keyStr;
	HKEY hRegKey;
	keyStr =_T("Software\\Microsoft\\Windows\\CurrentVersion\\Run");
	if(::RegOpenKey(HKEY_LOCAL_MACHINE,keyStr,&hRegKey) !=  ERROR_SUCCESS)
		return; 
	else
	{
		//获得fileName
		//_wsplitpath(strPath.GetBuffer(0),NULL,NULL,fileName.GetBufferSetLength(MAX_PATH+1),NULL);
		if(::RegDeleteValue(hRegKey,
			EXENAME
			) != ERROR_SUCCESS)
		{
			RegCloseKey(hRegKey); 
			return;
		}

		RegCloseKey(hRegKey); 
	}
}

BOOL Utilities::IsSysWindow(CString& proName)
{
	BOOL bRet = FALSE;
	vector<CString>::iterator iter = m_vecProtectedPro.begin();
	for (;iter != m_vecProtectedPro.end(); ++iter)
	{
		if (proName.CompareNoCase(*iter) == 0){
			bRet = TRUE;
			break;
		}
	}
	return bRet;
}

void Utilities::InitProtectExe()
{
	m_vecProtectedPro.push_back(L"IEXPLORE.EXE");
	m_vecProtectedPro.push_back(L"CHROME.EXE");
	m_vecProtectedPro.push_back(L"360SE.EXE");
	m_vecProtectedPro.push_back(L"SOGOUEXPLORER.EXE");
	m_vecProtectedPro.push_back(L"explorer.exe");
	m_vecProtectedPro.push_back(L"NOTEPAD.EXE");
	m_vecProtectedPro.push_back(L"TASKMGR.EXE");
	m_vecProtectedPro.push_back(L"DLLHOST.EXE");
}

BOOL Utilities::GetProcessNameAndPath(HWND hWnd, CString& path, CString& proName)
{
	BOOL bRet = FALSE;
	do 
	{
		ULONG nProcessId = 0;
		HANDLE	hProcessHandle = NULL;
		bRet = TRUE;
		if (hWnd == NULL)
		{
			bRet = FALSE;
			break;
		}
		::GetWindowThreadProcessId(hWnd,&nProcessId);

		hProcessHandle = ::OpenProcess(PROCESS_ALL_ACCESS,FALSE,(DWORD)nProcessId);

		TCHAR szPath[MAX_PATH];
		memset(szPath,0,MAX_PATH);
		GetModuleFileNameEx(hProcessHandle, NULL, szPath, MAX_PATH);
		path = szPath;

		int index1 = path.ReverseFind('\\');
		proName  = path.Mid(index1+1, path.GetLength() - index1);

	} while (0);

	return bRet;
}

void Utilities::KillProcessFromHWnd(HWND hwnd)
{
	do 
	{
		ULONG nProcessId = 0;
		HANDLE	hProcessHandle = NULL;

		if (hwnd == NULL)
			break;

		::GetWindowThreadProcessId(hwnd,&nProcessId);
		hProcessHandle = ::OpenProcess(PROCESS_TERMINATE,FALSE,(DWORD)nProcessId);
		if(hProcessHandle!=0)
		{ 
			if(::TerminateProcess(hProcessHandle,1)==0) //关闭程序
			{
				printf("End Proc:%d\n",(int)nProcessId);
				break;
			}
		}
	} while (0);
}
