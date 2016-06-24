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

	dlg->m_staticCurFile = csFolderName;
	(dlg->m_allFolders).push_back(csFolderName);	
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
