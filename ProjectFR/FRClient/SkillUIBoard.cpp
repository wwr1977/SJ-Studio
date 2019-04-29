#include "Precom.h"
#include "SkillUIBoard.h"
#include "SkillUIMgr.h"
#include "SkillUITab.h"
#include "SkillUI.h"


CSkillUIBoard::CSkillUIBoard()
	:m_BackBoard(nullptr), m_pSkillIndexList(nullptr)
	, m_CurSkillType(0), m_pSkillUIMgr(nullptr)
{
	m_vecIconPivot.push_back(Vec2{ -110.0f,60.0f });
	m_vecIconPivot.push_back(Vec2{ 94.0f,60.0f });
	m_vecIconPivot.push_back(Vec2{ -110.0f,0.0f });
	m_vecIconPivot.push_back(Vec2{ 94.0f,0.0f });
	m_vecIconPivot.push_back(Vec2{ -110.0f,-60.0f });
	m_vecIconPivot.push_back(Vec2{ 94.0f,-60.0f });
}


CSkillUIBoard::~CSkillUIBoard()
{
}
void CSkillUIBoard::Init() 
{
	m_BackBoard = ACTOR->CreateCom<CFixRenderer>(RENDATA(LAYER_UIBACK, 0.75f, true));
	
	if (nullptr == m_BackBoard)
	{
		assert(nullptr);
		return;
	}

	m_BackBoard->SetSprite(_T("ColorBase"));
	m_BackBoard->SetMaterial(D3DXCOLOR(0.15f, 0.15f, 0.15f, 1.0f));
	m_BackBoard->MaterialOn();

	CSkillUITab::SetSkillUIBoard(this);

	for (size_t i = 0; i < SKILLDATA::MAXSKILLTYPE; i++)
	{
		CActObject* TabAct = CActObject::CreateActObject(_T("Tab"), ACTOR->GetScene());
		
		if (nullptr == TabAct) 
		{
			assert(nullptr);
			continue;
		}

		CSkillUITab* NewTab = TabAct->CreateCom<CSkillUITab>();
		
		if (nullptr == NewTab)
		{
			assert(nullptr);
			return;
		}

		TabAct->GetTransCom()->SetParent(TRANS);
		NewTab->SetSkillType(i);
		m_vecSkillUITab.push_back(NewTab);
	}



}
void CSkillUIBoard::Update() 
{

}
void CSkillUIBoard::SetSkillUIMgr(CSkillUIMgr* _Mgr)
{
	if (nullptr == _Mgr)
		return;

	m_pSkillUIMgr = _Mgr;
}
void CSkillUIBoard::SetPos(const Vec3& _Pos)
{
	TRANS->SetPos(_Pos);
}
void CSkillUIBoard::SetSize(const Vec2& _Size)
{
	m_BackBoard->SetCustomSize(_Size);
}
void CSkillUIBoard::SetSkillIndexList(vector<vector<size_t>>* _SkillIndexList)
{
	if (nullptr == _SkillIndexList)
		return;

	m_pSkillIndexList = _SkillIndexList;
}
const  size_t CSkillUIBoard::GetTabCount()
{
	return m_vecSkillUITab.size();
}
const Vec3 CSkillUIBoard::GetUIPlacePos(const size_t& _Index)
{
	if (_Index >= m_vecIconPivot.size())
		return Vec3{ 0.0f,0.0f,0.0f };

	Vec3 Pivot = Vec3{ m_vecIconPivot[_Index].x,m_vecIconPivot[_Index].y,0.0f };
	return TRANS->GetPos() + Pivot;
}
SPTR<CSkillUITab> CSkillUIBoard::GetSkillUITab(const size_t& _SkillType)
{
	if (_SkillType >= m_vecSkillUITab.size())
		return nullptr;

	return m_vecSkillUITab[_SkillType];
}
void CSkillUIBoard::TabClickEvent(const size_t& _Index)
{
	if (_Index >= m_vecSkillUITab.size())
		return;

	// ��� ���� ������ UnFocus �������� �ٲ۴�.
	for (size_t i = 0; i < m_vecSkillUITab.size(); i++)
	{
		m_vecSkillUITab[i]->UnSelectTab();
	}

	// ������ On�� SkillUI�� Off

	for (size_t i = 0; i < (*m_pSkillIndexList)[m_CurSkillType].size(); i++)
	{
		size_t SkillIndex = (*m_pSkillIndexList)[m_CurSkillType][i];

		if (SkillIndex >= CClientGlobal::vecSkill.size())
			continue;

		(*m_pSkillUIMgr)[SkillIndex]->SkillUIOff();
	}

	// _Index�� ����Ű�� ��ųŸ���� ��ų UI���� ȭ�鿡 On �Ѵ�.
	m_CurSkillType = _Index;

	for (size_t i = 0; i < (*m_pSkillIndexList)[m_CurSkillType].size(); i++)
	{
		size_t SkillIndex = (*m_pSkillIndexList)[m_CurSkillType][i];

		if (SkillIndex >= CClientGlobal::vecSkill.size())
			continue;

		(*m_pSkillUIMgr)[SkillIndex]->CheckUIAvailable();
		(*m_pSkillUIMgr)[SkillIndex]->SkillUIOn();
		Vec2 Pos = Vec2{ TRANS->GetPos().x + m_vecIconPivot[i].x,TRANS->GetPos().y + m_vecIconPivot[i].y };
		(*m_pSkillUIMgr)[SkillIndex]->SetPos(Pos);

	}


}
void CSkillUIBoard::SkillUIClickEvent()
{

}
void CSkillUIBoard::BoardOn()
{
	size_t Count = 0;
	ACTOR->On();

	for (size_t i = 0; i < m_vecSkillUITab.size(); i++)
	{
		// ���������� ���õ� ���� ��ųŸ���� ��ų UI�� ��ġ
		if (true == m_vecSkillUITab[i]->IsSelect()) 
		{
			TabClickEvent(i);
			m_vecSkillUITab[i]->SelectTab();
			++Count;
		}
		m_vecSkillUITab[i]->UIOn();
	}

	// �������� ù�������� Default��  Action Tab��ų�� Ȱ��ȭ

	if (Count == 0) 
	{
		TabClickEvent(0);
		m_vecSkillUITab[0]->SelectTab();
	}

	// ���� ������ ���� 2���̻��� ���Ǵٰ� ���� ����׸� ����
	// �Ͷ߸���.
	if (Count > 1)
		assert(nullptr);


}
void CSkillUIBoard::BoardOff()
{
	ACTOR->Off();

	for (size_t i = 0; i < m_vecSkillUITab.size(); i++)
	{
		if (nullptr != m_vecSkillUITab[i])
			m_vecSkillUITab[i]->UIOff();
	}

	for (size_t i = 0; i < (*m_pSkillIndexList)[m_CurSkillType].size(); i++)
	{
		size_t SkillIndex = (*m_pSkillIndexList)[m_CurSkillType][i];

		if (SkillIndex >= CClientGlobal::vecSkill.size())
			continue;

		(*m_pSkillUIMgr)[SkillIndex]->SkillUIOff();
	}
}
