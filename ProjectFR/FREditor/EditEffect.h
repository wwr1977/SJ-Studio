#pragma once
#include <Logic.h>

class CEffectMgr;
class CEditEffect : public CLogic
{
	friend class CEffectMgr;

	struct Sound
	{
		tstring			SoundKey;
		int				PlayerIndex;

		Sound() :SoundKey(_T("")), PlayerIndex(0) {}
		Sound(const tstring& _Key, const int& _Index) :SoundKey(_Key), PlayerIndex(_Index) {}
		~Sound() {}
	};


	static bool							bCurCharacterSkill;	
public:
	static void SettingCurEffectLayer(const bool& _bChar);
private:
	size_t								m_EffectIndex;
	tstring								m_EffectName;
	tstring								m_EffectAniName;
	SPTR<CAniRenderer>					m_EffectRen;
	SPTR<CAniRenderer>					m_MonTurnEffectRen;	// �÷��̾� �տ� ������ �ǵ���
	vector<SPTR<CSoundPlayer>>			m_EffectSound;

	SPTR<CAniRenderer>					m_CurActiveRen;
	// bin ���Ͽ��� ������ Animation ������ ���´�
	//ANIMATIONDATA*						m_EffectAniData;

	float								m_fLifeTime;
	float								m_fAccTime;


	float								m_fPlayRender;
	vector<SoundData>					m_SoundList;
	queue<Sound>						m_SoundQueue;

	// �̹��� ũ�⸦ ������ ���� 
	float								m_fImageMag;
	// �ƴϸ� Ŀ���� ������� �Ұ����� 
	Vec2								m_CustomSize;

	bool								m_bOriginal;
	int									m_iRenLayer;
	// �޹�濡 ������ �ִ°�� �������� ������ �����Ѵ�.
	bool								m_bLoop;
	bool								m_bBlackBack;
	bool								m_bNonCamAffect;

	bool								m_bRendering;


public:
	void InitData(const EFFECTDATA& _EffectData);
	void InitData(SPTR<CEditEffect> _EffectData);
	void InitSoundList();
	void Init();
	void PreUpdate();
	void Update();
	BOOL ChangeAnimationData();
	const float GetEffectPlayTime();
	void AllSoundStop();
	void SetRenPivot(const Vec2& _Pivot);
	void SetAlpha(const float& _Alpha);
public:
	tstring GetEffectName();
	unsigned int GetEffectIndex();

public:
	void SetCustomSize(const Vec2& _CustomSize);
	void SetImageScale(const float& _ImageMag);

public:
	void		SetOriginal();
	bool		IsPlay();
	bool		IsRestEffect();
	void		EffectOn();
	void		EffectOff();
	EFFECTDATA  GetEffectData();
	// ��ų �������� ��ġ�� �Ű������� �޴´�
	void		SetSkillEffectData(const SKILLEFFECTDATA& _Data, const Vec2& _CasterPos);
	void		SetHitEffectData(const HITDATA& _HitData);
	void		SetPos(const Vec2& _Pos);


public:
	CEditEffect();
	~CEditEffect();
};


