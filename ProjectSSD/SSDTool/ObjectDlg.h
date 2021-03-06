#pragma once

#include "SSDToolGlobal.h"
// ObjectDlg 대화 상자

class ObjectDlg : public CDialogEx
{
	DECLARE_DYNAMIC(ObjectDlg)

public:
	ObjectDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~ObjectDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_OBJECTDLG };
#endif

public:
	std::list<FbxData*> m_FbxListDatalist;
	HTREEITEM StaticItem;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.


	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();



	afx_msg void OnBnClickedButton1();
	afx_msg void OnTvnSelchangedFbxtree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedReset();
	CTreeCtrl m_FbxTree;

	
	afx_msg void OnEnChangeMesh();
	afx_msg void OnEnChangeMat();

	void StringReset();
	virtual BOOL OnInitDialog();

	void StaticFbxFinder();
	afx_msg void OnLbnSelchangeMeshlist();

	CListBox m_MeshList;
	void SetMeshList();
	FbxData* TempName;
	class CActor* RenderActor;

};
