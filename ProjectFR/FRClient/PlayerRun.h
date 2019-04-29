#pragma once
#include "MainPlayerState.h"
class CPlayerRun : public CMainPlayerState
{
public:
	// 상태가 변경된 순간에 애니메이션 방향 + 이동을 초기화 한다.
	void StateReset();
	void Update();

public:
	// 상태가 변경되었을때 true를 리턴
	bool RunUpdate();
	const tstring GetCurAnimationName();


public:
	CPlayerRun();
	~CPlayerRun();
};

