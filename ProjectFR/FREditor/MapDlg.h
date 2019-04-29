#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CMapDlg 대화 상자입니다.

class CMapDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMapDlg)
public:
	CMapDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CMapDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MAPDLG };
#endif
	enum TABINDEX
	{
		MAPOBJECT,
		MAXTABINDEX
	};

	enum 
	{
		TILECOLOR,
		TILEMAPNAME,
		WORLDMAPOPTION,
		MAXTILEOPTION,
	};

	enum EDITMODE
	{
		TILE_EDIT,
		OBJECT_EDIT,
		MAXEDITMODE,
	};

private:
	vector<vector<CString>>		m_TileTypeString;
	vector<CEdit*>				m_vecEdit;
	EDITMODE					m_CurEditMode;

	RECT						m_DlgRect;
	bool						m_bMapSceneInit;
	CComboBox					m_TileMapCom;
	int							m_TileEditRadio;
	CString						m_strPortalTileMapSize;

	CComboBox					m_TileTypeCom;
	CComboBox					m_TileOptionCom;

	CEdit						m_SpawnTileIndexX;
	UINT						m_uiSpawnTileIndexX;

	CEdit						m_SpawnTileIndexY;
	UINT						m_uiSpawnTileIndexY;

	CComboBox					m_MapObjectCom;

	UINT						m_uiCurObjectNo;
	CString						m_strCurObjectSize;


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	void InitMapSceneEvent();

	void EnableTileDataEdit(const BOOL& _Enable);
	void EnableObjectDataEdit(const BOOL& _Enable);
	void EnableTileIndexEdit(const BOOL& _Enable);
	void CurEnableTileDataEdit();

	// Combo Box Click Event
	void SelectTileMapCombo(const int& _TileMapIndex);
	void SelectTileTypeCombo(const int& _TileTypeIndex);
	void SelectTileOptionCombo(const int& _TileIOptionIndex);
	void SelectMapObjectCombo(const int& _ObjectNo);

	void ChangeColorOptionEvent(const int& TileMapNo);
	void ChangeTileMapEvent(const int& TileMapNo);
	void ChangeWorldMapTypeEvnet(const int& TileMapNo);

	BOOL CheckButtonEvent(MSG* _pMsg);
	BOOL EditBoxClickCheck(MSG* _pMsg);
	BOOL CheckTileXIndex();
	BOOL CheckTileYIndex();


	BOOL LoadTileData();
	BOOL SaveTileData();
	BOOL LoadObjectData();
	BOOL SaveObjectData();
public:
	const EDITMODE GetCurEditMode();
	const TILEOPTION GetCurTileType();
	const UINT GetCurTileSubOption();
	const POINT GetSpawnTileIndex();
	const UINT GetCurMapObject();

public:
	afx_msg void SetRadioStatus(UINT value);
	virtual void OnOK();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	afx_msg void OnCbnSelchangeTilemapcom();
	afx_msg void OnCbnSelchangeTiletypecom();
	afx_msg void OnCbnSelchangeTileoptioncom();
	afx_msg void OnBnClickedTileloadbutton();
	afx_msg void OnBnClickedTilesavebutton();
	afx_msg void OnBnClickedObjectloadbutton();
	afx_msg void OnBnClickedObjectsavebutton();
	afx_msg void OnCbnSelchangeMapobjectcombo();
};
