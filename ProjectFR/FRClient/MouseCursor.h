#pragma once
#include<Logic.h>

class CMouseCursor : public CLogic
{
	SPTR<CAniRenderer>					m_AniMouse;
	SPTR<CFixRenderer>					m_FixMouse;
	SPTR<CCollision>					m_MouseColCom;
	
	Vec2								m_CurMousePos;
	Vec2								m_PrevMousePos;

	bool								m_bMove;
	Vec2								m_MoveDir;


public:
	void Init();
	void PreUpdate();
	void Update();
	void DebugRender();

public:
	const bool IsMouseMove();
	const Vec2 GetMoveDir();
public:
	CMouseCursor();
	~CMouseCursor();
};

