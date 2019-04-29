#pragma once
#include <Logic.h>
class CBattleResultWindow : public CLogic
{
	enum RESULTWINDOWSTATE
	{
		STATE_NOACTION,
		STATE_WAIT,
		STATE_POPUI,			// ResultUI�� ��ġ�� ������
		STATE_UIUPDATE_WAIT,	// ResultUI�� �������� ������Ʈ�� �Ǵ� ����
		STATE_RESULTWINDOW,		// ���â�� ���� ����
		MAXRESULTWINDOWSTATE,
	};

	vector<SPTR<class CCharResultUI>>				m_ResultCharUI;
	vector<Vec3>									m_ResultCharUIPos;
	Vec3											m_PopUpPivot;
	SPTR<CFixRenderer>								m_ResultWindowRen;


	UINT											m_WindowState;
	float											m_fAccTime;
	bool											m_bChangeScene;
	bool											m_bBackRen;

public:
	void Init();
	void Update();

public:
	void WaitUpdate();
	void PopUIUpdate();
	void UIUpdateWait();
	void ResultWindowUpdate();

	void SetAlpha(const float& _Alpha);

public:
	void PushResultUI(const UINT& _Index, SPTR<CCharResultUI> _UI);

public:
	void ResultWindowOn(UINT& _TotalExp,const UINT& _CharCount);
	void ResultWindowOff();

public:
	const bool CheckUIUpdateEnd();
	const bool CheckChangeScene();

public:
	CBattleResultWindow();
	~CBattleResultWindow();
};

