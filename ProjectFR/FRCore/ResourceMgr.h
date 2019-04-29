#pragma once
#include "ObjBase.h"


class CGameTexture;
class CGameMultiSprite;
class CGameSprite;
class CGameWindow;
class CGameSound;
class CGameFont;
class CGameMesh;
class CResourceMgr :public CObjBase
{
private:
	static VERTEX SpriteCalVtx[4];
	static VERTEX SpriteVtx[4];
	static VERTEX TriangleVtx[3];
	static VTXIDX16 SpriteIdx[2];

	static DWORD	 FontColor;
	static bool		 OneColorMode;
	
public:
	static VERTEX* GetSpriteVtx();
	static VERTEX* GetCalSpriteVtx();
	static VTXIDX16* GetSpriteIdx();
public:
	static void SetOneColorFont(D3DXCOLOR _Color);
	static void OneColorOn();
	static void OneColorOff();
	static bool IsOneColor();


public:
	friend class CGameWindow;


private:
	CGameWindow*											m_PairWindow;
	LPDIRECT3DDEVICE9										m_PairDevice;


	unordered_map<tstring, SPTR<CGameTexture>>				m_mapTexture;
	unordered_map<tstring, SPTR<CGameSprite>>				m_mapSprite;
	unordered_map<tstring, SPTR<CGameMultiSprite>>			m_mapMultiSprite;

	Mat														m_IdentifyMat;
public:
	void SetPairWindow(CGameWindow* _pWindow)
	{
		assert(_pWindow);

		if (nullptr == _pWindow)
		{
			return;
		}

		m_PairWindow = _pWindow;
	}
	CGameWindow* GetPairWindow()
	{
		return m_PairWindow;
	}
	LPDIRECT3DDEVICE9 GetPairDevice()
	{
		return m_PairDevice;
	}


public:
	SPTR<CGameTexture> FindGameTexture(const tstring& _Name);
	BOOL ExistGameTexture(const tstring& _TexKey);

	SPTR<CGameSprite> FindGameSprite(const tstring& _Name);
	BOOL ExistGameSprite(const tstring& _SpriteKey);

	SPTR<CGameMultiSprite> FindMultiSprite(const  tstring& _Name);
	BOOL ExistGameMultiSprite(const  tstring& _AniSpriteKey);

	SPTR<CGameSprite>	FindMultiSpriteToIndex(const tstring& _Name, size_t _Index);

	CGameTexture* LoadGameTexture(const tstring& _Path);
	SPTR<CGameTexture> ReLoadGameTexture(const tstring& _Path);

	CGameSprite* CreateGameSprite(const tstring& _TexKey, const tstring& _Key);
	CGameSprite*  CreateGameSprite(const tstring& Key);
	CGameSprite* NewGameSprite(const tstring& _TexKey);

	CGameSprite* CreateEditGameSprite(const tstring& _TexKey, const tstring& _ImageKey);
	CGameSprite* CreateEditGameSprite(const tstring& _Key);


	CGameSprite* CreateGameSprite(const tstring& _TexKey, const tstring& _ImageKey, Vec2 _StartPos, Vec2 _Size);
	CGameSprite* CreateGameSprite(const tstring& _Key, Vec2 _StartPos, Vec2 _Size);

	CGameSprite* CreateEditGameSprite(const tstring& _Key, Vec2 _StartPos, Vec2 _Size);
	CGameSprite* CreateEditGameSprite(const tstring& _TexKey, const tstring& _SpriteKey, Vec2 _StartPos, Vec2 _Size);

	BOOL EraseGameSprite(const tstring& _EraseKey);

	CGameMultiSprite* CreateMultiSprite(const tstring& _TextureKey
		, const tstring& _SpriteKey
		, Vec2 _StartPos
		, Vec2 _Size
		, int _ImageCount
		, int _XCount
		, int _YCount
		, Vec2 _Pivot = { 0.0f,0.0f }
		, int _StartFrameIndex = 0
	);

	CGameMultiSprite* CreateMultiSprite(const tstring& _TextureKey
		, const tstring& _SpriteKey
		, Vec2 _StartPos
		, Vec2 _Size
		, int _ImageCount
		, int _XCount
		, int _YCount
		, Vec2 _Pivot 
		, int _StartFrameIndex
		, unordered_map<tstring,SPTR<CGameMultiSprite>>* TempMap
	);
	

	CGameMultiSprite* CreateEditMultiSprite(const tstring& _TextureKey
		, const tstring& _SpriteKey
		, Vec2 _StartPos
		, Vec2 _Size
		, int ImageCount
		, int _XCount
		, int _YCount
		, Vec2 _Pivot = { 0.0f,0.0f }
		, int _StartFrameIndex = 0
	);

	CGameMultiSprite* CreateMultiSprite(MULTISPRITEDATA* _MultiData);
	CGameMultiSprite* CreateMultiSprite(const tstring& _Key, vector<SPTR<CGameSprite>>* _vecGameSprite);


	BOOL EraseMultiSprite(const tstring& _EraseKey);
	

//////////////////////////////// 이미지 데이터를 다른 맵에 저장하는 함수(쓰레드 전용)	///////////////////////////////////////////////
	
	CGameTexture* LoadGameTexture(const tstring& _Path, unordered_map<tstring, SPTR<CGameTexture>>* _LoadMap);
	CGameSprite* CreateGameSprite(CGameTexture* _Tex, const tstring& _Key, unordered_map<tstring, SPTR<CGameSprite>>* _LoadMap);
	void MergeTextureMap(unordered_map<tstring, SPTR<CGameTexture>>* _LoadMap);
	void MergeSpriteMap(unordered_map<tstring, SPTR<CGameSprite>>* _LoadMap);

private:
	static FMOD::System* m_FSystem;

	static unordered_map<tstring, SPTR<CGameSound>>				m_mapSound;
	static unordered_map<tstring, SPTR<CGameSound>>::iterator	m_FindSoundIter;

public:
	static FMOD::System* GetSoundSystem();
	static void SoundInit();
	static BOOL ExistGameSound(const tstring& _Key);
	static SPTR<CGameSound> FindSound(const tstring& _Key);
	static CGameSound* SoundLoad(const tstring& _SoundPath);
	static CGameSound* SoundLoad(const tstring& _SoundPath, unordered_map<tstring, SPTR<CGameSound>>& _TempMap);
	static void MergeSoundMap(unordered_map<tstring, SPTR<CGameSound>>& _TempMap);
	static unordered_map<tstring, SPTR<CGameSound>>* GetSoundMap();
	static void SoundUpdate();


private:
	SPTR<CGameFont>									m_CurFont;
	map<tstring, SPTR<CGameFont>>					m_mapFont;
	map<tstring, SPTR<CGameFont>>::iterator			m_FontIter;

public:
	BOOL SettingFont(const tstring& _FontName);
	SPTR<CGameFont> FindGameFont(const tstring& _FontKey);
	BOOL			ExistGameFont(const tstring& _FontKey);
	SPTR<CGameFont> LoadGameFont(const tstring& _FontName, Vec2 _FontSize);
	SPTR<CGameFont> LoadGameFont(const tstring& _FontName, const tstring& _FontKey, Vec2 _FontSize,const UINT& _Bold = FW_NORMAL);
	
public:
	BOOL DrawFont(const tstring& _OutText,const Vec2& _Pos, float _FontMag = 1.0f, DWORD Color = D3DCOLOR_XRGB(0, 0, 0));
	BOOL DrawFont(const tstring& _FontName, const tstring& _OutText, POINT _LeftStartPos, float _FontMag = 1.0f, DWORD Color = D3DCOLOR_XRGB(0, 0, 0));
	BOOL DrawFont_RightStart(const tstring& _FontName, const tstring& _OutText, const POINT& _RightStartPos, float _FontMag = 1.0f, DWORD Color = D3DCOLOR_XRGB(0, 0, 0));
	BOOL DrawFont(const tstring& _OutText, const Vec3& _Pos, float _FontMag = 1.0f, DWORD Color = D3DCOLOR_XRGB(0, 0, 0));
	BOOL DrawFont(const tstring& _FontName, const tstring& _OutText, const Vec2& _Pos, float _FontMag = 1.0f, DWORD Color = D3DCOLOR_XRGB(0, 0, 0));
	BOOL DrawFont(const tstring& _FontName, const tstring& _OutText, const Vec3& _Pos, float _FontMag = 1.0f, DWORD Color = D3DCOLOR_XRGB(0, 0, 0));

public:
	Vec2 SynchroWndPos(const Vec2& _WorldPos);
	POINT SynchroWndPos(const POINT& _WorldPos);
	unordered_map<tstring, SPTR<CGameSprite>>*		GetSpriteMap();
	unordered_map<tstring,SPTR<CGameMultiSprite>>*  GetAniSpriteMap();


/////////////////////////////////////////		Save MultiSprite Or Animation Data Logic		/////////////////////////////////////////////////////////

public:

	// Save 계열
	BOOL ConversionData(MULTISPRITEDATA* _Data,SPTR<CGameMultiSprite> _AniSprite);
	
	BOOL GetMultiSpriteData(vector<MULTISPRITEDATA>* _SaveBuf,ANISPRITETYPE _Type);

	// Load 계열
	BOOL LoadTextureInFolder(const tstring& _FolderKey);
	BOOL LoadSpriteData(FILE* _LoadFile);
	// Edit.ver의 로드 함수
	// Vector는 로드한 MultiSprite를 BaseAniModel에게 등록하기 위해서
	BOOL LoadSpriteData(FILE* _LoadFile,ANISPRITETYPE _AniType, vector<MULTISPRITEDATA>* _vecMulData);
	BOOL LoadMultiSpriteToData(MULTISPRITEDATA* _Data, ANISPRITETYPE _AniType);
	BOOL LoadMultiSpriteToData(MULTISPRITEDATA* _Data, ANISPRITETYPE _AniType, unordered_map<tstring, SPTR<CGameMultiSprite>>* TempMap);
	void MergeMultiSpriteMap(unordered_map<tstring, SPTR<CGameMultiSprite>>* _TempMap);

public:
	CGameMesh* CreateTriMesh(const Vec2& _TriPoint0, const Vec2& _TriPos1, const Vec2& _TriPos2);
	CGameMesh* CreateTriMesh(VERTEX* _VtxBuf);
	// {0.0f,0.0f,0.0f}와 매개변수의 2개점을 포함하고, 이동할 위치와 , 각 점들과 원점사이의 거리를 크기로 하는 삼각형을 그린다.
	void DrawTriangle(const Vec2& _TriPoint0, const Vec2& _TriPoint1, const Vec2& _TriPoint2, D3DXCOLOR _Color);
	
public:
	CResourceMgr(CGameWindow* _pWindow,LPDIRECT3DDEVICE9 _pDevice);
	~CResourceMgr();
};

