#include "stdafx.h"
#include "ZMyFileThread.h"
#include "PHPScanDlg.h"
#include "ZMyFile.h"
int CZMyFileThread::m_intThreadMax=5;

CZMyFileThread::CZMyFileThread()
{
	CZMyThread::CZMyThread();
}


CZMyFileThread::~CZMyFileThread()
{
	CZMyThread::~CZMyThread();
}


UINT CZMyFileThread::ThreadFunc(LPVOID pParm)
{
	pair<int, LPVOID>*lpParm = (pair<int, LPVOID>*)pParm;
	CPHPScanDlg* dlg = (CPHPScanDlg*)lpParm->second;
	CString strCurCount, strThreadIndex;
	int intThreadIndex = lpParm->first;
	// 等待事件置位
	//WaitForSingleObject(dlg->m_hEvent, INFINITE);
	CZMyFile::m_bRecycleFlag = true;
	//CString csExt = L".php";
	//bool bReturn=CZMyFile::GetAllFileByExt(pParm, csExt);
	vector<CString> vcExt = dlg->m_vcExtName;
	int vcExtSize = vcExt.size();
	bool bReturn;
	for (int i = 0; i < vcExtSize; i++)
	{
		CString csExt = vcExt.at(i);
		bReturn = CZMyFile::GetAllFileByExt(pParm, csExt);
	}

	// 处理完成后即将事件对象置位
	//SetEvent(dlg->m_hEvent);
	if (bReturn) {
		dlg->PostMessageW(WM_ZMY_GETALLFILE_FINISH, 0, NULL);
	}else {
		dlg->PostMessageW(WM_ZMY_GETALLFILE_EXIT, 0, NULL);
	}
	return 0;
}

// 线程创建
vector<CWinThread*> CZMyFileThread::CreateThread(LPVOID lpPara)
{
	m_dlgWnd = lpPara;
	vector<CWinThread*> p_ctThread;
	CPHPScanDlg* dlg = (CPHPScanDlg*)lpPara;
	dlg->m_vcAllFileResult.clear();
	dlg->m_intProgCur = 0;
	for (int i = 0; i < m_intThreadMax; i++)
	{
		pair<int, LPVOID> *pLpPara = new pair<int, LPVOID>;
		pLpPara->first = i;
		pLpPara->second = m_dlgWnd;
		CWinThread *thread = AfxBeginThread(
			ThreadFunc,
			(LPVOID)pLpPara
			);
		p_ctThread.push_back(thread);
	}
	m_ctThread = p_ctThread;
	return m_ctThread;
}