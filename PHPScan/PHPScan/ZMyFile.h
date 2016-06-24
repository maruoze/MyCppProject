#pragma once
#include <vector>
#include <mutex>
#include "PHPScanDlg.h"
using namespace std;

class CZMyFile
{
public:
	CZMyFile();
	~CZMyFile();
	static void GetAllUnderFolderByFolder(CString csFolderName, vector<CString> &m_allFolders);
	static bool CZMyFile::GetAllUnderFolderByFolderEx(CString csFolderName, CPHPScanDlg * dlg);
	static bool m_bRecycleFlag;
	static bool GetAllFileByExt(LPVOID pParm, CString ext);
	//static mutex m_mutVector;
};

