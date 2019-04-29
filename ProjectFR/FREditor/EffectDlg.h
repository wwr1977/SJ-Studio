#pragma once
#include "afxwin.h"


// CEffectDlg 대화 상자입니다.

class CEffectDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CEffectDlg)
	enum
	{
		EFFECTANIMATION,
		SOUND,
		EFFECT
	};

	enum LAYERINDEX
	{
		MONBACK,
		// 플레이어 뒤 == 몬스터 앞 
		PLAYERBACK,
		PLAYERFRONT,
		MAXLAYERINDEX
	} m_iCurLayer;

public:
	CEffectDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CEffectDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EFFECT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	//다이얼로그가 처음 활성화 되었을때 필요한 텍스쳐를 로딩 
	bool				 m_bInit;
	// 다이얼로그가 활성화 된 뒤 처음으로 콤보박스가 눌렸을때 
	// 멀티스프라이트를 로드하기 위한 변수(멀티스프라이트 수정 고려 X,1회 로딩)
	bool				 m_bMultiSpriteInit;

	// 다이얼로그가 처음 활성화 되었거나 
	// AnimationDlg에서 Save작업을 수행한 경우
	// 모델에 적용된 애니메이션을 재적용을 하기 위한 변수
	bool				 m_bModelSetting;
	vector<CEdit*>		 m_vecEditBox;


	CListBox m_EffectListCtrl;
	int		 m_iPrevIndex;
	// 리스트에 출력할 목록을 지정하는 콤보 박스
	CComboBox m_ListModeCom;
	int		  m_iListMode;

	// 현재 지정된 이팩트 이름을 출력하는 리스트
	CListBox m_EffectAniNameCtrl;
	// 이팩트 이름
	CEdit m_EffectNameCtrl;
	CString m_EffectName;

	// 이펙트의 성질을 체크하는 변수
	BOOL m_bLoop;
	BOOL m_bBlackBack;
	BOOL m_bFixPos;
	

	CListBox			m_CurSoundCtrl;
	CComboBox			m_SoundListComCtrl;
	list<SoundData>		m_listSoundData;
	CEdit				m_SoundStartTimeCtrl;
	CString				m_SoundStartTime;
	float				m_fSoundStartTime;

	CEdit m_LifeTimeCtrl;
	CString m_LifeTime;
	float	m_fLifeTime;

	CEdit m_CustomSizeXCtrl;
	int m_iCustomSizeX;
	

	CEdit m_CustomSizeYCtrl;
	int m_iCustomSizeY;


	CEdit m_ImageMagCtrl;
	CString m_ImageMag;
	float	m_fImageMag;

	// 랜더링 층을 출력하는 콤보 박스
	CComboBox m_LayerComCtrl;

	CEdit m_RenderStartTimeCtrl;
	CString m_RenderStartTime;
	float	m_fRenderStartTime;

	size_t m_EffectNumber;


	void InitEffectDlg();
	void CompleteEffectSetting();
	// 이팩트 다이얼로그에 필요한 리소스가 메모리에 적재안된경우 로드하는 함수
	void LoadEffectTexture();
	BOOL LoadEffectMultiSprite();
	void LoadEffectAnimation();


	// 리스트 콤보 박스가 눌렷을때 어떤리스트로 리스트박스를 업데이트 하는 함수
	void UpdateEffectAniList();
	void UpdateSoundList();
	void UpdateEffectList();
	void UpdateList(const int& _Index);
	
	// 리스트의 Item이 선택되었을때 어떤 모드냐에 따른 함수
	void SelectEffectAniList();
	void SelectSoundList();
	void SelectEffectList();

	void InitEffectData(const EFFECTDATA& _EffectData = EFFECTDATA());

	void SettingEffectModel();

	// Button클릭 이벤트 처리 함수
	BOOL SoundPlayEvent();
	BOOL SoundAddButtonEvent();
	BOOL SoundDeleteButtonEvent();
	BOOL SoundTimeButtonEvent();
	BOOL CreateButtonEvent();
	BOOL DeleteButtonEvent();
	BOOL DataLoadButtonEvent();
	BOOL DataSaveButtonEvent();

	// 이팩트를 매니져에 생성전에 에디터안의 값이 올바른치 판단하는 함수
	BOOL CheckEffectDataValue();
	// 에디터 박스가 클릭된 경우 전체 선택을 하기 위한 함수
	BOOL EditBoxClickCheck(MSG* pMsg);


	virtual void OnOK();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnCbnSelchangeEffectlistcom();
	afx_msg void OnLbnSelchangeEffectlist();
	afx_msg void OnEnKillfocusStarttime();
	afx_msg void OnEnKillfocusEffectlifetime();
	afx_msg void OnEnKillfocusImagemag();
	afx_msg void OnEnKillfocusImagetime();
	afx_msg void OnCbnSelchangeSoundcombo();
	afx_msg void OnCbnSelchangeLayercombo();
};
