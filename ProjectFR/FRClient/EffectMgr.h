#pragma once
#include <ObjBase.h>

class CEffect;
class CEffectMgr : public CObjBase
{
private:
	// ����Ʈ �Ŵ����� �� ������ ���� ����ְ� Build�ɶ� ����� ����Ʈ�� 
	// �ε��ϰ� �̸� 1�� ����� ���� 2�� �̻� ����Ʈ�� �������Ҷ��� 
	//�׶� �׶� ������ �����ؼ� �����.
	CGameScene*													m_PairScene;
	vector<SPTR<CEffect>>										m_vecEffect;
	vector<list<SPTR<CEffect>>>									m_vecSpareEffect;


public:
	void SettingScene(CGameScene* _Scene);
	void SettingCurEffectLayer(const bool& _bChar);
public:
	//Effect�� ������ �������� ���
	SPTR<CEffect> operator[](const size_t& _Index);
	SPTR<CEffect> operator[](const int& _Index);
	SPTR<CEffect> operator[](const tstring& _EffectName);

public:
	int	 RandomEffectOn();
	// Effect ���� & �纻 ���� ����Ҷ� ���
	SPTR<CEffect> PopEffect(const size_t& _Index);
	SPTR<CEffect> PopSpareEffect(const size_t _Index);
	SPTR<CEffect> EffectOn(const size_t& _Index);
	SPTR<CEffect> EffectOn(const int& _Index);
	SPTR<CEffect> EffectOn(const unsigned int& _Index);
	SPTR<CEffect> EffectOn(const tstring& _EffectName);

public:
	BOOL ExistEffect(const tstring& _EffectName);
	BOOL LoadEffectData();
	
	void EffectAllOff();
	
	SPTR<CEffect> FindEffect(const tstring& _EffectName);
	SPTR<CEffect> FindEffect(const size_t& _EffectIndex);
	CEffect* CreateEffect(const EFFECTDATA& _EffectData);
	CEffect* CreateSpareEffect(const size_t& _Index);
	CEffect* CreateSpareEffect(const unsigned int& _Index);
	bool isEffectOn(const size_t& _Index);


public:
	void DebugButtonEvent();

public:
	CEffectMgr();
	~CEffectMgr();
};

