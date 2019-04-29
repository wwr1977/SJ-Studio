#pragma once
#include "Logic.h"

class CFractalTriangle : public CLogic
{
	static 	Mat							TriangleScaleMat;
public:
	static void SetTriangleScale(Mat& _Mat);

	enum
	{
		TRICOUNT = 3,
	};

private:
	VERTEX						m_TriangleVertax[TRICOUNT];
	SPTR<CGameMesh>				m_TriMesh;

public:
	void Init();
	void InitData(void* _VtxBuf);
	void Update();
	void UIFontRender();

public:
	void CreateFractalTriangle(list<SPTR<CFractalTriangle>>* _List);

public:
	CFractalTriangle();
	~CFractalTriangle();
};

