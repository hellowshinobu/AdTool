#pragma once
class Utilities
{
public:
	Utilities(void);
	~Utilities(void);
private:
	static vector<CString> m_vecProtectedPro;				//�ܱ���������
public:
	
	/*!
	function:KillProcessFromHWnd
	describe:ͨ�����ھ����������
	para:hwnd ���ھ��
	return: null
	*/
	static void KillProcessFromHWnd(HWND hwnd);
	
	/*!
	function:KillProcessFromName
	describe:ͨ����������������
	para:proName ������
	return: null
	*/
	static void KillProcessFromName(CString proName);

	/*!
	function:FindProcess
	describe:ͨ�����������ҽ���ID
	para:strProcessName ������
	return: �ɹ�,���ؽ���ID��
			   ʧ��,����0;
	*/
	static DWORD FindProcess(CString strProcessName);

	/*!
	function:GetProcessNameAndPath
	describe:ͨ�����ھ����ȡ����·���ͽ�����
	para:hWnd ���ʵĴ��ھ��
			path[out] ����·��
			proName[out] ������
	return:	�ɹ�,TRUE;
				ʧ��,FALSE;
	*/
	static BOOL GetProcessNameAndPath(HWND hWnd, CString& path, CString& proName);

	/*!
	function:PaternMatch
	describe:ƥ������ַ����Ƿ����
	para:	   pat ������
			   str	 �����еĽ�����
	return: �ɹ�,TRUE��
			   ʧ��,FALSE;
	*/
	static BOOL PaternMatch(const TCHAR *pat, const TCHAR *str);

	/*!
	function:AutoRun
	describe:��������
	return: null
	*/
	static void AutoRun();
	
	/*!
	function:AutoRun
	describe:�رտ�������
	return: null
	*/
	static void NoRun();
	
	/*!
	function:IsSysWindow
	describe:�ж��Ƿ���ϵͳ���ڣ�����һЩ�������
	para:	 proName ��������
	return: �ɹ�����TRUE
			   ʧ�ܷ���FALSE
	*/
	static BOOL IsSysWindow(CString& proName);

	/*!
	function:InitProtectExe
	describe:��ʼ��ϵͳ���ڣ�����һЩ�������
	return: null
	*/
	static void InitProtectExe();
};

