#pragma once
#include <Logic.h>

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
	VERTEX						m_TriCenter;
	SPTR<CGameMesh>				m_TriMesh;

	float						m_TriRotRad;
	Vec2						m_TriTransVec;

	Mat							m_TriRot;
	Mat							m_TriTrans;

	Mat							m_TriWorld;
private:
	Vec2						m_InitVec;
	float						m_fInitSpeed;
	float						m_fGravitySpeed;

	float						m_fAccTime;
	float						m_fBrokenTime;


public:
	void Init();
	void InitData(TRI_VERTEX& _VtxBuf);
	void Update();
	void TriangleRender();
	
public:
	void BrokenUpdate(const float& _Time);
	void MoveUpdate(const float& _Time);


public:
	void CreateFractalTriangle(list<SPTR<CFractalTriangle>>* _List);
	const VERTEX RandomMiddleVertex(const float& _w);

public:
	void ChangeMiddlePoint(const VERTEX& _Mid);
	void ChangeTriangleMatData();

public:
	void StandBy();

// Test
private:
	//SPTR<CGameMesh>					m_TestMesh;
	VERTEX								m_TestVertax[TRICOUNT];
	
	void CreateTestMesh();
	void CheckTriRatio();

public:
	CFractalTriangle();
	~CFractalTriangle();
};

