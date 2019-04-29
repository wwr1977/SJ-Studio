#include "stdafx.h"
#include "EditNumberUI.h"
#include<GameMultiSprite.h>

map<tstring, FontSizeData>	CEditNumberUI::m_mapFontSizeData;


CEditNumberUI::CEditNumberUI()
	:m_NumberSprite(nullptr), m_NumInterval(0.9f)
	, m_iCurNumber(-1),m_iPrevNumber(-1)
	, m_StartNumPivot(0.0f,0.0f), m_iNumberCount(0)
	, m_CurNumberFontSize(Vec2(0.0f,0.0f))
	, m_CurFirstFontHSize(Vec2(0.0f,0.0f))
	, m_CurFontName(_T(""))
{

}


CEditNumberUI::~CEditNumberUI()
{
	m_vecNumUI.clear();
}
void CEditNumberUI::InitFontNumberData()
{
	m_mapFontSizeData.insert(map<tstring, FontSizeData>::value_type(_T("Number_Red"), FontSizeData(0.6f, 1.0f)));
	m_mapFontSizeData.insert(map<tstring, FontSizeData>::value_type(_T("Number_Blue"), FontSizeData(0.6f, 1.0f)));
	m_mapFontSizeData.insert(map<tstring, FontSizeData>::value_type(_T("Number_Black"), FontSizeData(0.8f, 1.0f)));
	m_mapFontSizeData.insert(map<tstring, FontSizeData>::value_type(_T("Number_Sp"), FontSizeData(1.0f, 0.9f)));
}
void CEditNumberUI::InitData(const tstring& _Data)
{
	m_NumberSprite = COMRESMGR->FindMultiSprite(_Data);

	if (nullptr == m_NumberSprite) 
	{
		assert(nullptr);
		return;
	}

	m_vecNumUI.push_back(ACTOR->CreateCom<CFixRenderer>(RENDATA(LAYER_UI, 1.0f)));
	m_vecNumUI.push_back(ACTOR->CreateCom<CFixRenderer>(RENDATA(LAYER_UI, 1.0f)));
	m_vecNumUI.push_back(ACTOR->CreateCom<CFixRenderer>(RENDATA(LAYER_UI, 1.0f)));
	m_vecNumUI.push_back(ACTOR->CreateCom<CFixRenderer>(RENDATA(LAYER_UI, 1.0f)));
	m_vecNumUI.push_back(ACTOR->CreateCom<CFixRenderer>(RENDATA(LAYER_UI, 1.0f)));

	
	
	
	
}

BOOL CEditNumberUI::SetNumber(const int& _Value)
{
	if (-1 >= _Value) 
	{
		return FALSE;
	}
	
	m_iCurNumber = _Value;

	
	AllOffNumberUI();

	if (0 == _Value) 
	{
		m_vecNumUI[0]->SetSprite(m_NumberSprite->GetSprite(0));
		m_vecNumUI[0]->On();
		m_iNumberCount = 1;
		SyncNumberSpacing();
		return TRUE;
	}

	// 부호 UI가 +,- 인지 판단하는 값(true(+) , false(-) )
	bool sign = true;

	if (_Value < 0) 
	{
		sign = false;
	}

	
	int Value = min(_Value , 99999);
	
	
	m_iNumberCount = CalNumberSize(Value);
	int Base = (int)powf(10.0f, (float)m_iNumberCount);

	for (int Index = 0; Index <= m_iNumberCount; ++Index)
	{
		int SpriteIndex = Value / Base;
		m_vecNumUI[Index]->SetSprite(m_NumberSprite->GetSprite(SpriteIndex));
		//m_vecNumUI[Index]->On();
		Value -= SpriteIndex * Base;
		Base /= 10;
	}

	SyncNumberSpacing();

	return TRUE;
}
void CEditNumberUI::OnNumberUI()
{
	if (0 == m_iNumberCount) 
	{
		return;
	}

	for (int i = 0; i <= m_iNumberCount; i++)
	{
		m_vecNumUI[i]->On();
	}
}
int CEditNumberUI::CalNumberSize(const int& _Value)
{
	if (_Value <= 0) 
	{
		return 0;
	}

	int Base = 10000;
	int NumSize = 4;

	while ((_Value/Base) == 0 && Base > 1 ) 
	{
		Base /= 10;
		--NumSize;
	}

	return NumSize;
}
void CEditNumberUI::AllOffNumberUI()
{
	for (size_t i = 0; i < m_vecNumUI.size(); ++i)
	{
		m_vecNumUI[i]->Off();
	}

}

void CEditNumberUI::Init()
{
	TRANS->SetPos({ 0.0f,0.0f });
}
void CEditNumberUI::Update()
{
	if (m_iPrevNumber != m_iCurNumber) 
	{
		SetNumber(m_iCurNumber);
	}

	m_iPrevNumber = m_iCurNumber;
}
void CEditNumberUI::SetImageScale(const float& _Scale)
{
	ACTOR->SetImageScale(_Scale);

}
void CEditNumberUI::SetCustomSize(const Vec2& _CustomSize)
{
	for (size_t i = 0; i < m_vecNumUI.size(); i++)
	{
		m_vecNumUI[i]->SetCustomSize(_CustomSize);
	}
	return;
}
void CEditNumberUI::SetCustomSize(const float& _X, const float& _Y)
{
	for (size_t i = 0; i < m_vecNumUI.size(); i++)
	{
		m_vecNumUI[i]->SetCustomSize(_X,_Y);
	}

	return;
}
void CEditNumberUI::SetNumInterval(const float& _Interval)
{
	m_NumInterval = _Interval;
}
void CEditNumberUI::SyncNumberSpacing()
{
	float ImageScale = ACTOR->GetMagnification();

	Vec2 SpriteSize;

	if(m_vecNumUI[0]->isCustomSize())
		m_CurNumberFontSize = Vec2(m_vecNumUI[0]->GetCustomSize().x * ImageScale * 0.5f, m_vecNumUI[0]->GetCustomSize().y * ImageScale);
	else
		m_CurNumberFontSize = Vec2(m_vecNumUI[0]->GetCurSpriteSize().x* ImageScale * 0.5f, m_vecNumUI[0]->GetCurSpriteSize().y);

	m_CurFirstFontHSize = m_CurNumberFontSize;

	m_vecNumUI[0]->SetRenPivot(m_StartNumPivot);

	
	float AccSpacing = 0.0f;
	Vec2  LastFontSizeH = Vec2{0.0f,0.0f};

	for (int i = 1; i <= m_iNumberCount; ++i)
	{
		if (true == m_vecNumUI[i - 1]->isCustomSize()) 
		{
			SpriteSize = m_vecNumUI[i - 1]->GetCustomSize();
		}
		else 
		{
			SpriteSize = m_vecNumUI[i - 1]->GetCurSpriteSize();
		}

		LastFontSizeH = Vec2(SpriteSize.x*ImageScale * 0.5f , 0.0f);
		AccSpacing += SpriteSize.x*m_NumInterval*ImageScale;
		m_vecNumUI[i]->SetRenPivot({ m_StartNumPivot.x + AccSpacing, m_StartNumPivot.y });
	}

	m_CurNumberFontSize = m_CurNumberFontSize + LastFontSizeH + Vec2(AccSpacing,0.0f);

}

void CEditNumberUI::SetRenPivot(const Vec2& _Pivot)
{
	m_StartNumPivot = _Pivot;
}
void CEditNumberUI::ChangeNumberSprite(const tstring& _SpriteKey)
{
	SPTR<CGameMultiSprite> NumberSprite = COMRESMGR->FindMultiSprite(_SpriteKey);

	if (nullptr == NumberSprite)
	{
		return;
	}

	m_NumberSprite = NumberSprite;
	m_CurFontName = _SpriteKey;
	SettingSpriteSize(_SpriteKey);
	SetNumber(m_iCurNumber);
}
void CEditNumberUI::SetPos(const Vec2& _Pos)
{
	TRANS->SetPos(_Pos);
}
void CEditNumberUI::SetPos(const Vec3& _Pos)
{
	TRANS->SetPos(_Pos);
}
void CEditNumberUI::SetMiddlePos(const Vec2& _MiddlePos)
{
	float MiddlePivot = m_CurNumberFontSize.x * 0.5f - m_CurFirstFontHSize.x;
	TRANS->SetPos(Vec2(_MiddlePos.x - MiddlePivot, _MiddlePos.y));
}
void CEditNumberUI::SetCurFontName(const tstring& _FontName)
{
	m_CurFontName = _FontName;
}
void CEditNumberUI::SettingSpriteSize(const tstring& _SpriteKey)
{
	map<tstring, FontSizeData>::iterator FindData = m_mapFontSizeData.find(_SpriteKey);

	if (m_mapFontSizeData.end() == FindData) 
	{
		return;
	}

	ACTOR->SetImageScale(FindData->second.ImageMag);
	SetNumInterval(FindData->second.NumInterval);
	return;
}
void CEditNumberUI::SetAlpha(const float& _Alpha)
{
	for (size_t i = 0; i < m_vecNumUI.size(); i++)
	{
		if (nullptr == m_vecNumUI[i] ) 
			continue;
		
		m_vecNumUI[i]->SetAlpha(_Alpha);
	}
}
void CEditNumberUI::FontRenderOn()
{
	for (size_t i = 0; i <= m_iCurNumber; i++)
	{
		if (nullptr != m_vecNumUI[i] && false == m_vecNumUI[i]->IsActive())
			m_vecNumUI[i]->On();
	}
}
void CEditNumberUI::FontRenderOff()
{
	for (size_t i = 0; i < m_vecNumUI.size(); i++)
	{
		if (nullptr != m_vecNumUI[i] && true == m_vecNumUI[i]->IsActive())
			m_vecNumUI[i]->Off();
	}
}