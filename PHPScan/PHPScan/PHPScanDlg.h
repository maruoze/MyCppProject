
// PHPScanDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CPHPScanDlg �Ի���
class CPHPScanDlg : public CDialogEx
{
// ����
public:
	CPHPScanDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PHPSCAN_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnAbout();
	// ��ʼ��ʱ�趨���ڵĴ�С
	void SetWindowDisplay();
	CButton m_buttonBrowser;
	CButton m_buttonStart;
	CButton m_buttonStop;
	CListCtrl m_listResult;
	CProgressCtrl m_progScan;
	CString m_staticCurCount;
	CString m_staticCurFile;
	CString m_staticPath;
	CString m_staticTotalCount;
};
