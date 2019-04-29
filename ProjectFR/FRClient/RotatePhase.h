#pragma once
#include "BasePhase.h"

class CTurnOrderUI;
class CRotatePhase : public CBasePhase
{
private:
	// ������ Turn UI�� �󸶳� �ڷ� �о�������� ����
	// ����Ʈ(0)�� ���� �Ʒ��� �о������
	SPTR<CTurnOrderUI>				m_CurTurnUI;
	int								m_iRotateCount;
	float							m_fAccTime;

private:
	void StateReset();
	void Update();

private:
	bool TurnRotation();

public:
	CRotatePhase();
	~CRotatePhase();
};

