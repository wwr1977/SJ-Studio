#pragma once

#include "SSDToolGlobal.h"
#include "afxwin.h"
#include <DeadByDaylightGlobal.h>
// ControlDlg 대화 상자


enum BUTTON_IMAGE_TYPE
{
	IDLE_BUTTON,
	FOCUS_BUTTON,
	PUSH_BUTTON,
	MAX_BUTTON_IMAGE_TYPE
};

class ControlDlg : public CDialogEx
{
	DECLARE_DYNAMIC(ControlDlg)

public:
	ControlDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~ControlDlg();

	// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CONTROLDLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedDelete();
	afx_msg void OnBnClickedActorcreate();
	afx_msg void OnBnClickedFbxrender();
	afx_msg void OnBnClickedSkelrender();

public:
	afx_msg void OnBnClickedBaserender();
	afx_msg void OnBnClickedObbcoll();
	afx_msg void OnBnClickedSpherecoll();
	afx_msg void OnBnClickedPrecreate();
	afx_msg void OnBnClickedPredelete();
	afx_msg void OnLbnSelchangePrelist();
	afx_msg void OnEnChangeCollgroup();
	virtual BOOL OnInitDialog();

	afx_msg void OnBnClickedRenddelete();
	afx_msg void OnBnClickedColldelete();
	afx_msg void OnEnChangePrefabname();

	class CActor*									NewActor;
	CListBox										m_PreList;
	CEdit											m_CollGroupName;
	CEdit											m_PreFabName;

	std::list<SPTR<class CFbxRenderer>>				m_FbxList;
	std::list<SPTR<class COBBCollider>>				m_ObbList;
	std::list<SPTR<class CSphereCollider>>			m_SphereList;
	std::list<SPTR<class CNavigationObject>>		m_NaviObjList;

	std::map<std::wstring, SPTR<PrefabData>>		m_PreFabDataMap;

	void SetPrefabData();

	void SavePrefabData(const wchar_t* _Path);
	void ReadPrefabData();
	afx_msg void OnBnClickedBinartsave();
	CEdit m_FileName;
	afx_msg void OnEnChangeDataname();
	afx_msg void OnBnClickedBinaryload();

	BOOL m_ForwardValue;
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
	afx_msg void OnBnClickedLight();
	afx_msg void OnBnClickedFilter();
	afx_msg void OnBnClickedVolumelight();
	afx_msg void OnBnClickedBloom();
	afx_msg void OnBnClickedGau();
	afx_msg void OnBnClickedOn();
	afx_msg void OnBnClickedOff();
	BOOL m_ActorOnOff;

	void ActorRadioOn()
	{
		m_ActorOnOff = false;
		UpdateData(FALSE);
	}
	void ActorRadioOff()
	{
		m_ActorOnOff = true;
		UpdateData(FALSE);
	}
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedScenesave();
	afx_msg void OnBnClickedSceneload();
	afx_msg void OnBnClickedNewsceneload();

public:
	std::map<int, std::list<SPTR<CActor>>>				m_AllActorMap;
	std::map<int, std::list<SPTR<CActor>>>::iterator	m_StartIter;
	std::map<int, std::list<SPTR<CActor>>>::iterator	m_EndIter;

	std::vector<SPTR<ActorSaveData>>					m_vecActor;
	std::list<SPTR<CFilterEffect>>						m_listFilter;
	std::list<SPTR<CFilterEffect>>::iterator			m_StartList;
	std::list<SPTR<CFilterEffect>>::iterator			m_EndList;

	std::vector<SPTR<ActorSaveData>>					m_vecLoadActor;
	std::map<int, std::list<SPTR<CActor>>>				m_mapAllLoadActor;

	std::vector<SPTR<FilterSaveData>>					m_vecFilter;

	std::list<SPTR<class CFilterEffect>>				m_listDeleteFilter;

	void SaveScene(const CString& _Path);
	void SaveBinaryScene(const CString& _Path);

	// 기존의 씬파일,씬배치 함수
	void ReadScene(const CString& _Path);
	void LoadScene();

	// 새로운 씬파일,씬배치 함수
	void NewReadScene(const CString& _Path);
	void NewLoadScene(const SCENEFILEVISION& _Version);

	void SaveFilter(const CString& _Path, SPTR<class CCamera> _SaveCam);
	void SaveBinaryFilter(const CString& _Path);
	void LoadFilter(const CString & _Path, SPTR<CCamera> _Cam);

	afx_msg void OnBnClickedCamsave();
	afx_msg void OnBnClickedCamload();
	afx_msg void OnBnClickedCollmove();

	afx_msg void OnBnClickedGizmo();
	afx_msg void OnBnClickedGizmo2();
	afx_msg void OnBnClickedNavi();
	afx_msg void OnBnClickedNavidelete();
	afx_msg void OnBnClickedPlaybutton1();
	afx_msg void OnBnClickedPausebutton1();
	afx_msg void OnBnClickedStopbutton1();

	CMFCButton m_PlayButton;
	CImage m_PlayImage[MAX_BUTTON_IMAGE_TYPE];

	CMFCButton m_PauseButton;
	CImage m_PauseImage[MAX_BUTTON_IMAGE_TYPE];

	CMFCButton m_StopButton;
	CImage m_StopImage[MAX_BUTTON_IMAGE_TYPE];
	afx_msg void OnBnClickedFog();
	afx_msg void OnBnClickedAddcoll();
	void ReadCollPreData();
	std::list<SPTR<CFbxRenderer>>						m_listAddCollFbx;
	std::map<std::wstring, SPTR<PrefabData>>			m_mapAddCollPreData;


	TerrainData											m_TerrainLoadData;

	afx_msg void OnBnClickedMagic();
	afx_msg void OnBnClickedGreen();


public:
	void SetForward();
	std::map<int, std::list<SPTR<CActor>>>				m_mapChangeActor;
	std::list<SPTR<CFbxRenderer>>						m_listChangeFbx;
	afx_msg void OnBnClickedChange();
};
