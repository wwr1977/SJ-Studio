#pragma once

/*
 struct SkillData 변경점 
 Skill Level은 5 까지 한정
 SkillLevelData (스킬 레벨에 영향을 받는 데이터는 따로 저장한다)
 SkillLevelData는 다른 파일 & 다른 맵에서 별도로 저장한다(10월22일)
 ActionScript에 있던 InitPos 관련 변수들을 옮김 (11월 12일)
 */
struct VERTEX 
{
	static const DWORD FVF() 
	{
		return  D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;
	}

	Vec3		vPos;
	Vec3		vNormal;
	Vec2		vTexCoord;
	
	void operator=(const VERTEX& _Other) 
	{
		vPos= _Other.vPos;
		vNormal = _Other.vNormal;
		vTexCoord = _Other.vTexCoord;
	}

	void operator+=(const VERTEX& _Other)
	{
		vPos += _Other.vPos;
		vTexCoord += _Other.vTexCoord;
	}

	void operator-=(const VERTEX& _Other)
	{
		vPos -= _Other.vPos;
		vTexCoord -= _Other.vTexCoord;
	}

	VERTEX operator+(const VERTEX& _Other) 
	{
		VERTEX Temp;
		Temp.vPos = vPos + _Other.vPos;
		Temp.vTexCoord = vTexCoord + _Other.vTexCoord;
		return Temp;
	}
	VERTEX operator-(const VERTEX& _Other)
	{
		VERTEX Temp;
		Temp.vPos = vPos - _Other.vPos;
		Temp.vTexCoord = vTexCoord - _Other.vTexCoord;
		return Temp;
	}
	VERTEX operator*(const float& _Scala) 
	{
		VERTEX Temp;
		Temp.vPos = vPos*_Scala;
		Temp.vTexCoord = vTexCoord*_Scala;
		return Temp;
	}


	VERTEX() 
		:vPos(0.f,0.f,0.f), vNormal({ 0,0,-1 }), vTexCoord({0.f,0.f})
	{}
	VERTEX(const VERTEX& _Other)
		:vPos(_Other.vPos), vNormal(_Other.vNormal), vTexCoord(_Other.vTexCoord)
	{
	
	}
	VERTEX(const Vec3& _vPos,const Vec2& _vTex) : vPos(_vPos), vNormal({ 0,0,-1 }), vTexCoord(_vTex) {}
	VERTEX(const Vec3& _vPos) : vPos(_vPos), vNormal({ 0,0,-1 }), vTexCoord({ 0,0 }) {}
};

struct VTXLINE
{
	static const DWORD FVF()
	{
		return  D3DFVF_XYZ | D3DFVF_DIFFUSE;
	}

	Vec3 vPos; // 포스고
	DWORD Color;

	VTXLINE() : Color(D3DCOLOR_XRGB(255, 0, 0))
	{
		memset(vPos, 0, sizeof(Vec3));
	}
	VTXLINE(Vec3 _vPos) : vPos(_vPos), Color(D3DCOLOR_XRGB(255, 0, 0)) {}
	VTXLINE(Vec3 _vPos,DWORD _Color) : vPos(_vPos), Color(_Color) {}
};


struct VTXIDX16
{
	static const D3DFORMAT FMT()
	{
		return D3DFMT_INDEX16;
	}

	WORD _0; 
	WORD _1; 
	WORD _2; 

	VTXIDX16() : _0(0), _1(0), _2(0) {}

	VTXIDX16(int _f, int _s, int _t) :_0(_f), _1(_s), _2(_t) {}
};

struct VTXIDX32
{
	static const D3DFORMAT FMT()
	{
		return D3DFMT_INDEX32;
	}


	DWORD _0; 
	DWORD _1; 
	DWORD _2; 
};

struct TRI_VERTEX 
{
	struct VERTEX			TriVertex[3];

public:
	void operator=(TRI_VERTEX& _Other)
	{
		for (size_t i = 0; i < 3; i++)
			TriVertex[i] = _Other.TriVertex[i];
	}

public:
	TRI_VERTEX()
	{	
		for (size_t i = 0; i < 3; i++)
			TriVertex[i] = VERTEX();
	}
	TRI_VERTEX(VERTEX* _Buf)
	{
		for (size_t i = 0; i < 3; i++)
			TriVertex[i] = VERTEX();

		if (nullptr == _Buf)
			return;

		memcpy_s(_Buf, sizeof(*_Buf), TriVertex, sizeof(*TriVertex));
	}
	TRI_VERTEX(const TRI_VERTEX& _Other)
	{
		for (size_t i = 0; i < 3; i++)
			TriVertex[i] = _Other.TriVertex[i];
	}
	~TRI_VERTEX() {}
};

struct COLORFACTOR 
{
	int Red;
	int Green;
	int Blue;

	COLORFACTOR() :Red(0), Green(0), Blue(0) {};
	COLORFACTOR(const int& _Red, const int& _Green, const int& _Blue) :Red(_Red), Green(_Green), Blue(_Blue) {};
	~COLORFACTOR() {}
};


struct MULTISPRITEDATA
{
	TCHAR						TexKey[MAXKEYLENGTH];
	TCHAR						MultiSpriteKey[MAXKEYLENGTH];
	float 						StartPosX;
	float						StartPosY;
	float						SizeX;
	float						SizeY;
	int							StartFrame;
	int							ImageCount;
	int							XCount;
	int							YCount;
	float						PivotX;
	float						PivotY;

public:
	static BOOL ReadMultiSpriteData(FILE* _ReadFile, std::map<std::tstring, MULTISPRITEDATA*>* _mapMultiSpriteData);


	MULTISPRITEDATA() :
		TexKey(_T("")), MultiSpriteKey(_T(""))
		, StartPosX(0.0f),StartPosY(0.0f)
		, SizeX(0.0f), SizeY(0.0f)
		, PivotX(0.0f),PivotY(0.0f)
		, ImageCount(0), XCount(0), YCount(0)
		, StartFrame(0)
	{}
	MULTISPRITEDATA(TCHAR*  _TexKey
		, TCHAR* _MultiSpriteKey
		, const float& _StartPosX, const float& _StartPosY
		, const float& _SizeX, const float& _SizeY
		, const int& _ImageCount
		, const int& _XCount
		, const int& _YCount
		, const float& _PivotX, const float& _PivotY
		,const int& _StartFrame
	)
		: StartPosX(_StartPosX), StartPosY(_StartPosY)
		, SizeX(_SizeX), SizeY(_SizeY)
		, PivotX(_PivotX), PivotY(_PivotY)
		, ImageCount(_ImageCount), XCount(_XCount), YCount(_YCount)
		, StartFrame(0)
	{
		_tcscpy_s(TexKey, _countof(TexKey), _TexKey);
		_tcscpy_s(MultiSpriteKey, _countof(MultiSpriteKey), _MultiSpriteKey);
	}
	~MULTISPRITEDATA() {}
};

typedef struct ANIRANGE
{
	int Start;
	int End;

	ANIRANGE():Start(0), End(0){}
	ANIRANGE(int _Start,int _End)
		:Start(_Start), End(_End)
	{
		if (0 > Start) 
		{
			Start = 0;
		}
		if (0 > End) 
		{
			End = 0;
		}
		if (_Start > _End) 
		{
			Start = _End;
			End = _Start;
		}
	}

}AniRange;

// 애니메이션을 동작하기 위한 데이터
struct ANIMATIONDATA;
struct ANIDATA
{
public:
	// SetData시 잘못된 인자값이 입력되었을때 이 데이터가 실패값임을 알리는 변수
	bool					InvalidData;
	std::tstring			AniSpriteName;
	std::tstring			AnimationName;

public:

	AniRange		FullRange;		// 전체 애니메이션의 범위
	AniRange		RepeatRange;	// 지정된 반복수를 넘긴후의 애니메이션 범위

	int		AnimationCount;			//	몇번 전체 반복을 할 것 인가(기본 1)

	// Edit에서 정보 확인용
	float   DefaultInterval;
	float	DefaultRepeatInterval;

	float*	Interval;
	float*	RepeatInterval;
	bool	Loop;

public:
	BOOL SettingFullRage(ANIRANGE _FullRange, const float& _IntervalTime = 0.1f)
	{
		FullRange = _FullRange;
		int AniSize = FullRange.End - FullRange.Start + 1;
		Interval = new float[AniSize];
		if (nullptr == Interval)
		{
			return FALSE;
		}
		std::fill_n(Interval, AniSize, _IntervalTime);
		DefaultInterval = _IntervalTime;
		return TRUE;
	}
	BOOL SettingRepeatRage(ANIRANGE _RepeatRange, const float& _IntervalTime = 0.135f)
	{
		RepeatRange = _RepeatRange;
		int ReAniSize = _RepeatRange.End - _RepeatRange.Start + 1;
		RepeatInterval = new float[ReAniSize];
		if (nullptr == Interval)
		{
			return FALSE;
		}
		std::fill_n(RepeatInterval, ReAniSize, _IntervalTime);
		Loop = false;
		DefaultRepeatInterval = _IntervalTime;
		return TRUE;
	}

public:
	void operator=(const ANIDATA& _AniData) 
	{
		AniSpriteName = _AniData.AniSpriteName;
		AnimationName = _AniData.AnimationName;
		SettingFullRage(_AniData.FullRange, _AniData.DefaultInterval);
		if (true == _AniData.Loop) 
		{
			Loop = true;
			return;
		}
		SettingRepeatRage(_AniData.RepeatRange, _AniData.DefaultRepeatInterval);
		AnimationCount = _AniData.AnimationCount;
		DefaultInterval = _AniData.DefaultInterval;
		DefaultRepeatInterval = _AniData.DefaultRepeatInterval;
		Loop = false;

	}

public:
	ANIDATA()
		:InvalidData(true)
		, AniSpriteName(_T("")), AnimationName(_T(""))
		, Interval(nullptr), RepeatInterval(nullptr)
		, AnimationCount(1), Loop(true)
		,DefaultInterval(0.0f),DefaultRepeatInterval(0.0f)
	{

	}

	ANIDATA(const std::tstring& _AniSpriteName)
		:InvalidData(false)
		, AniSpriteName(_AniSpriteName), AnimationName(_AniSpriteName)
		, Interval(nullptr), RepeatInterval(nullptr)
		, AnimationCount(1), Loop(true)
		, DefaultInterval(0.0f), DefaultRepeatInterval(0.0f)
	{

	}


	ANIDATA(const std::tstring& _AniSpriteName ,const std::tstring& _AnimationName)
		:InvalidData(false)
		, AniSpriteName(_AniSpriteName), AnimationName(_AnimationName)
		, Interval(nullptr), RepeatInterval(nullptr)
		, AnimationCount(1), Loop(true)
		, DefaultInterval(0.0f), DefaultRepeatInterval(0.0f)
	{

	}

	ANIDATA(ANIRANGE _FullRange,bool _Loop)
		:AniSpriteName(_T("")), AnimationName(_T(""))
		, FullRange(_FullRange) , Loop(_Loop)
		, AnimationCount(1), RepeatRange({0,0}), InvalidData(false)
		, DefaultInterval(0.0f), DefaultRepeatInterval(0.0f)
		, Interval(nullptr), RepeatInterval(nullptr)
	{
		RepeatRange = { _FullRange.End,_FullRange.End };
		int AniSize = FullRange.End - FullRange.Start + 1;
		int ReAniSize = 1;

		Interval = new float[AniSize];
		RepeatInterval = new float[ReAniSize];

		std::fill_n(Interval, AniSize, 0.1f);
		DefaultInterval = 0.1f;
		std::fill_n(RepeatInterval, ReAniSize, 0.135f);
		DefaultRepeatInterval = 0.135f;
	}

	ANIDATA(ANIRANGE _FullRange, int _AniCount, ANIRANGE _RepeatRange)
		:AniSpriteName(_T("")), AnimationName(_T(""))
		, FullRange(_FullRange), Loop(false), AnimationCount(_AniCount)
		, RepeatRange(_RepeatRange), InvalidData(false)
		, DefaultInterval(0.0f), DefaultRepeatInterval(0.0f)
		, Interval(nullptr), RepeatInterval(nullptr)
	{
		int AniSize = _FullRange.End - _FullRange.Start + 1;
		int ReAniSize = _RepeatRange.End - _RepeatRange.Start + 1;

		Interval = new float[AniSize];
		RepeatInterval = new float[ReAniSize];

		std::fill_n(Interval, AniSize, 0.1f);
		DefaultInterval = 0.1f;
		std::fill_n(RepeatInterval, ReAniSize, 0.135f);
		DefaultRepeatInterval = 0.135f;
	}
	ANIDATA(ANIRANGE _FullRange, int _AniCount, ANIRANGE _RepeatRange, float _Interval, float _RepeatInterval = 0.125f)
		:AniSpriteName(_T("")), AnimationName(_T(""))
		, FullRange(_FullRange), Loop(false), AnimationCount(_AniCount)
		, RepeatRange(_RepeatRange), InvalidData(false)
		, DefaultInterval(0.0f), DefaultRepeatInterval(0.0f)
		, Interval(nullptr), RepeatInterval(nullptr)
	{
		int AniSize = _FullRange.End - _FullRange.Start + 1;
		int ReAniSize = _RepeatRange.End - _RepeatRange.Start + 1;

		Interval = new float[AniSize];
		RepeatInterval = new float[ReAniSize];

		std::fill_n(Interval, AniSize, _Interval);
		DefaultInterval = _Interval;
		std::fill_n(RepeatInterval, ReAniSize, _RepeatInterval);
		DefaultRepeatInterval = _RepeatInterval;
	}
	ANIDATA(const std::tstring& _AniSpriteName ,const std::tstring& _AnimationName,ANIRANGE _FullRange, int _AniCount, ANIRANGE _RepeatRange, float _Interval, float _RepeatInterval = 0.125f)
		:AniSpriteName(_AniSpriteName), AnimationName(_AnimationName)
		, FullRange(_FullRange), Loop(false), AnimationCount(_AniCount)
		, RepeatRange(_RepeatRange), InvalidData(false)
		, DefaultInterval(0.0f), DefaultRepeatInterval(0.0f)
		, Interval(nullptr), RepeatInterval(nullptr)
	{
		int AniSize = _FullRange.End - _FullRange.Start + 1;
		int ReAniSize = _RepeatRange.End - _RepeatRange.Start + 1;

		Interval = new float[AniSize];
		RepeatInterval = new float[ReAniSize];

		std::fill_n(Interval, AniSize, _Interval);
		DefaultInterval = _Interval;
		std::fill_n(RepeatInterval, ReAniSize, _RepeatInterval);
		DefaultRepeatInterval = _RepeatInterval;
	}
	ANIDATA(const ANIDATA& _AniData) 
		:InvalidData(false)
		, AniSpriteName(_T("")), AnimationName(_T(""))
		, Interval(nullptr), RepeatInterval(nullptr)
		, AnimationCount(1), Loop(true)
		, DefaultInterval(0.0f), DefaultRepeatInterval(0.0f)
	{
		AniSpriteName = _AniData.AniSpriteName;
		AnimationName = _AniData.AnimationName;
		SettingFullRage(_AniData.FullRange, _AniData.DefaultInterval);
		if (true == _AniData.Loop)
		{
			Loop = true;
			return;
		}
		SettingRepeatRage(_AniData.RepeatRange, _AniData.DefaultRepeatInterval);
		AnimationCount = _AniData.AnimationCount;
		DefaultInterval = _AniData.DefaultInterval;
		DefaultRepeatInterval = _AniData.DefaultRepeatInterval;
		Loop = false;

	}
	ANIDATA(const ANIMATIONDATA& _AnimationData);
	
	~ANIDATA() 
	{
		SAFE_DELETE_ARR(Interval);
		SAFE_DELETE_ARR(RepeatInterval);
	}
};

//애니메이션을 파일로 저장하기 위한 데이터 
struct ANIMATIONDATA 
{
	TCHAR					MultiSpriteKey[MAXKEYLENGTH];
	TCHAR					AniKey[MAXKEYLENGTH];
	TCHAR					AniIntervalData[MAXSTRING];
	int						FullRangeStart;
	int						FullRangeEnd;
	int						RepeatRangeStart;
	int						RepeatRangeEnd;
	int						AnimationCount;
	float					BaseFullInterval;
	float					BaseRepeatInterval;
	int						AniStyle;

public:
	size_t WriteData(FILE* _WriteFile);

	//Client 전용 애니메이션 로드 함수
	static BOOL ReadAnimationData(FILE* _ReadFile, std::map<std::tstring, ANIMATIONDATA*>* _mapAnimationData);
	//Editor 전용 애니메인션 로드 함수
	static BOOL ReadAnimationData(FILE* _ReadFile, std::map<std::tstring, ANIMATIONDATA>* _mapAnimationData);
	
	ANIDATA GetAniData() 
	{
		ANIDATA AniData;
		AniData.AniSpriteName = MultiSpriteKey;
		AniData.AnimationName = AniKey;

		AniData.SettingFullRage({FullRangeStart,FullRangeEnd }, BaseFullInterval);
		AniData.DefaultInterval = BaseFullInterval;
		if ((ANISTYLE)ANI_LOOP == AniStyle)
		{
			AniData.Loop = true;
			return AniData;
		}
		AniData.Loop = false;
		AniData.SettingRepeatRage({ RepeatRangeStart,RepeatRangeEnd }, BaseRepeatInterval);
		AniData.AnimationCount = AnimationCount;
		AniData.DefaultRepeatInterval = BaseRepeatInterval;

		return AniData;
	}
	
	void operator=(const ANIMATIONDATA& _Other) 
	{
		_tcscpy_s(MultiSpriteKey, _countof(MultiSpriteKey), _Other.MultiSpriteKey);
		_tcscpy_s(AniKey, _countof(AniKey), _Other.AniKey);
		_tcscpy_s(AniIntervalData, _countof(AniIntervalData), _Other.AniIntervalData);

		FullRangeStart = _Other.FullRangeStart;
		FullRangeEnd = _Other.FullRangeEnd;
		BaseFullInterval = _Other.BaseFullInterval;
		RepeatRangeStart = _Other.RepeatRangeStart;
		RepeatRangeEnd = _Other.RepeatRangeEnd;
		BaseRepeatInterval = _Other.BaseRepeatInterval;
		AnimationCount = _Other.AnimationCount;
		AniStyle = _Other.AniStyle;
	}

public:
	ANIMATIONDATA()
		:MultiSpriteKey(_T("")), AniKey(_T("")), AniIntervalData(_T(""))
		, FullRangeStart(0), FullRangeEnd(0), RepeatRangeStart(0), RepeatRangeEnd(0)
		, AnimationCount(1), BaseFullInterval(0.1f), BaseRepeatInterval(0.12f)
		, AniStyle(MAXANISTYLE)
	{}
	ANIMATIONDATA(ANIDATA _AniData) 
		:MultiSpriteKey(_T("")), AniKey(_T("")), AniIntervalData(_T(""))
		, FullRangeStart(0), FullRangeEnd(0), RepeatRangeStart(0), RepeatRangeEnd(0)
		, AnimationCount(1), BaseFullInterval(0.1f), BaseRepeatInterval(0.12f)
		, AniStyle(MAXANISTYLE)
	{
		_tcscpy_s(MultiSpriteKey, _countof(MultiSpriteKey), _AniData.AniSpriteName.c_str());
		_tcscpy_s(AniKey, _countof(AniKey), _AniData.AnimationName.c_str());
		FullRangeStart = _AniData.FullRange.Start;
		FullRangeEnd = _AniData.FullRange.End;
		BaseFullInterval = _AniData.DefaultInterval;
		RepeatRangeStart = _AniData.RepeatRange.Start;
		RepeatRangeEnd = _AniData.RepeatRange.End;
		BaseRepeatInterval = _AniData.DefaultRepeatInterval;
		AnimationCount = _AniData.AnimationCount;
	}
	ANIMATIONDATA(const ANIMATIONDATA& _Other)
		:MultiSpriteKey(_T("")), AniKey(_T("")), AniIntervalData(_T(""))
		, FullRangeStart(0), FullRangeEnd(0), RepeatRangeStart(0), RepeatRangeEnd(0)
		, AnimationCount(1), BaseFullInterval(0.1f), BaseRepeatInterval(0.12f)
		, AniStyle(MAXANISTYLE)
	{
		_tcscpy_s(MultiSpriteKey, _countof(MultiSpriteKey), _Other.MultiSpriteKey);
		_tcscpy_s(AniKey, _countof(AniKey), _Other.AniKey);
		_tcscpy_s(AniIntervalData, _countof(AniIntervalData), _Other.AniIntervalData);
		
		FullRangeStart = _Other.FullRangeStart;
		FullRangeEnd = _Other.FullRangeEnd;
		BaseFullInterval = _Other.BaseFullInterval;
		RepeatRangeStart = _Other.RepeatRangeStart;
		RepeatRangeEnd = _Other.RepeatRangeEnd;
		BaseRepeatInterval = _Other.BaseRepeatInterval;
		AnimationCount = _Other.AnimationCount;
		AniStyle = _Other.AniStyle;
	}
};


struct SOUNDSAVEDATA;
struct SoundData
{
	std::tstring		SoundName;
	float				StartTime;
	bool				Playing;

	void operator=(const SoundData& _Other)
	{
		SoundName = _Other.SoundName;
		StartTime = _Other.StartTime;
		Playing = _Other.Playing;
	}

	SoundData() :SoundName(_T("")), StartTime(0.0f), Playing(false) {}
	SoundData(const std::tstring& _SoundName, const float& _StartTime)
		:SoundName(_SoundName), StartTime(_StartTime), Playing(false)
	{

	}
	SoundData(const SoundData& _Other)
		:SoundName(_Other.SoundName), StartTime(_Other.StartTime), Playing(_Other.Playing)
	{

	}

	SoundData(const SOUNDSAVEDATA& _SaveData);

	~SoundData() {}
};


struct SOUNDSAVEDATA 
{
	TCHAR			SoundName[SOUNDNAMELENGTH];
	float			PlayStartTime;

	void SetSoundName(const std::tstring& _SoundName);

	void operator=(const SOUNDSAVEDATA& _Other) 
	{
		_tcscpy_s(SoundName, _countof(SoundName), _Other.SoundName);
		PlayStartTime = _Other.PlayStartTime;
	}

	SOUNDSAVEDATA() 
		:SoundName(_T("")), PlayStartTime(0.0f)
	{}
	SOUNDSAVEDATA(const std::tstring& _SoundName, const float& _PlayTime) 
		:SoundName(_T("")), PlayStartTime(_PlayTime)
	{
		_tcscpy_s(SoundName, _countof(SoundName), _SoundName.c_str());
	}
	SOUNDSAVEDATA(const SOUNDSAVEDATA& _Other)
		:SoundName(_T("")), PlayStartTime(_Other.PlayStartTime)
	{
		_tcscpy_s(SoundName, _countof(SoundName), _Other.SoundName);
	}
};

struct EFFECTDATA
{
	enum 
	{
		MAXSOUNDCOUNT = 5
	};
	TCHAR					EffectName[MAXKEYLENGTH];
	TCHAR					EffectAniName[MAXKEYLENGTH];
	TCHAR					SoundName[MAXSOUNDCOUNT][MAXKEYLENGTH];
	float					SoundStartTime[MAXSOUNDCOUNT];
	int						SoundCount;
	float					LifeTime;

	float					PlayRender;
	
	float					ImageScale;
	Vec2					CustomSize;

	int						RenderLayer;
	
	bool					Loop;
	bool					BlackBack;
	bool					NonCameraAffect;
	unsigned int			EffectIndex;

public:
	BOOL SaveDataToFile(FILE* _WriteFile);
	static BOOL LoadDataToFile(FILE* _ReadFile, std::vector<EFFECTDATA>* _Vec);
	BOOL AddSoundData(const std::tstring& _SoundName, const float& _SoundStartTime);
	BOOL DeleteSoundData(const int& _Index);

public:
	

	void operator=(const EFFECTDATA& _Other) 
	{
		memset(SoundName, 0, sizeof(TCHAR)*MAXKEYLENGTH*MAXSOUNDCOUNT);
		memset(SoundStartTime, 0, sizeof(float)*MAXSOUNDCOUNT);

		_tcscpy_s(EffectName, _countof(EffectName), _Other.EffectName);
		_tcscpy_s(EffectAniName, _countof(EffectAniName),_Other.EffectAniName);

		for (int i = 0; i < _Other.SoundCount; i++)
		{
			AddSoundData(_Other.SoundName[i], _Other.SoundStartTime[i]);
		}

		EffectIndex = _Other.EffectIndex;
		SoundCount = _Other.SoundCount;
		LifeTime = _Other.LifeTime;
		PlayRender = _Other.PlayRender;
		ImageScale = _Other.ImageScale;
		CustomSize = _Other.CustomSize;
		RenderLayer = _Other.RenderLayer;
		Loop = _Other.Loop;
		BlackBack = _Other.BlackBack;
		NonCameraAffect = _Other.NonCameraAffect;
		EffectIndex = _Other.EffectIndex;
	}

public:
	EFFECTDATA() :EffectName(_T("")), EffectAniName(_T("")), LifeTime(0.0f), PlayRender(0.0f), ImageScale(1.0f), CustomSize({ 0.0f,0.0f })
		, BlackBack(false), NonCameraAffect(false), RenderLayer(0) , Loop(false)
		, SoundCount(0), EffectIndex(-1)
	{
		memset(SoundName, 0, sizeof(TCHAR)*MAXKEYLENGTH*MAXSOUNDCOUNT);
		memset(SoundStartTime, 0, sizeof(float)*MAXSOUNDCOUNT);

	
	}
	EFFECTDATA(const std::tstring& _EffAnimationName
		, const float& _fLifeTime
		, const float& _fPlayRender)
		:EffectName(_T("")), EffectAniName(_T("")), LifeTime(_fLifeTime)
		, PlayRender(_fPlayRender), Loop(false)
		, SoundCount(0), EffectIndex(-1)
	{
		memset(SoundName, 0, sizeof(TCHAR)*MAXKEYLENGTH*MAXSOUNDCOUNT);
		memset(SoundStartTime, 0, sizeof(float)* MAXSOUNDCOUNT);

	
	}

	EFFECTDATA(const std::tstring& _EffAnimationName
		, const float& _fLifeTime
		, const float& _fPlayRender
		, const float& _ImageScale
	)
		:EffectName(_T("")), EffectAniName(_T("")), LifeTime(_fLifeTime)
		, PlayRender(_fPlayRender), ImageScale(_ImageScale)
		, CustomSize({ 0.0f,0.0f }), Loop(false)
		, SoundCount(0), EffectIndex(-1)
	{
		_tcscpy_s(EffectName, _countof(EffectName), _EffAnimationName.c_str());
		memset(SoundName, 0, sizeof(TCHAR)*MAXKEYLENGTH*MAXSOUNDCOUNT);
		memset(SoundStartTime, 0, sizeof(float)*MAXSOUNDCOUNT);

	
	}
	EFFECTDATA(const std::tstring& _EffAnimationName
		, const float& _fLifeTime
		, const float& _fPlayRender
		, const Vec2& _CustomSize
	)
		:EffectName(_T("")), LifeTime(_fLifeTime)
		, PlayRender(_fPlayRender), ImageScale(1.0f)
		, CustomSize(_CustomSize), Loop(false)
		, SoundCount(0), EffectIndex(-1)
	{
		_tcscpy_s(EffectName, _countof(EffectName), _EffAnimationName.c_str());
		
		memset(SoundName, 0, sizeof(TCHAR)*MAXKEYLENGTH*MAXSOUNDCOUNT);
		memset(SoundStartTime, 0, sizeof(float)*MAXSOUNDCOUNT);
	}
	EFFECTDATA(const std::tstring& _EffectName
		, const std::tstring& _EffectAniName
	) 
		:EffectName(_T("")),EffectAniName(_T("")), SoundCount(0), LifeTime(0.0f)
		, PlayRender(0.0f), ImageScale(0.0f), CustomSize(Vec2{0.0f,0.0f})
		, RenderLayer(LAYER_BACKEFFECT)
		, Loop(false), BlackBack(false), NonCameraAffect(false)
		, EffectIndex(-1)
	{
		_tcscpy_s(EffectName, _countof(EffectName), _EffectName.c_str());
		_tcscpy_s(EffectAniName, _countof(EffectAniName), _EffectAniName.c_str());

		memset(SoundName, 0, sizeof(TCHAR)*MAXKEYLENGTH*MAXSOUNDCOUNT);
		memset(SoundStartTime, 0, sizeof(float)* MAXSOUNDCOUNT);
	}
	EFFECTDATA(const std::tstring& _EffectName
		, const std::tstring& _EffectAniName
		, const size_t& _EffectIndex
	)
		:EffectName(_T("")), EffectAniName(_T("")), SoundCount(0), LifeTime(0.0f)
		, PlayRender(0.0f), ImageScale(0.0f), CustomSize(Vec2{ 0.0f,0.0f })
		, RenderLayer(LAYER_BACKEFFECT)
		, Loop(false), BlackBack(false), NonCameraAffect(false)
		, EffectIndex(0)
	{
		EffectIndex = (UINT)_EffectIndex;
		_tcscpy_s(EffectName, _countof(EffectName), _EffectName.c_str());
		_tcscpy_s(EffectAniName, _countof(EffectAniName), _EffectAniName.c_str());

		memset(SoundName, 0, sizeof(TCHAR)*MAXKEYLENGTH*MAXSOUNDCOUNT);
		memset(SoundStartTime, 0, sizeof(float)* MAXSOUNDCOUNT);
	}
	EFFECTDATA(const EFFECTDATA& _Other)
		:EffectName(_T("")), EffectAniName(_T("")), SoundCount(), LifeTime(0.0f)
		, PlayRender(0.0f), ImageScale(0.0f), CustomSize(Vec2{ 0.0f,0.0f })
		, RenderLayer(LAYER_BACKEFFECT)
		, Loop(false), BlackBack(false), NonCameraAffect(false)
		, EffectIndex(-1)
	{
		memset(SoundName, 0, sizeof(TCHAR)*MAXKEYLENGTH*MAXSOUNDCOUNT);
		memset(SoundStartTime, 0, sizeof(float)*MAXSOUNDCOUNT);

		_tcscpy_s(EffectName, _countof(EffectName), _Other.EffectName);
		_tcscpy_s(EffectAniName, _countof(EffectAniName), _Other.EffectAniName);

		for (int i = 0; i < _Other.SoundCount; i++)
		{
			AddSoundData(_Other.SoundName[i], _Other.SoundStartTime[i]);
		}

		SoundCount = _Other.SoundCount;
		LifeTime = _Other.LifeTime;
		PlayRender = _Other.PlayRender;
		ImageScale = _Other.ImageScale;
		CustomSize = _Other.CustomSize;
		RenderLayer = _Other.RenderLayer;
		Loop = _Other.Loop;
		BlackBack = _Other.BlackBack;
		NonCameraAffect = _Other.NonCameraAffect;
		EffectIndex = _Other.EffectIndex;
	}
	~EFFECTDATA() {}
};


struct RENDATA 
{
public:
	int					Index;
	bool				bAlpha;
	bool				bUI;
	PROJECTIONMODE		Projection;
	float				AlphaFactor;
	Vec3				BackColor;
	

	
public:
	RENDATA(int _Index) :Index(_Index), bAlpha(true), AlphaFactor(1.0f), BackColor({-1.0f,-1.0f,-1.0f }) ,bUI(false), Projection(ORTHOGONAL){}
	RENDATA(int _Index,float _AlphaFactor) :Index(_Index), bAlpha(true), AlphaFactor(_AlphaFactor), BackColor({ -1.0f,-1.0f,-1.0f }), bUI(false), Projection(ORTHOGONAL) {}
	RENDATA(int _Index, float _AlphaFactor,bool _UI) :Index(_Index), bAlpha(true), AlphaFactor(_AlphaFactor), BackColor({ -1.0f,-1.0f,-1.0f }) , bUI(_UI), Projection(ORTHOGONAL) {}
	RENDATA(int _Index, float _AlphaFactor, PROJECTIONMODE _Projection) :Index(_Index), bAlpha(true), AlphaFactor(_AlphaFactor), BackColor({ -1.0f,-1.0f,-1.0f }), bUI(false),Projection(_Projection) {}
	RENDATA(int _Index, float _AlphaFactor, const Vec3& _BackColor, PROJECTIONMODE _Projection) :Index(_Index), bAlpha(true), AlphaFactor(_AlphaFactor), BackColor(_BackColor), bUI(false), Projection(_Projection) {}
	RENDATA(int _Index, float _AlphaFactor, const Vec3& _BackColor,  bool _UI) :Index(_Index), bAlpha(true), AlphaFactor(_AlphaFactor), BackColor(_BackColor), bUI(_UI), Projection(ORTHOGONAL) {}
	RENDATA(int _Index, bool _bAlpha) :Index(_Index), bAlpha(_bAlpha), AlphaFactor(1.0f), BackColor({ -1.0f,-1.0f,-1.0f }), bUI(false), Projection(ORTHOGONAL) {}
	RENDATA(int _Index, float _AlphaFactor,const Vec3& _BackColor) :Index(_Index), bAlpha(true), AlphaFactor(_AlphaFactor), BackColor(_BackColor), bUI(false), Projection(ORTHOGONAL) {}

};


typedef struct COLFIGURE
{
	Vec3			Pos;
	Vec3			Size;

public:

	const float HSizeX() const
	{
		return Size.x*0.5f;
	}
	const float HSizeY() const
	{
		return Size.y*0.5f;
	}

	const float Left() const
	{
		return Pos.x - HSizeX();
	}

	const float Right() const
	{
		return Pos.x + HSizeX();
	}

	const float Top() const
	{
		return Pos.y + HSizeY();
	}

	const float Bottom() const
	{
		return Pos.y - HSizeY();
	}

	const float Radius() const
	{
		return Size.x * 0.5f;
	}

	//원의 지름
	const float Diameter() const
	{
		return Size.x;
	}

	Vec2 LT() const 
	{
		return { Left(),Top() };
	}

	Vec2 LB() const
	{
		return { Left(),Bottom() };
	}

	Vec2 RT() const
	{
		return { Right(),Top() };
	}
	
	Vec2 RB() const
	{
		return { Right(),Bottom() };
	}

	void operator=(const COLFIGURE& _Other) 
	{
		Pos = _Other.Pos;
		Size = _Other.Size;
	}

	COLFIGURE() :Pos({0.0f,0.0f,0.0f}), Size({ 1.0f,1.0f,1.0f}){}
	COLFIGURE(Vec3 _Pos) :Pos(_Pos), Size({ 1.0f,1.0f,1.0f }) {}
	COLFIGURE(Vec2 _Pos) :Pos({_Pos.x,_Pos.y,0.0f}), Size({ 1.0f,1.0f,1.0f }) {}
	COLFIGURE(Vec3 _Pos, Vec3 _Size) :Pos(_Pos), Size(_Size) {}
	COLFIGURE(Vec3 _Pos, Vec2 _Size) :Pos(_Pos), Size({ _Size.x,_Size.y,1.0f }) {}
	COLFIGURE(Vec2 _Pos, Vec2 _Size) :Pos({ _Pos.x,_Pos.y,0.0f }), Size({_Size.x,_Size.y,1.0f}) {}
	COLFIGURE(Vec3 _Pos, float _Radius) :Pos(_Pos), Size({_Radius*2.0f,_Radius * 2.0f ,_Radius * 2.0f }) {}
	COLFIGURE(Vec2 _Pos, float _Radius) :Pos({_Pos.x,_Pos.y,0.0f}), Size({ _Radius*2.0f,_Radius * 2.0f ,_Radius * 2.0f }) {}
	COLFIGURE(const COLFIGURE& _Other) 
		:Pos(_Other.Pos), Size(_Other.Size)
	{
	}
	~COLFIGURE() {}
}COLCIRCLE, COLRECT, COLPOINT;


struct COLDATA
{
	std::tstring		GroupName;
	COLFIGURE			Figure;
	COLTYPE				ColType;
	COLMODE				ColMode;

public:
	COLDATA() :GroupName(_T("")), Figure(COLFIGURE()), ColType(CT_RECT), ColMode(CM_ACTOR) {}
	COLDATA(const std::tstring& _GroupName) :GroupName(_GroupName), Figure(COLFIGURE()), ColType(CT_RECT), ColMode(CM_ACTOR) {}
	~COLDATA() {}
};


struct STATPARAMETERDATA
{
	TCHAR				UnitName[MAXKEYLENGTH];
	MAINSTAT			MainStat;
	float				StrPara;
	float				DexPara;
	float				IntPara;
	float				LuckPara;
	float				DamagePara;
	float				DefPara;
	float				HpPara;
	float				MpPara;

	// Editor 창에 계산없이 Init하기 위해
	// 크리티컬 확률(%)
	int					Critical;
	// 크리티컬 데미지 비율(%)
	int					CriticalDamage;
	// 회피율(%)
	int					Avoidability;
	// 데미지 간격(+-%)
	int					DamageRange;

	BOOL SaveDataToFile(FILE* _WriteFile);
	BOOL LoadDataToFile(FILE* _ReadFile, std::map<std::tstring, STATPARAMETERDATA>* _Table);


	void operator=(const STATPARAMETERDATA& _Other)
	{
		_tcscpy_s(UnitName, _countof(UnitName), _Other.UnitName);
		MainStat = _Other.MainStat;
		StrPara = _Other.StrPara;
		DexPara = _Other.DexPara;
		IntPara = _Other.IntPara;
		LuckPara = _Other.LuckPara;
		DamagePara = _Other.DamagePara;
		DefPara = _Other.DefPara;
		HpPara = _Other.HpPara;
		MpPara = _Other.MpPara;
		Critical = _Other.Critical;
		CriticalDamage = _Other.CriticalDamage;
		Avoidability = _Other.Avoidability;
		DamageRange = _Other.DamageRange;

	}

	STATPARAMETERDATA()
		:UnitName(_T("")), MainStat(MAXMAINSTAT)
		, StrPara(0.0f), DexPara(0.0f), IntPara(0.0f), LuckPara(0.0f)
		, DamagePara(0.0f), DefPara(0.0f), HpPara(0.0f), MpPara(0.0f)
		, Critical(0), CriticalDamage(0), Avoidability(0), DamageRange(0)
	{}
	STATPARAMETERDATA(const std::tstring& _UnitName, const MAINSTAT& _MainStat
		, float _StrPara, float _DexPara, float _IntPara, float _LuckPara
		, float _DamagePara, float _DefPara, float _HpPara, float _MpPara
		, int _Cri, int _CriDamage, int _Avd, int DamageRange
	)
		:UnitName(_T("")), MainStat(_MainStat)
		, StrPara(_StrPara), DexPara(_DexPara), IntPara(_IntPara), LuckPara(_LuckPara)
		, DamagePara(_DamagePara), DefPara(_DefPara), HpPara(_HpPara), MpPara(_MpPara)
		, Critical(_Cri), CriticalDamage(_CriDamage), Avoidability(_Avd), DamageRange(DamageRange)
	{
		_tcscpy_s(UnitName, _countof(UnitName), _UnitName.c_str());
	}
	~STATPARAMETERDATA() {}
};



// Stat Table 데이터(레벨마다 변하므로 각각을 저장)
struct STAT
{
public:
	int				Str;
	int				Dex;
	int				Int;
	int				Luck;
	int				Heath;
	int				Mana;
	// 데미지의 중간값 초기값 지정을 위해 
	int				Damage;
	int				MinDamage;
	int				MaxDamage;
	int				Defense;
	
	//// 크리티컬 확률(%)
	int				Critical;
	//// 크리티컬 데미지 비율
	int				CriticalDamage;
	//// 회피율
	int				Avoidability;


	
	// 몬스터만 사용하는 변수
	unsigned int	Exp;
	int				Level;

	BOOL SaveDataToFile(FILE* _WriteFile);
	BOOL LoadDataToFile(FILE* _ReadFile,std::vector<STAT>* _Table);
	void operator=(const STAT& _Other) 
	{
		Str = _Other.Str;
		Dex = _Other.Dex; 
		Int = _Other.Int; 
		Luck = _Other.Luck;
		Damage = _Other.Damage;
		Defense = _Other.Defense; 
		Heath = _Other.Heath;
		Mana = _Other.Mana;
		MinDamage = _Other.MinDamage; MaxDamage = _Other.MaxDamage;

		Critical = _Other.Critical; CriticalDamage = _Other.CriticalDamage;
		Avoidability = _Other.Avoidability;

		Exp = _Other.Exp; Level = _Other.Level;

	}

	STAT operator+(const STAT& _Other);
	STAT operator-(const STAT& _Other);
	void operator+=(const STAT& _Other);
	void operator-=(const STAT& _Other);
	static STAT AddUnit_ItemStat(const STAT& _UnitStat,const STATPARAMETERDATA& _UnitPara, const STAT& _ItemStat);

public:
	STAT() : Str(0), Dex(0), Int(0), Luck(0), Heath(0), Mana(0)
		, Damage(0),MinDamage(0), MaxDamage(0), Defense(0)
		,Exp(0), Level(0), Critical(0), CriticalDamage(0), Avoidability(0)
	{
		
	}
	STAT(const int& _InitStr, const int& _InitDex, const int& _InitInt, const int& _InitLuck
		, const int& _InitDamage, const int& _InitDef, const int& _InitHp, const int& _InitMp)
		:Str(_InitStr), Dex(_InitDex)
		, Int(_InitInt), Luck(_InitLuck), Heath(_InitHp), Mana(_InitMp)
		, Damage(_InitDamage), MinDamage(_InitDamage), MaxDamage(_InitDamage), Defense(_InitDef)
		, Exp(0), Level(0), Critical(0), CriticalDamage(0), Avoidability(0)
	{
		
	}
	STAT(const STAT& _Other) 
		:Str(_Other.Str), Dex(_Other.Dex)
		, Int(_Other.Int), Luck(_Other.Luck)
		, Heath(_Other.Heath), Mana(_Other.Mana)
		, Damage(_Other.Damage), MinDamage(_Other.MinDamage)
		, MaxDamage(_Other.MaxDamage), Defense(_Other.Defense)
		, Exp(_Other.Exp), Level(_Other.Level)
		, Critical(_Other.Critical), CriticalDamage(_Other.CriticalDamage)
		, Avoidability(_Other.Avoidability)

	{
	}

	~STAT() {}
};

// Editor에서 설정한 스탯 증가량과 메인 스탯



template<typename T>
struct Less : public std::binary_function<T, T, bool>
{
public:
	bool							m_bSetting;
	bool(*m_LessFunc)(T, T);

public:
	void SetLessFunc(bool(*_Func)(T, T))
	{
		if (nullptr == _Func)
		{
			assert(nullptr);
			return;
		}
		m_LessFunc = _Func;
		m_bSetting = true;
	}

	bool operator()(T _Left, T _Right)
	{
		if (false == m_bSetting)
		{
			return false;
		}
		return m_LessFunc(_Left, _Right);
	}

public:
	Less() :m_LessFunc(nullptr), m_bSetting(false) {}
	Less(bool(*_Func)(T, T)) : m_LessFunc(_Func),m_bSetting(true){}
	~Less() {}
};

/////////////////////////////////////   스킬 관련 구조체	////////////////////////////////////////

// 애니메이션 변경을 알려주는 구조체
struct ANIMATIONSCRIPT 
{
	UINT						AnimationIndex;
	float						StartTime;
	// 해당 애니메이션으로 변경되었을 경우 true로 변환

	void operator=(const ANIMATIONSCRIPT& _Other) 
	{
		AnimationIndex = _Other.AnimationIndex;
		StartTime = _Other.StartTime;
		
	}

	ANIMATIONSCRIPT() :AnimationIndex(MAXANINAME), StartTime(0.0f) {}
	ANIMATIONSCRIPT(const ANIMATIONNAME& _Index, const float& _StartTime) 
		:AnimationIndex(_Index), StartTime(_StartTime)
	{}
	ANIMATIONSCRIPT(const ANIMATIONSCRIPT& _Other) 
		:AnimationIndex(_Other.AnimationIndex),StartTime(_Other.StartTime)
	{}
	~ANIMATIONSCRIPT() {}
};

// 스킬에 포함된 움직이는 모든 오브젝트의 움직임을 저장(직선운동만 저장)
struct MOVEDATA
{
	enum
	{
		MOVEMODE_MOVE,		// 무브 Data 를 나타내는 인덱스
		MOVEMODE_HIDE,		// 지정한 Hide Time기준으로 알파값 선형 감소
		MOVEMODE_SHOW,		// 지정한 Hide Time기준으로 알파값 선형 증가
		MAXMOVEMODE,
		};

	UINT		MoveMode;

	float		MoveStartTime;
	float		MoveTime;
	Vec2		MoveStartPos;
	Vec2		MoveEndPos;

	float		HideTime;
	// 캐릭터의 스킬 시전전 위치로 이동
	bool		bSetTeamPos;
	// 캐릭터를 스킬 Init위치로 이동
	bool		bSetInitPos;
	
	void SetMoveData(const float& _StartTime, const float& _MoveTime, const Vec2& _StartPos, const Vec2& _EndPos);
	void SetHideData(const float& _StartTime, const float& _HideTime);
	void SetShowData(const float& _StartTime, const float& _HideTime);
	void SetTeamPos(const float& _SetTime);
	void SetInitPos(const float& _SetTime);

	bool operator>(const MOVEDATA& _Other) 
	{
		return MoveStartTime > _Other.MoveStartTime;
	}
	bool operator>=(const MOVEDATA& _Other)
	{
		return MoveStartTime >= _Other.MoveStartTime;
	}
	bool operator<(const MOVEDATA& _Other)
	{
		return MoveStartTime < _Other.MoveStartTime;
	}
	bool operator<=(const MOVEDATA& _Other)
	{
		return MoveStartTime < _Other.MoveStartTime;
	}

	void operator=(const MOVEDATA& _Other)
	{
		MoveStartTime = _Other.MoveStartTime;
		MoveTime = _Other.MoveTime;
		MoveStartPos = _Other.MoveStartPos;
		MoveEndPos = _Other.MoveEndPos;
		MoveMode = _Other.MoveMode;
		HideTime = _Other.HideTime;
		bSetInitPos = _Other.bSetInitPos;
		bSetTeamPos = _Other.bSetTeamPos;
	}

	MOVEDATA() : MoveStartTime(0.0f), MoveTime(0.0f), MoveStartPos(Vec2{ 0.0f,0.0f }), MoveEndPos(Vec2{0.0f,0.0f})
		, MoveMode(MOVEMODE_MOVE), HideTime(0.0f)
		,bSetInitPos(false),bSetTeamPos(false)
	{}
	MOVEDATA(const float& _StartTime, const float& _MoveTime, const Vec2& _StartPos, const Vec2& _EndPos)
		:MoveStartTime(_StartTime), MoveTime(_MoveTime), MoveStartPos(_StartPos), MoveEndPos(_EndPos)
		, MoveMode(MOVEMODE_MOVE), HideTime(0.0f)
		, bSetInitPos(false), bSetTeamPos(false)
	{}
	MOVEDATA(const MOVEDATA& _Other) 
		:MoveStartTime(_Other.MoveStartTime), MoveTime(_Other.MoveTime)
		, MoveStartPos(_Other.MoveStartPos), MoveEndPos(_Other.MoveEndPos)
		, MoveMode(_Other.MoveMode),HideTime(_Other.HideTime)
		, bSetInitPos(_Other.bSetInitPos), bSetTeamPos(_Other.bSetTeamPos)
	{}

	~MOVEDATA() {}
};


// 스킬에 포함된 이펙트 데이터(움직임,시작시간,크기)
struct SKILLEFFECTDATA
{
	enum 
	{
		INITPOS_CUSTOM,
		INITPOS_CASTERPOS,
		MAXINITPOS,
	};

	UINT				EffectIndex;
	Vec2				InitPos;
	UINT				InitPosMode;
	
	float				StartTime;
	MOVEDATA			MoveData;
	
	Vec2				EffectCustomSize;
	float				EffectImageMag;

	// ActionPhase에서 이 값을 변경하면서 
	// Update를 수행(프로세스 실행중에 변경되는 변수)
	bool				Apply;
	// 데이터와 연결된 Effect가 들어있는 배열의 Index
	// 게임중에 실시간으로 변경됨
	size_t				PIndex;

	bool SetInitPos(const Vec2& _InitPos);
	bool SetInitPos(const int& _InitPosMode, const Vec2& _InitPos = Vec2{ 0.0f,0.0f });
	
	void operator=(const SKILLEFFECTDATA& _Other) 
	{
		EffectIndex = _Other.EffectIndex;
		InitPos = _Other.InitPos;
		InitPosMode = _Other.InitPosMode;
		StartTime = _Other.StartTime;
		MoveData = _Other.MoveData;
		EffectCustomSize = _Other.EffectCustomSize;
		EffectImageMag = _Other.EffectImageMag;
		Apply = _Other.Apply;
		PIndex = _Other.PIndex;
	}

	SKILLEFFECTDATA() :EffectIndex(-1), InitPos(Vec2{0.0f,0.0f}), StartTime(0.0f),
		MoveData(MOVEDATA()), EffectCustomSize(Vec2{0.0f,0.0f}), EffectImageMag(0.0f),
		Apply(false), InitPosMode(0), PIndex(0)
	{}
	SKILLEFFECTDATA(const size_t& _EffectIndex,
		const Vec2& _InitPos,
		const float _StartTime,
		const MOVEDATA& _MoveData,
		const Vec2& _CustomSize,
		const float& _ImageMag
	)
		:EffectIndex(0),InitPos(_InitPos), StartTime(_StartTime), MoveData(_MoveData),
		EffectCustomSize(_CustomSize), EffectImageMag(_ImageMag), Apply(false),InitPosMode(0)
	{
		EffectIndex = (UINT)_EffectIndex;
	}
	SKILLEFFECTDATA(const SKILLEFFECTDATA& _Other)
		:EffectIndex(_Other.EffectIndex), InitPos(_Other.InitPos), StartTime(_Other.StartTime), MoveData(_Other.MoveData)
		, EffectCustomSize(_Other.EffectCustomSize), EffectImageMag(_Other.EffectImageMag)
		, Apply(false),InitPosMode(_Other.InitPosMode)
		, PIndex(_Other.PIndex)
	{
	}
	~SKILLEFFECTDATA() {}
};

struct SCENEEFFECTDATA 
{ 
	enum ZOOMPOSMODE 
	{
		ZOOMPOS_UNIT,
		ZOOMPOS_CUSTOM,
		MAXZOOMPOSMODE,
	};

	DWORD				SceneEffectFlag;
	float				SceneEffectStartTime;
	float				SceneEffectTime;
	// Fade Time & Zoom Time
	float				SubTime;

	Vec2				Earthquake_SpaceSize;
	UINT				ZoomPosMode;
	Vec3				ZoomPos;
	Vec2				ZoomRatio;
	
	bool SetEarthEffect(const DWORD& _Flag
		, const float& _StartTime
		, const float& _EffectTime
		, const  Vec2& _SpaceSize
	);
	bool SetBlackOutEffect(const DWORD& _Flag
		, const float& _StartTime
		, const float& _EffectTime
	);
	bool SetFadeEffect(const DWORD& _Flag
		, const float& _StartTime
		, const float& _EffectTime
		, const float& _SubTime
	);
	bool SetZoomEffect(const DWORD& _Flag
		, const float& _StartTime
		, const float& _EffectTime
		, const float& _SubTime
		, const UINT& _ZoomMode
		, const Vec3& _ZoomPos
		, const Vec2& _ZoomRatio
	);



	void operator=(const SCENEEFFECTDATA& _Other) 
	{
		SceneEffectFlag = _Other.SceneEffectFlag;
		SceneEffectStartTime = _Other.SceneEffectStartTime;
		SceneEffectTime = _Other.SceneEffectTime;
		SubTime = _Other.SubTime;
		Earthquake_SpaceSize = _Other.Earthquake_SpaceSize;
		ZoomPosMode = _Other.ZoomPosMode;
		ZoomPos = _Other.ZoomPos;
		ZoomRatio = _Other.ZoomRatio;
	}
	SCENEEFFECTDATA() :SceneEffectFlag(0),SceneEffectTime(0.0f),SubTime(0.0f),
		Earthquake_SpaceSize(Vec2(0.0f, 0.0f)), ZoomPos(Vec3{ 0.0f,0.0f,0.0f }), ZoomRatio(Vec2{0.0f,0.0f}),
		SceneEffectStartTime(0.0f), ZoomPosMode(0)
	{}
	SCENEEFFECTDATA(const DWORD& _Flag,
		const float& _EffectStartTime,
		const float& _EffectTime,
		const float& _SubTime,
		const Vec2& _SpaceSize,
		const UINT& _ZoomPosMode,
		const Vec3& _ZoomPos,
		const Vec2& _ZoomRatio
	): 
		SceneEffectFlag(_Flag),SceneEffectTime(_EffectTime),SubTime(_SubTime),
		Earthquake_SpaceSize(_SpaceSize),ZoomPos(_ZoomPos),ZoomRatio(_ZoomRatio),
		SceneEffectStartTime(_EffectStartTime), ZoomPosMode(_ZoomPosMode)
	{}
	SCENEEFFECTDATA(const SCENEEFFECTDATA& _Other)
		:SceneEffectFlag(_Other.SceneEffectFlag), SceneEffectTime(_Other.SceneEffectTime), SubTime(_Other.SubTime),
		Earthquake_SpaceSize(_Other.Earthquake_SpaceSize), ZoomPos(_Other.ZoomPos), ZoomRatio(_Other.ZoomRatio),
		SceneEffectStartTime(_Other.SceneEffectStartTime), ZoomPosMode(_Other.ZoomPosMode)
	{}
	~SCENEEFFECTDATA() {}
};

typedef struct ACTIONSCRIPT 
{
	enum
	{
		MAXEFFECTCOUNT = 10,
		MAXMOVECOUNT = 10,
		MAXANISCRIPTCOUNT = 5,
		MAXCHARSOUNDCOUNT = 5,
		MAXSCENEEFFECTCOUNT = 5,
	};
	
	// 자기가 속한 스킬의 번호
	UINT					SkillNo;
	UINT					CharIndex;

	// 스킬이 플레이 되는 시간
	float					TotalSkillTime;


	// 캐릭터의 애니메이션 Script
	ANIMATIONSCRIPT			AniSctipt[MAXANISCRIPTCOUNT];
	UINT					AniCount;

	// 캐릭터의 움직임 or HideMode를 저장하는 배열
	MOVEDATA				CharMoveData[MAXMOVECOUNT];
	UINT					MoveCount;
	
	// 스킬에 사용되는 이펙트를 저장하는 배열
	SKILLEFFECTDATA			SkillEffect[MAXEFFECTCOUNT];
	UINT					EffectCount;

	// 대사 혹은 스킬 사운드를 저장하기 위한 배열
	SOUNDSAVEDATA			CharSound[MAXCHARSOUNDCOUNT];
	UINT					CharSoundCount;

	// 카메라 워킹 or 특수효과등을 저장하기 위한 배열
	SCENEEFFECTDATA			SceneEffectData[MAXSCENEEFFECTCOUNT];
	UINT					SceneEffectCount;

	bool AddAniScript(const ANIMATIONSCRIPT& _AniScript);
	bool AddCharMoveData(const MOVEDATA& _MoveData);
	bool AddSkillEffectData(const SKILLEFFECTDATA& _SkillEffectData);
	bool AddCharSoundData(const SOUNDSAVEDATA& _SoundData);
	bool AddSceneEffectData(const SCENEEFFECTDATA& _SceneEffectData);

	bool DeleteAniScript(const UINT& _Index);
	bool DeleteCharMoveData(const UINT& _Index);
	bool DeleteSkillEffectData(const UINT& _Index);
	bool DeleteCharSoundData(const UINT& _Index);
	bool DeleteSceneEffectData(const UINT& _Index);

	bool isEmpty() const ;
	
	void operator=(const ACTIONSCRIPT& _Other);
	

	ACTIONSCRIPT() 
		:SkillNo(-1),CharIndex(0), EffectCount(0)
		, CharSoundCount(0), SceneEffectCount(0), AniCount(0),
		TotalSkillTime(0.0f), MoveCount(0)
	{
		memset(AniSctipt, 0, sizeof(ANIMATIONSCRIPT)*MAXANISCRIPTCOUNT);
		memset(CharMoveData, 0, sizeof(MOVEDATA)*MAXMOVECOUNT);
		memset(SkillEffect, 0, sizeof(SKILLEFFECTDATA)*MAXEFFECTCOUNT);
		memset(CharSound, 0, sizeof(SOUNDSAVEDATA)*MAXCHARSOUNDCOUNT);
		memset(SceneEffectData, 0, sizeof(SCENEEFFECTDATA)*MAXSCENEEFFECTCOUNT);
	}
	ACTIONSCRIPT(const ACTIONSCRIPT& _Other)
		:SkillNo(_Other.SkillNo),CharIndex(_Other.CharIndex), EffectCount(_Other.EffectCount)
		, CharSoundCount(_Other.CharSoundCount), SceneEffectCount(_Other.SceneEffectCount)
		, TotalSkillTime(_Other.TotalSkillTime), MoveCount(_Other.MoveCount)
		, AniCount(_Other.AniCount)	
	{
		memset(AniSctipt, 0, sizeof(ANIMATIONSCRIPT)*MAXANISCRIPTCOUNT);
		memset(CharMoveData, 0, sizeof(MOVEDATA)*MAXMOVECOUNT);
		memset(SkillEffect, 0, sizeof(SKILLEFFECTDATA)*MAXEFFECTCOUNT);
		memset(CharSound, 0, sizeof(SOUNDSAVEDATA)*MAXCHARSOUNDCOUNT);
		memset(SceneEffectData, 0, sizeof(SCENEEFFECTDATA)*MAXSCENEEFFECTCOUNT);
		
		for (UINT i = 0; i < _Other.AniCount; i++)
		{
			AniSctipt[i] = _Other.AniSctipt[i];
		}
		
		for (UINT i = 0; i < _Other.MoveCount; i++)
		{
			CharMoveData[i] = _Other.CharMoveData[i];
		}
		for (UINT i = 0; i < _Other.CharSoundCount; i++)
		{
			CharSound[i] = _Other.CharSound[i];
		}
		for (UINT i = 0; i < _Other.EffectCount; i++)
		{
			SkillEffect[i] = _Other.SkillEffect[i];
		}
		for (UINT i = 0; i < _Other.SceneEffectCount; i++)
		{
			SceneEffectData[i] = _Other.SceneEffectData[i];
		}

	}
	~ACTIONSCRIPT() {}
} ActionScript;

struct HITDATA 
{
	enum
	{
		POS_TARGET,
		POS_CASTER,
		POS_TARGETMIDDLE,
		MAXPOSOPTION,
	};

	float				StartTime;

	// Demage Font를 한번에 몇개 띄울지 결정
	UINT				HitCount;
	
	// 스킬 데미지 계수
	float				HitDemageMag;
	UINT				FixHitDemage;
	// Demage Font의 크키를 저장(NumberUI안의 FontData를 기본값으로)
	float				DemageFontMag;

	// 타이밍에 맞추어 출력할 효과의 인덱스
	UINT				HitEffectIndex;
	UINT				PosOption;
	Vec2				HitEffectCustomSize;
	float				HitEffectImageMag;
	Vec2				HitEffectPivot;

	UINT				HitSoundIndex;
	UINT				ReActionIndex;

	// 프로그램 실행중에 사용 & 변경하는 변수
	// HitData를 적용했는지 체크하는 변수
	bool				Apply;

	void operator=(const HITDATA&  _Other);
	bool operator>(const HITDATA& _Other);
	bool operator>=(const HITDATA& _Other);
	bool operator<(const HITDATA& _Other);
	bool operator<=(const HITDATA& _Other);


	void InitHitEffect();
	void InitHitSound();
	void InitReAction();
	bool SetHitEffect(const size_t& _EffectIndex);
	bool SetHitSound(const size_t& _SoundIndex);
	bool SetHitReAction(const size_t& _ReActionIndex);
	HITDATA()
		:StartTime(0.0f), HitCount(1), DemageFontMag(0.0f)
		, HitEffectIndex(-1), HitSoundIndex(-1), ReActionIndex(-1)
		, Apply(false), HitDemageMag(0.0f), FixHitDemage(0)
		, HitEffectCustomSize(Vec2(0.0f,0.0f)), HitEffectImageMag(1.0f)
		, HitEffectPivot(Vec2(0.0f,0.0f)), PosOption(0)
	{

	}
	HITDATA(const HITDATA& _Other) 
		:StartTime(_Other.StartTime)
		, HitCount(_Other.HitCount), DemageFontMag(_Other.DemageFontMag)
		, HitEffectIndex(_Other.HitEffectIndex)	, HitSoundIndex(_Other.HitSoundIndex)
		, ReActionIndex(_Other.ReActionIndex), Apply(_Other.Apply)
		, HitDemageMag(_Other.HitDemageMag), FixHitDemage(_Other.FixHitDemage)
		, HitEffectCustomSize(_Other.HitEffectCustomSize), HitEffectImageMag(_Other.HitEffectImageMag)
		, HitEffectPivot(_Other.HitEffectPivot), PosOption(_Other.PosOption)
	{
	}
	~HITDATA() {}
};
struct BUFFDATA 
{	
	enum 
	{
		BUFFTYPE_HP,
		BUFFTYPE_MP,
		BUFFTYPE_MAINSTAT,
		BUFFTYPE_STR,
		BUFFTYPE_DEX,
		BUFFTYPE_INT,
		BUFFTYPE_LUCK,
		BUFFTYPE_DAMAGE,
		BUFFTYPE_DEF,
		BUFFTYPE_CRITICAL,
		BUFFTYPE_CRITICALDAMAGE,
		BUFFTYPE_AVOID,
		BUFFTYPE_TURNCOUNT,
		MAXBUFFTYPE,
	};

	UINT	BuffType;
	// 버프타입에 맞는 버프량(스탯 -+% , Unit TurnCount 등등)
	float	BuffValue;
	// 버프 지속 시간(Turn으로 체크)
	UINT	BuffCount;

	UINT	BuffEffectIndex;
	// 버프 적용시 해당 버프가 적용된 Unit머리위에 붙는 작은 버프알림
	// SpriteIndex(한 멀티스프라이에 모든 작은 버프 알림 Sprite를 모을 예정)
	UINT	BuffIconIndex;

public:
	void operator=(const BUFFDATA& _Other);

	BUFFDATA() 
		:BuffType(-1), BuffValue(0.0f)
		, BuffCount(0), BuffIconIndex(0)
		, BuffEffectIndex(-1)
	{}
	BUFFDATA(const BUFFDATA& _Other)
		:BuffType(_Other.BuffType), BuffValue(_Other.BuffValue)
		, BuffCount(_Other.BuffCount), BuffIconIndex(_Other.BuffIconIndex)
		, BuffEffectIndex(_Other.BuffEffectIndex)
	{}
};
// SkillLevelData (스킬 레벨에 영향을 받는 데이터는 따로 저장한다)
// 내부에 HitData를 배열로 갖는다.
struct LEVELDATA
{
	enum 
	{
		MAXHITCOUNT = 10,
		MAXSKILLLEVEL = 5,
	};

	enum
	{
		SKILLCOST_HP,
		SKILLCOST_MP,
		SKILLCOST_BURST,
		MAXSKILLCOST
	};

	// 자기가 속한 스킬의 번호
	UINT		SkillNo;
	// 스킬 사용 행동력을 저장(밀리는턴 = 스킬 행동력 - 캐릭터의 행동력)
	UINT		SkillCount;
	// 스킬 비용(Hp,Mp,Burst)
	UINT		SkillCost[MAXSKILLCOST];

	// Font Style
	UINT		DemageStyle;
	int			Option;				// 계단식 데미지 스타일에서 사용하는 변수(X축으로 얼마나 땡길지,0 의경우 일직선,+ 오른쪽,- 왼쪽)


	//Hit Data가 MAXHITCOUNT만큼 배열로 저장
	HITDATA		HitData[MAXHITCOUNT];
	UINT		HitDataCount;

	BUFFDATA	BuffData;



	void operator=(const LEVELDATA& _Other);
	void ClearHitData();
	bool AddHitData(const HITDATA& _Data);
	bool DelHitData(const int& _Index);
	bool SwapHitData(const int& _a, const int& _b);
	void SortHitData();
	bool SetSkillCost(const size_t& _CostType, const size_t& _CostValue);

	LEVELDATA()
		:SkillNo(-1), HitDataCount(0), SkillCount(0)
		, DemageStyle(0), Option(0)
	{
		memset(SkillCost, 0, sizeof(size_t)*MAXSKILLCOST);
		ClearHitData();
	}
	LEVELDATA(const LEVELDATA& _Other)
		:SkillNo(_Other.SkillNo), HitDataCount(_Other.HitDataCount)
		, SkillCount(_Other.SkillCount)
		, DemageStyle(_Other.DemageStyle), Option(_Other.Option)
	{
		memset(SkillCost, 0, sizeof(size_t)*MAXSKILLCOST);
		
		for (size_t i = 0; i < MAXSKILLCOST; i++)
		{
			SkillCost[i] = _Other.SkillCost[i];
		}

		ClearHitData();
	
		for (size_t i = 0; i < _Other.HitDataCount; ++i)
			AddHitData(_Other.HitData[i]);

	}
	~LEVELDATA() {}
};

struct SKILLDATA
{
	enum INITPOSMODE
	{
		INITPOS_NONE,			// 팀 위치 기준
		INITPOS_FIXPOS,			// 지정 위치 기준  
		INITPOS_TARGETPOS,		// 타겟 위치 기준(SingleTarget에서 사용)
		INITPOS_TARGET_FRONTPOS,// 타겟 Pos + Target ImageSize.x * 0.5f
		INITPOS_TARGET_BACKPOS, // 타겟 Pos - Target ImageSize.x * 0.5f
		MAXINITPOS,
	};

	enum SKILLTYPE
	{
		SKILLTYPE_ACTION,
		SKILLTYPE_ABILITY,
		SKILLTYPE_BURSTSKILL,
		SKILLTYPE_ITEM,
		MAXSKILLTYPE,
	};

	enum 
	{	
		MAXSKILLLEVEL = 5,
	};

	enum
	{
		TARGET_NONE,				// 타겟타입이 NONE = > 대상이 아니다. 
		TARGET_SINGLE,				// 몬스터 스킬인 이 상태는 랜덤 SINGLE
		TARGET_COLUMN,
		TARGET_ALL,
		TARGET_RANDOM_ONE,
		TARGET_RANDOM_TWO,
		TARGET_RANDOM_THREE,
		TARGET_RANDOM_FOUR,
		MAXTARGET
	};

	enum
	{
		BUFFTYPE_NONE,			// 이 스킬은 버프스킬이 아니다
		BUFFTYPE_ENEMYBUFF,		// 적군에게 거는 버프 타입
		BUFFTYPE_TEAMBUFF,		// 아군에게 거는 버프 타입
		MAXBUFFTYPE
	};

	// 자기가 속한 스킬의 번호
	UINT		SkillNo;
	// 스킬이 어떤 카테고리에 소속될지 결정할 변수
	UINT		SkillType;
	
	// 스킬이 버프스킬인지 아닌지 결정하는 변수
	UINT		SkillBuffType;

	// 아군 , 적군 타겟의 타입을 결정
	UINT		TeamTargetType;
	UINT		EnemyTargetType;

	// 해당 스킬이 Character가 사용할 스킬인지 확인 변수
	bool		CharacterSkill;

	UINT		InitPosMode;
	Vec2		InitFixPos;
	Vec2		InitPosPivot;


	void operator=(const SKILLDATA& _Other);

	
	
	SKILLDATA()
		:SkillNo(-1), SkillType(-1)
		, TeamTargetType(TARGET_NONE), EnemyTargetType(TARGET_NONE)
		, CharacterSkill(true), SkillBuffType(BUFFTYPE_NONE)
		, InitPosMode(0), InitFixPos(Vec2{0.0f,0.0f})
		, InitPosPivot(0.0f,0.0f)
	{
		
	}
	SKILLDATA(const SKILLDATA& _Other)
		:SkillNo(_Other.SkillNo), SkillType(_Other.SkillType)
		, TeamTargetType(_Other.TeamTargetType), EnemyTargetType(_Other.EnemyTargetType)
		, CharacterSkill(_Other.CharacterSkill), SkillBuffType(_Other.SkillBuffType)
		, InitPosMode(_Other.InitPosMode), InitFixPos(_Other.InitFixPos)
		, InitPosPivot(_Other.InitPosPivot)
	{

	}
	~SKILLDATA() {}
};

struct SKILLUIDATA 
{
	UINT				SkillNo;
	UINT				SkillIconIndex;
	// 스킬 영어명
	std::tstring		SkillName;
	// 스킬 한국명
	std::tstring		SkillKName;
	// 스킬 설명
	std::tstring		SkillDescription;
	

	// 위의 모든 데이트는 한 스트링으로 표현
	// [스킬번호][아이콘번호]영어명$한국어명$스킬설명
	CHAR				SkillUISaveData[MAXSTRING];

	void operator=(const SKILLUIDATA& _Other);

	void SetSkillSaveData();
	void ApplySaveData();
	void ClearData();

	BOOL SaveDataToFile(FILE* _WriteFile);
	static BOOL LoadTextFile(FILE* _ReadFile, std::list<SKILLUIDATA>* _TempVec);

	SKILLUIDATA()
		:SkillNo(-1), SkillIconIndex(-1),SkillName(_T(""))
		, SkillKName(_T("")),SkillDescription(_T(""))
	{
		memset(SkillUISaveData, 0, sizeof(CHAR)*MAXSTRING);
	}
	SKILLUIDATA(CHAR* _TextData) 
		:SkillNo(-1), SkillIconIndex(-1), SkillName(_T(""))
		, SkillKName(_T("")), SkillDescription(_T(""))
	{
		memset(SkillUISaveData, 0, sizeof(CHAR)*MAXSTRING);
		if (nullptr != _TextData)
			strcpy_s(SkillUISaveData, sizeof(CHAR)*MAXSTRING, _TextData);
	}
	SKILLUIDATA(const SKILLUIDATA& _Other) 
		:SkillNo(_Other.SkillNo), SkillIconIndex(_Other.SkillIconIndex), SkillName(_Other.SkillName)
		, SkillKName(_Other.SkillKName), SkillDescription(_Other.SkillDescription)
	{
		memset(SkillUISaveData, 0, sizeof(CHAR)*MAXSTRING);
		strcpy_s(SkillUISaveData, _Other.SkillUISaveData);
	}

};


// 액션 스크립트 + 스킬 세부 정보 + UI정보를 모두 포함한 
// 스킬 구조체
struct SKILL 
{
	UINT				SkillNo;
	UINT				CurSkillLevel;
	UINT				CurSkillExp;
	TCHAR				SkillName[MAXKEYLENGTH];
	ACTIONSCRIPT		SkillActionScript;
	// LevelData는 같은 SkillNo 으로 LevelData의 Vector에서
	// SkillLevel에 맞는 LevelData를 뽑아온다.
	SKILLDATA			SkillData;
	//UI 정보 포함 
	SKILLUIDATA			SkillUIData;

	void SetSkillName(const std::tstring& _SkillName);
	const UINT	 GetUnitIndex();

	void operator=(const SKILL& _Other);

	//void WriteData( FILE* _WriteFile);

	SKILL() 
		:SkillNo(-1),SkillName(_T(""))
		, SkillActionScript(ACTIONSCRIPT())
		, SkillData(SKILLDATA())
		, SkillUIData(SKILLUIDATA())
		, CurSkillLevel(0)
		, CurSkillExp(0)
	{
	}
	SKILL(const size_t& SkillNo, const std::tstring& _SkillName
		, const ACTIONSCRIPT& _Action, const SKILLDATA& _SkillData
		, const SKILLUIDATA& _UIData);
	
	~SKILL() {}

};

struct SKILLSAVEDATA 
{
	ACTIONSCRIPT		ActionScript;
	SKILLDATA			SkillData;

	void operator=(const SKILLSAVEDATA& _Other);

	size_t WriteData(FILE* _WriteFile);
	
public:
	SKILLSAVEDATA() :ActionScript(ACTIONSCRIPT()), SkillData(SKILLDATA()) {}
	SKILLSAVEDATA(const SKILL& _Skill);
	SKILLSAVEDATA(const SKILLSAVEDATA& _Other)
		: ActionScript(_Other.ActionScript)
		, SkillData(_Other.SkillData)
	{}
	~SKILLSAVEDATA() {}
};

// 유닛의 현재 스킬레벨을 나타내기 위한 구조체
// 나중에 파일에서 정보를 읽어올 예정
struct UNITSKILLDATA 
{
	UINT				SkillNo;
	UINT				SkillLevel;

	void operator=(const UNITSKILLDATA& _Other);

	UNITSKILLDATA()
		:SkillNo(-1), SkillLevel(-1) 
	{}
	UNITSKILLDATA(const UINT& _SkillNo,const UINT& _SkillLevel)
		:SkillNo(_SkillNo), SkillLevel(_SkillLevel)
	{}
	UNITSKILLDATA(const UNITSKILLDATA& _Other)
		:SkillNo(_Other.SkillNo), SkillLevel(_Other.SkillLevel)
	{}
	~UNITSKILLDATA(){}
};

struct SAVELEVELDATA 
{
	UINT					SkillNo;
	LEVELDATA				LevelData[LEVELDATA::MAXSKILLLEVEL];

	void operator=(const SAVELEVELDATA& _Other);
	
	size_t LoadDataFile(FILE* _ReadFile);
	size_t SaveDataFile(FILE* _WriteFile);

	SAVELEVELDATA(const SAVELEVELDATA& _Other);
	SAVELEVELDATA(std::vector<LEVELDATA>* _LevelData);

	SAVELEVELDATA();
	~SAVELEVELDATA() {}
};

struct DIRSET
{
	Vec3				Dir;
	std::tstring		DirString;

	void operator=(const DIRSET& _Other);

	DIRSET()
		:Dir(Vec3{ 0.0f,0.0f,0.0f }), DirString(_T(""))
	{}
	DIRSET(const Vec3& _Dir, const std::tstring& _DirString)
		:Dir(_Dir), DirString(_DirString)

	{}
	DIRSET(const DIRSET& _Other)
		:Dir(_Other.Dir), DirString(_Other.DirString)
	{}
	~DIRSET() {}
};

struct TILEDATA 
{
	UINT					TileType;

	UINT					TileXIndex;
	UINT					TileYIndex;
	
	UINT					TileColorNo;

	UINT					ChangeTileMapNo;
	UINT					SpawnTileXIndex;
	UINT					SpawnTileYIndex;

	UINT					WorldMapOption;

	void operator=(const TILEDATA& _Other);
	static size_t ReadTileMapData(FILE* _ReadFile,TILEDATA* _Buf,const size_t& _ReadCount);
	static size_t WriteTileData(FILE* _WriteFile, TILEDATA* _Buf,const size_t& _WriteCount);

public:
	TILEDATA() 
		:TileType(-1), TileXIndex(-1), TileYIndex(-1)
		, TileColorNo(-1), ChangeTileMapNo(-1)
		, SpawnTileXIndex(-1), SpawnTileYIndex(-1)
		, WorldMapOption(-1)
	{
	}
	TILEDATA(const TILEDATA& _Other);
	~TILEDATA() {}
};
struct TILEMAPDATA 
{
	UINT					TileMapNo;
	TCHAR					TileMapName[MAXKEYLENGTH];
	UINT					TileSizeX;
	UINT					TileSizeY;

public:
	size_t ReadTileMapData(FILE* _ReadFile);
	size_t WriteTileMapData(FILE* _WriteFile);

public:
	void SetTileMapName(const std::tstring& _Name);

public:
	void operator=(const TILEMAPDATA& _Other);

	TILEMAPDATA() :TileMapNo(-1) , TileMapName(_T(""))
		, TileSizeX(0), TileSizeY(0)
	{}
	TILEMAPDATA(const UINT& _TileMapNo ,const std::tstring& _TileMapName)
		:TileMapNo(_TileMapNo), TileMapName(_T(""))
		,TileSizeX(0), TileSizeY(0)
	{
		_tcscpy_s(TileMapName, _countof(TileMapName), _TileMapName.c_str());
	}	
	TILEMAPDATA(const UINT& _TileMapNo, const std::tstring& _TileMapName
	, const UINT& _MapSizeX, const UINT& _MapSizeY)
		:TileMapNo(_TileMapNo), TileMapName(_T(""))
		, TileSizeX(_MapSizeX), TileSizeY(_MapSizeY)
	{
		_tcscpy_s(TileMapName, _countof(TileMapName), _TileMapName.c_str());
	}
	TILEMAPDATA(const TILEMAPDATA& _Other);
	~TILEMAPDATA() {}

};

struct MAPOBJECTDATA 
{
	UINT					TileMapNo;
	UINT					MapObjectNo;
	UINT					LeftIndex;
	UINT					BottomIndex;

	void operator=(const MAPOBJECTDATA& _Other);
	
	static size_t WriteData(FILE* _WriteFile, MAPOBJECTDATA* _Buf,const UINT& _WriteCount);
	static size_t ReadData(FILE* _ReadFile, MAPOBJECTDATA* _Buf, const size_t& BufSize , const UINT& _ReadCount);
	
	MAPOBJECTDATA():TileMapNo(-1), MapObjectNo(-1), LeftIndex(-1), BottomIndex(-1){}
	MAPOBJECTDATA(const MAPOBJECTDATA& _Other) 
		:TileMapNo(_Other.TileMapNo), MapObjectNo(_Other.MapObjectNo)
		, LeftIndex(_Other.LeftIndex), BottomIndex(_Other.BottomIndex)
	{
		
	}

	~MAPOBJECTDATA() {}
};

struct MONSTERDATA
{
	enum MONSTAT
	{
		MON_STR,
		MON_DEX,
		MON_INT,
		MON_LUK,
		MON_HP,
		MON_MP,
		MON_DMG,
		MON_MINDMG,
		MON_MAXDMG,
		MON_DEF,
		MON_CRI,
		MON_CRIDMG,
		MON_AVD,
		MON_EXP,
		MON_LEVEL,
		MAXMONSTAT,
		MAXMONSTERSKILL = 10,
	};
	std::tstring		MonsterName;
	std::tstring		MonsterKName;
	STAT				MonsterStat;
	UINT				MonsterSkill[MAXMONSTERSKILL];
	UINT				MonsterSkillCount;
	bool				bBoss;
	CHAR				MonsterScriptData[MAXSTRING];


	static bool LoadMonsterScriptFile(FILE* _ReadFile, std::list<MONSTERDATA>* _TempList);
	void DecodingStringData();
	void ClearData();

	void operator=(const MONSTERDATA& _Other);
	
	MONSTERDATA() 
		: MonsterStat(STAT()), MonsterName(_T("")), MonsterKName(_T(""))
		, MonsterSkillCount(0), bBoss(false)
	{
		memset(MonsterSkill, -1, sizeof(UINT)*MAXMONSTERSKILL);
		memset(MonsterScriptData, 0, sizeof(CHAR)*MAXSTRING);
	}
	MONSTERDATA(const MONSTERDATA& _Other) 
		: MonsterStat(_Other.MonsterStat)
		, MonsterName(_Other.MonsterName)
		, MonsterKName(_Other.MonsterKName)
		, MonsterSkillCount(_Other.MonsterSkillCount)
		, bBoss(_Other.bBoss)
	{
		memcpy_s(MonsterSkill, sizeof(UINT)*MAXMONSTERSKILL, _Other.MonsterSkill, sizeof(UINT)*MAXMONSTERSKILL);
		strcpy_s(MonsterScriptData, _Other.MonsterScriptData);
	}
	MONSTERDATA(CHAR* _ScriptString)
		:MonsterStat(STAT()), MonsterName(_T("")), MonsterKName(_T(""))
		, MonsterSkillCount(0)
		, bBoss(false)
	{
		memset(MonsterSkill, -1, sizeof(UINT)*MAXMONSTERSKILL);
		strcpy_s(MonsterScriptData, _ScriptString);
	}
	~MONSTERDATA(){	}
};
