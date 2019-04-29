#pragma once
#include <Logic.h>

class CSkillUIMgr;
class CSkillUITab;
class CSkillUIBoard : public CLogic
{
private:
	CSkillUIMgr*						m_pSkillUIMgr;
	SPTR<CFixRenderer>					m_BackBoard;
	vector<SPTR<CSkillUITab>>			m_vecSkillUITab;
	// ��ų Ÿ������ �з��� ���� ���õ� ĳ���Ͱ� ���� 
	
	// ��ų�� �ε����� ���´�.
	// 0 : Action , 1 : Ability , 2 : Burst , 3 : Item 
	vector<vector<size_t>>*				m_pSkillIndexList;
	size_t								m_CurSkillType;

	// ��ų �������� ��ġ Pivot
	vector<Vec2>						m_vecIconPivot;

public:
	void Init();
	void Update();

public:
	void SetSkillUIMgr(CSkillUIMgr* _Mgr);
	void SetPos(const Vec3& _Pos);
	void SetSize(const Vec2& _Size);
	void SetSkillIndexList(vector<vector<size_t>>* _SkillIndexList);

public:
	// �� ��ư ������ ���
	const  size_t GetTabCount();
	const Vec3 GetUIPlacePos(const size_t& _Index);
public:
	// �ش� ��ų Ÿ���� Tab ��ư�� ���
	SPTR<CSkillUITab> GetSkillUITab(const size_t& _SkillType);
	
	// Tab�� Ŭ���Ǿ����� Tab�� ���忡�� Ŭ���Ǿ����� �˸��� �Լ�
	void TabClickEvent(const size_t& _Index);
	void SkillUIClickEvent();

	// ���� ���� ��ųŸ���� ��ų�� UI�� ����.
	void BoardOff();

	// ����� Character�� ���õǾ������� Ȱ��ȭ(SelectPhase������ �۵�)
	// �� �̿��� ���¿����� ������.
	void BoardOn();

public:
	CSkillUIBoard();
	~CSkillUIBoard();
};

