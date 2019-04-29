#pragma once
#include "Logic.h"
class CBattleEventObject : public CLogic
{
	enum
	{
		TRI_COUNT = 4,
	};
private:
	vector<SPTR<class CFractalTriangle>>		m_FractalSource;
	
	list<SPTR<CFractalTriangle>>				m_TriangleObject;

public:
	void Init();
	void Update();
	void UIFontRender();


public:
	void CreateFractalSource();

public:
	CBattleEventObject();
	~CBattleEventObject();
};

