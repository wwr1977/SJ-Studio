#include "stdafx.h"
#include "GameMultiSprite.h"
#include"GameSprite.h"



CGameMultiSprite::CGameMultiSprite(LPDIRECT3DDEVICE9 _Device)
	:m_pDevice(_Device), m_SpritePivot({0.0f,0.0f}), m_AniSpriteType(ANI_ETC)
	, m_TexKey(_T("")), m_StartPos(Vec2{0.0f,0.0f})
	,m_XCount(0),m_YCount(0),m_ImageCount(0), m_StartFrame(0)
	, m_MultiSpriteSize(Vec2(0.0f,0.0f))
{
	
}


CGameMultiSprite::~CGameMultiSprite()
{

}
CGameMultiSprite* CGameMultiSprite::CreateMultiSprite(LPDIRECT3DDEVICE9 _pDevice)
{
	if (nullptr == _pDevice)
	{
		return nullptr;
	}

	return new CGameMultiSprite(_pDevice);
}
SPTR<CGameSprite> CGameMultiSprite::operator[](const size_t& _Index)
{
	return m_VecSprite[_Index];
}
void CGameMultiSprite::PushSprite(CGameSprite* _Sprite) 
{
	if (nullptr == _Sprite) 
	{
		return;
	}

	m_VecSprite.push_back(_Sprite);
}
SPTR<CGameSprite> CGameMultiSprite::GetSprite(size_t _Index) 
{
	if (_Index < 0 || _Index >= m_VecSprite.size()) 
	{
		return nullptr;
	}
	return m_VecSprite[_Index];
}
void CGameMultiSprite::SetSpritePivot(const Vec2& _Pivot)
{
	m_SpritePivot = _Pivot;
}
const Vec2 CGameMultiSprite::GetSpriteSize(const int& _Index /*= 0*/)
{
	if (_Index <= 0) 
	{
		m_VecSprite[0]->GetSpriteSize();
	}
	return m_VecSprite[_Index]->GetSpriteSize();
}
const Vec2 CGameMultiSprite::GetSpritePivot()
{
	return m_SpritePivot;
}
void CGameMultiSprite::SetSpriteType(ANISPRITETYPE _Type)
{
	int Type = _Type;
	// 기본적으로 폴더키를 기준으로 값이 들어와서 지정된 폴더가
	// ANI_ETC 값을 넘어가는경우 ANI_ETC로 강제 배정(CHAR,MON,EFFECT 폴더 이외의 멀티스프라이트를 만들때)
	if ((int)ANI_ETC <= Type) 
	{
		Type = ANI_ETC;
	}

	m_AniSpriteType = (ANISPRITETYPE)Type;
}
ANISPRITETYPE CGameMultiSprite::GetSpriteType()
{
	return m_AniSpriteType;
}
void CGameMultiSprite::SetTexKey(const tstring& _TexKey) 
{
	m_TexKey = _TexKey;
}
tstring CGameMultiSprite::GetTexKey()
{
	return m_TexKey;
}
const TCHAR* CGameMultiSprite::GetTexKey_str()
{
	return m_TexKey.c_str();
}
void CGameMultiSprite::SetStartPos(const Vec2& _StartPos)
{
	m_StartPos = _StartPos;
}
Vec2 CGameMultiSprite::GetStartPos() 
{
	return m_StartPos;
}
void CGameMultiSprite::SetCountData(int _XCount, int _YCount, int _ImageCount) 
{
	m_XCount = _XCount;
	m_YCount = _YCount;
	m_ImageCount = _ImageCount;
}
int CGameMultiSprite::GetXCount() 
{
	return m_XCount;
}
int CGameMultiSprite::GetYCount() 
{
	return  m_YCount;
}
int CGameMultiSprite::GetImageCount()
{
	return m_ImageCount;
}
void CGameMultiSprite::SetStartFrame(const int& _StartFrame)
{
	m_StartFrame = _StartFrame;
}
int CGameMultiSprite::GetStartFrame()
{
	return m_StartFrame;
}
void CGameMultiSprite::CopySpriteVec(vector<SPTR<CGameSprite>>* _vecSprite)
{
	m_VecSprite.reserve(_vecSprite->size());
	m_VecSprite.assign(_vecSprite->begin(), _vecSprite->end());
}
void CGameMultiSprite::AddVecSprite(const vector<SPTR<CGameSprite>>& _vecSprite) 
{
	if (0 >= _vecSprite.size()) 
	{
		return;
	}

	m_VecSprite.assign(_vecSprite.begin(), _vecSprite.end());
}
void CGameMultiSprite::AddSprite(const SPTR<CGameSprite> _Sprite) 
{
	if (nullptr == _Sprite) 
	{
		return;
	}

	m_VecSprite.push_back(_Sprite);
}
void CGameMultiSprite::MergeMultiSprtie( SPTR<CGameMultiSprite> _MultiSprite)
{
	if (nullptr == _MultiSprite) 
	{
		return;
	}

	if (0 >= _MultiSprite->m_VecSprite.size()) 
	{
		return;
	}

	m_VecSprite.reserve(m_VecSprite.size() + _MultiSprite->m_VecSprite.size());
	m_VecSprite.insert(m_VecSprite.end(),_MultiSprite->m_VecSprite.begin(), _MultiSprite->m_VecSprite.end());
}
