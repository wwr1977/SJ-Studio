#pragma once
#include "MainPlayerState.h"
class CPlayerWalk : public CMainPlayerState
{

public:
	// ���°� ����� ������ �ִϸ��̼� ���� + �̵��� �ʱ�ȭ �Ѵ�.
	void StateReset();
	void Update();

public:
	// ���°� ����Ǿ����� true�� ����
	bool WalkUpdate();
	const tstring GetCurAnimationName();

public:
	CPlayerWalk();
	~CPlayerWalk();
};

