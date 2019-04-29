#pragma once
#include <Logic.h>
class CEditBackGround : public CLogic
{
private:
	SPTR<CFixRenderer>					m_BackRen;

public:
	void Init();
	void Update();

public:
	void SetSprite(const tstring& _BackName);

public:
	CEditBackGround();
	~CEditBackGround();
};

