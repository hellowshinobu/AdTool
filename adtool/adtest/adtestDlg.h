
// adtestDlg.h : 头文件
//

#pragma once


#include "global.h"
#include "afxcmn.h"
#include "AdToolDb.h"

typedef struct _tagSysParam
{
	_tagSysParam()
		:	m_pWnd(NULL)
		,	m_strUrl(L"")
	{
	}
	
	CWnd * m_pWnd;
	CString m_strUrl;
}stSysParam;

// CadtestDlg 对话框


class CAdTipsDlg;
class CadtestDlg : public CDialogEx
{
// 构造
public:
	CadtestDlg(CWnd* pParent = NULL);	// 标准构造函数
	~CadtestDlg();

	/*!
	function:解析json数据
	param: msg  解析内容
	param: wnd  窗口指针
	return: null
	*/
	void ParseSysData(const char *msg,CWnd * wnd);

	void CreateTray();
	
	/*!
	function:开始捕获窗口
	return: null
	*/
	void FindAdWindow();
	void DrawAdRect();

	void ReadConfig();
	void WriteConfig();
	void WriteData();
	void InitMyList();
	void GetCurrentPath(CString& path);

	/*!
	function:ShowMainWnd
	describe:显示屏蔽列表
	return: null
	*/
	void ShowMainWnd();
	void HideMainWnd();
	void IsPauseKill(BOOL bPause);
	void AddData(stAdInfo& info);
	void AddSysData(stSysInfo& info);
	BOOL ModifyUsrWndStatus();
	BOOL ModifySysWndStatus();
	BOOL GetInfoByPos(unsigned index,stAdInfo& info);
	BOOL GetSysInfoByPos(unsigned index,stSysInfo& info);
	BOOL CheckDB(CString szFilePath);
	BOOL CheckSameRule(stSysInfo& info);
	void DeleteAllItem();
	
	void KillProcessFromVector();
	BOOL CompareWinTextAndClassName(string& winText,string& className);
	BOOL CompareWndName(string& wndName);

// 对话框数据
	enum { IDD = IDD_ADTEST_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
public:
//回调封装
	static BOOL CALLBACK lpEnumFunc(HWND hwnd, LPARAM lParam);
	
private:
	NOTIFYICONDATA m_nid;
	HWND m_hwndPointOld;
	CAdTipsDlg* m_dlgTips;
	AdToolDb* m_pAdDb;
	BOOL m_bOpenInfo;
	BOOL m_bAddToList;
	BOOL m_bAutoRun;
	BOOL m_bRunLua;
	BOOL m_bIsSameRule;
	CString m_strFBUrl;
	CString m_strSysUrl;
	CListCtrl m_list;
	CListCtrl m_listSys;
	CTabCtrl m_tabList;

	int m_nCurUserItem;
	int m_nCurSysItem;
	vector<stAdInfo>  m_vecAdInfo;						//用户规则数组
	vector<stSysInfo> m_vecAdSysInfo;					//系统规则数组

	static string m_strBase64New;
	static string m_strBase64Gg;
public:
	BOOL m_bDown;
// 实现
protected:
	HICON m_hIcon;
	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	 LRESULT OnShowTask(WPARAM wParam, LPARAM lParam);
	 LRESULT OnTipsMoveLeave(WPARAM wParam, LPARAM lParam);
	 LRESULT OnTipsClickOk(WPARAM wParam, LPARAM lParam);
	 LRESULT OnTipsClickCancel(WPARAM wParam, LPARAM lParam);
	 afx_msg void OnSize(UINT nType, int cx, int cy);
	 virtual BOOL DestroyWindow();
	 afx_msg void OnTimer(UINT_PTR nIDEvent);
	 virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	 afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	 afx_msg void OnNMRClickList1(NMHDR *pNMHDR, LRESULT *pResult);
	 afx_msg void OnNMRClickList2(NMHDR *pNMHDR, LRESULT *pResult);
	 afx_msg void OnDelete();
	 afx_msg void OnClose();
	 virtual BOOL PreTranslateMessage(MSG* pMsg);
	
	 afx_msg void OnTcnSelchangeTabList(NMHDR *pNMHDR, LRESULT *pResult);
};
