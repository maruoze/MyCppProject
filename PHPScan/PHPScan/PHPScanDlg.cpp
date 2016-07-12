
// PHPScanDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "PHPScan.h"
#include "PHPScanDlg.h"
#include "afxdialogex.h"

#include "ZMyFile.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CPHPScanDlg 对话框



CPHPScanDlg::CPHPScanDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_PHPSCAN_DIALOG, pParent)
	, m_staticCurCount(_T(""))
	, m_staticCurFile(_T(""))
	, m_staticPath(_T(""))
	, m_staticTotalCount(_T(""))
	, m_intRunTime(0)
	, m_staticRunTime(_T(""))
	, m_ctThreadFlag(0)
	, m_staticTotalCountFile(_T(""))
	, m_intThreadFinshed(0)
	, m_cmThreadCount(_T("5"))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_strButtonStart = _T("");
	m_intProgStart = 0;
	m_intProgEnd = 100;
	//  m_intThreadIndex = 0;
	m_strWorkDir = _T("");
	m_strPHPTrojanFeatureFile = _T("");
	m_strTrojanFeatureFullPath = _T("");
}

void CPHPScanDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_BROWSER, m_buttonBrowser);
	DDX_Control(pDX, IDC_BUTTON_START, m_buttonStart);
	DDX_Control(pDX, IDC_BUTTON_STOP, m_buttonStop);
	DDX_Control(pDX, IDC_LIST_RESULT, m_listResult);
	DDX_Control(pDX, IDC_PROGRESS_SCAN, m_progScan);
	DDX_Text(pDX, IDC_STATIC_CUR_COUNT, m_staticCurCount);
	DDX_Text(pDX, IDC_STATIC_CUR_FILE, m_staticCurFile);
	DDX_Text(pDX, IDC_STATIC_PATH, m_staticPath);
	DDX_Text(pDX, IDC_STATIC_TOTAL_COUNT, m_staticTotalCount);
	DDX_Text(pDX, IDC_STATIC_RUNTIME, m_staticRunTime);
	DDX_Text(pDX, IDC_STATIC_TOTAL_COUNT_FILE, m_staticTotalCountFile);
	//  DDX_CBIndex(pDX, IDC_COMBO_THREAD_COUNT, m_cmThreadCount);
	DDX_CBString(pDX, IDC_COMBO_THREAD_COUNT, m_cmThreadCount);
	DDX_Control(pDX, IDC_CHECK_PHP, m_cbCheckPHP);
	DDX_Control(pDX, IDC_CHECK_INC, m_cbCheckINC);
	DDX_Control(pDX, IDC_CHECK_TXT, m_cbCheckTXT);
}

BEGIN_MESSAGE_MAP(CPHPScanDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_COMMAND(ID_ABOUT, &CPHPScanDlg::OnAbout)
	ON_BN_CLICKED(IDC_BUTTON_BROWSER, &CPHPScanDlg::OnClickedButtonBrowser)
	ON_BN_CLICKED(IDC_BUTTON_START, &CPHPScanDlg::OnClickedButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_STOP, &CPHPScanDlg::OnClickedButtonStop)
	ON_MESSAGE(WM_ZMY_REFRESH, &CPHPScanDlg::OnZmyRefresh)
	ON_MESSAGE(WM_ZMY_GETALLFOLDER_FINISH, &CPHPScanDlg::OnZmyGetallfolderFinish)
	ON_WM_TIMER()
	ON_MESSAGE(WM_ZMY_GETALLFILE_FINISH, &CPHPScanDlg::OnZmyGetallfileFinish)
	ON_MESSAGE(WM_ZMY_GETALLFOLDER_EXIT, &CPHPScanDlg::OnZmyGetallfolderExit)
	ON_MESSAGE(WM_ZMY_GETALLFILE_EXIT, &CPHPScanDlg::OnZmyGetallfileExit)
	ON_CBN_SELCHANGE(IDC_COMBO_THREAD_COUNT, &CPHPScanDlg::OnCbnSelchangeComboThreadCount)
END_MESSAGE_MAP()


// CPHPScanDlg 消息处理程序

BOOL CPHPScanDlg::OnInitDialog()
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	// 重设
	this->SetWindowDisplay();
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CPHPScanDlg::OnSysCommand(UINT nID, LPARAM lParam)
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
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CPHPScanDlg::OnPaint()
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
HCURSOR CPHPScanDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CPHPScanDlg::OnAbout()
{
	// TODO: 在此添加命令处理程序代码
	MessageBox(L"测试弹出", L"关于",MB_OK);
}


// 初始化时设定窗口的大小
void CPHPScanDlg::SetWindowDisplay()
{
	//设定窗口大小
	int screenWidth = ::GetSystemMetrics(SM_CXSCREEN);
	int screenHeight= ::GetSystemMetrics(SM_CYSCREEN);
	int windowWidth = 1200;
	int windowHeight = 700;
	int x = (screenWidth - windowWidth) / 2;
	int y = (screenHeight - windowHeight) / 2;
	CRect widowRect = CRect(x,y,windowWidth,windowHeight);
	this->SetWindowPos(NULL, 0, 0, widowRect.Width(), widowRect.Height(), SWP_NOZORDER | SWP_NOMOVE);
	//初始化相关控件
	m_staticTotalCount.Format(L"%d", 0);
	m_staticPath = L"E:\\";
	m_cbCheckPHP.SetCheck(1);
	this->InitConfig();
	this->InitControl();
	this->InitListResult();
	UpdateData(false);
}


void CPHPScanDlg::OnClickedButtonBrowser()
{
	// TODO: 在此添加控件通知处理程序代码
	m_staticPath=this->GetFolderFullpath(L"C:\\");
	if (m_staticPath != "") {
		m_buttonStart.EnableWindow(true);
	}	
	UpdateData(false);
}


CString CPHPScanDlg::GetFolderFullpath(LPCTSTR lpszDefault)
{
	TCHAR buffDisplayName[MAX_PATH];
	TCHAR fullpath[MAX_PATH];
	BROWSEINFO  browseinfo;
	LPITEMIDLIST lpitemidlist;

	ZeroMemory(&browseinfo, sizeof(BROWSEINFO));
	browseinfo.pszDisplayName = buffDisplayName;
	browseinfo.lpszTitle = _T("请选择目录");
	browseinfo.ulFlags = BIF_RETURNONLYFSDIRS;
	browseinfo.lParam = (LPARAM)lpszDefault;
	browseinfo.lpfn = CZMyCallbackProc::BrowseCallbackProc;

	if (!(lpitemidlist = SHBrowseForFolder(&browseinfo)))
	{
		AfxMessageBox(_T("没有选择目录"));
		return CString(_T(""));
	}
	else
	{
		SHGetPathFromIDList(lpitemidlist, fullpath);
		CoTaskMemFree(lpitemidlist);
		return CString(fullpath);
	}
	return CString();
}


void CPHPScanDlg::OnClickedButtonStart()
{
	// TODO: 在此添加控件通知处理程序代码
	//控件相关状态操作
	CString strButtonStart,strButtonPause;
	GetDlgItemTextW(IDC_BUTTON_START, m_strButtonStart);
	strButtonStart.LoadStringW(IDS_STRING_START);
	strButtonPause.LoadStringW(IDS_STRING_SUSPEND);
	if (m_strButtonStart == strButtonStart) {
		// 创建事件
		m_hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
		// 事件置位
		SetEvent(m_hEvent);
		InitControl();
		InitExtNameVC();
		if (!m_vcExtName.empty()) {
			m_listResult.DeleteAllItems();
			m_strButtonStart.LoadStringW(IDS_STRING_SUSPEND);
			m_buttonStart.SetWindowTextW(m_strButtonStart);
			//任务线程操作
			m_ctMyThread.m_intThreadMax = 1;
			m_ctThread = m_ctMyThread.CreateThread(this);
			m_intRunTime = 0;
			m_ctThreadFlag = 0;//运行
			GetDlgItem(IDC_COMBO_THREAD_COUNT)->EnableWindow(FALSE);
			PostMessage(WM_ZMY_REFRESH);
			m_staticTotalCountFile = L"0";
			//刷新显示
			KillTimer(ID_TIMER_REFRESH);
			SetTimer(ID_TIMER_REFRESH, 100, NULL);
			if (m_buttonStop.IsWindowEnabled() == FALSE) {
				m_buttonStop.EnableWindow(true);
			}
			if (m_buttonBrowser.IsWindowEnabled() == TRUE) {
				m_buttonBrowser.EnableWindow(FALSE);
			}
		}else {
			this->MessageBox(L"请选择搜索文件的扩展名", L"提示：", MB_OK);
		}
	}else if(m_strButtonStart==strButtonPause) {
		m_strButtonStart.LoadStringW(IDS_STRING_CONTINUE);
		m_buttonStart.SetWindowTextW(m_strButtonStart);
		m_ctMyThread.ThreadSuspend(m_ctThread);
		m_ctThreadFlag = 1;//暂停
	}else {
		m_strButtonStart.LoadStringW(IDS_STRING_SUSPEND);
		m_buttonStart.SetWindowTextW(m_strButtonStart);
		m_ctMyThread.ThreadResume(m_ctThread);
		m_ctThreadFlag = 0;
	}
}


void CPHPScanDlg::OnClickedButtonStop()
{
	// TODO: 在此添加控件通知处理程序代码
	//控件相关状态操作
	m_buttonStop.EnableWindow(false);
	m_strButtonStart.LoadStringW(IDS_STRING_START);
	m_buttonStart.SetWindowTextW(m_strButtonStart);
	//任务线程操作
	m_ctMyThread.ThreadStop(m_ctThread);
	m_ctThreadFlag = 2;//停止
}


afx_msg LRESULT CPHPScanDlg::OnZmyRefresh(WPARAM wParam, LPARAM lParam)
{
	int iExtNameSize = m_vcExtName.size();
	int iTatal = m_allFolders.size()*iExtNameSize;
	int pos = 0;
	if (iTatal > 0) {
		pos = m_intProgCur * 100 / (m_allFolders.size()*iExtNameSize);
	}
	m_progScan.SetPos(pos);
	CString staticTotalCount,staticTotalCountFile, staticCurCount;
	staticTotalCount.Format(L"%d", m_allFolders.size()*iExtNameSize);
	m_staticTotalCount = staticTotalCount;
	staticTotalCountFile.Format(L"%d", m_vcAllFileResult.size());
	m_staticTotalCountFile = staticTotalCountFile;
	staticCurCount.Format(L"%d", m_intProgCur);
	m_staticCurCount = staticCurCount;
	UpdateData(false);
	return 0;
}


afx_msg LRESULT CPHPScanDlg::OnZmyGetallfolderFinish(WPARAM wParam, LPARAM lParam)
{
	m_intThreadFinshed = 0;
	m_ctMyFileThread.m_intThreadMax = _ttoi(m_cmThreadCount)==0?5: _ttoi(m_cmThreadCount);
	m_ctThread = m_ctMyFileThread.CreateThread(this);
	return 0;
}


void CPHPScanDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	switch (nIDEvent)
	{
	default:
		break;
	case ID_TIMER_REFRESH:
		GetRunTime();
		PostMessage(WM_ZMY_REFRESH);
		break;
	}
	CDialogEx::OnTimer(nIDEvent);
}


int CPHPScanDlg::GetRunTime()
{
	if (m_ctThreadFlag == 0) {
		m_intRunTime++;
	}
	int i_usec = m_intRunTime % 10;
	int i_sec = (m_intRunTime / 10) % 60;
	int i_min = (m_intRunTime / 10 / 60) % 60;
	int i_hour = (m_intRunTime / 10 / 3600) % 60;
	CString cs_runTime;
	cs_runTime.Format(L"%03d:%02d:%02d.%d",i_hour,i_min,i_sec,i_usec);
	m_staticRunTime = cs_runTime;
	return 0;
}


afx_msg LRESULT CPHPScanDlg::OnZmyGetallfileFinish(WPARAM wParam, LPARAM lParam)
{
	m_intThreadFinshed++;
	if (m_intThreadFinshed == m_ctMyFileThread.m_intThreadMax) {
		m_buttonStop.EnableWindow(false);
		m_strButtonStart.LoadStringW(IDS_STRING_START);
		m_buttonStart.SetWindowTextW(m_strButtonStart);
		m_ctThreadFlag = 2;
		if (m_buttonBrowser.IsWindowEnabled() == false) {
			m_buttonBrowser.EnableWindow(true);
		}
		GetDlgItem(IDC_COMBO_THREAD_COUNT)->EnableWindow(TRUE);
		KillTimer(ID_TIMER_REFRESH);
		PostMessage(WM_ZMY_REFRESH);
	}
	return 0;
}


afx_msg LRESULT CPHPScanDlg::OnZmyGetallfolderExit(WPARAM wParam, LPARAM lParam)
{
	if (m_buttonBrowser.IsWindowEnabled() == false) {
		m_buttonBrowser.EnableWindow(true);
	}
	return 0;
}


afx_msg LRESULT CPHPScanDlg::OnZmyGetallfileExit(WPARAM wParam, LPARAM lParam)
{
	m_intThreadFinshed++;
	if (m_intThreadFinshed == m_ctMyFileThread.m_intThreadMax) {
		m_buttonStop.EnableWindow(false);
		m_strButtonStart.LoadStringW(IDS_STRING_START);
		m_buttonStart.SetWindowTextW(m_strButtonStart);
		m_ctThreadFlag = 2;
		if (m_buttonBrowser.IsWindowEnabled() == false) {
			m_buttonBrowser.EnableWindow(true);
		}
		GetDlgItem(IDC_COMBO_THREAD_COUNT)->EnableWindow(TRUE);
		KillTimer(ID_TIMER_REFRESH);
		PostMessage(WM_ZMY_REFRESH);
	}
	return 0;
}


// //控件信息初始化
int CPHPScanDlg::InitControl()
{
	m_staticCurCount = L"0";
	m_staticTotalCount = L"0";
	m_staticTotalCountFile = L"0";
	m_progScan.SetRange(m_intProgStart, m_intProgEnd);
	m_progScan.SetStep(1);
	m_progScan.SetPos(0);
	m_allFolders.clear();
	m_vcAllFileResult.clear();
	m_intProgCur = 0;
	return 0;
}


void CPHPScanDlg::OnCbnSelchangeComboThreadCount()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
}


int CPHPScanDlg::InitConfig()
{
	m_strWorkDir = CZMyFile::GetWorkDir();
	m_strPHPTrojanFeatureFile.LoadStringW(IDS_STRING_PHPTROJANF);
	m_strTrojanFeatureFullPath = m_strWorkDir + "\\" + m_strPHPTrojanFeatureFile;
	BOOL flag = CZMyFile::ReadFileToVsctor(m_strTrojanFeatureFullPath, m_vcPHPTrajonFeature);
	if (!flag) {
		CZMyFile::DefaultTrajonFeature(m_vcPHPTrajonFeature);
	}
	return 0;
}


int CPHPScanDlg::InitExtNameVC()
{
	m_vcExtName.clear();
	/*
	m_vcExtName.push_back(L".php");
	m_vcExtName.push_back(L".txt");
	m_vcExtName.push_back(L".log");
	*/
	if (m_cbCheckPHP.GetCheck()) {
		m_vcExtName.push_back(L".php");
	}
	if (m_cbCheckINC.GetCheck()) {
		m_vcExtName.push_back(L".inc");
	}
	if (m_cbCheckTXT.GetCheck()) {
		m_vcExtName.push_back(L".txt");
	}
	//m_vcExtName.push_back(L".php");
	return 0;
}


int CPHPScanDlg::InitListResult()
{
	m_listResult.SetExtendedStyle(LVS_EX_CHECKBOXES);
	m_listResult.InsertColumn(0, L"", LVCFMT_LEFT, 30);
	m_listResult.InsertColumn(1, L"ID", LVCFMT_LEFT, 60);
	m_listResult.InsertColumn(2, L"文件名", LVCFMT_LEFT, 120);
	m_listResult.InsertColumn(3, L"文件路径", LVCFMT_LEFT, 360);
	return 0;
}
