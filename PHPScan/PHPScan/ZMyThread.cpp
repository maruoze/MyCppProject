#include "stdafx.h"
#include "ZMyThread.h"
#include "PHPScanDlg.h"
#include "ZMyFile.h"

int CZMyThread::m_intThreadMax = 5;

CZMyThread::CZMyThread()
{
}

CZMyThread::~CZMyThread()
{
}


UINT CZMyThread::ThreadFunc(LPVOID pParm)
{
	pair<int, LPVOID>*lpParm = (pair<int, LPVOID>*)pParm;
	CPHPScanDlg* dlg = (CPHPScanDlg*)lpParm->second;
	CString strCurCount,strThreadIndex;
	int intThreadIndex = lpParm->first;
	// 等待事件置位
	WaitForSingleObject(dlg->m_hEvent, INFINITE);
	dlg->m_allFolders.clear();
	CZMyFile::m_bRecycleFlag = true;
	CZMyFile::GetAllUnderFolderByFolderEx(dlg->m_staticPath,dlg);
	// 处理完成后即将事件对象置位
	SetEvent(dlg->m_hEvent);
	dlg->PostMessageW(WM_ZMY_GETALLFOLDER_FINISH,0,NULL);
	return 0;
}


int CZMyThread::ThreadSuspend(vector<CWinThread*> p_ctThread)
{
	m_ctThread = p_ctThread;
	vector<CWinThread*>::iterator iter;
	for (iter = m_ctThread.begin();iter != m_ctThread.end();iter++)
	{
		(*iter)->SuspendThread();
	}
	return 0;
}


// 线程恢复
int CZMyThread::ThreadResume(vector<CWinThread*> p_ctThread)
{
	m_ctThread = p_ctThread;
	vector<CWinThread*>::iterator iter;
	for (iter = m_ctThread.begin();iter != m_ctThread.end();iter++)
	{
		(*iter)->ResumeThread();
	}
	return 0;
}


// 线程创建
vector<CWinThread*> CZMyThread::CreateThread(LPVOID lpPara)
{
	m_dlgWnd = lpPara;
	vector<CWinThread*> p_ctThread;
	for (int i = 0; i < m_intThreadMax; i++)
	{
		pair<int, LPVOID> *pLpPara=new pair<int, LPVOID>;
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


int CZMyThread::ThreadStop(vector<CWinThread*> p_ctThread)
{
	m_ctThread = p_ctThread;
	vector<CWinThread*>::iterator iter;
	for (iter = m_ctThread.begin();iter != m_ctThread.end();iter++)
	{
		//(*iter)->ResumeThread();
		PostThreadMessage((*iter)->m_nThreadID, WM_THREAD_STOP,NULL,NULL);
	}
	return 0;
}
