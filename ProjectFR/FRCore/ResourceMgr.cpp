#include "stdafx.h"
#include "ResourceMgr.h"
#include"GameSprite.h"
#include"GameMultiSprite.h"
#include"GameMesh.h"
#include"GameTexture.h"
#include"GameFont.h"
#include"GameSound.h"

VERTEX CResourceMgr::SpriteVtx[4];
VTXIDX16 CResourceMgr::SpriteIdx[2];
VERTEX CResourceMgr::SpriteCalVtx[4];
VERTEX CResourceMgr::TriangleVtx[3];

DWORD  CResourceMgr::FontColor = 0;
bool   CResourceMgr::OneColorMode = false;

VERTEX* CResourceMgr::GetSpriteVtx()
{
	return SpriteVtx;
}
VERTEX* CResourceMgr::GetCalSpriteVtx()
{
	return SpriteCalVtx;
}
VTXIDX16* CResourceMgr::GetSpriteIdx()
{
	return SpriteIdx;
}
CResourceMgr::CResourceMgr(CGameWindow* _pWindow, LPDIRECT3DDEVICE9 _pDevice)
	:m_PairWindow(_pWindow),m_PairDevice(_pDevice)
{
	assert(_pWindow);
	assert(_pDevice);

	SpriteVtx[0] = VERTEX({ -0.5f, 0.5f, 1.0f }, { 0.0f, 0.0f });
	SpriteVtx[1] = VERTEX({ 0.5f, 0.5f, 1.0f }, { 1.0f,0.0f });
	SpriteVtx[2] = VERTEX({ 0.5f, -0.5f, 1.0f }, { 1.0f,1.0f });
	SpriteVtx[3] = VERTEX({ -0.5f, -0.5f, 1.0f }, { 0.0f,1.0f });

	SpriteCalVtx[0] = VERTEX({ -0.5f, 0.5f, 1.0f }, { 0, 0 });
	SpriteCalVtx[1] = VERTEX({ 0.5f, 0.5f, 1.0f }, { 1.0f,0 });
	SpriteCalVtx[2] = VERTEX({ 0.5f, -0.5f, 1.0f }, { 1.0f,1.0f });
	SpriteCalVtx[3] = VERTEX({ -0.5f, -0.5f, 1.0f }, { 0,1.0f });

	float Cos30 = cosf(D3DX_PI*0.166666f);
	float Sin30 = sinf(D3DX_PI*0.166666f);
	float R = 1.0f / ROOTTHREE;

	TriangleVtx[0] = VERTEX({ -0.5f, 0.5f, 1.0f }, { 0, 0 });
	TriangleVtx[1] = VERTEX({ -0.5f, 0.5f, 1.0f }, { 1.0f, 0 });
	TriangleVtx[2] = VERTEX({ -0.5f, 0.5f, 1.0f }, { 0, 1.0f });


	SpriteIdx[0] = { 0, 1, 2 };
	SpriteIdx[1] = { 0, 2, 3 };

	D3DXMatrixIdentity(&m_IdentifyMat);
}


CResourceMgr::~CResourceMgr()
{
	m_mapTexture.clear();
	m_mapSprite.clear();
	m_mapMultiSprite.clear();
	
}

void CResourceMgr::SetOneColorFont(D3DXCOLOR _Color) 
{
	int Alpha = (int)(max(_Color.a,1.0f) * 255);
	int Red = (int)(max(_Color.r, 1.0f) * 255);
	int Green = (int)(max(_Color.g, 1.0f) * 255);
	int Blue = (int)(max(_Color.b, 1.0f) * 255);

	FontColor = D3DCOLOR_ARGB(Alpha, Red, Green, Blue);
}
void CResourceMgr::OneColorOn()
{
	OneColorMode = true;
}
void CResourceMgr::OneColorOff() 
{
	OneColorMode = false;
}
bool CResourceMgr::IsOneColor() 
{
	return OneColorMode;
}

SPTR<CGameTexture> CResourceMgr::FindGameTexture(const tstring& _Name)
{	
	return FINDMAP_SPTR<CGameTexture>(_Name, m_mapTexture);
}
BOOL CResourceMgr::ExistGameTexture(const tstring& _TexKey)
{
	return EXISTMAP_SPTR<CGameTexture>(_TexKey, m_mapTexture);
}
SPTR<CGameSprite> CResourceMgr::FindGameSprite(const tstring& _Name)
{
	return FINDMAP_SPTR<CGameSprite>(_Name, m_mapSprite);
}
BOOL CResourceMgr::ExistGameSprite(const tstring& _SpriteKey)
{
	return EXISTMAP_SPTR<CGameSprite>(_SpriteKey, m_mapSprite);
}

CGameTexture* CResourceMgr::LoadGameTexture(const tstring& _Path)
{
	tstring TexKey = CFilePathMgr::PathToFileName(_Path);
	SPTR<CGameTexture> FindTex = FindGameTexture(TexKey);

	if (nullptr != FindTex)
	{
		return nullptr;
	}

	CGameTexture* NewTex = CGameTexture::CreateTexture(m_PairDevice, _Path);
	

	if (nullptr != NewTex) 
	{
		CCriSectionObject<CResourceMgr>();
		m_mapTexture.insert(unordered_map<tstring, SPTR<CGameTexture>>::value_type(TexKey, NewTex));
	}
	
	return NewTex;
}
SPTR<CGameTexture> CResourceMgr::ReLoadGameTexture(const tstring& _Path)
{
	tstring TexKey = CFilePathMgr::PathToFileName(_Path);
	unordered_map<tstring, SPTR<CGameTexture>>::iterator Find = m_mapTexture.find(TexKey);

	if (Find != m_mapTexture.end())
	{
		Find->second->ReLoadTexture(m_PairDevice, _Path);
		return Find->second;
	}
	
	CGameTexture* NewTex = CGameTexture::CreateTexture(m_PairDevice, _Path);

	if (nullptr != NewTex)
	{
		CCriSectionObject<CResourceMgr>();
		m_mapTexture.insert(unordered_map<tstring, SPTR<CGameTexture>>::value_type(TexKey, NewTex));
	}

	return NewTex;
}
CGameSprite* CResourceMgr::CreateGameSprite(const tstring& _TexKey, const tstring& _Key)
{
	SPTR<CGameTexture> Texture = FindGameTexture(_TexKey);

	if (nullptr == Texture)
	{
		assert(nullptr);
		return nullptr;
	}

	SPTR<CGameSprite> Sprite = FindGameSprite(_Key);

	if (nullptr != Sprite)
	{
		return nullptr;
	}

	CGameSprite* NewSprite = CGameSprite::CreateGameSprite(m_PairDevice);

	if (nullptr == NewSprite->GetGameMesh())
	{
		return nullptr;
	}

	NewSprite->SetGameTexture(Texture);

	Vec2 SpriteSize = Vec2{ (float)Texture->GetImageInfo().Width ,(float)Texture->GetImageInfo().Height };
	NewSprite->SetSpriteSize(SpriteSize);

	CCriSectionObject<CResourceMgr>();
	NewSprite->GetGameMesh()->FILLVTX<VERTEX>(SpriteVtx);
	NewSprite->GetGameMesh()->FILLIDX<VTXIDX16>(SpriteIdx);


	m_mapSprite.insert(unordered_map<tstring, SPTR<CGameSprite>>::value_type(_Key, NewSprite));

	return NewSprite;
}
CGameSprite*  CResourceMgr::CreateGameSprite(const tstring& _Key)
{
	return CreateGameSprite(_Key, _Key);
}
// Sprite를 생성만 해주는 함수
// 스프라이트를 직접 지워야한다.
CGameSprite* CResourceMgr::NewGameSprite(const tstring& _TexKey)
{
	SPTR<CGameTexture> Texture = FindGameTexture(_TexKey);

	if (nullptr == Texture)
	{
		assert(nullptr);
		return nullptr;
	}

	CGameSprite* NewSprite = CGameSprite::CreateGameSprite(m_PairDevice);

	if (nullptr == NewSprite->GetGameMesh())
	{
		return nullptr;
	}

	NewSprite->SetGameTexture(Texture);

	Vec2 SpriteSize = Vec2{ (float)Texture->GetImageInfo().Width ,(float)Texture->GetImageInfo().Height };
	NewSprite->SetSpriteSize(SpriteSize);

	CCriSectionObject<CResourceMgr>();
	NewSprite->GetGameMesh()->FILLVTX<VERTEX>(SpriteVtx);
	NewSprite->GetGameMesh()->FILLIDX<VTXIDX16>(SpriteIdx);

	return NewSprite;
}

CGameSprite* CResourceMgr::CreateEditGameSprite(const tstring& _TexKey, const tstring& _ImageKey)
{
	SPTR<CGameTexture> Texture = FindGameTexture(_TexKey);

	if (nullptr == Texture)
	{
		return nullptr;
	}

	SPTR<CGameSprite> Sprite = FindGameSprite(_ImageKey);

	CGameSprite* NewSprite = CGameSprite::CreateGameSprite(m_PairDevice);

	if (nullptr == NewSprite->GetGameMesh())
	{
		return nullptr;
	}

	NewSprite->SetGameTexture(Texture);

	Vec2 SpriteSize = Vec2{ (float)Texture->GetImageInfo().Width ,(float)Texture->GetImageInfo().Height };
	NewSprite->SetSpriteSize(SpriteSize);

	CCriSectionObject<CResourceMgr>();
	NewSprite->GetGameMesh()->FILLVTX<VERTEX>(SpriteVtx);
	NewSprite->GetGameMesh()->FILLIDX<VTXIDX16>(SpriteIdx);

	//Editor 전용 Image제작방식 -> 같은 이름이 있을경우 기존 키에 연결된
	//내용을 교체
	if (TRUE == ExistGameSprite(_ImageKey))
	{
		unordered_map<tstring, SPTR<CGameSprite>>::iterator Find = m_mapSprite.find(_ImageKey);
		Find->second = NewSprite;
	}
	else 
	{
		m_mapSprite.insert(unordered_map<tstring, SPTR<CGameSprite>>::value_type(_ImageKey, NewSprite));
	}
	return NewSprite;
}
CGameSprite* CResourceMgr::CreateEditGameSprite(const tstring& _Key)
{
	return CreateEditGameSprite(_Key, _Key);
}

CGameSprite* CResourceMgr::CreateGameSprite(const tstring& _TexKey, const tstring& _ImageKey, Vec2 _StartPos, Vec2 _Size)
{
	SPTR<CGameTexture> Texture = FindGameTexture(_TexKey);

	if (nullptr == Texture)
	{
		return nullptr;
	}

	SPTR<CGameSprite> Sprite = FindGameSprite(_ImageKey);

	if (nullptr != Sprite)
	{
		return nullptr;
	}

	CGameSprite* NewSprite = CGameSprite::CreateGameSprite(m_PairDevice);

	if (nullptr == NewSprite->GetGameMesh())
	{
		return nullptr;
	}

	NewSprite->SetGameTexture(Texture);
	NewSprite->SetSpriteSize(_Size);

	CCriSectionObject<CResourceMgr>();
	Texture->CalTexCoord(_StartPos, _Size, SpriteCalVtx);
	NewSprite->GetGameMesh()->FILLVTX<VERTEX>(SpriteCalVtx);
	NewSprite->GetGameMesh()->FILLIDX<VTXIDX16>(SpriteIdx);


	m_mapSprite.insert(unordered_map<tstring, SPTR<CGameSprite>>::value_type(_ImageKey, NewSprite));

	return NewSprite;
}
CGameSprite* CResourceMgr::CreateGameSprite(const tstring& _Key, Vec2 _StartPos, Vec2 _Size)
{
	return CreateGameSprite(_Key, _Key, _StartPos, _Size);
}

CGameSprite* CResourceMgr::CreateEditGameSprite(const tstring& _TexKey, const tstring& _SpriteKey, Vec2 _StartPos, Vec2 _Size)
{
	SPTR<CGameTexture> Texture = FindGameTexture(_TexKey);

	if (nullptr == Texture)
	{
		return nullptr;
	}

	CGameSprite* NewSprite = CGameSprite::CreateGameSprite(m_PairDevice);

	if (nullptr == NewSprite->GetGameMesh())
	{
		return nullptr;
	}

	NewSprite->SetGameTexture(Texture);
	NewSprite->SetSpriteSize(_Size);

	CCriSectionObject<CResourceMgr>();
	Texture->CalTexCoord(_StartPos, _Size, SpriteCalVtx);
	NewSprite->GetGameMesh()->FILLVTX<VERTEX>(SpriteCalVtx);
	NewSprite->GetGameMesh()->FILLIDX<VTXIDX16>(SpriteIdx);

	//Editor 전용 Image제작방식 -> 같은 이름이 있을경우 기존 키에 연결된
	//내용을 교체
	if (TRUE == ExistGameSprite(_SpriteKey))
	{
		unordered_map<tstring, SPTR<CGameSprite>>::iterator Find = m_mapSprite.find(_SpriteKey);
		Find->second = NewSprite;
	}
	else
	{
		m_mapSprite.insert(unordered_map<tstring, SPTR<CGameSprite>>::value_type(_SpriteKey, NewSprite));
	}
	return NewSprite;
}
CGameSprite* CResourceMgr::CreateEditGameSprite(const tstring& _Key, Vec2 _StartPos, Vec2 _Size)
{
	return CreateEditGameSprite(_Key, _Key, _StartPos, _Size);
}
SPTR<CGameMultiSprite> CResourceMgr::FindMultiSprite(const  tstring& _Name)
{
	return FINDMAP_SPTR<CGameMultiSprite>(_Name, m_mapMultiSprite);
}
BOOL CResourceMgr::ExistGameMultiSprite(const  tstring& _AniSpriteKey)
{
	return EXISTMAP_SPTR<CGameMultiSprite>(_AniSpriteKey, m_mapMultiSprite);
}
CGameMultiSprite* CResourceMgr::CreateMultiSprite(const tstring& _TextureKey
	, const tstring& _SpriteKey
	, Vec2 _StartPos
	, Vec2 _Size
	, int _ImageCount
	, int _XCount
	, int _YCount
	,Vec2 _Pivot /*= {0.0f,0.0f}*/
	, int _StartFrameIndex /*= 0*/
)
{
	
	SPTR<CGameTexture> Texture = FindGameTexture(_TextureKey);

	if (nullptr == Texture) 
	{
		assert(nullptr);
		return nullptr;
	}

	CGameMultiSprite*	NewMultiSprite = CGameMultiSprite::CreateMultiSprite(GetPairDevice());

	Vec2 CurPos = _StartPos;
	Vec2 Size = Vec2{ _Size.x / _XCount ,_Size.y / _YCount };
	
	// Sprite가 입력한 Count이상 넣으려 할때 반복문을 빠져나가기 위한 변수들
	bool End = false;
	int Count = 0;
	int StartFrameIndex = _StartFrameIndex;

	NewMultiSprite->VecReserve( _XCount* _YCount);
	for (int  y = 0; y < _YCount; ++y)
	{
		for (int x = 0; x < _XCount ; ++x)
		{
			if (StartFrameIndex > 0)
			{
				--StartFrameIndex;
				continue;
			}
			if (_ImageCount <= Count) 
			{
				End = true;
				break;
			}
			CGameSprite* NewSprite = CGameSprite::CreateGameSprite(GetPairDevice());
			NewSprite->SetGameTexture(Texture);
			
			CCriSectionObject<CResourceMgr>();
			Texture->CalTexCoord(CurPos, Size, SpriteCalVtx);
			NewSprite->GetGameMesh()->FILLVTX<VERTEX>(SpriteCalVtx);
			NewSprite->GetGameMesh()->FILLIDX<VTXIDX16>(SpriteIdx);
			NewSprite->SetSpriteSize(Size);
			NewSprite->SetSpritePivot(_Pivot);
			NewMultiSprite->PushSprite(NewSprite);
			CurPos.x += Size.x;
			++Count;
		}
		if (true == End) 
		{
			break;
		}
		CurPos.x = _StartPos.x;
		CurPos.y += Size.y;
	}

	NewMultiSprite->SetTexKey(_TextureKey);
	NewMultiSprite->Name(_SpriteKey);
	NewMultiSprite->SetStartPos(_StartPos);
	NewMultiSprite->SetMultiSpriteSize(_Size);
	NewMultiSprite->SetCountData(_XCount, _YCount, _ImageCount);
	NewMultiSprite->SetSpritePivot(_Pivot);
	NewMultiSprite->SetStartFrame(_StartFrameIndex);

	m_mapMultiSprite.insert(unordered_map<tstring, SPTR<CGameMultiSprite>>::value_type(_SpriteKey, NewMultiSprite));
	
	return NewMultiSprite;
}
CGameMultiSprite* CResourceMgr::CreateMultiSprite(const tstring& _TextureKey
	, const tstring& _SpriteKey
	, Vec2 _StartPos
	, Vec2 _Size
	, int _ImageCount
	, int _XCount
	, int _YCount
	, Vec2 _Pivot
	, int _StartFrameIndex
	, unordered_map<tstring, SPTR<CGameMultiSprite>>* _TempMap
) 
{
	SPTR<CGameTexture> Texture = FindGameTexture(_TextureKey);

	if (nullptr == Texture)
	{
		return nullptr;
	}

	if (nullptr == _TempMap) 
	{
		assert(nullptr);
		return nullptr;
	}

	CGameMultiSprite*	NewMultiSprite = CGameMultiSprite::CreateMultiSprite(GetPairDevice());

	Vec2 CurPos = _StartPos;
	Vec2 Size = Vec2{ _Size.x / _XCount ,_Size.y / _YCount };

	// Sprite가 입력한 Count이상 넣으려 할때 반복문을 빠져나가기 위한 변수들
	bool End = false;
	int Count = 0;
	int StartFrameIndex = _StartFrameIndex;

	NewMultiSprite->VecReserve(_XCount* _YCount);
	CCriSectionObject<CResourceMgr>();
	
	for (int y = 0; y < _YCount; ++y)
	{
		for (int x = 0; x < _XCount; ++x)
		{
			if (StartFrameIndex > 0)
			{
				--StartFrameIndex;
				continue;
			}
			if (_ImageCount <= Count)
			{
				End = true;
				break;
			}
			CGameSprite* NewSprite = CGameSprite::CreateGameSprite(GetPairDevice());
			NewSprite->SetGameTexture(Texture);

			Texture->CalTexCoord(CurPos, Size, SpriteCalVtx);
			NewSprite->GetGameMesh()->FILLVTX<VERTEX>(SpriteCalVtx);
			NewSprite->GetGameMesh()->FILLIDX<VTXIDX16>(SpriteIdx);
			NewSprite->SetSpriteSize(Size);
			NewSprite->SetSpritePivot(_Pivot);
			NewMultiSprite->PushSprite(NewSprite);
			CurPos.x += Size.x;
			++Count;
		}
		if (true == End)
		{
			break;
		}
		CurPos.x = _StartPos.x;
		CurPos.y += Size.y;
	}

	NewMultiSprite->SetTexKey(_TextureKey);
	NewMultiSprite->Name(_SpriteKey);
	NewMultiSprite->SetStartPos(_StartPos);
	NewMultiSprite->SetMultiSpriteSize(_Size);
	NewMultiSprite->SetCountData(_XCount, _YCount, _ImageCount);
	NewMultiSprite->SetSpritePivot(_Pivot);
	NewMultiSprite->SetStartFrame(_StartFrameIndex);

	_TempMap->insert(unordered_map<tstring, SPTR<CGameMultiSprite>>::value_type(_SpriteKey, NewMultiSprite));

	return NewMultiSprite;
}
// Editor 전용 AniSprite 제작
// 키가 중복되면 해당 키의 second를 새로 만든 AniSprite로 교체
CGameMultiSprite* CResourceMgr::CreateEditMultiSprite(const tstring& _TextureKey
	, const tstring& _SpriteKey
	, Vec2 _StartPos
	, Vec2 _Size
	, int _ImageCount
	, int _XCount
	, int _YCount
	, Vec2 _Pivot /*= { 0.0f,0.0f }*/
	, int _StartFrameIndex /*= 0*/
) 
{
	SPTR<CGameTexture> Texture = FindGameTexture(_TextureKey);

	if (nullptr == Texture)
	{
		return nullptr;
	}

	CGameMultiSprite*	NewMultiSprite = CGameMultiSprite::CreateMultiSprite(GetPairDevice());

	Vec2 CurPos = _StartPos;
	Vec2 Size = Vec2{ _Size.x / _XCount ,_Size.y / _YCount };

	// Sprite가 입력한 Count이상 넣으려 할때 반복문을 빠져나가기 위한 변수들
	bool End = false;
	int Count = 0;
	int StartFrameIndex = _StartFrameIndex;

	NewMultiSprite->VecReserve(_XCount* _YCount);
	for (int y = 0; y < _YCount; ++y)
	{
		for (int x = 0; x < _XCount; ++x)
		{
			if (StartFrameIndex > 0)
			{
				--StartFrameIndex;
				CurPos.x += Size.x;
				continue;
			}
			if (_ImageCount <= Count)
			{
				End = true;
				break;
			}
			CGameSprite* NewSprite = CGameSprite::CreateGameSprite(GetPairDevice());
			NewSprite->SetGameTexture(Texture);

			CCriSectionObject<CResourceMgr>();
			Texture->CalTexCoord(CurPos, Size, SpriteCalVtx);
			NewSprite->GetGameMesh()->FILLVTX<VERTEX>(SpriteCalVtx);
			NewSprite->GetGameMesh()->FILLIDX<VTXIDX16>(SpriteIdx);
			NewSprite->SetSpriteSize(Size);
			NewSprite->SetSpritePivot(_Pivot);
			NewMultiSprite->PushSprite(NewSprite);
			CurPos.x += Size.x;
			++Count;
		}
		if (true == End)
		{
			break;
		}
		CurPos.x = _StartPos.x;
		CurPos.y += Size.y;
	}
	NewMultiSprite->SetTexKey(_TextureKey);
	NewMultiSprite->Name(_SpriteKey);
	NewMultiSprite->SetStartPos(_StartPos);
	NewMultiSprite->SetMultiSpriteSize(_Size);
	NewMultiSprite->SetCountData(_XCount, _YCount, _ImageCount);
	NewMultiSprite->SetSpritePivot(_Pivot);
	NewMultiSprite->SetStartFrame(_StartFrameIndex);

	unordered_map<tstring, SPTR<CGameMultiSprite>>::iterator Find = m_mapMultiSprite.find(_SpriteKey);
	if (Find != m_mapMultiSprite.end())
	{
		Find->second = NewMultiSprite;
	}
	else
	{
		m_mapMultiSprite.insert(unordered_map<tstring, SPTR<CGameMultiSprite>>::value_type(_SpriteKey, NewMultiSprite));
	}
	return NewMultiSprite;
}

CGameMultiSprite* CResourceMgr::CreateMultiSprite(const tstring& _Key, vector<SPTR<CGameSprite>>* _vecGameSprite)
{
	CGameMultiSprite*	NewMultiSprite = CGameMultiSprite::CreateMultiSprite(GetPairDevice());

	NewMultiSprite->CopySpriteVec(_vecGameSprite);

	if (NewMultiSprite->GetSpriteCount() != _vecGameSprite->size()) 
	{
		SAFE_DELETE(NewMultiSprite);
		return nullptr;
	}

	NewMultiSprite->Name(_Key);

	m_mapMultiSprite.insert(unordered_map<tstring, SPTR<CGameMultiSprite>>::value_type(_Key, NewMultiSprite));
	return NewMultiSprite;

}

CGameMultiSprite* CResourceMgr::CreateMultiSprite(MULTISPRITEDATA* _MultiData)
{
	return CreateMultiSprite(_MultiData->TexKey
		, _MultiData->MultiSpriteKey
		, { _MultiData->StartPosX, _MultiData->StartPosY }
		, { _MultiData->SizeX, _MultiData->SizeY }
		, _MultiData->ImageCount
		, _MultiData->XCount
		, _MultiData->YCount
		, { _MultiData->PivotX,_MultiData->PivotY }
		, _MultiData->StartFrame
	);
}
BOOL CResourceMgr::EraseMultiSprite(const tstring& _EraseKey)
{
	unordered_map<tstring, SPTR<CGameMultiSprite>>::iterator FindIter = m_mapMultiSprite.find(_EraseKey);


	if (FindIter != m_mapMultiSprite.end()) 
	{
		m_mapMultiSprite.erase(FindIter);
		return TRUE;
	}
	return FALSE;
}

SPTR<CGameSprite>	CResourceMgr::FindMultiSpriteToIndex(const tstring& _Name, size_t _Index)
{
	SPTR<CGameMultiSprite> MultiSprite = FindMultiSprite(_Name);

	if (nullptr == MultiSprite) 
	{
		return nullptr;
	}

	if (_Index >= MultiSprite->GetSpriteCount()) 
	{
		return nullptr;
	}

	return MultiSprite->GetSprite(_Index);
}


SPTR<CGameFont> CResourceMgr::FindGameFont(const tstring& _FontKey)
{
	return FINDMAP_SPTR<CGameFont>(_FontKey, m_mapFont);
}
BOOL CResourceMgr::ExistGameFont(const tstring& _FontKey)
{
	return EXISTMAP_SPTR<CGameFont>(_FontKey, m_mapFont);
}
SPTR<CGameFont> CResourceMgr::LoadGameFont(const tstring& _FontName, Vec2 _FontSize)
{
	SPTR<CGameFont> FindFont = FindGameFont(_FontName);

	if (nullptr != FindFont) 
	{
		return FindFont;
	}

	CGameFont* NewFont = CGameFont::CreateGameFont(m_PairDevice, _FontSize, _FontName);

	if (FALSE == NewFont->IsUseFont()) 
	{
		return nullptr;
	}

	m_mapFont.insert(map<tstring, SPTR<CGameFont>>::value_type(_FontName, NewFont));

	if (1 == m_mapFont.size()) 
	{
		m_CurFont = NewFont;
	}

	return NewFont;
}
SPTR<CGameFont> CResourceMgr::LoadGameFont(const tstring& _FontName
	, const tstring& _FontKey
	, Vec2 _FontSize
	, const UINT& _Bold/* = FW_NORMAL*/
)
{
	SPTR<CGameFont> FindFont = FindGameFont(_FontKey);

	if (nullptr != FindFont)
	{
		return FindFont;
	}

	CGameFont* NewFont = CGameFont::CreateGameFont(m_PairDevice, _FontSize, _FontName, _Bold);

	if (FALSE == NewFont->IsUseFont())
	{
		return nullptr;
	}

	m_mapFont.insert(map<tstring, SPTR<CGameFont>>::value_type(_FontKey, NewFont));

	if (1 == m_mapFont.size())
	{
		m_CurFont = NewFont;
	}

	return NewFont;
}

BOOL CResourceMgr::SettingFont(const tstring& _FontName)
{
	SPTR<CGameFont> FindFont = FindGameFont(_FontName);

	if (nullptr == FindFont) 
	{
		return FALSE;
	}

	m_CurFont = FindFont;

	return TRUE;
}
BOOL CResourceMgr::DrawFont(const tstring& _FontName, const tstring& _OutText,const Vec2& _Pos, float _FontMag /*= 1.0f*/, DWORD Color /*= D3DCOLOR_XRGB(0, 0, 0)*/)
{
	SPTR<CGameFont> Font = FindGameFont(_FontName);

	if (nullptr == Font)
	{
		return FALSE;
	}
	Vec2 SyncPos = SynchroWndPos(_Pos);

	Font->DrawGameText(m_PairDevice, _OutText, SyncPos, _FontMag, Color);
	return TRUE;
}
BOOL CResourceMgr::DrawFont(const tstring& _OutText,const  Vec2& _Pos, float _FontMag /*= 1.0f*/, DWORD Color /*= D3DCOLOR_XRGB(0, 0, 0)*/)
{
	if (nullptr == m_CurFont) 
	{
		return FALSE;
	}
	Vec2 SyncPos = SynchroWndPos(_Pos);

	m_CurFont->DrawGameText(m_PairDevice, _OutText, SyncPos, _FontMag, Color);
	return TRUE;
}
BOOL CResourceMgr::DrawFont(const tstring& _FontName, const tstring& _OutText, POINT _LeftStartPos, float _FontMag /*= 1.0f*/, DWORD Color/* = D3DCOLOR_XRGB(0, 0, 0)*/)
{
	SPTR<CGameFont> Font = FindGameFont(_FontName);

	if (nullptr == Font)
	{
		return FALSE;
	}
	
	POINT WndPos = SynchroWndPos(_LeftStartPos);

	Font->DrawGameText(m_PairDevice, _OutText, WndPos, _FontMag, Color);
	return TRUE;
}
BOOL CResourceMgr::DrawFont_RightStart(const tstring& _FontName, const tstring& _OutText, const POINT& _RightStartPos, float _FontMag /*= 1.0f*/, DWORD Color /*= D3DCOLOR_XRGB(0, 0, 0)*/)
{
	SPTR<CGameFont> Font = FindGameFont(_FontName);

	if (nullptr == Font)
	{
		return FALSE;
	}

	POINT WndPos = SynchroWndPos(_RightStartPos);

	Font->DrawGameText_RightStart(m_PairDevice, _OutText, WndPos, _FontMag, Color);
	return TRUE;
}
BOOL CResourceMgr::DrawFont(const tstring& _OutText, const Vec3& _Pos, float _FontMag /*= 1.0f*/, DWORD Color/* = D3DCOLOR_XRGB(0, 0, 0)*/)
{
	return DrawFont(_OutText, Vec2{ _Pos.x,_Pos.y }, _FontMag, Color);
}
BOOL CResourceMgr::DrawFont(const tstring& _FontName, const tstring& _OutText, const Vec3& _Pos, float _FontMag /*= 1.0f*/, DWORD Color /*= D3DCOLOR_XRGB(0, 0, 0)*/)
{
	return DrawFont(_FontName, _OutText, Vec2{ _Pos.x,_Pos.y }, _FontMag, Color);
}
Vec2 CResourceMgr::SynchroWndPos(const Vec2& _WorldPos)
{
	POINT WndSize = m_PairWindow->WndSize();
	Vec2 ChangePos = _WorldPos;

	int HalfWidth = WndSize.x / 2;
	int HalfHeight = WndSize.y / 2;
	
	if (1 == (WndSize.x % 2)) 
	{
		HalfWidth += 1;
	}
	if (1 == (WndSize.y % 2))
	{
		HalfHeight += 1;
	}

	// ViewPort 변환의 2차원의 한정 변환은
	// 윈도우의 (너비 절반,-높이 절반)을 더한 뒤
	// 높이 축을 -1을 곱하면 된다.

	ChangePos.x = ChangePos.x + HalfWidth;
	ChangePos.y = -1 * (ChangePos.y - HalfHeight);

	
	return ChangePos;
}
POINT CResourceMgr::SynchroWndPos(const POINT& _WorldPos)
{
	POINT WndSize = m_PairWindow->WndSize();
	POINT ChangePos = _WorldPos;

	int HalfWidth = WndSize.x / 2;
	int HalfHeight = WndSize.y / 2;

	if (1 == (WndSize.x % 2))
	{
		HalfWidth += 1;
	}
	if (1 == (WndSize.y % 2))
	{
		HalfHeight += 1;
	}

	// ViewPort 변환의 2차원의 한정 변환은
	// 윈도우의 (너비 절반,-높이 절반)을 더한 뒤
	// 높이 축을 -1을 곱하면 된다.

	ChangePos.x = ChangePos.x + HalfWidth;
	ChangePos.y = -1 * (ChangePos.y - HalfHeight);


	return ChangePos;
}
unordered_map<tstring, SPTR<CGameSprite>>*		CResourceMgr::GetSpriteMap()
{
	return &m_mapSprite;
}
unordered_map<tstring, SPTR<CGameMultiSprite>>* CResourceMgr::GetAniSpriteMap() 
{
	return &m_mapMultiSprite;
}


/////////////////////////////////// Image 생성 함수(서브 맵에 저장할 경우) ////////////////////////////////////
CGameTexture* CResourceMgr::LoadGameTexture(const tstring& _Path, unordered_map<tstring, SPTR<CGameTexture>>* _LoadMap)
{
	if (nullptr == _LoadMap) 
	{
		return nullptr;
	}

	tstring TexKey = CFilePathMgr::PathToFileName(_Path);
	
	unordered_map<tstring, SPTR<CGameTexture>>::iterator FindIter = m_mapTexture.find(TexKey);

	if (m_mapTexture.end() != FindIter)
	{
		return nullptr;
	}


	 FindIter = _LoadMap->find(TexKey);


	if (FindIter != _LoadMap->end()) 
	{
		return nullptr;
	}


	CGameTexture* NewTex = CGameTexture::CreateTexture(m_PairDevice, _Path);

	if (nullptr != NewTex)
	{
		_LoadMap->insert(unordered_map<tstring, SPTR<CGameTexture>>::value_type(TexKey, NewTex));
	}


	return NewTex;
}
CGameSprite* CResourceMgr::CreateGameSprite(CGameTexture* _Tex, const tstring& _Key, unordered_map<tstring, SPTR<CGameSprite>>* _LoadMap)
{
	if (nullptr == _Tex || nullptr == _LoadMap)
	{
		return nullptr;
	}

	unordered_map<tstring, SPTR<CGameSprite>>::iterator FindIter = _LoadMap->find(_Key);

	if (FindIter != _LoadMap->end()) 
	{
		return nullptr;
	}

	CGameSprite* NewSprite = CGameSprite::CreateGameSprite(m_PairDevice);

	if (nullptr == NewSprite->GetGameMesh())
	{
		SAFE_DELETE(NewSprite);
		return nullptr;
	}

	NewSprite->SetGameTexture(_Tex);

	Vec2 SpriteSize = Vec2{ (float)_Tex->GetImageInfo().Width ,(float)_Tex->GetImageInfo().Height };
	NewSprite->SetSpriteSize(SpriteSize);

	NewSprite->GetGameMesh()->FILLVTX<VERTEX>(SpriteVtx);
	NewSprite->GetGameMesh()->FILLIDX<VTXIDX16>(SpriteIdx);


	_LoadMap->insert(unordered_map<tstring, SPTR<CGameSprite>>::value_type(_Key, NewSprite));

	return NewSprite;
}
BOOL CResourceMgr::EraseGameSprite(const tstring& _EraseKey)
{
	unordered_map<tstring, SPTR<CGameSprite>>::iterator FindIter = m_mapSprite.find(_EraseKey);

	if (FindIter != m_mapSprite.end())
	{
		m_mapSprite.erase(FindIter);
		return TRUE;
	}
	return FALSE;
}
void CResourceMgr::MergeTextureMap(unordered_map<tstring, SPTR<CGameTexture>>* _LoadMap)
{
	if (nullptr == _LoadMap || 0 >=_LoadMap->size()) 
	{
		return;
	}

	m_mapTexture.insert(_LoadMap->begin(), _LoadMap->end());

}
void CResourceMgr::MergeSpriteMap(unordered_map<tstring, SPTR<CGameSprite>>* _LoadMap)
{

	if (nullptr == _LoadMap || 0 >= _LoadMap->size())
	{
		return;
	}

	m_mapSprite.insert(_LoadMap->begin(), _LoadMap->end());
}

/////////////////////////////////////////		Save MultiSprite Or Animation Data Logic		/////////////////////////////////////////////////////////
BOOL CResourceMgr::ConversionData(MULTISPRITEDATA* _Data, SPTR<CGameMultiSprite> _AniSprite)
{
	if (nullptr == _AniSprite || nullptr == _Data)
	{
		return FALSE;
	}


	_tcscpy_s(_Data->TexKey, _countof(_Data->TexKey), _AniSprite->GetTexKey_str());
	_tcscpy_s(_Data->MultiSpriteKey, _countof(_Data->MultiSpriteKey), _AniSprite->Name_str());
	
	_Data->StartPosX = _AniSprite->GetStartPos().x;
	_Data->StartPosY = _AniSprite->GetStartPos().y;
	_Data->SizeX = _AniSprite->GetMultiSpriteSize().x;
	_Data->SizeY = _AniSprite->GetMultiSpriteSize().y;
	_Data->XCount = _AniSprite->GetXCount();
	_Data->YCount = _AniSprite->GetYCount();
	_Data->StartFrame = _AniSprite->GetStartFrame();
	_Data->ImageCount = _AniSprite->GetImageCount();
	_Data->PivotX = _AniSprite->GetSpritePivot().x;
	_Data->PivotY = _AniSprite->GetSpritePivot().y;
	
	return TRUE;	
}

BOOL CResourceMgr::GetMultiSpriteData(vector<MULTISPRITEDATA>* _SaveBuf, ANISPRITETYPE _Type)
{
	if (nullptr == _SaveBuf) 
	{
		return FALSE;
	}

	if (MAXANISPRITETYPE <= _Type) 
	{
		return FALSE;
	}

	unordered_map<tstring, SPTR<CGameMultiSprite>>::iterator Start = m_mapMultiSprite.begin();
	unordered_map<tstring, SPTR<CGameMultiSprite>>::iterator End = m_mapMultiSprite.end();
	
	for (; Start != End ; ++Start)
	{
		if (_Type == Start->second->GetSpriteType()) 
		{
			MULTISPRITEDATA Data;
			ConversionData(&Data, Start->second);
			_SaveBuf->push_back(Data);
		}
	}

	return TRUE;
}
BOOL CResourceMgr::LoadTextureInFolder(const tstring& _FolderKey)
{
	tstring Path = CFilePathMgr::GetPath(_FolderKey);

	if (_T("") == Path) 
	{
		return FALSE;
	}


	_tfinddata_t Fd;

	unordered_map<tstring, SPTR<CGameTexture>> TempMap;
	TempMap.clear();

	for (int Index = (int)LOADINGFILE::PNG; Index < (int)LOADINGFILE::FILEMAX; ++Index)
	{
		tstring FilePath = Path + CFilePathMgr::ImageFile[Index];
		intptr_t handle = _tfindfirst(FilePath.c_str(), &Fd);

		if (handle == -1) 
		{
			continue;
		}
	
		int Check = 0;
		
		do
		{
			tstring FileName = Fd.name;
			LoadGameTexture(Path + FileName,&TempMap);
			Check = _tfindnext(handle, &Fd);

		} while (Check != -1);
	}

	CCriSectionObject<CResourceMgr>();
	MergeTextureMap(&TempMap);
	//m_mapTexture.insert(TempMap.begin(), TempMap.end());

	TempMap.clear();

	return TRUE;
}
BOOL CResourceMgr::LoadSpriteData(FILE* _LoadFile)
{
	if (nullptr == _LoadFile) 
	{
		return FALSE;
	}

	size_t LoadCount;
	MULTISPRITEDATA LoadSpriteData;
	while (0 < (LoadCount = fread_s(&LoadSpriteData, sizeof(LoadSpriteData), sizeof(MULTISPRITEDATA), 1, _LoadFile))) 
	{
		
		LoadSpriteData;
		
		CreateMultiSprite(LoadSpriteData.TexKey
			, LoadSpriteData.MultiSpriteKey
			, { LoadSpriteData.StartPosX, LoadSpriteData.StartPosY}
			, { LoadSpriteData.SizeX, LoadSpriteData.SizeY }
			, LoadSpriteData.ImageCount
			, LoadSpriteData.XCount
			, LoadSpriteData.YCount
			, { LoadSpriteData.PivotX,LoadSpriteData.PivotY }
			, LoadSpriteData.StartFrame
		);
	}
	
	return TRUE;
}
BOOL CResourceMgr::LoadSpriteData(FILE* _LoadFile, ANISPRITETYPE _AniType, vector<MULTISPRITEDATA>* _vecMulData)
{
	if (nullptr == _LoadFile)
	{
		return FALSE;
	}

	size_t LoadCount;
	MULTISPRITEDATA LoadSpriteData;
	while (0 < (LoadCount = fread_s(&LoadSpriteData, sizeof(LoadSpriteData), sizeof(MULTISPRITEDATA), 1, _LoadFile)))
	{

		_vecMulData->push_back(LoadSpriteData);

		SPTR<CGameMultiSprite> LoadMultiSprite = CreateEditMultiSprite(LoadSpriteData.TexKey
			, LoadSpriteData.MultiSpriteKey
			, { LoadSpriteData.StartPosX, LoadSpriteData.StartPosY }
			, { LoadSpriteData.SizeX, LoadSpriteData.SizeY }
			, LoadSpriteData.ImageCount
			, LoadSpriteData.XCount
			, LoadSpriteData.YCount
			, { LoadSpriteData.PivotX  ,LoadSpriteData.PivotY }
			, LoadSpriteData.StartFrame
		);

		

		LoadMultiSprite->SetSpriteType(_AniType);
	}

	return TRUE;
}
BOOL CResourceMgr::LoadMultiSpriteToData(MULTISPRITEDATA* _Data, ANISPRITETYPE _AniType)
{
	if (nullptr == _Data )
	{
		assert(nullptr);
		return FALSE;
	}
	CCriSectionObject<CResourceMgr>();

	SPTR<CGameMultiSprite> LoadMultiSprite = CreateMultiSprite(_Data->TexKey
		, _Data->MultiSpriteKey
		, { _Data->StartPosX, _Data->StartPosY }
		, { _Data->SizeX,_Data->SizeY }
		, _Data->ImageCount
		, _Data->XCount
		, _Data->YCount
		, { _Data->PivotX  ,_Data->PivotY }
		, _Data->StartFrame
	);

	if (nullptr == LoadMultiSprite)
	{
		return FALSE;
	}

	LoadMultiSprite->SetSpriteType(_AniType);
	return TRUE;
}
BOOL CResourceMgr::LoadMultiSpriteToData(MULTISPRITEDATA* _Data, ANISPRITETYPE _AniType, unordered_map<tstring, SPTR<CGameMultiSprite>>* _TempMap)
{
	if (nullptr == _Data || nullptr == _TempMap) 
	{
		assert(nullptr);
		return FALSE;
	}
	CCriSectionObject<CResourceMgr>();

	SPTR<CGameMultiSprite> LoadMultiSprite = CreateMultiSprite(_Data->TexKey
		, _Data->MultiSpriteKey
		, { _Data->StartPosX, _Data->StartPosY }
		, { _Data->SizeX,_Data->SizeY }
		, _Data->ImageCount
		, _Data->XCount
		, _Data->YCount
		, { _Data->PivotX  ,_Data->PivotY }
		, _Data->StartFrame
		, _TempMap
	);

	if (nullptr == LoadMultiSprite) 
	{
		return FALSE;
	}
	LoadMultiSprite->SetSpriteType(_AniType);
	return TRUE;
}

void CResourceMgr::MergeMultiSpriteMap(unordered_map<tstring, SPTR<CGameMultiSprite>>* _TempMap) 
{
	if (nullptr == _TempMap) 
	{
		assert(nullptr);
		return;
	}

	if (0 >= _TempMap->size()) 
	{
		assert(nullptr);
		return;
	}

	CCriSectionObject<CResourceMgr>();
	m_mapMultiSprite.insert(_TempMap->begin(), _TempMap->end());
}

CGameMesh* CResourceMgr::CreateTriMesh(const Vec2& _TriPos0, const Vec2& _TriPos1, const Vec2& _TriPos2)
{
	if (nullptr == m_PairDevice)
	{
		return nullptr;
	}

	LPDIRECT3DVERTEXBUFFER9 MeshVBuff = CGameMesh::CreateVtxBuffer<VERTEX>(m_PairDevice, 3);

	if (nullptr == MeshVBuff)
	{
		return nullptr;
	}

	CGameMesh* NewMesh = new CGameMesh(m_PairDevice, MeshVBuff);

	if (nullptr == NewMesh)
	{
		assert(nullptr);
		return nullptr;
	}

	NewMesh->SetMesh<VERTEX>(3);
	
	TriangleVtx[0].vPos = Vec3{ _TriPos0.x,_TriPos0.y,1.0f };
	TriangleVtx[1].vPos = Vec3{ _TriPos1.x,_TriPos1.y,1.0f };
	TriangleVtx[2].vPos = Vec3{ _TriPos2.x,_TriPos2.y,1.0f };
	
	
	NewMesh->FILLVTX<VERTEX>(TriangleVtx);
	
	return NewMesh;
}
CGameMesh* CResourceMgr::CreateTriMesh(VERTEX* _VtxBuf)
{
	if (nullptr == m_PairDevice || nullptr == _VtxBuf)
		return nullptr;
	

	LPDIRECT3DVERTEXBUFFER9 MeshVBuff = CGameMesh::CreateVtxBuffer<VERTEX>(m_PairDevice, 3);

	if (nullptr == MeshVBuff)
	{
		return nullptr;
	}

	CGameMesh* NewMesh = new CGameMesh(m_PairDevice, MeshVBuff);

	if (nullptr == NewMesh)
	{
		assert(nullptr);
		return nullptr;
	}

	NewMesh->SetMesh<VERTEX>(3);
	bool Check = NewMesh->FILLVTX<VERTEX>(_VtxBuf);

	return NewMesh;
}
void CResourceMgr::DrawTriangle(const Vec2& _TriPoint0,const Vec2& _TriPoint1, const Vec2& _TriPoint2,D3DXCOLOR _Color)
{
	CGameMesh* TriMesh = CreateTriMesh(_TriPoint0,_TriPoint1, _TriPoint2);

	if (nullptr != TriMesh)
	{
		m_PairDevice->SetTransform(D3DTS_WORLD, &m_IdentifyMat);

		m_PairDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);

		m_PairDevice->SetTexture(0, nullptr);
		m_PairDevice->SetRenderState(D3DRS_BLENDFACTOR, _Color);
		m_PairDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVBLENDFACTOR);
		m_PairDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_BLENDFACTOR);
		m_PairDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);

		TriMesh->Render(m_PairDevice, D3DPRIMITIVETYPE::D3DPT_TRIANGLELIST);

		m_PairDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, false);


		SAFE_DELETE(TriMesh);
	}

}

void CResourceMgr::SoundUpdate()
{
	m_FSystem->update();
}
unordered_map<tstring, SPTR<CGameSound>>* CResourceMgr::GetSoundMap()
{
	if (nullptr == &m_mapSound) 
	{
		assert(nullptr);
		return nullptr;
	}

	return &m_mapSound;
}