#include "stdafx.h"
#include "ZMyCallbackProc.h"


CZMyCallbackProc::CZMyCallbackProc()
{
}


CZMyCallbackProc::~CZMyCallbackProc()
{
}


// 文件夹选择的回调
int CALLBACK CZMyCallbackProc::BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM pData)
{
	TCHAR szDir[MAX_PATH];
	switch (uMsg)
	{
	case BFFM_INITIALIZED:
		// WParam is TRUE since you are passing a path.
		// It would be FALSE if you were passing a pidl.
		SendMessage(hwnd, BFFM_SETSELECTION, TRUE, (LPARAM)pData);
		break;
	case BFFM_SELCHANGED:
		// Set the status window to the currently selected path.
		if (SHGetPathFromIDList((LPITEMIDLIST)lParam, szDir))
		{
			SendMessage(hwnd, BFFM_SETSTATUSTEXT, 0, (LPARAM)szDir);
		}
		break;
	}
	return 0;
}
