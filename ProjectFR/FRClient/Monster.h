#pragma once
#include <Logic.h>

class CCollision;
class CFixRenderer;
class CMonster : public CLogic
{
	SPTR<CFixRenderer>				m_Renderer;
	SPTR<CCollision>				m_Col;

public:
	void Init();
	void PreUpdate();
	void Update();
	void PostUpdate();

public:
	void Enter(SPTR<CCollision> _pThis, SPTR<CCollision> _pOtherCol);


public:
	CMonster();
	~CMonster();
};

