#pragma once
#define WM_THREAD_STOP WM_USER+1001
#include <vector>
using namespace std;

class CZMyThread
{
public:
	CZMyThread();
	~CZMyThread();
	static int m_intThreadMax;
	static UINT ThreadFunc(LPVOID pParm);
	LPVOID m_dlgWnd;
	vector<CWinThread*> m_ctThread;
	//线程暂停
	int ThreadSuspend(vector<CWinThread*> m_ctThread);
	// 线程恢复
	int ThreadResume(vector<CWinThread*> m_ctThread);
	// 线程创建
	vector<CWinThread*> CreateThread(LPVOID lpPara);
	// 线程终止
	int ThreadStop(vector<CWinThread*> p_ctThread);
};

