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
	//�߳���ͣ
	int ThreadSuspend(vector<CWinThread*> m_ctThread);
	// �ָ̻߳�
	int ThreadResume(vector<CWinThread*> m_ctThread);
	// �̴߳���
	vector<CWinThread*> CreateThread(LPVOID lpPara);
	// �߳���ֹ
	int ThreadStop(vector<CWinThread*> p_ctThread);
};

