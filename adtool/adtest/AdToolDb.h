#pragma once
#include "sqlite3.h"

class AdToolDb
{
public:
	AdToolDb(void);
	~AdToolDb(void);
	
	/*!
	function:InitDb
	describe:初始化数据库
	param:szFilePath 数据库路径
	return: TRUE 成功 FALSE 失败
	*/
	BOOL InitDb(CString szFilePath);
	void CloseDb();
	
	BOOL AddSys(const stSysInfo& info);
	BOOL ModifySys(const stSysInfo& info);
	BOOL SelectSys(vector<stSysInfo>& vecInfo);


	//用户规则相关数据库操作
	BOOL AddUser(const stAdInfo& info);
	BOOL DeleteUser(const stAdInfo& info);  
	BOOL ModifyUser(const stAdInfo& info);  
	BOOL SelectUser(vector<stAdInfo>& vecInfo);
	BOOL SelectLastUser(vector<stAdInfo>& vecInfo);

private:
	BOOL CreateUserDb();
	BOOL CreateSysDb();

private:
	sqlite3 * m_pDB;  
	stAdInfo m_stInfo;
};

