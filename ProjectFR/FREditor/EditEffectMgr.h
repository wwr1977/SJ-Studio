#pragma once
#include <ObjBase.h>

class CEditEffect;
class CEditEffectMgr : public CObjBase
{
private:
	// ����Ʈ �Ŵ����� �� ������ ���� ����ְ� Build�ɶ� ����� ����Ʈ�� 
	// �ε��ϰ� �̸� 1�� ����� ���� 2�� �̻� ����Ʈ�� �������Ҷ��� 
	//�׶� �׶� ������ �����ؼ� �����.
	CGameScene*													m_PairScene;
	
	vector<SPTR<CEditEffect>>									m_vecEffect;
	vector<list<SPTR<CEditEffect>>>								m_vecSpareEffect;
	
	// ������ ��ϵ� Effect�� Index�� ����
	size_t														m_NextEffectNumber;

public:
	void SettingScene(CGameScene* _Scene);

public:
	//Effect�� ������ �������� ���
	SPTR<CEditEffect> operator[](const size_t& _Index);
	SPTR<CEditEffect> operator[](const int& _Index);
	SPTR<CEditEffect> operator[](const tstring& _EffectName);

public:
	int	 RandomEffectOn();
	// Effect ���� & �纻 ���� ����Ҷ� ���
	SPTR<CEditEffect> PopEffect(const size_t& _Index);
	SPTR<CEditEffect> PopSpareEffect(const size_t _Index);
	SPTR<CEditEffect> EffectOn(const size_t& _Index);
	SPTR<CEditEffect> EffectOn(const int& _Index);
	SPTR<CEditEffect> EffectOn(const unsigned int& _Index);
	SPTR<CEditEffect> EffectOn(const tstring& _EffectName);
	
public:
	BOOL ExistEffect(const tstring& _EffectName);
	BOOL LoadEffectData();

	void EffectAllOff();

	SPTR<CEditEffect> FindEffect(const tstring& _EffectName);
	SPTR<CEditEffect> FindEffect(const size_t& _EffectIndex);
	CEditEffect* CreateEffect(const EFFECTDATA& _EffectData);
	CEditEffect* ChangeEffect(const EFFECTDATA& _EffectData);
	CEditEffect* CreateSpareEffect(const size_t& _Index);
	CEditEffect* CreateSpareEffect(const unsigned int& _Index);
	bool isEffectOn(const size_t& _Index);

	// Edit��
	BOOL EmptyEffectMap();
	vector<SPTR<CEditEffect>>*	GetEffectVector();
	size_t SearchNextEffectIndex();
	BOOL DeleteEffect(const tstring& _EffectName);

	BOOL LoadEffectData(FILE* _ReadFile, vector<EFFECTDATA>* _TempVec);
	BOOL SaveEffectData(FILE* _WriteFile);

	void SettingCurEffectLayer(const bool& _bChar);
public:
	CEditEffectMgr();
	~CEditEffectMgr();
};


