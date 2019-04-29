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
	// StrGauge Bar�� ���� ��ġ
	Vec2								m_Pos;
	// CharStat Edit�� �����̴��� ���� ����Ű�� ����
	CString								m_CurCharacter;
	int									m_iCurLevel;
	MAINSTAT							m_CurMainStat;

	// ���� �������� ������ ������ ���̺��������� �ƴ� �ʱ� ���������� �Ǵ�
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
	// 1���� ���� ���� ����(EditBox ����)
	void UpdateStat(const PARA& _Para);
	void UpdateStat(const int& _Para);
	void UpdateStatGauge(const PARA& _Para);
	void UpdateStatString(const PARA& _Para);
	int GetDetailStat(const PARA& _Para);
	// �Ϻ� ���� ���� ���� (Level ����)
	void SetLevel(const int& _Level);
	// ��ü �������� ����
	void UpdateStat();
	void SetCurMainStat(const MAINSTAT& _MainStat);
	void SetCurCharacter(const CString& _Character);

public:
	CDataRenderer();
	~CDataRenderer();
};

