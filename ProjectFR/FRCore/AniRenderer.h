#pragma once
#include "Renderer.h"

class CGameMultiSprite;
class CAniRenderer : public CRenderer
{

public:
	class CAnimation : public CObjBase 
	{
	public:
		friend CAniRenderer;


	private:
		CAniRenderer*					m_pAniRenderer;
		SPTR<CGameMultiSprite>			m_AniSprite;
		
		ANIDATA							m_AniData;

		int								m_iCurIndex;
		int								m_iCurAniCount;
		float							m_fCurTime;

		bool							m_bEndFrame;			// 현재 프레임이 마지막인지 체크
		bool							m_bEndAni;				// 지정된 AniCount만큼 돌았는지 체크
	

		int								m_AniStyle;
	
	public:
		void Init(int _StartIndex) 
		{
			m_iCurIndex = _StartIndex;
		}
		void SetAniSprite(SPTR<CGameMultiSprite> _AniSprite);
		void SetInterval(float _Interval) 
		{
			fill_n(m_AniData.Interval ,m_AniData.FullRange.End- m_AniData.FullRange.Start + 1 , _Interval);
		}
		void SetRepeatInterval(float _Interval) 
		{
			fill_n(m_AniData.RepeatInterval, m_AniData.RepeatRange.End - m_AniData.RepeatRange.Start + 1, _Interval);
		}
		void SetInterval(float _Interval,int _Index)
		{
			if (_Index > m_AniData.FullRange.End || _Index < m_AniData.FullRange.Start) 
			{
				return;
			}
			m_AniData.Interval[_Index] = _Interval;
		}
		void SetInterval(float _Interval, ANIRANGE _ChangeIndex) 
		{
			fill_n(m_AniData.Interval + _ChangeIndex.Start, _ChangeIndex.End - _ChangeIndex.Start + 1, _Interval);
		}
		SPTR<CGameSprite> UpdateAni();

		Vec2 GetSpriteSize();
		Vec2 GetSpritePivot();
		void SetAniStyle(ANISTYLE _AniStyle);
		ANISTYLE GetAniStyle();
		ANISPRITETYPE GetAniSpriteType();
		ANIDATA GetAniData();
		
		
	public:
		static CAnimation* CreateAni(CAniRenderer* _AniRenderer, ANIDATA _AniData);

	public:
		void AniReset();
	
	private:
		CAnimation(CAniRenderer* _AniRenderer,ANIDATA _AniData);
		~CAnimation();
	};
	

typedef  class CAniRenderer::CAnimation CAnimation;

private:
	SPTR<CAnimation>							m_CurAni;
	map<tstring, SPTR<CAnimation>>				m_mapAnimation;
	

private:
	SPTR<CAnimation>	FindAnimation(const tstring& _Key);
public:
	BOOL				ExistAnimation(const tstring& _Key);
public:
	BOOL				EraseAnimation(const tstring& _EraseKey);
public:
	BOOL				ClearAnimationMap();

public:
	bool	ChangeAni(const tstring& _Key);
	BOOL	CurAniReset();
	void SetRenPivot(const Vec2& _Pivot) 
	{
		m_RenPivot = _Pivot;
	}
	void SetRenPivot(const Vec3& _Pivot) 
	{
		m_RenPivot = Vec2{ _Pivot.x,_Pivot.y };
	}
	void SetInterval(float _Interval, int _Index) 
	{
		m_CurAni->SetInterval(_Interval, _Index);
	}
	void SetInterval(float _Interval)
	{
		m_CurAni->SetInterval(_Interval);
	}
	void SetRepeatInterval(float _Interval) 
	{
		m_CurAni->SetRepeatInterval(_Interval);
	}
	void SetInterval(float _Interval, ANIRANGE _ChangeIndex)
	{
		m_CurAni->SetInterval(_Interval, _ChangeIndex);
	}
	bool IsLastFrame() 
	{
		return m_CurAni->m_bEndFrame;
	}
	bool IsEndAni() 
	{
		return m_CurAni->m_bEndAni;
	}
	BOOL IsSettingAni() 
	{
		if (nullptr == m_CurAni) 
		{
			return FALSE;
		}
		return TRUE;
	}
	const float GetFullAnimationTime();
	

public:
	tstring CurAniSpriteName();
	tstring CurAniName();
	ANIDATA* GetCurAniData();
	const Vec2 GetCurAniSpriteSize();
	const Vec2 GetCurAniSpritePivot();
	ANISPRITETYPE GetCurAniSpriteType();
	void  SettingWorldMat(Mat* _Mat);
	void SetCurAniStyle(ANISTYLE _AniStyle);
	ANISTYLE GetCurAniStyle();
	map<tstring, SPTR<CAnimation>>* GetAniMap();
	int GetCurAniFrame();
	int GetCurAniCount();
	void SetImageMag(const float& _Mag);
	void SetActorPos(const Vec3& _Pos);
	void ActorOn();
	void ActorOff();

public:


	CAnimation* CreateAnimation(const tstring& _AniKey, SPTR<CGameMultiSprite> _MultiSprite,const ANIDATA& _AniData);
	CAnimation* CreateAnimation(const ANIDATA& _AniData);
	CAnimation* CreateAnimation(ANIMATIONDATA* _AnimationData);
	CAnimation* CreateEditAnimation(ANIDATA _AniData);
	CAnimation* CreateAnimation(const tstring& _AniKey, const tstring& _MultiSpriteKey, ANISTYLE _AniStyle);
	CAnimation* CreateAnimation(const tstring& _AniKey, const tstring& _MultiSpriteKey, ANISTYLE _AniStyle,int _StartIndex,int _EndIndex,int _AniCount = 1);
	CAnimation* CreateAnimation(const tstring& _AniKey, const tstring& _MultiSpriteKey, ANISTYLE _AniStyle, ANIRANGE _FullRange, int _AniCount = 1);
	CAnimation* CreateAnimation(const tstring& _AniKey, const tstring& _MultiSpriteKey, ANISTYLE _AniStyle, int _StartIndex, int _EndIndex, int _AniCount ,int _RepeatStartIndex,int _RepreatEndIndex);
	CAnimation* CreateAnimation(const tstring& _AniKey, const tstring& _MultiSpriteKey, ANISTYLE _AniStyle, ANIRANGE _FullRange, int _AniCount, ANIRANGE _RepeatRange);
	CAnimation* CreateAnimation(const tstring& _MultiSpriteKey, ANISTYLE _AniStyle);
	CAnimation* CreateAnimation(const tstring& _MultiSpriteKey, ANISTYLE _AniStyle, int _StartIndex, int _EndIndex, int _AniCount = 1);
	CAnimation* CreateAnimation(const tstring& _MultiSpriteKey, ANISTYLE _AniStyle, ANIRANGE _FullRange,int _AniCount = 1);
	CAnimation* CreateAnimation(const tstring& _MultiSpriteKey, ANISTYLE _AniStyle, int _StartIndex, int _EndIndex, int _AniCount, int _RepeatStartIndex, int _RepreatEndIndex);
	CAnimation* CreateAnimation(const tstring& _MultiSpriteKey, ANISTYLE _AniStyle, ANIRANGE _FullRange, int _AniCount ,ANIRANGE _RepeatRange);


///////////////////////////////////////////		애니메이션 Save & Load 함수
	BOOL SaveAnimationData(ANISPRITETYPE _AniSpriteType);

public:
	virtual void Render();
	void RenderOff();

public:
	CAniRenderer();
	~CAniRenderer();
};

