#pragma once
#include "afxwin.h"


// CPacketDlg ��ȭ �����Դϴ�.

class CPacketDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CPacketDlg)

public:
	CPacketDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CPacketDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PACKETDLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	CListBox m_PacketLogList;
	afx_msg void OnClose();
	virtual void OnOK();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
