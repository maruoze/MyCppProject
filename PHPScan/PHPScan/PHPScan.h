
// PHPScan.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CPHPScanApp: 
// �йش����ʵ�֣������ PHPScan.cpp
//

class CPHPScanApp : public CWinApp
{
public:
	CPHPScanApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CPHPScanApp theApp;