
// SSDServerEditor.h : SSDServerEditor ���� ���α׷��� ���� �� ��� ����
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"       // �� ��ȣ�Դϴ�.


// CSSDServerEditorApp:
// �� Ŭ������ ������ ���ؼ��� SSDServerEditor.cpp�� �����Ͻʽÿ�.
//

class CSSDServerEditorApp : public CWinApp
{
public:
	CSSDServerEditorApp();


// �������Դϴ�.
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// �����Դϴ�.

public:
	UINT  m_nAppLook;
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	virtual BOOL OnIdle(LONG lCount);
};

extern CSSDServerEditorApp theApp;
