
// PHPScanDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include <atomic>

//新类添加
#include "ZMyCallbackProc.h"
#include "ZMyFileThread.h"
#include "ZMyCompareThread.h"

//自定义消息
#define WM_ZMY_REFRESH WM_USER+1
#define WM_ZMY_GETALLFOLDER_FINISH WM_USER+2
#define WM_ZMY_GETALLFILE_FINISH WM_USER+3
#define WM_ZMY_GETALLFOLDER_EXIT WM_USER+4
#define WM_ZMY_GETALLFILE_EXIT WM_USER+5
#define WM_ZMY_COMPARE_FINISH WM_USER+6
#define WM_ZMY_COMPARE_EXIT WM_USER+7


// CPHPScanDlg 对话框
class CPHPScanDlg : public CDialogEx
{
// 构造
public:
	CPHPScanDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PHPSCAN_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


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
	afx_msg void OnAbout();
	// 初始化时设定窗口的大小
	void SetWindowDisplay();
	CButton m_buttonBrowser;
	CButton m_buttonStart;
	CButton m_buttonStop;
	CListCtrl m_listResult;
	CProgressCtrl m_progScan;
	CString m_staticCurCount;
	CString m_staticCurFile;
	CString m_staticPath;
	CString m_staticTotalCount;
	afx_msg void OnClickedButtonBrowser();
	CString GetFolderFullpath(LPCTSTR lpszDefault);
	afx_msg void OnClickedButtonStart();
	CString m_strButtonStart;
	afx_msg void OnClickedButtonStop();
	int m_intProgStart;
	int m_intProgEnd;
	int m_intProgCur;
protected:
	afx_msg LRESULT OnZmyRefresh(WPARAM wParam, LPARAM lParam);
public:
	//线程索引
//	int m_intThreadIndex;
	CZMyThread m_ctMyThread;
	CZMyFileThread m_ctMyFileThread;
	CZMyCompareThread m_ctMyCompareThread;
	vector<CWinThread*> m_ctThread;
	HANDLE m_hEvent;
	vector<CString> m_allFolders;
protected:
	afx_msg LRESULT OnZmyGetallfolderFinish(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	enum m_emIDTimer
	{
		ID_TIMER_REFRESH = 100,
	};
	int m_intRunTime;
	CString m_staticRunTime;
	int m_ctThreadFlag;
	int GetRunTime();
protected:
	afx_msg LRESULT OnZmyGetallfileFinish(WPARAM wParam, LPARAM lParam);
public:
	// 存储查询文件结果
	vector<CString> m_vcAllFileResult;
	CString m_staticTotalCountFile;
	int m_intThreadFinshed;
protected:
	afx_msg LRESULT OnZmyGetallfolderExit(WPARAM wParam, LPARAM lParam);
//	afx_msg LRESULT OnZmygetallfileExit(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnZmyGetallfileExit(WPARAM wParam, LPARAM lParam);
public:
	// //控件信息初始化
	int InitControl();
//	int m_cmThreadCount;
	afx_msg void OnCbnSelchangeComboThreadCount();
	CString m_cmThreadCount;
	CString m_strWorkDir;
	CString m_strPHPTrojanFeatureFile;
	CString m_strTrojanFeatureFullPath;
	int InitConfig();
	vector<CString> m_vcPHPTrajonFeature;
	vector<CString> m_vcExtName;
	int InitExtNameVC();
	int InitListResult();
	CButton m_cbCheckPHP;
	CButton m_cbCheckINC;
	CButton m_cbCheckTXT;
protected:
	afx_msg LRESULT OnZmyCompareFinish(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnZmyCompareExit(WPARAM wParam, LPARAM lParam);
public:
	vector<CString> m_vcAllTrajonResult;
	afx_msg void OnSize(UINT nType, int cx, int cy);
	POINT m_old;
	void ReSize();
};
