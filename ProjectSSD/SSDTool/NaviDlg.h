#pragma once
#include "afxwin.h"

#define RECTAREA 0 
#define CIRCLEAREA 1
// NaviDlg 대화 상자

#include <TerrainRenderer.h>



enum NAVIEIDT_MODE
{
	LINK_MODE,
	TERRAINON_MODE,
	TERRAINOFF_MODE,
	MAX_NAVIEIDT_MODE
};
class NaviDlg : public CDialogEx
{
	DECLARE_DYNAMIC(NaviDlg)

public:
	NaviDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~NaviDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_NAVIDLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	UINT m_NaviAreaType;
	UINT m_NaviAreaIdx;
	CComboBox m_NaviAreaCombo;
	CEdit m_NavimeshPathEdit;
	CEdit m_NavimeshFileEdit;

public:
	CEdit m_AreaPivotXEdit;
	CString m_strAreaPivotX;
	float m_fAreaPivotX;

	CEdit m_AreaPivotYEdit;
	CString m_strAreaPivotY;
	float m_fAreaPivotY;

public:
	UINT m_LinkMode;
	UINT m_LinkRadio1;
	UINT m_LinkRadio2;
	UINT m_LinkRadio3;
	UINT m_SelectLinkVtx[3];


public:
	class CNavigationMesh*							m_CurSelectNaviMesh;
	CNaviLinkTri*									m_EditNaviLinkTri;
	CListBox										m_LinkTriList;

public:
	void InitNaviMeshEdit();
	void InitNaviAreaEdit();
	void ClickAreaTypeEvent(UINT ID);
	void AddNaviArea();
	void ModifyNaviArea(const int& _Idx);
	void DeleteNaviArea();
	void UpdataNaviDlgData();

public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedNavimeshbutton();
	afx_msg void OnEnChangeNavimeshfile();
	afx_msg void OnBnClickedAddarea();
	afx_msg void OnBnClickedDelarea();

	afx_msg void OnCbnSelchangeAreacombo();
	afx_msg void OnEnChangeAreapivotx();
	afx_msg void OnEnChangeAreapivoty();
	afx_msg void OnEnChangeAreasizex();
	afx_msg void OnEnChangeAreasizey();

public:
	void ChangeLinkMode(UINT _ID);
	void LinkRadioEvent(UINT _ID);
	void SelectEventLinkV1(const int& _ID);
	void SelectEventLinkV2(const int& _ID);
	void SelectEventLinkV3(const int& _ID);

	
	void AllLinkRadioButtonOff();
	void ActiveNaviVtxRadioButton(const bool& _bActive);
	void ActiveTerrainVtxRadioButton(const bool& _bActive);
	void AllLinkRadioButtonOn();

public:
	void SelectTriOff();

public:
	void TerrainLeftClickEvent(SPTR<class CTerrainRenderer> _pTR);
	void TerrainRightClickEvent(SPTR<class CTerrainRenderer> _pTR);
	void NaviMeshClickEvent(SPTR<class CNavigationMesh> _pNaviMesh);

public:
	void SelectLinkTerrain(SPTR<CTerrainRenderer> _pTR);
	void TerrainNaviTriOn(SPTR<CTerrainRenderer> _pTR);
	void TerrainNaviTriOff(SPTR<CTerrainRenderer> _pTR);
	void SelectLinkNaviMeshTri(SPTR<CNavigationMesh> _pNaviMesh);
	

public:
	void AddNavigationMeshEvent(SPTR<CNavigationMesh> _pNaviMesh);
	void UpdateLinkTriangle();
public:
	void InitSelectLinkVtx();

public:
	void DebugRender(SPTR<class CCamera> _Cam);
	virtual BOOL DestroyWindow();
	afx_msg void OnBnClickedAddlinkvtx();
	afx_msg void OnBnClickedDeletelinkvtx();

	afx_msg void OnLbnSelchangeLinkvtxlist();
};
