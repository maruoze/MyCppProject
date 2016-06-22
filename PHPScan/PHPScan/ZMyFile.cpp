#include "stdafx.h"
#include "ZMyFile.h"
bool CZMyFile::m_bRecycleFlag = true;

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
	while (bFinished)  //ÿ��ѭ����Ӧһ�����Ŀ¼
	{
		bFinished = fileFinder.FindNextFile();
		if (fileFinder.IsDirectory() && !fileFinder.IsDots())  //����Ŀ¼��ݹ���ô˷���
		{
			CString curFolder = fileFinder.GetFilePath();
			CZMyFile::GetAllUnderFolderByFolder(curFolder,m_allFolders);
		}
	}
	fileFinder.Close();
}

void CZMyFile::GetAllUnderFolderByFolderEx(CString csFolderName,CPHPScanDlg * dlg)
{
	//��ȡ�˳���Ϣ
	MSG msg;
	while (PeekMessage(&msg, NULL, WM_THREAD_STOP, WM_THREAD_STOP, PM_REMOVE)) {
		switch (msg.message) {
			case WM_THREAD_STOP:
				m_bRecycleFlag=false;
		}
	}

	(dlg->m_allFolders).push_back(csFolderName);	
	CFileFind fileFinder;
	CString filePath = csFolderName + _T("//*.*");
	BOOL bFinished = fileFinder.FindFile(filePath);
	while (bFinished)  //ÿ��ѭ����Ӧһ�����Ŀ¼
	{
		bFinished = fileFinder.FindNextFile();
		if (fileFinder.IsDirectory() && !fileFinder.IsDots()&&m_bRecycleFlag)  //����Ŀ¼��ݹ���ô˷���
		{
			CString curFolder = fileFinder.GetFilePath();
			CZMyFile::GetAllUnderFolderByFolderEx(curFolder, dlg);
		}
	}
	fileFinder.Close();
}

