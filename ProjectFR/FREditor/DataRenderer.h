#pragma once
#include <Logic.h>

class CGauge;
class CDataRenderer : public CLogic
{
	enum PARA
	{
		HPPARA,
		MPPARA,
		STRPARA,
		DEXPARA,
		INTPARA,
		LUCKPARA,
		DAMAGEPARA,
		DEFPARA,
		CRIPARA,
		CRIDAMAGEPARA,
		AVOIDPARA,
		LEVELPARA,
		MAXPARA,
	};
private:
	vector<SPTR<CGauge>>				m_vecGauge;
	// StrGauge Bar의 시작 위치
	Vec2								m_Pos;
	// CharStat Edit의 슬라이더가 현재 가리키는 레벨
	CString								m_CurCharacter;
	int									m_iCurLevel;
	MAINSTAT							m_CurMainStat;

	// 현재 적용중인 데이터 정보가 테이블데이터인지 아님 초기 설정값인지 판단
	bool								m_bTableData;
	vector<STAT>*						m_mapCurTable;
	STAT								m_CurRenStat;

	float*								m_pCurParameterData;
	vector<CString>						m_vecStatString;



public:
	void Init();
	void Update();
	void FontRender();
	void TableDataRender();
	void InitDataRender();

public:
	void ConnectParameterArray(float* _ParameterArr);
	void SettingTableData(vector<STAT>* _Table);
	void SettingTableData(const tstring& _UnitName);
	void SettingInitData(const STAT& _InitStat);
	// 1개의 스탯 정보 수정(EditBox 수정)
	void UpdateStat(const PARA& _Para);
	void UpdateStat(const int& _Para);
	void UpdateStatGauge(const PARA& _Para);
	void UpdateStatString(const PARA& _Para);
	int GetDetailStat(const PARA& _Para);
	// 일부 스탯 정보 수정 (Level 수정)
	void SetLevel(const int& _Level);
	// 전체 스탯정보 수정
	void UpdateStat();
	void SetCurMainStat(const MAINSTAT& _MainStat);
	void SetCurCharacter(const CString& _Character);

public:
	CDataRenderer();
	~CDataRenderer();
};

