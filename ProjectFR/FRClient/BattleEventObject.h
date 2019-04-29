#pragma once
#include <Logic.h>
class CBattleEventObject : public CLogic
{
	enum
	{
		TRI_COUNT = 4,
	};
private:
	SPTR<CGameTexture>							m_ScreenTex;
	SPTR<CSoundPlayer>							m_EventSpeaker;
	Vec2										m_ScreenSize;
	SPTR<CFixRenderer>							m_ObjectBackRen;
	vector<SPTR<class CFractalTriangle>>		m_FractalSource;
	list<SPTR<CFractalTriangle>>				m_TriangleObject;

	float										m_fAccTime;
	bool										m_bEvent;

	
public:
	void Init();
	void Update();
	void UIFontRender();


public:
	void LoadScreenImage();
	void CreateFractalSource();
	void CreateFractalSource_Octa();
	void ChangeMiddlePoint();
	void InitFractalTriangle();
	void FractalTriangle(const UINT& _FractalCount);

public:
	void EventTriger();

public:
	CBattleEventObject();
	~CBattleEventObject();
};

