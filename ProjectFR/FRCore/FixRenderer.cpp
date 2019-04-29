#include "stdafx.h"
#include "FixRenderer.h"
#include "GameSprite.h"
#include"GameMultiSprite.h"
#include"GameTexture.h"
#include"GameMesh.h"



CFixRenderer::CFixRenderer()
{
}


CFixRenderer::~CFixRenderer()
{
	m_Sprite = nullptr;
}

void CFixRenderer::Render()
{
	if (nullptr == m_Sprite) 
		return;
	
	Mat World;
	SettingWorldMat(&World);
	GetComDevice()-> SetTransform(D3DTS_WORLD, &World);
	
	if (nullptr != m_Sprite) 
	{
		for (int i = 0; i < m_iOverlapCount; i++)		
			m_Sprite->Render();
		
	}

	
}
void CFixRenderer::SetSprite(SPTR<CGameSprite> _pSprite)
{
	assert(_pSprite);

	if (nullptr == _pSprite) 
	{
		return;
	}

	m_Sprite = _pSprite;
	
}
void CFixRenderer::SetSprite(const tstring& _Key)
{
	m_Sprite = COMRESMGR->FindGameSprite(_Key);
	assert(m_Sprite);

}

void CFixRenderer::SetSprite(const tstring& _Key, size_t _Index)
{
	m_Sprite = COMRESMGR->FindMultiSpriteToIndex(_Key, _Index);
	assert(m_Sprite);
}
void CFixRenderer::SetSpritePivot(const Vec2& _Pivot)
{
	m_Sprite->SetSpritePivot(_Pivot);
}
void CFixRenderer::SettingWorldMat(Mat* _Mat)
{
	if (true == m_bCustom)
	{
		Vec2 SpritePivot = m_Sprite->GetSpritePivot() - m_RenPivot;
		TransForm()->GetRenWorld(_Mat, SpritePivot, m_CustomSize);
	}
	else if (SPRITESIZE == ParentActObject()->GetRenderStyle())
	{
		Vec2 SpriteSize = m_Sprite->GetSpriteSize();
		Vec2 SpritePivot = ACTOR->GetMagnification() *  m_Sprite->GetSpritePivot() - m_RenPivot;
		TransForm()->GetRenWorld(_Mat, SpritePivot, SpriteSize);
	}
	else if (ACTSIZE == ParentActObject()->GetRenderStyle())
	{
		TransForm()->GetObjWorld(_Mat,m_RenPivot);
	}

}
const float  CFixRenderer::GetCurSpriteAspect() 
{
	if (nullptr == m_Sprite) 
	{
		return 1.0f;
	}
	return m_Sprite->GetSpriteAspect();
}
const float CFixRenderer::GetCurLongestSide()
{
	if (nullptr == m_Sprite)
	{
		return 1.0f;
	}
	return m_Sprite->GetLongestSide();
}
void CFixRenderer::SyncSpriteSize()
{
	POINT WndSize = ParentActObject()->GetScene()->GetWindow()->WndSize();

	float SpriteAspect = GetCurSpriteAspect();
	float SpriteLongestSide = GetCurLongestSide();

	// 이미지가 가로로 긴경우(이미지의 크기를 가로 길이 기준으로 조정)
	if (1 <= SpriteAspect)
	{
		if (SpriteLongestSide >= WndSize.x)
		{
			ParentActObject()->SetImageScale(WndSize.x / SpriteLongestSide);
			return;
		}
	}
	else
	{
		if (SpriteLongestSide >= WndSize.y)
		{
			ParentActObject()->SetImageScale(WndSize.y / SpriteLongestSide);
			return;
		}
	}

	ParentActObject()->SetImageScale(1.0f);
}
const Vec2 CFixRenderer::GetCurSpriteSize()
{
	if (nullptr == m_Sprite)
	{
		return Vec2{ 0.0f,0.0f };
	}
	return m_Sprite->GetSpriteSize();
}
SPTR<CGameSprite>	CFixRenderer::GetCurSprite()
{
	return m_Sprite;
}
void CFixRenderer::RenderOff()
{
	m_Sprite = nullptr;
}
const Vec3 CFixRenderer::GetCurSpritePivot()
{
	return Vec3{ m_Sprite->GetSpritePivot().x,m_Sprite->GetSpritePivot().y,0.0f };
}

