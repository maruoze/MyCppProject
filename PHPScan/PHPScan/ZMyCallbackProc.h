#pragma once
class CZMyCallbackProc
{
public:
	CZMyCallbackProc();
	~CZMyCallbackProc();
	// �ļ���ѡ��Ļص�
	static int CALLBACK BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM pData);
};

