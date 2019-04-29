#pragma once
#include "BaseComponent.h"
#include"DXHeader.h"


typedef struct _tagSubTransData 
{
	Vec3					SubPivot;
	Vec3					SubSize;

	Mat						SubWorldMat;
	_tagSubTransData()
		: SubPivot(Vec3()),SubSize(Vec3())
	{
		SubWorldMat.SetIdentityMat();
	}
	~_tagSubTransData() {}

}SubTransData,SubData;

class CSubTrans : public CBaseComponent
{
protected:
	SubTransData*						m_pSubData;
	
protected:
	void SetSubPivot(const Vec3& _Pivot);
	void SetSubSize(const Vec3& _Size);


public:
	void SubTransUpdate();
	Mat GetSubTransWorld();
	const Mat& GetSubTransCRWorld();

public:
	CSubTrans();
	~CSubTrans();
};


