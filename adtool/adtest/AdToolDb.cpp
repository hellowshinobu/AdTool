#include "StdAfx.h"
#include "AdToolDb.h"
#include "CharTransform.h"

int	 UserResult(void *para, int argc, char **argv, char **azColName);
int	 SysResult(void *para, int argc, char **argv, char **azColName);

AdToolDb::AdToolDb(void)
{
}


AdToolDb::~AdToolDb(void)
{
}

BOOL AdToolDb::InitDb(CString szFilePath)
{
	m_pDB = NULL;
	BOOL bRet = FALSE;
	do 
	{
		bRet = TRUE;
		if (!PathFileExists(szFilePath))
		{
			string path = CharTransform::WideByte2Acsi(szFilePath.GetString());

			int rc = sqlite3_open(path.c_str(),&m_pDB);
			if (rc != SQLITE_OK)
			{
				sqlite3_close(m_pDB);
				bRet =FALSE;
				break;
			}
			BOOL result1 = CreateUserDb();
			BOOL result2 = CreateSysDb();

			if (!result1 || !result2 )
				bRet = FALSE;
		}
		else
		{
			string path = CharTransform::WideByte2Acsi(szFilePath.GetString());
			int rc = sqlite3_open(path.c_str(),&m_pDB);
			if (rc != SQLITE_OK)
			{
				sqlite3_close(m_pDB);
				bRet =FALSE;
				break;
			}
		}		
	}while (0);

	return bRet;
}

BOOL AdToolDb::CreateUserDb()
{
	string sql = "create table adinfo ( \
				 id integer primary key autoincrement, \
				 wndname varchar(100), \
				 classname varchar(100), \
				 proname varchar(100), \
				 company varchar(100), \
				 killprocess boolean default(0), \
				 propath	varchar(100)	,\
				 wndstatus boolean default(1)\
				 );";

	char* cErrMsg;  
	int nRes = sqlite3_exec(m_pDB , sql.c_str() ,0 ,0, &cErrMsg);  
	if (nRes != SQLITE_OK)    
	{  
		return FALSE;
	}  

	return TRUE;
}

BOOL AdToolDb::CreateSysDb()
{
	string sql = "create table sysinfo ( \
				 wndname varchar(100) primary key, \
				 wndstatus boolean default(1)\
				 );";

	char* cErrMsg;  
	int nRes = sqlite3_exec(m_pDB , sql.c_str() ,0 ,0, &cErrMsg);  
	if (nRes != SQLITE_OK)    
	{  
		return FALSE;
	}  
	return TRUE;
}

BOOL AdToolDb::AddUser(const stAdInfo& info)
{
	if (info.wndName.empty() && info.className.empty())
		return FALSE;


	string utfwndName = info.wndName;
	string utf8proName = info.proName;
	string utf8proPath   = info.proPath;

	wstring tmp1 = CharTransform::Acsi2WideByte(utfwndName);
	utfwndName = CharTransform::Unicode2Utf8(tmp1);

	wstring tmp2 = CharTransform::Acsi2WideByte(utf8proName);
	utf8proName = CharTransform::Unicode2Utf8(tmp2);

	wstring tmp3 = CharTransform::Acsi2WideByte(utf8proPath);
	utf8proPath = CharTransform::Unicode2Utf8(tmp3);

	string addSql = "insert into adinfo(wndname,classname,proname,killprocess,propath,wndstatus) values('";
	addSql += utfwndName;
	addSql += "','";
	addSql += info.className;
	addSql += "','";
	addSql += utf8proName;
	addSql += "',";
	char buff[2];
	ZeroMemory(buff,2);
	_itoa_s(info.bKillProcess,buff,10);
	addSql += buff;
	addSql += ",'";
	addSql += utf8proPath;
	addSql += "',";
	ZeroMemory(buff,2);
	_itoa_s(info.bWndStatus,buff,10);
	addSql += buff;
	addSql += ");";
	//UnicodeToMB(sql,s);

	char* cErrMsg;  
	int nRes = sqlite3_exec(m_pDB , addSql.c_str() ,0 ,0, &cErrMsg);  
	if (nRes != SQLITE_OK)    
	{  
		return FALSE;
	}  

	return TRUE;
}

BOOL AdToolDb::AddSys(const stSysInfo& info)
{
	if (info.wndName.empty())
		return FALSE;


	string utfwndName = info.wndName;


	wstring tmp1 = CharTransform::Acsi2WideByte(utfwndName);
	utfwndName = CharTransform::Unicode2Utf8(tmp1);


	string addSql = "insert into sysinfo(wndname,wndstatus) values('";
	addSql += utfwndName;
	addSql += "',";
	char buff[2];
	ZeroMemory(buff,2);
	_itoa_s(info.bWndStatus,buff,10);
	addSql += buff;
	addSql += ");";

	char* cErrMsg;  
	int nRes = sqlite3_exec(m_pDB , addSql.c_str() ,0 ,0, &cErrMsg);  
	if (nRes != SQLITE_OK)    
	{  
		return FALSE;
	}  

	return TRUE;
}

int UserResult(void *para, int argc, char **argv, char **azColName)
{
	stAdInfo info;
	info.id = atoi(argv[0]);
	if (argv[1] != NULL)
	{
		info.wndName = argv[1];
		wstring wstr(L"");
		wstr = CharTransform::Utf82Unicode(info.wndName);
		info.wndName = CharTransform::WideByte2Acsi(wstr);
	}

	if (argv[2] != NULL)
	{
		info.className = argv[2];
	}	

	if (argv[3] != NULL)
	{
		info.proName = argv[3];
		wstring wstr(L"");
		wstr = CharTransform::Utf82Unicode(info.proName);
		info.proName = CharTransform::WideByte2Acsi(wstr);
	}

	if (argv[4] != NULL)
	{
		info.proCompanyName = argv[4];		
	}	

	if (argv[5] != NULL)
	{
		info.bKillProcess = atoi(argv[5]);		
	}	

	if (argv[6] != NULL)
	{
		info.proPath = argv[6];
		wstring wstr(L"");
		wstr = CharTransform::Utf82Unicode(info.proPath);
		info.proPath = CharTransform::WideByte2Acsi(wstr);
	}

	if (argv[7] != NULL)
	{
		info.bWndStatus = atoi(argv[7]);		
	}	

	vector<stAdInfo> &vecInfo = *(vector<stAdInfo> *)para;
	vecInfo.push_back(info);
	return 0;
}

int SysResult(void *para, int argc, char **argv, char **azColName) 
{

	stSysInfo info;

	if (argv[0] != NULL)
	{
		info.wndName = argv[0];
		wstring wstr(L"");
		wstr = CharTransform::Utf82Unicode(info.wndName);
		info.wndName = CharTransform::WideByte2Acsi(wstr);
	}

	if (argv[1] != NULL)
	{
		info.bWndStatus = atoi(argv[1]);		
	}	

	vector<stSysInfo> &vecInfo = *(vector<stSysInfo> *)para;
	vecInfo.push_back(info);

	return 0;
}


BOOL AdToolDb::SelectUser(vector<stAdInfo>& vecInfo)
{
	char* cErrMsg;  
	int res = sqlite3_exec(m_pDB, "select * from adinfo;", UserResult , &vecInfo , &cErrMsg);    

	if (res != SQLITE_OK)  
	{ 
		return FALSE;  
	}  

	return TRUE;
}

BOOL AdToolDb::SelectLastUser(vector<stAdInfo>& vecInfo)
{

	char* cErrMsg;  
	int res = sqlite3_exec(m_pDB, "select * from adinfo order by id desc limit 1;", UserResult , &vecInfo , &cErrMsg);    

	if (res != SQLITE_OK)  
	{ 
		return FALSE;  
	}  

	return TRUE;

}

BOOL AdToolDb::DeleteUser(const stAdInfo& info)
{
	if (info.className.empty())
		return FALSE;

	string addSql = "delete from adinfo where classname='";
	addSql += info.className;
	addSql += "' and id=";
	char buff[MAX_PATH];
	ZeroMemory(buff,MAX_PATH);
	_itoa_s(info.id,buff,10);
	addSql += buff;
	addSql += ";";

	char* cErrMsg;  
	int nRes = sqlite3_exec(m_pDB , addSql.c_str() ,0 ,0, &cErrMsg);  
	if (nRes != SQLITE_OK)    
	{  
		return FALSE;
	}  

	return TRUE;
}

BOOL AdToolDb::ModifyUser(const stAdInfo& info)
{
	if (info.id == -1)
		return FALSE;
	string addSql = "update adinfo set wndstatus =";
	char buff[MAX_PATH];
	ZeroMemory(buff,MAX_PATH);
	_itoa_s(info.bWndStatus,buff,10);
	addSql += buff;
	addSql += " where id=";
	ZeroMemory(buff,MAX_PATH);
	_itoa_s(info.id,buff,10);
	addSql += buff;
	addSql += ";";

	char* cErrMsg;  
	int nRes = sqlite3_exec(m_pDB , addSql.c_str() ,0 ,0, &cErrMsg);  
	if (nRes != SQLITE_OK)    
	{  
		return FALSE;
	}  

	return TRUE;
}

BOOL AdToolDb::ModifySys(const stSysInfo& info)
{
	if (info.wndName.empty())
	{
		return FALSE;
	}

	string utfwndName = info.wndName;


	wstring tmp1 = CharTransform::Acsi2WideByte(utfwndName);
	utfwndName = CharTransform::Unicode2Utf8(tmp1);

	string addSql = "update sysinfo set wndstatus =";
	char buff[MAX_PATH];
	ZeroMemory(buff,MAX_PATH);
	_itoa_s(info.bWndStatus,buff,10);
	addSql += buff;
	addSql += " where wndname='";
	addSql += utfwndName;
	addSql += "';";

	char* cErrMsg;  
	int nRes = sqlite3_exec(m_pDB , addSql.c_str() ,0 ,0, &cErrMsg);  
	if (nRes != SQLITE_OK)    
	{  
		return FALSE;
	}  

	return TRUE;
}

BOOL AdToolDb::SelectSys(vector<stSysInfo>& vecInfo)
{
	char* cErrMsg;  
	int res = sqlite3_exec(m_pDB, "select * from sysinfo;", SysResult, &vecInfo, &cErrMsg);    

	if (res != SQLITE_OK)  
	{ 
		return FALSE;  
	}  

	return TRUE;
}

void AdToolDb::CloseDb()
{
	if (m_pDB != NULL)
	{
		sqlite3_close(m_pDB);
	}
}


