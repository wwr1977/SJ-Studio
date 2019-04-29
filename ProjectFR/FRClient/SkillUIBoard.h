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
	// 스킬 타입으로 분류된 현재 선택된 캐릭터가 가진 
	
	// 스킬의 인덱스를 갖는다.
	// 0 : Action , 1 : Ability , 2 : Burst , 3 : Item 
	vector<vector<size_t>>*				m_pSkillIndexList;
	size_t								m_CurSkillType;

	// 스킬 아이콘의 배치 Pivot
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
	// 탭 버튼 갯수를 출력
	const  size_t GetTabCount();
	const Vec3 GetUIPlacePos(const size_t& _Index);
public:
	// 해당 스킬 타입의 Tab 버튼을 출력
	SPTR<CSkillUITab> GetSkillUITab(const size_t& _SkillType);
	
	// Tab이 클릭되었을때 Tab이 보드에게 클릭되었음을 알리는 함수
	void TabClickEvent(const size_t& _Index);
	void SkillUIClickEvent();

	// 눌립 탭의 스킬타입의 스킬만 UI로 띄운다.
	void BoardOff();

	// 보드는 Character가 선택되었을때만 활성화(SelectPhase에서만 작동)
	// 그 이외의 상태에서는 꺼진다.
	void BoardOn();

public:
	CSkillUIBoard();
	~CSkillUIBoard();
};

