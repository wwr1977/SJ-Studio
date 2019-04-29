#pragma once
#include <ObjBase.h>

class CEffect;
class CEffectMgr : public CObjBase
{
private:
	// 이팩트 매니져는 각 씬들이 각자 들고있고 Build될때 사용할 이팩트를 
	// 로드하고 미리 1개 만들고 만일 2개 이상 이펙트를 돌려야할때는 
	//그때 그때 원본을 복제해서 만든다.
	CGameScene*													m_PairScene;
	vector<SPTR<CEffect>>										m_vecEffect;
	vector<list<SPTR<CEffect>>>									m_vecSpareEffect;


public:
	void SettingScene(CGameScene* _Scene);
	void SettingCurEffectLayer(const bool& _bChar);
public:
	//Effect의 원본을 뽑을떄만 사용
	SPTR<CEffect> operator[](const size_t& _Index);
	SPTR<CEffect> operator[](const int& _Index);
	SPTR<CEffect> operator[](const tstring& _EffectName);

public:
	int	 RandomEffectOn();
	// Effect 원본 & 사본 등을 사용할때 사용
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

