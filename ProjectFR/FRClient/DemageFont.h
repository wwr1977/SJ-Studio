#pragma once
#include "NumberUI.h"

class CTurnMgr;
class CDemageFont : public CNumberUI
{
	static int	DemageFontCount;

private:
	CTurnMgr*					m_pTurnMgr;
	float						m_fAccTime;
	float						m_fUpSpeed;
	//	랜더 순서를 저장하기 위한 변수
	int							m_iRenderIndex;
	float						m_fRenderStartTime;
	
	float						m_fExpUpdateTime;
	float						m_fStairUpdateTime;

	float						m_fFontMag;
	float						m_fMaxFontMag;

	UINT						m_FontStyle;
	int							m_Option;

	Vec2						m_MiddlePos;
public:	
	// 조건자 함수
	// m_iRenderIndex를 오름차순으로 정렬하기위한 조건자
	static bool RenderOrder(SPTR<CDemageFont> _Left, SPTR<CDemageFont> _Right);
	
	bool operator>(SPTR<CDemageFont> _Other);
	bool operator>=(SPTR<CDemageFont> _Other);
	bool operator<(SPTR<CDemageFont> _Other);
	bool operator<=(SPTR<CDemageFont> _Other);

public:
	void Init();
	void PreUpdate();
	void Update();
	

	void ExpansionUpdate();
	void StairsUpdate();

public:
	const int GetRenderIndex();
	void SetTurnMgr(CTurnMgr* _TurnMgr);
	void On(const int& _Number, const Vec2& _Pos);
	void On(const int& _Number, const Vec2& _Pos, const tstring& _SpriteName);
	void On(const int& _Number, const Vec2& _Pos, const size_t& _DemageFontIndex);
	void ActorOn();
	void MiddleOn(const int& _Number, const Vec2& _Pos, const size_t& _DemageFontIndex);
	void Off();
	
public:
	void SetRenderTimer(const float& _RenderTimer);
	void SetExpStyle();
	void SetStairStyle(const int& _Option);

public:
	CDemageFont();
	~CDemageFont();
};

