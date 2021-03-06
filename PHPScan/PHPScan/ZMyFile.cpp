#include "stdafx.h"
#include "ZMyFile.h"
bool CZMyFile::m_bRecycleFlag = true;
mutex m_mutVector;

CZMyFile::CZMyFile()
{
}


CZMyFile::~CZMyFile()
{
}


void CZMyFile::GetAllUnderFolderByFolder(CString csFolderName, vector<CString> &m_allFolders)
{
	m_allFolders.push_back(csFolderName);
	CFileFind fileFinder;
	CString filePath = csFolderName + _T("//*.*");
	BOOL bFinished = fileFinder.FindFile(filePath);
	while (bFinished)  //每次循环对应一个类别目录
	{
		bFinished = fileFinder.FindNextFile();
		if (fileFinder.IsDirectory() && !fileFinder.IsDots())  //若是目录则递归调用此方法
		{
			CString curFolder = fileFinder.GetFilePath();
			CZMyFile::GetAllUnderFolderByFolder(curFolder,m_allFolders);
		}
	}
	fileFinder.Close();
}

bool CZMyFile::GetAllUnderFolderByFolderEx(CString csFolderName,CPHPScanDlg * dlg)
{
	//获取退出消息
	MSG msg;
	while (PeekMessage(&msg, NULL, WM_THREAD_STOP, WM_THREAD_STOP, PM_REMOVE)) {
		switch (msg.message) {
			case WM_THREAD_STOP:
				m_bRecycleFlag=false;
		}
	}
	m_mutVector.lock();
	dlg->m_staticCurFile = csFolderName;
	(dlg->m_allFolders).push_back(csFolderName);	
	m_mutVector.unlock();
	CFileFind fileFinder;
	CString filePath = csFolderName + _T("//*.*");
	BOOL bFinished = fileFinder.FindFile(filePath);
	while (bFinished)  //每次循环对应一个类别目录
	{
		bFinished = fileFinder.FindNextFile();
		if (fileFinder.IsDirectory() && !fileFinder.IsDots()&&m_bRecycleFlag)  //若是目录则递归调用此方法
		{
			CString curFolder = fileFinder.GetFilePath();
			CZMyFile::GetAllUnderFolderByFolderEx(curFolder, dlg);
		}
		else if(!m_bRecycleFlag){
			return false;
		}
	}
	fileFinder.Close();
	return true;
}



bool CZMyFile::GetAllFileByExt(LPVOID pParm, CString ext)
{
	pair<int, LPVOID>*lpParm = (pair<int, LPVOID>*)pParm;
	int intThreadIndex = lpParm->first;
	CPHPScanDlg* dlg = (CPHPScanDlg*)lpParm->second;
	int intThreadMax = CZMyFileThread::m_intThreadMax;
	int intFolderTotal = dlg->m_allFolders.size();
	int intStep = intFolderTotal / intThreadMax + 1;
	int intLoopStart = intStep*intThreadIndex;
	int intLoopStop = intStep*(intThreadIndex + 1);
	if (intLoopStart != 0) {
		intLoopStart = intLoopStart;
	}
	if (intLoopStop >= intFolderTotal) {
		intLoopStop = intFolderTotal;
	}
	CFileFind fileFinder;
	MSG msg;
	for (int i = intLoopStart; i < intLoopStop; i++)
	{
		//获取退出消息
		while (PeekMessage(&msg, NULL, WM_THREAD_STOP, WM_THREAD_STOP, PM_REMOVE)) {
			switch (msg.message) {
			case WM_THREAD_STOP:
				m_bRecycleFlag = false;
			}
		}
		if (!m_bRecycleFlag) {
			//break;
			return false;
		}
		CString csForlder = dlg->m_allFolders.at(i);
		CString filePath= csForlder+ _T("//*.*");
		BOOL bFinished = fileFinder.FindFile(filePath);
		while (bFinished&&m_bRecycleFlag)
		{
			
			bFinished = fileFinder.FindNextFile();
			if (!fileFinder.IsDirectory()) {
				CString filePath = fileFinder.GetFilePath();
				CString fileName = fileFinder.GetFileName();
				int dotPos = fileName.ReverseFind('.');
				CString fileExt = fileName.Right(fileName.GetLength() - dotPos);
				if (fileExt == ext) {
					m_mutVector.lock();
					dlg->m_vcAllFileResult.push_back(filePath);
					dlg->m_staticCurFile = filePath;
/*
					int nIndex = dlg->m_listResult.GetItemCount();
					int nRow = dlg->m_listResult.InsertItem(nIndex, L"");// 插入行
					CString strIndex;
					strIndex.Format(L"%d", nRow + 1);
					dlg->m_listResult.SetItemText(nRow, 1, strIndex);
					dlg->m_listResult.SetItemText(nRow, 2, fileName);
					dlg->m_listResult.SetItemText(nRow, 3, filePath);
*/
					m_mutVector.unlock();
				}
			}
			
		}
		m_mutVector.lock();
		dlg->m_intProgCur++;
		m_mutVector.unlock();
	}
	fileFinder.Close();
	return true;
}


 BOOL CZMyFile::ReadFileToVsctor(CString filePath, vector<CString> &vc)
{
	if(PathFileExistsW(filePath)) {
		CStdioFile stdioFile;
		CFileException fileExpt;
		BOOL fileOpenFlag = stdioFile.Open(filePath, CFile::modeRead, &fileExpt);
		if (fileOpenFlag) {
			CString str;
			while (stdioFile.ReadString(str))
			{
				vc.push_back(str);
			}
		}
		stdioFile.Close();
		return TRUE;
	}
	else {
		return FALSE;
	}
}


CString CZMyFile::GetWorkDir()
{
	wchar_t pFileName[MAX_PATH];
	int nPos = GetCurrentDirectory(MAX_PATH, pFileName);
	CString csFullPath(pFileName);
	if (nPos < 0) {
		return CString("");
	}else {
		return csFullPath;
	}	
}


BOOL CZMyFile::DefaultTrajonFeature(vector<CString> &vc)
{
	CString str(L"$_POST");
	vc.push_back(str);
	str = L"$_GET";
	vc.push_back(str);
	return 0;
}


bool CZMyFile::GetAllTrajonResult(LPVOID pParm)
{
	//设置线程跑的文件范围
	pair<int, LPVOID>*lpParm = (pair<int, LPVOID>*)pParm;
	int intThreadIndex = lpParm->first;
	CPHPScanDlg* dlg = (CPHPScanDlg*)lpParm->second;
	int intThreadMax = CZMyFileThread::m_intThreadMax;
	int intFileTotal = dlg->m_vcAllFileResult.size();
	int intStep = intFileTotal / intThreadMax + 1;
	int intLoopStart = intStep*intThreadIndex;
	int intLoopStop = intStep*(intThreadIndex + 1);
	if (intLoopStart != 0) {
		intLoopStart = intLoopStart;
	}
	if (intLoopStop >= intFileTotal) {
		intLoopStop = intFileTotal;
	}
	vector<CString> vcDefaultTrajonFeature;
	CZMyFile::DefaultTrajonFeature(vcDefaultTrajonFeature);
	MSG msg;
	for (int i = intLoopStart; i < intLoopStop; i++)
	{
		//获取退出消息
		while (PeekMessage(&msg, NULL, WM_THREAD_STOP, WM_THREAD_STOP, PM_REMOVE)) {
			switch (msg.message) {
			case WM_THREAD_STOP:
				m_bRecycleFlag = false;
			}
		}
		if (!m_bRecycleFlag) {
			//break;
			return false;
		}
		for (unsigned int j = 0;j < vcDefaultTrajonFeature.size();j++) {
			m_mutVector.lock();
			CString trajonTmp = vcDefaultTrajonFeature.at(j);
			CString filePath = dlg->m_vcAllFileResult.at(i);
			int slashPos = filePath.ReverseFind('\\');
			CString fileName= filePath.Right(filePath.GetLength() - slashPos - 1);
			CString fileContent;
			bool readSucc = CZMyFile::ReadFileToCString(filePath, fileContent);
			if (readSucc) {
				int findIndex=fileContent.Find(trajonTmp);
				if (findIndex != -1) {
					dlg->m_vcAllTrajonResult.push_back(filePath);
					dlg->m_staticCurFile = filePath;
					int nIndex = dlg->m_listResult.GetItemCount();
					int nRow = dlg->m_listResult.InsertItem(nIndex, L"");// 插入行
					CString strIndex;
					strIndex.Format(L"%d", nRow + 1);
					dlg->m_listResult.SetItemText(nRow, 1, strIndex);
					dlg->m_listResult.SetItemText(nRow, 2, fileName);
					dlg->m_listResult.SetItemText(nRow, 3, filePath);
					CString strfindIndex;
					strfindIndex.Format(L"%d", findIndex);
					dlg->m_listResult.SetItemText(nRow, 4, strfindIndex);
					dlg->m_listResult.SetItemText(nRow, 5, trajonTmp);
				}
			}else {
				continue;
			}
			m_mutVector.unlock();
		}
	}
	return true;
}


bool CZMyFile::ReadFileToCString(CString filePath, CString &fileContent)
{
	if (PathFileExistsW(filePath)) {
		CStdioFile stdioFile;
		CFileException fileExpt;
		BOOL fileOpenFlag = stdioFile.Open(filePath, CFile::modeRead, &fileExpt);
		if (fileOpenFlag) {
			CString str;
			while (stdioFile.ReadString(str))
			{
				fileContent+=str+L" ";
			}
		}
		stdioFile.Close();
		return TRUE;
	}
	else {
		return FALSE;
	}
	return false;
}
