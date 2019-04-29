#pragma once
#include <Script.h>
#include "MiddleStruct.h"


#define MAXGENERATORCOUNT 20.f
#define RECORVERYTIME 100.f
#define REPAIRFACTOR 1.f
#define BREAKFACTOR 0.5f

enum MAGICCIRCLE_COLOR
{
	MC_YELLOW,
	//������
	MC_GREEN,
	//����
	MC_RED,
	MC_MAX
};


// �������� ����
enum MAGICCIRCLE_GENSTATE 
{
	GENERATOR_IDLE,
	GENERATOR_REPAIL,
	GENERATOR_DEMAGED,
	GENERATOR_REPAILCOMPLETE,
	MAX_MAGICCIRCLE_GENSTATE
};

// ������ ����
enum MAGICCIRCLE_BOARDSTATE
{
	BOARD_IDLE,	
	BOARD_TRAPACTIVE,
	BOARD_STOP,
	MAX_MAGICCIRCLE_BOARDSTATE
};


typedef struct _tagMCIRCLEDATA 
{
	MAGICCIRCLE_COLOR				CircleColor;
	bool							bServerObj;
}MCData;

class CFbxRenderer;
class CMagicCircle : public CScript
{
	friend class CDBDRule;
private:
	bool							m_bServerFlag;
	MAGICCIRCLE_COLOR				m_CircleColor;
	unsigned int					m_CircleID;

	// �÷��̾ �ش� �浹ü�� �浹�ϰ� �Ǹ� ��ȣ�ۿ��� Ȱ��ȭ(���� or �Ѿ�߸���)
	SPTR<class CSphereCollider>		m_PlayerDetectCol;
	SPTR<CSphereCollider>			m_KillerAttackCol;
	SPTR<class CSphereCollider>		m_MagicSphere;
	SPTR<CFbxRenderer>				m_CircleRen;
	void(CMagicCircle::*m_pMagicCircleUpdate)(const float&);

private:
	float							m_fAccTime;
	float 							m_GeneratorCount;
	MAGICCIRCLE_GENSTATE			m_eGeneratorState;

private:
	MAGICCIRCLE_BOARDSTATE			m_eBoardState;


public:
	void SetCircleID(const unsigned int& _ID)
	{
		m_CircleID = _ID;
	}
	unsigned int GetCircleID()
	{
		return m_CircleID;
	}
	MAGICCIRCLE_COLOR GetMagicCircleColor()
	{
		return m_CircleColor;
	}
	SPTR<CFbxRenderer> GetMagicCircleRen();
	const MAGICCIRCLE_GENSTATE GetGenState()
	{
		return m_eGeneratorState;
	}
	const MAGICCIRCLE_BOARDSTATE GetBoardState()
	{
		return m_eBoardState;
	}

public:
	const bool Init(MAGICCIRCLE_COLOR _Color);
	const bool Init(const MCData& _InitData);
	void Update(const float& _DeltaTime) override;
	void LastUpdate(const float& _DeltaTime) override;

private:
	void GeneratorUpdate(const float& _DeltaTime);
	void BoardUpdate(const float& _DeltaTime);

public:
	// ������ �������� �÷��̾� �浹�̺�Ʈ�� �Լ�
	void GeneratorActiveEvent(SPTR<class CCollider> _This, SPTR<class CCollider> _Other);
	void GeneratorActiveEnd(SPTR<CCollider> _This, SPTR<CCollider> _Other);
	// ����(����) �������� �÷��̾� �浹�̺�Ʈ �Լ�
	void BoardActiveEvent(SPTR<CCollider> _This, SPTR<CCollider> _Other);
	void BoardActiveEnd(SPTR<CCollider> _This, SPTR<CCollider> _Other);
	// ����(���� ����) �浹ü�� ������ �浹�̺�Ʈ �Լ�
	void BoardTrapAttackEvent(SPTR<CCollider> _This, SPTR<CCollider> _Other);

public:
	void RepairGenerator();
	void CancelRepairGenerator();
	void BreakGenerator();
	void RepairCompleteEvent();
	const bool CheckRepairComplete();
	const bool CheckNoRepair();
	const bool CheckBreakableGenerator();

public:
	void RecoveryCompleteTrap();
	void ActiveTrap();
	void StopTrap();
	const bool CheckTrapAvailable();
	const bool CheckTrapActive();
	//const bool CheckBreakableTrap();
public:
	MagicCircleData GetCircleData();

public:
	const bool RepairGenerator(const float& _GenCount) 
	{
		if (m_eGeneratorState == GENERATOR_REPAILCOMPLETE)
			return false;

		m_GeneratorCount += _GenCount;

		if (m_GeneratorCount >= MAXGENERATORCOUNT)
		{
			m_GeneratorCount = MAXGENERATORCOUNT;
			//m_eGeneratorState = GENERATOR_REPAILCOMPLETE;
			return true;
		}
		
		return false;
	}
	void SetGeneratorCount(const float& _Count)
	{
		m_GeneratorCount = _Count;
	}
	const bool IsMaximumGeneratorCount()
	{
		return m_GeneratorCount>= MAXGENERATORCOUNT;
	}
	const float GetGeneratorCount() { return m_GeneratorCount; }
	const float GetGeneratorGauge() { return m_GeneratorCount / MAXGENERATORCOUNT; }
public:
	CMagicCircle();
	~CMagicCircle();
};

