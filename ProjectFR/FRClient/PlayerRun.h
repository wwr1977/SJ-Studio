#pragma once
#include "MainPlayerState.h"
class CPlayerRun : public CMainPlayerState
{
public:
	// ���°� ����� ������ �ִϸ��̼� ���� + �̵��� �ʱ�ȭ �Ѵ�.
	void StateReset();
	void Update();

public:
	// ���°� ����Ǿ����� true�� ����
	bool RunUpdate();
	const tstring GetCurAnimationName();


public:
	CPlayerRun();
	~CPlayerRun();
};

