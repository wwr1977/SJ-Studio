#include "stdafx.h"
#include "AniRenderer.h"
#include"GameMultiSprite.h"
#include"GameSprite.h"

typedef CAniRenderer::CAnimation CAnimation;

////////////////////////////////    Ani   ///////////////////////////////////

CAnimation::CAnimation(CAniRenderer* _AniRenderer, ANIDATA _AniData)
	:m_pAniRenderer(_AniRenderer), m_AniData(_AniData)
	, m_bEndFrame(false), m_bEndAni(false), m_iCurAniCount(0)
	, m_fCurTime(0.0f), m_AniSprite(nullptr),m_iCurIndex(0)
	, m_AniStyle(MAXANISTYLE)
{
	
}
CAnimation::~CAnimation()
{
	/*SAFE_DELETE_ARR(m_AniData.Interval);
	SAFE_DELETE_ARR(m_AniData.RepeatInterval);*/
}
void CAnimation::SetAniSprite(SPTR<CGameMultiSprite> _AniSprite)
{
	m_AniSprite = _AniSprite;
}
CAnimation* CAnimation::CreateAni(CAniRenderer* _AniRenderer, ANIDATA _AniData)
{
	if (true == _AniData.InvalidData) 
	{
		return nullptr;
	}

	
	CAnimation* NewAni =  new CAnimation(_AniRenderer, _AniData);
	NewAni->Init(_AniData.FullRange.Start);
	return NewAni;
}
void CAnimation::AniReset()
{
	m_iCurIndex = m_AniData.FullRange.Start;
	m_iCurAniCount = 0;
	m_fCurTime = 0.0f;

	m_bEndFrame = false;
	m_bEndAni = false;
}

SPTR<CGameSprite> CAnimation::UpdateAni()
{
	m_fCurTime += DELTATIME;

	if (true == m_bEndAni && false == m_AniData.Loop) 
	{
		if (m_fCurTime >= m_AniData.RepeatInterval[m_iCurIndex - m_AniData.RepeatRange.Start]) 
		{
			m_fCurTime = 0.0f;
			++m_iCurIndex;
		}

	}
	else 
	{
		if (m_fCurTime >= m_AniData.Interval[m_iCurIndex-m_AniData.FullRange.Start]) 
		{
			m_fCurTime = 0.0f;
			++m_iCurIndex;
		}
	}

	// 유한 반복 애니메이션 루틴
	if (true != m_AniData.Loop) 
	{
		// 지정한 AnimationCount 이상 돌았을 경우 (마지막 & 일부분 반복)
		if (true == m_bEndAni)
		{
			if (m_iCurIndex >= m_AniData.RepeatRange.End)
			{
				if (m_iCurIndex == m_AniData.RepeatRange.End)
				{
					m_bEndFrame = true;
				}
				else
				{
					m_iCurIndex = m_AniData.RepeatRange.Start;
					m_bEndFrame = false;
				}
			}
		}
		// 지정한 AnimationCount 이하로 돌았을경우 (전체 반복)
		else 
		{
			if (m_iCurIndex >= m_AniData.FullRange.End)
			{
				if (m_iCurIndex == m_AniData.FullRange.End)
				{
					m_bEndFrame = true;
				}
				else
				{
					m_iCurIndex = m_AniData.FullRange.Start;
					++m_iCurAniCount;
					if (m_iCurAniCount >= m_AniData.AnimationCount)
					{
						m_bEndAni = true;
						m_iCurIndex = m_AniData.RepeatRange.Start;
					}
					m_bEndFrame = false;
				}
			}
		}

	}
	// 무한 반복 애니메이션 루틴
	else 
	{
		if (true == m_bEndAni)
		{
			m_bEndAni = false;
		}

		if (m_iCurIndex >= m_AniData.FullRange.End)
		{
			if (m_iCurIndex == m_AniData.FullRange.End)
			{
				m_bEndFrame = true;
			}
			else
			{
				m_iCurIndex = m_AniData.FullRange.Start;
				++m_iCurAniCount;
				m_bEndFrame = false;
				m_bEndAni = true;
				return m_AniSprite->GetSprite(m_AniData.FullRange.End);
			}
		}
	}

	return m_AniSprite->GetSprite(m_iCurIndex);

}
Vec2 CAnimation::GetSpriteSize() 
{
	return m_AniSprite->GetSpriteSize();
}
Vec2 CAnimation::GetSpritePivot()
{
	return m_AniSprite->GetSpritePivot();
}
void CAnimation::SetAniStyle(ANISTYLE _AniStyle)
{
	m_AniStyle = (int)_AniStyle;
}
ANISTYLE CAnimation::GetAniStyle()
{
	return (ANISTYLE)m_AniStyle;
}
ANISPRITETYPE CAnimation::GetAniSpriteType()
{
	return m_AniSprite->GetSpriteType();
}
ANIDATA CAnimation::GetAniData()
{
	return m_AniData;
}
///////////////////////////////// AniRenderer		/////////////////////////////


CAniRenderer::CAniRenderer()
	:m_CurAni(nullptr)
{

}


CAniRenderer::~CAniRenderer()
{

}

SPTR<CAnimation> CAniRenderer::FindAnimation(const tstring& _Key) 
{
	return FINDMAP_SPTR<CAnimation>(_Key, m_mapAnimation);
}
BOOL CAniRenderer::ExistAnimation(const tstring& _Key)
{
	return EXISTMAP_SPTR<CAnimation>(_Key, m_mapAnimation);
}
bool CAniRenderer::ChangeAni(const tstring& _Key)
{
	SPTR<CAnimation> ChangeAni = FindAnimation(_Key);

	if (nullptr == ChangeAni) 
		return false;
	

	m_CurAni = ChangeAni;

	m_CurAni->AniReset();
	return true;
}


void CAniRenderer::Render()
{
	if (nullptr == m_CurAni) 
	{
		return;
	}
	Mat World;

	SettingWorldMat(&World);
	COMDEV->SetTransform(D3DTS_WORLD, &World);

	SPTR<CGameSprite> CurFrame = m_CurAni->UpdateAni();

	for (int i = 0; i < m_iOverlapCount; i++)
	{
		CurFrame->Render();
	}
	
}

tstring CAniRenderer::CurAniName()
{
	return m_CurAni->Name();
}
tstring CAniRenderer::CurAniSpriteName()
{
	return m_CurAni->m_AniSprite->Name();
}

void CAniRenderer::SettingWorldMat(Mat* _Mat)
{
	if (true == m_bCustom)
	{
		Vec2 SpritePivot = ParentActObject()->GetMagnification() * m_CurAni->GetSpritePivot() - m_RenPivot;
		TransForm()->GetRenWorld(_Mat, SpritePivot, m_CustomSize);
	}
	else if (SPRITESIZE == ParentActObject()->GetRenderStyle())
	{
		
		Vec2 SpriteSize = m_CurAni->GetSpriteSize();
		Vec2 SpritePivot = ParentActObject()->GetMagnification() * m_CurAni->GetSpritePivot() - m_RenPivot;
		TransForm()->GetRenWorld(_Mat,SpritePivot, SpriteSize);
	}
	else if (ACTSIZE == ParentActObject()->GetRenderStyle())
	{
		TransForm()->GetObjWorld(_Mat);
	}

	return;
}
BOOL CAniRenderer::CurAniReset()
{
	if (nullptr == m_CurAni) 
	{
		return FALSE;
	}

	m_CurAni->AniReset();
	return TRUE;
}
const float CAniRenderer::GetFullAnimationTime()
{
	ANIDATA Ani = m_CurAni->GetAniData();
	
	int Count = Ani.FullRange.End - Ani.FullRange.Start + 1 ;

	float FullAccTime = 0.0f;

	for (int i = 0; i < Count; i++)
	{
		FullAccTime += Ani.Interval[i];
	}

	return FullAccTime;
}
ANISPRITETYPE CAniRenderer::GetCurAniSpriteType()
{
	if (nullptr == m_CurAni) 
	{
		return  MAXANISPRITETYPE;
	}
	return m_CurAni->m_AniSprite->GetSpriteType();
}
ANIDATA* CAniRenderer::GetCurAniData()
{
	if (nullptr == m_CurAni) 
	{
		return nullptr;
	}
	return &m_CurAni->m_AniData;
}
int CAniRenderer::GetCurAniFrame()
{
	return m_CurAni->m_iCurIndex;
}
int CAniRenderer::GetCurAniCount()
{
	return m_CurAni->m_iCurAniCount;
}
void CAniRenderer::SetImageMag(const float& _Mag)
{
	ACTOR->SetImageScale(_Mag);
}
void CAniRenderer::SetActorPos(const Vec3& _Pos)
{
	TRANS->SetPos(_Pos);
}
void CAniRenderer::ActorOn()
{
	ACTOR->On();
}
void CAniRenderer::ActorOff()
{
	ACTOR->Off();
}
//////////////////////////////////////        CreateAnimation OverLording		///////////////////////////////////////////



CAnimation* CAniRenderer::CreateAnimation(const tstring& _MultiSpriteKey, ANISTYLE _AniStyle)
{
	return CreateAnimation(_MultiSpriteKey, _MultiSpriteKey, _AniStyle);
	
}
CAnimation* CAniRenderer::CreateAnimation(const tstring& _MultiSpriteKey, ANISTYLE _AniStyle, int _StartIndex, int _EndIndex, int _AniCount /*= 1*/ )
{
	return CreateAnimation(_MultiSpriteKey, _MultiSpriteKey, _AniStyle, _StartIndex, _EndIndex, _AniCount);
}
CAnimation* CAniRenderer::CreateAnimation(const tstring& _MultiSpriteKey, ANISTYLE _AniStyle, int _StartIndex, int _EndIndex, int _AniCount, int _RepeatStartIndex, int _RepreatEndIndex)
{
	return CreateAnimation(_MultiSpriteKey, _MultiSpriteKey, _AniStyle, _StartIndex, _EndIndex, _AniCount, _RepeatStartIndex, _RepreatEndIndex);

}
CAnimation* CAniRenderer::CreateAnimation(const tstring& _MultiSpriteKey, ANISTYLE _AniStyle, ANIRANGE _FullRange, int _AniCount /*= 1*/) 
{
	return CreateAnimation(_MultiSpriteKey, _MultiSpriteKey, _AniStyle, _FullRange, _AniCount);
}
CAnimation* CAniRenderer::CreateAnimation(const tstring& _MultiSpriteKey, ANISTYLE _AniStyle, ANIRANGE _FullRange, int _AniCount, ANIRANGE _RepeatRange) 
{
	return CreateAnimation(_MultiSpriteKey, _MultiSpriteKey, _AniStyle, _FullRange, _AniCount, _RepeatRange);

}
CAnimation* CAniRenderer::CreateAnimation(const tstring& _AniKey, SPTR<CGameMultiSprite> _MultiSprite,const ANIDATA& _AniData)
{
	
	if (nullptr == _MultiSprite)
	{
		return nullptr;
	}

	SPTR<CAnimation> Ani = FindAnimation(_AniKey);

	if (nullptr != Ani) 
	{
		return nullptr;
	}

	CAnimation* NewAni = CAnimation::CreateAni(this, _AniData);
	
	if (nullptr == NewAni) 
	{
		return nullptr;
	}

	NewAni->SetAniSprite(_MultiSprite);
	NewAni->AniReset();
	NewAni->Name(_AniKey);
	m_mapAnimation.insert(map<tstring, SPTR<CAnimation>>::value_type(_AniKey, NewAni));

	if (1 == m_mapAnimation.size()) 
	{
		m_CurAni = NewAni;
	}

	return NewAni;


}

CAnimation* CAniRenderer::CreateAnimation(const tstring& _AniKey, const tstring& _MultiSpriteKey, ANISTYLE _AniStyle)
{
	SPTR<CGameMultiSprite> AniSprite = COMRESMGR->FindMultiSprite(_MultiSpriteKey);

	if (nullptr == AniSprite) 
	{
		return nullptr;
	}

	int StartIndex = 0;
	int EndIndex = (int)AniSprite->GetSpriteCount() - 1;

	switch (_AniStyle)
	{
	case ANI_DEFAULT:
	{
		return CreateAnimation(_AniKey, AniSprite, ANIDATA({ StartIndex, EndIndex }, false));
	}
	break;
	case ANI_COUNTCYCLE_REPEAT:
	{
		return CreateAnimation(_AniKey, AniSprite, ANIDATA({ StartIndex, EndIndex }, 1, { EndIndex, EndIndex }));
	}
	break;
	case ANI_LOOP:
	{
		return CreateAnimation(_AniKey, AniSprite, ANIDATA({ StartIndex, EndIndex }, true));
	}
	break;
	default:
	{
		return nullptr;
	}
		break;
	}
}

CAnimation* CAniRenderer::CreateAnimation(const tstring& _AniKey, const tstring& _MultiSpriteKey, ANISTYLE _AniStyle, int _StartIndex, int _EndIndex, int _AniCount /*= 1*/)
{
	SPTR<CGameMultiSprite> AniSprite = COMRESMGR->FindMultiSprite(_MultiSpriteKey);

	if (nullptr == AniSprite)
	{
		return nullptr;
	}

	int StartIndex = 0;
	int EndIndex = (int)AniSprite->GetSpriteCount() - 1;

	if (_StartIndex < StartIndex || _StartIndex > EndIndex)
	{
		_StartIndex = StartIndex;
	}
	if (_EndIndex < 0 || _EndIndex > EndIndex)
	{
		_EndIndex = EndIndex;
	}

	switch (_AniStyle)
	{
	case ANI_DEFAULT:
	{
		return CreateAnimation(_AniKey, AniSprite, ANIDATA({ _StartIndex, _EndIndex }, false));
	}
	break;
	case ANI_COUNTCYCLE_REPEAT:
	{
		return CreateAnimation(_AniKey, AniSprite, ANIDATA({ StartIndex, EndIndex }, _AniCount, { _StartIndex, _EndIndex }));
	}
	break;
	case ANI_LOOP:
	{
		return CreateAnimation(_AniKey, AniSprite, ANIDATA({ _StartIndex, _EndIndex }, true));
	}
	break;
	default: 
	{
		return nullptr;
	}
	break;
	}
}
CAnimation* CAniRenderer::CreateAnimation(const tstring& _AniKey, const tstring& _MultiSpriteKey, ANISTYLE _AniStyle, ANIRANGE _FullRange, int _AniCount/* = 1*/)
{
	SPTR<CGameMultiSprite> AniSprite = COMRESMGR->FindMultiSprite(_MultiSpriteKey);

	if (nullptr == AniSprite)
	{
		return nullptr;
	}

	int StartIndex = 0;
	int EndIndex = (int)AniSprite->GetSpriteCount() - 1;

	if (_FullRange.Start < StartIndex || _FullRange.Start  > EndIndex)
	{
		_FullRange.Start = StartIndex;
	}
	if (_FullRange.End < StartIndex || _FullRange.End > EndIndex)
	{
		_FullRange.End = EndIndex;
	}

	switch (_AniStyle)
	{
	case ANI_DEFAULT:
	{
		return CreateAnimation(_AniKey, AniSprite, ANIDATA(_FullRange , false));
	}
	break;
	case ANI_COUNTCYCLE_REPEAT:
	{
		return CreateAnimation(_AniKey, AniSprite, ANIDATA({ StartIndex,EndIndex }, _AniCount, _FullRange));
	}
	break;
	case ANI_LOOP:
	{
		return CreateAnimation(_AniKey, AniSprite, ANIDATA( _FullRange, true));
	}
	break;
	default:
	{
		return nullptr;
	}
	break;
	}
}
CAnimation* CAniRenderer::CreateAnimation(const tstring& _AniKey, const tstring& _MultiSpriteKey, ANISTYLE _AniStyle, int _StartIndex, int _EndIndex, int _AniCount, int _RepeatStartIndex, int _RepeatEndIndex)
{
	SPTR<CGameMultiSprite> AniSprite = COMRESMGR->FindMultiSprite(_MultiSpriteKey);

	if (nullptr == AniSprite)
	{
		return nullptr;
	}

	int StartIndex = 0;
	int EndIndex = (int)AniSprite->GetSpriteCount() - 1;

	if (_StartIndex < StartIndex || _StartIndex > EndIndex)
	{
		_StartIndex = StartIndex;
	}
	if (_EndIndex < 0 || _EndIndex > EndIndex)
	{
		_EndIndex = EndIndex;
	}

	if (_RepeatStartIndex < StartIndex || _RepeatStartIndex > EndIndex)
	{
		_RepeatStartIndex = StartIndex;
	}
	if (_RepeatEndIndex < 0 || _RepeatEndIndex > EndIndex)
	{
		_RepeatEndIndex = EndIndex;
	}

	switch (_AniStyle)
	{
	case ANI_DEFAULT:
	{
		return CreateAnimation(_AniKey, AniSprite, ANIDATA({ _StartIndex, _EndIndex }, false));
	}
	break;
	case ANI_COUNTCYCLE_REPEAT:
	{
		return CreateAnimation(_AniKey, AniSprite, ANIDATA({ _StartIndex, _EndIndex }, _AniCount, { _RepeatStartIndex, _RepeatEndIndex }));
	}
	break;
	case ANI_LOOP:
	{
		return CreateAnimation(_AniKey, AniSprite, ANIDATA({ _StartIndex, _EndIndex }, true));
	}
	break;
	default:
	{
		return nullptr;
	}
	break;
	}
}
CAnimation* CAniRenderer::CreateAnimation(const tstring& _AniKey, const tstring& _MultiSpriteKey, ANISTYLE _AniStyle, ANIRANGE _FullRange, int _AniCount, ANIRANGE _RepeatRange)
{
	SPTR<CGameMultiSprite> AniSprite = COMRESMGR->FindMultiSprite(_MultiSpriteKey);

	if (nullptr == AniSprite)
	{
		return nullptr;
	}

	int StartIndex = 0;
	int EndIndex = (int)AniSprite->GetSpriteCount() - 1;

	if (_FullRange.Start < StartIndex || _FullRange.Start  > EndIndex)
	{
		_FullRange.Start = StartIndex;
	}
	if (_FullRange.End < StartIndex || _FullRange.End > EndIndex)
	{
		_FullRange.End = EndIndex;
	}
	if (_RepeatRange.Start < StartIndex || _RepeatRange.Start  > EndIndex)
	{
		_RepeatRange.Start = StartIndex;
	}
	if (_RepeatRange.End < StartIndex || _RepeatRange.End > EndIndex)
	{
		_RepeatRange.End = EndIndex;
	}


	switch (_AniStyle)
	{
	case ANI_DEFAULT:
	{
		return CreateAnimation(_AniKey, AniSprite, ANIDATA(_FullRange, false));
	}
	break;
	case ANI_COUNTCYCLE_REPEAT:
	{
		return CreateAnimation(_AniKey, AniSprite, ANIDATA(_FullRange, _AniCount, _RepeatRange));
	}
	break;
	case ANI_LOOP:
	{
		return CreateAnimation(_AniKey, AniSprite, ANIDATA(_FullRange, true));
	}
	break;
	default:
	{
		return nullptr;
	}
	break;
	}
}
CAnimation* CAniRenderer::CreateAnimation(const ANIDATA& _AniData)
{
	SPTR<CGameMultiSprite> AniSprite = COMRESMGR->FindMultiSprite(_AniData.AniSpriteName);

	if (nullptr == AniSprite)
	{
		return nullptr;
	}

	SPTR<CAnimation> Ani = FindAnimation(_AniData.AnimationName);
	
	if (nullptr != Ani)
	{
		return nullptr;
	}

	CAnimation* NewAni = CAnimation::CreateAni(this, _AniData);

	if (nullptr == NewAni)
	{
		return nullptr;
	}

	NewAni->SetAniSprite(AniSprite);
	NewAni->AniReset();
	NewAni->Name(_AniData.AnimationName);


	m_mapAnimation.insert(map<tstring, SPTR<CAnimation>>::value_type(_AniData.AnimationName, NewAni));
	
	return NewAni;
}
CAnimation* CAniRenderer::CreateAnimation(ANIMATIONDATA* _AnimationData)
{
	if (nullptr == _AnimationData) 
	{
		return nullptr;
	}

	return CreateAnimation(_AnimationData->GetAniData());
}
CAnimation* CAniRenderer::CreateEditAnimation(ANIDATA _AniData)
{
	SPTR<CGameMultiSprite> AniSprite = COMRESMGR->FindMultiSprite(_AniData.AniSpriteName);

	if (nullptr == AniSprite)
	{
		return nullptr;
	}

	SPTR<CAnimation> Ani = FindAnimation(_AniData.AnimationName);

	CAnimation* NewAni = CAnimation::CreateAni(this, _AniData);

	if (nullptr == NewAni)
	{
		return nullptr;
	}

	NewAni->SetAniSprite(AniSprite);
	NewAni->AniReset();
	NewAni->Name(_AniData.AnimationName);
	
	if (nullptr != Ani)
	{
		map<tstring, SPTR<CAnimation>>::iterator Find = m_mapAnimation.find(_AniData.AnimationName);
		Find->second = NewAni;
	}
	else
	{
		m_mapAnimation.insert(map<tstring, SPTR<CAnimation>>::value_type(_AniData.AnimationName, NewAni));
	}
	

	return NewAni;


}
map<tstring, SPTR<CAnimation>>* CAniRenderer::GetAniMap() 
{
	return &m_mapAnimation;
}
BOOL CAniRenderer::EraseAnimation(const tstring& _EraseKey)
{
	map<tstring, SPTR<CAnimation>>::iterator FindIter = m_mapAnimation.find(_EraseKey);

	if (FindIter != m_mapAnimation.end()) 
	{
		m_mapAnimation.erase(_EraseKey);
		return TRUE;
	}
	return FALSE;
}
BOOL CAniRenderer::ClearAnimationMap()
{
	if (true == m_mapAnimation.empty()) 
	{
		return FALSE;
	}

	m_mapAnimation.clear();
	return TRUE;
}
void CAniRenderer::RenderOff()
{
	m_CurAni = nullptr;
}
void CAniRenderer::SetCurAniStyle(ANISTYLE _AniStyle)
{
	if (nullptr == m_CurAni) 
	{
		return;
	}
	m_CurAni->SetAniStyle(_AniStyle);
	return;
}
ANISTYLE CAniRenderer::GetCurAniStyle()
{
	if (nullptr == m_CurAni)
	{
		return MAXANISTYLE;
	}
	return  m_CurAni->GetAniStyle();
}

BOOL CAniRenderer::SaveAnimationData(ANISPRITETYPE _AniSpriteType)
{
	if (0 >= m_mapAnimation.size()) 
	{
		return FALSE;
	}

	CGameFile SaveFile = CGameFile(_T("EditData"), CGameFile::vecAniDataFileName[(int)_AniSpriteType]);

	if (FALSE == SaveFile.Open(_T("wb")))
	{
		return FALSE;
	}

	map<tstring, SPTR<CAnimation>>::iterator FindIter = m_mapAnimation.begin();

	for (; FindIter != m_mapAnimation.end(); FindIter++)
	{
		if (_AniSpriteType == FindIter->second->GetAniSpriteType())
		{
			ANIMATIONDATA SaveData = ANIMATIONDATA(FindIter->second->GetAniData());
			SaveData.AniStyle = FindIter->second->GetAniStyle();
			size_t Check = SaveData.WriteData(SaveFile.GetFile());
			
		}

	}

	
	return TRUE;
}
const Vec2 CAniRenderer::GetCurAniSpriteSize()
{
	return m_CurAni->m_AniSprite->GetSpriteSize();
}
const Vec2 CAniRenderer::GetCurAniSpritePivot()
{
	return m_CurAni->m_AniSprite->GetSpritePivot();
}
//////////////////////////////////// Set Animation Data ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ANIDATA SetAniData(int _StartIndex, int _EndIndex, int _AniCount, int _RepeatStartIndex, int _RepeatEndIndex,float _Interval)
{
	return ANIDATA({ _StartIndex, _EndIndex }, _AniCount, { _RepeatStartIndex, _RepeatEndIndex }, _Interval);
}

