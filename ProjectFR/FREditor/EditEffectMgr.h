#pragma once
#include <ObjBase.h>

class CEditEffect;
class CEditEffectMgr : public CObjBase
{
private:
	// 이팩트 매니져는 각 씬들이 각자 들고있고 Build될때 사용할 이팩트를 
	// 로드하고 미리 1개 만들고 만일 2개 이상 이펙트를 돌려야할때는 
	//그때 그때 원본을 복제해서 만든다.
	CGameScene*													m_PairScene;
	
	vector<SPTR<CEditEffect>>									m_vecEffect;
	vector<list<SPTR<CEditEffect>>>								m_vecSpareEffect;
	
	// 다음에 등록될 Effect의 Index를 저장
	size_t														m_NextEffectNumber;

public:
	void SettingScene(CGameScene* _Scene);

public:
	//Effect의 원본을 뽑을떄만 사용
	SPTR<CEditEffect> operator[](const size_t& _Index);
	SPTR<CEditEffect> operator[](const int& _Index);
	SPTR<CEditEffect> operator[](const tstring& _EffectName);

public:
	int	 RandomEffectOn();
	// Effect 원본 & 사본 등을 사용할때 사용
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

	// Edit용
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


