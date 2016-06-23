#pragma once
#include "ZMyThread.h"
class CZMyFileThread :
	public CZMyThread
{
public:
	CZMyFileThread();
	~CZMyFileThread();
	static int m_intThreadMax;
	static UINT ThreadFunc(LPVOID pParm);
	vector<CWinThread*> CZMyFileThread::CreateThread(LPVOID lpPara);
};

