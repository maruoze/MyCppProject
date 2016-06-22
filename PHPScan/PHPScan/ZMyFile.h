#pragma once
#include <vector>
#include "PHPScanDlg.h"
using namespace std;

class CZMyFile
{
public:
	CZMyFile();
	~CZMyFile();
	static void GetAllUnderFolderByFolder(CString csFolderName, vector<CString> &m_allFolders);
	static void CZMyFile::GetAllUnderFolderByFolderEx(CString csFolderName, CPHPScanDlg * dlg);
	static bool m_bRecycleFlag;
};

