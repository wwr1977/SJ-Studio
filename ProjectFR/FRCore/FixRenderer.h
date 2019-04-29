#pragma once
#include "Renderer.h"

class CGameSprite;
class CFixRenderer :	public CRenderer
{
	SPTR<CGameSprite>			m_Sprite;

	
	// ACTOR�� ������ ���� �ʴ� �ڽ��� ������ ũ��� ������ ����
	// Custom Size -> Renderer�� �̵�
	
public:
	void RenderOff();
	void SetSprite(SPTR<CGameSprite> _pSprite);
	void SetSprite(const tstring& _Key);
	void SetSprite(const tstring& _Key, size_t _Index);
	void SetSpritePivot(const Vec2& _Pivot);
	const float GetCurSpriteAspect();
	const float GetCurLongestSide();
	const Vec3 GetCurSpritePivot();
	const Vec2 GetCurSpriteSize();


	void SyncSpriteSize();

	void SettingWorldMat(Mat* _Mat);
	SPTR<CGameSprite>	GetCurSprite();
	BOOL IsSettingSprite() 
	{
		if (nullptr == m_Sprite) 
		{
			return FALSE;
		}
		return TRUE;
	}

public:
	virtual void Render();

public:
	CFixRenderer();
	~CFixRenderer();
};

