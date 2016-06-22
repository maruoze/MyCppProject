#pragma once
class CZMyCallbackProc
{
public:
	CZMyCallbackProc();
	~CZMyCallbackProc();
	// 文件夹选择的回调
	static int CALLBACK BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM pData);
};

