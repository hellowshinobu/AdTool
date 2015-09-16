#pragma once
#include "sqlite3.h"

class AdToolDb
{
public:
	AdToolDb(void);
	~AdToolDb(void);
	
	/*!
	function:InitDb
	describe:��ʼ�����ݿ�
	param:szFilePath ���ݿ�·��
	return: TRUE �ɹ� FALSE ʧ��
	*/
	BOOL InitDb(CString szFilePath);
	void CloseDb();
	
	BOOL AddSys(const stSysInfo& info);
	BOOL ModifySys(const stSysInfo& info);
	BOOL SelectSys(vector<stSysInfo>& vecInfo);


	//�û�����������ݿ����
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

