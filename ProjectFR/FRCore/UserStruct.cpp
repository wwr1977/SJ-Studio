#include"stdafx.h"
#include"UserStruct.h"

ANIDATA::ANIDATA(const ANIMATIONDATA& _AnimationData)
	: InvalidData(false)
	, AniSpriteName(_T("")), AnimationName(_T(""))
	, Interval(nullptr), RepeatInterval(nullptr)
	, AnimationCount(1), Loop(true)
	, DefaultInterval(0.0f), DefaultRepeatInterval(0.0f)
{
	AniSpriteName = _AnimationData.MultiSpriteKey;
	AnimationName = _AnimationData.AniKey;

	SettingFullRage({ _AnimationData.FullRangeStart,_AnimationData.FullRangeEnd }, _AnimationData.BaseFullInterval);
	DefaultInterval = _AnimationData.BaseFullInterval;
	if ((ANISTYLE)ANI_LOOP == _AnimationData.AniStyle)
	{
		Loop = true;
		return;
	}
	Loop = false;
	SettingRepeatRage({ _AnimationData.RepeatRangeStart,_AnimationData.RepeatRangeEnd }, _AnimationData.BaseRepeatInterval);
	AnimationCount = _AnimationData.AnimationCount;
	DefaultRepeatInterval = _AnimationData.BaseRepeatInterval;

	return;
}
size_t ANIMATIONDATA::WriteData(FILE* _WriteFile)
{
	return fwrite(this, sizeof(ANIMATIONDATA), 1, _WriteFile);
}
BOOL ANIMATIONDATA::ReadAnimationData(FILE* _ReadFile, std::map<std::tstring, ANIMATIONDATA*>* _mapAnimationData) 
{
	if (nullptr == _ReadFile)
	{
		return FALSE;
	}


	size_t LoadCount = -1;
	ANIMATIONDATA AnimationData;
	memset(&AnimationData, 0, sizeof(ANIMATIONDATA));

	while (LoadCount = fread_s(&AnimationData, sizeof(AnimationData), sizeof(ANIMATIONDATA), 1, _ReadFile))
	{

		ANIMATIONDATA* NewData  = new ANIMATIONDATA(AnimationData);
		_mapAnimationData->insert(map<tstring, ANIMATIONDATA*>::value_type(AnimationData.AniKey, NewData));
	}

	return TRUE;
}
STAT STAT::operator+(const STAT& _Other)
{
	STAT NewStat;

	NewStat.Str =  Str + _Other.Str;
	NewStat.Dex = Dex + _Other.Dex;
	NewStat.Int = Int + _Other.Int;
	NewStat.Luck = Luck + _Other.Luck;
	NewStat.Heath = Heath + _Other.Heath;
	NewStat.Mana = Mana + _Other.Mana;
	NewStat.Damage = Damage + _Other.Damage;
	NewStat.MinDamage = MinDamage + _Other.MinDamage;
	NewStat.MaxDamage = MaxDamage + _Other.MaxDamage;
	NewStat.Defense = Defense + _Other.Defense;
	NewStat.Critical = Critical + _Other.Critical;
	NewStat.CriticalDamage = CriticalDamage + _Other.CriticalDamage;
	NewStat.Avoidability = Avoidability + _Other.Avoidability;

	return NewStat;
}
STAT STAT::operator-(const STAT& _Other)
{
	STAT NewStat;

	NewStat.Str = Str - _Other.Str;
	NewStat.Dex = Dex - _Other.Dex;
	NewStat.Int = Int - _Other.Int;
	NewStat.Luck = Luck - _Other.Luck;
	NewStat.Heath = Heath - _Other.Heath;
	NewStat.Mana = Mana - _Other.Mana;
	NewStat.Damage = Damage - _Other.Damage;
	NewStat.MinDamage = MinDamage - _Other.MinDamage;
	NewStat.MaxDamage = MaxDamage - _Other.MaxDamage;
	NewStat.Defense = Defense - _Other.Defense;
	NewStat.Critical = Critical - _Other.Critical;
	NewStat.CriticalDamage = CriticalDamage - _Other.CriticalDamage;
	NewStat.Avoidability = Avoidability - _Other.Avoidability;

	return NewStat;
}
void STAT::operator+=(const STAT& _Other)
{
	Str += _Other.Str;
	Dex += _Other.Dex;
	Int += _Other.Int;
	Luck += _Other.Luck;
	Heath += _Other.Heath;
	Mana += _Other.Mana;
	Damage += _Other.Damage;
	MinDamage += _Other.MinDamage;
	MaxDamage += _Other.MaxDamage;
	Defense += _Other.Defense;
	Critical += _Other.Critical;
	CriticalDamage += _Other.CriticalDamage;
	Avoidability += _Other.Avoidability;

}
void STAT::operator-=(const STAT& _Other)
{
	Str -= _Other.Str;
	Dex -= _Other.Dex;
	Int -= _Other.Int;
	Luck -= _Other.Luck;
	Heath -= _Other.Heath;
	Mana -= _Other.Mana;
	Damage -= _Other.Damage;
	MinDamage -= _Other.MinDamage;
	MaxDamage -= _Other.MaxDamage;
	Defense -= _Other.Defense;
	Critical -= _Other.Critical;
	CriticalDamage -= _Other.CriticalDamage;
	Avoidability -= _Other.Avoidability;
}
STAT STAT::AddUnit_ItemStat(const STAT& _UnitStat, const STATPARAMETERDATA& _UnitPara, const STAT& _ItemStat)
{
	STAT NewStat;

	NewStat.Str = _UnitStat.Str + _ItemStat.Str;
	NewStat.Dex = _UnitStat.Dex + _ItemStat.Dex;
	NewStat.Int = _UnitStat.Int + _ItemStat.Int;
	NewStat.Luck = _UnitStat.Luck + _ItemStat.Luck;
	NewStat.Heath = _UnitStat.Heath + _ItemStat.Heath;
	NewStat.Mana = _UnitStat.Mana + _ItemStat.Mana;
	NewStat.Damage = _UnitStat.Damage + _ItemStat.Damage;

	float Range = _UnitPara.DamageRange * 0.01f;

	NewStat.MinDamage = _UnitStat.MinDamage + (int)ceilf(_ItemStat.Damage *(1.f - Range));
	NewStat.MaxDamage = _UnitStat.MaxDamage + (int)ceilf(_ItemStat.Damage *(1.f + Range));

	NewStat.Defense = _UnitStat.Defense + _ItemStat.Defense;
	NewStat.Critical = _UnitStat.Critical + _ItemStat.Critical;
	NewStat.CriticalDamage = _UnitStat.CriticalDamage + _ItemStat.CriticalDamage;
	NewStat.Avoidability = _UnitStat.Avoidability + _ItemStat.Avoidability;

	return NewStat;
}
BOOL ANIMATIONDATA::ReadAnimationData(FILE* _ReadFile, map<tstring, ANIMATIONDATA>* _mapAnimationData) 
{
	if (nullptr == _ReadFile) 
	{
		return FALSE;
	}

	
	size_t LoadCount =-1;
	ANIMATIONDATA AnimationData;
	memset(&AnimationData, 0, sizeof(ANIMATIONDATA));

	while (LoadCount = fread_s(&AnimationData, sizeof(AnimationData), sizeof(ANIMATIONDATA), 1, _ReadFile)) 
	{
		_mapAnimationData->insert(map<tstring, ANIMATIONDATA>::value_type(AnimationData.AniKey, AnimationData));
	}

	return TRUE;
}

BOOL MULTISPRITEDATA::ReadMultiSpriteData(FILE* _ReadFile, map<tstring, MULTISPRITEDATA*>* _mapMultiSpriteData) 
{
	if (nullptr == _ReadFile)
	{
		return FALSE;
	}

	size_t LoadCount = -1;
	MULTISPRITEDATA MultiSpriteData;
	memset(&MultiSpriteData, 0, sizeof(MULTISPRITEDATA));

	while (LoadCount = fread_s(&MultiSpriteData, sizeof(MultiSpriteData), sizeof(MULTISPRITEDATA), 1, _ReadFile))
	{
		MULTISPRITEDATA* NewData = new MULTISPRITEDATA(MultiSpriteData);
		_mapMultiSpriteData->insert(map<tstring, MULTISPRITEDATA*>::value_type(MultiSpriteData.MultiSpriteKey, NewData));
	}

	return TRUE;
}

BOOL STAT::SaveDataToFile(FILE* _WriteFile) 
{
	if (nullptr == _WriteFile) 
	{
		assert(nullptr);
		return FALSE;
	}

	fwrite(this, sizeof(STAT), 1, _WriteFile);
	return TRUE;
}
BOOL STAT::LoadDataToFile(FILE* _ReadFile, vector<STAT>* _Table)
{
	if (nullptr == _ReadFile || nullptr == _Table) 
	{
		assert(nullptr);
		return FALSE;
	}

	STAT LoadData;
	fread_s(&LoadData, sizeof(LoadData), sizeof(STAT), 1, _ReadFile);
	return TRUE;
}
BOOL STATPARAMETERDATA::SaveDataToFile(FILE* _WriteFile)
{
	if (nullptr == _WriteFile)
	{
		assert(nullptr);
		return FALSE;
	}

	fwrite(this, sizeof(STATPARAMETERDATA), 1, _WriteFile);
	return TRUE;
}
BOOL STATPARAMETERDATA::LoadDataToFile(FILE* _ReadFile, map<tstring, STATPARAMETERDATA>* _Table)
{
	if (nullptr == _ReadFile || nullptr == _Table)
	{
		assert(nullptr);
		return FALSE;
	}

	STATPARAMETERDATA LoadData;
	fread_s(&LoadData, sizeof(LoadData), sizeof(STATPARAMETERDATA), 1, _ReadFile);
	return TRUE;
}
BOOL EFFECTDATA::AddSoundData(const tstring& _SoundName, const float& _SoundStartTime)
{
	if (MAXSOUNDCOUNT <= SoundCount)
	{
		return FALSE;
	}
	_tcscpy_s(SoundName[SoundCount], _countof(SoundName[SoundCount]), _SoundName.c_str());
	SoundStartTime[SoundCount] = _SoundStartTime;

	++SoundCount;
	return TRUE;
}
BOOL EFFECTDATA::DeleteSoundData(const int& _Index)
{
	if (MAXSOUNDCOUNT <= SoundCount) 
	{
		return FALSE;
	}

	for (int i = _Index; i < SoundCount - 1; ++i)
	{
		_tcscpy_s(SoundName[i], _countof(SoundName[i]), SoundName[i+1]);

	}
	
	memset(SoundName[--SoundCount], 0, sizeof(TCHAR)*MAXKEYLENGTH);
	return TRUE;
}
BOOL EFFECTDATA::SaveDataToFile(FILE* _WriteFile)
{
	if (nullptr == _WriteFile)
	{
		assert(nullptr);
		return FALSE;
	}

	fwrite(this, sizeof(EFFECTDATA), 1, _WriteFile);
	return TRUE;
}
BOOL EFFECTDATA::LoadDataToFile(FILE* _ReadFile, std::vector<EFFECTDATA>* _Vec)
{
	if (nullptr == _ReadFile || nullptr == _Vec)
	{
		assert(nullptr);
		return FALSE;
	}

	size_t LoadCount = -1;
	EFFECTDATA EffectData = EFFECTDATA();
	
	while (LoadCount = fread_s(&EffectData, sizeof(EffectData) , sizeof(EFFECTDATA) , 1, _ReadFile))
	{
		_Vec->push_back(EffectData);
	}

	return TRUE;
}
bool ACTIONSCRIPT::AddAniScript(const ANIMATIONSCRIPT& _AniScript)
{
	if (AniCount >= MAXANISCRIPTCOUNT)
	{
		return false;
	}

	AniSctipt[AniCount++] = _AniScript;
	return true;
}
bool ACTIONSCRIPT::AddCharMoveData(const MOVEDATA& _MoveData)
{
	if (MoveCount >= MAXMOVECOUNT) 
	{
		return false;
	}

	CharMoveData[MoveCount++] = _MoveData;
	return true;
}
bool ACTIONSCRIPT::AddSkillEffectData(const SKILLEFFECTDATA& _SkillEffectData)
{
	if (EffectCount >= MAXEFFECTCOUNT)
	{
		return false;
	}

	SkillEffect[EffectCount++] = _SkillEffectData;
	return true;
}
bool ACTIONSCRIPT::AddCharSoundData(const SOUNDSAVEDATA& _SoundData)
{
	if (CharSoundCount >= MAXCHARSOUNDCOUNT)
	{
		return false;
	}

	CharSound[CharSoundCount++] = _SoundData;
	return true;
}
bool ACTIONSCRIPT::AddSceneEffectData(const SCENEEFFECTDATA& _SceneEffectData)
{
	if (SceneEffectCount >= MAXSCENEEFFECTCOUNT)
	{
		return false;
	}
	SceneEffectData[SceneEffectCount++] = _SceneEffectData;
	return true;
}
bool ACTIONSCRIPT::DeleteAniScript(const UINT& _Index)
{
	if ( _Index >= MAXANISCRIPTCOUNT)
	{
		return false;
	}

	if (AniCount <= 0 || AniCount <= _Index)
	{
		return false;
	}


	--AniCount;

	for (UINT i = _Index; i < AniCount; ++i)
	{
		AniSctipt[i] = AniSctipt[i + 1];
	}

	AniSctipt[AniCount] = ANIMATIONSCRIPT();

	return true;
}
bool ACTIONSCRIPT::DeleteCharMoveData(const UINT& _Index)
{
	if ( _Index >= MAXMOVECOUNT) 
	{
		return false;
	}

	if (MoveCount <= 0 || MoveCount <= _Index)
	{
		return false;
	}

	
	--MoveCount;

	for (UINT i = _Index; i < MoveCount; ++i)
	{
		CharMoveData[i] = CharMoveData[i + 1];
	}

	CharMoveData[MoveCount] = MOVEDATA();

	return true;
}
bool ACTIONSCRIPT::DeleteSkillEffectData(const UINT& _Index)
{
	if (_Index >= MAXEFFECTCOUNT)
	{
		return false;
	}

	if (EffectCount <= 0 || EffectCount <= _Index)
	{
		return false;
	}


	--EffectCount;

	for (UINT i = _Index; i < EffectCount; ++i)
	{
		SkillEffect[i] = SkillEffect[i + 1];
	}

	SkillEffect[EffectCount] = SKILLEFFECTDATA();

	return true;
}
bool ACTIONSCRIPT::DeleteCharSoundData(const UINT& _Index)
{
	if (_Index < 0 || _Index >= MAXCHARSOUNDCOUNT)
	{
		return false;
	}

	if (CharSoundCount <= 0 || CharSoundCount <= _Index)
	{
		return false;
	}

	--CharSoundCount;

	for (UINT i = _Index; i < CharSoundCount; ++i)
	{
		CharSound[i] = CharSound[i + 1];
	}

	CharSound[CharSoundCount] = SOUNDSAVEDATA();

	return true;
}
bool ACTIONSCRIPT::DeleteSceneEffectData(const UINT& _Index)
{
	if (_Index < 0 || _Index >= MAXSCENEEFFECTCOUNT)
	{
		return false;
	}

	if (SceneEffectCount <= 0 || SceneEffectCount <= _Index)
	{
		return false;
	}

	--SceneEffectCount;

	for (UINT i = _Index; i < SceneEffectCount; ++i)
	{
		SceneEffectData[i] = SceneEffectData[i + 1];
	}

	SceneEffectData[SceneEffectCount] = SCENEEFFECTDATA();

	return true;
}
bool  ACTIONSCRIPT::isEmpty() const 
{
	if (0 >= AniCount && 0 >= MoveCount && 0 >= EffectCount && 0 >= CharSoundCount && 0 >= SceneEffectCount)
		return true;

	return false;
}
SoundData::SoundData(const SOUNDSAVEDATA& _SaveData)
	:SoundName(_SaveData.SoundName), StartTime(_SaveData.PlayStartTime), Playing(false)
{
}
void SOUNDSAVEDATA::SetSoundName(const std::tstring& _SoundName)
{
	_tcscpy_s(SoundName, _countof(SoundName), _SoundName.c_str());
}

void ACTIONSCRIPT::operator=(const ACTIONSCRIPT& _Other)
{
	SkillNo = _Other.SkillNo;
	CharIndex = _Other.CharIndex;
	AniCount = _Other.AniCount;
	MoveCount = _Other.MoveCount;
	CharSoundCount = _Other.CharSoundCount;
	EffectCount = _Other.EffectCount;
	SceneEffectCount = _Other.SceneEffectCount;
	TotalSkillTime = _Other.TotalSkillTime;
	
	//memcpy(this, &_Other, sizeof(ACTIONSCRIPT));
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
void MOVEDATA::SetMoveData(const float& _StartTime, const float& _MoveTime, const Vec2& _StartPos, const Vec2& _EndPos) 
{
	MoveMode = MOVEMODE_MOVE;
	MoveStartTime = _StartTime;
	MoveTime = _MoveTime;
	HideTime = 0.0f;
	MoveStartPos = _StartPos;
	MoveEndPos = _EndPos;
	bSetTeamPos = false;
	bSetInitPos = false;
}
void MOVEDATA::SetHideData(const float& _StartTime, const float& _HideTime) 
{
	MoveMode = MOVEMODE_HIDE;
	MoveStartTime = _StartTime;
	MoveTime = _HideTime;
	MoveStartPos = Vec2{0.0f,0.0f};
	MoveEndPos = Vec2{ 0.0f,0.0f };
	HideTime = _HideTime;
	bSetTeamPos = false;
	bSetInitPos = false;
}
void MOVEDATA::SetShowData(const float& _StartTime, const float& _HideTime) 
{
	MoveMode = MOVEMODE_SHOW;
	MoveStartTime = _StartTime;
	MoveTime = _HideTime;
	MoveStartPos = Vec2{ 0.0f,0.0f };
	MoveEndPos = Vec2{ 0.0f,0.0f };
	HideTime = _HideTime;
	bSetTeamPos = false;
	bSetInitPos = false;
}
void MOVEDATA::SetTeamPos(const float& _SetTime)
{
	MoveStartTime = _SetTime;
	MoveTime = 0.1f;
	HideTime = 0.0f;
	MoveStartPos = Vec2{ 0.0f,0.0f };
	MoveEndPos = Vec2{ 0.0f,0.0f };
	bSetTeamPos = true;
	bSetInitPos = false;
	MoveMode = MOVEMODE_MOVE;
}
void MOVEDATA::SetInitPos(const float& _SetTime)
{
	MoveStartTime = _SetTime;
	MoveTime = 0.1f;
	HideTime = 0.0f;
	MoveStartPos = Vec2{ 0.0f,0.0f };
	MoveEndPos = Vec2{ 0.0f,0.0f };
	bSetTeamPos = false;
	bSetInitPos = true;
	MoveMode = MOVEMODE_MOVE;
}

bool SKILLEFFECTDATA::SetInitPos(const Vec2& _InitPos) 
{
	InitPosMode = INITPOS_CUSTOM;
	InitPos = _InitPos;
	return true;
}
bool SKILLEFFECTDATA::SetInitPos(const int& _InitPosMode, const Vec2& _InitPos /* = Vec2{ 0.0f,0.0f }*/) 
{
	if (_InitPosMode < INITPOS_CASTERPOS || _InitPosMode >= MAXINITPOS)
	{
		return false;
	}

	InitPosMode = _InitPosMode;
	InitPos = _InitPos;
	return true;
}

void HITDATA::operator=(const HITDATA&  _Other) 
{
	StartTime = _Other.StartTime;
	HitCount = _Other.HitCount;

	DemageFontMag = _Other.DemageFontMag;
	HitDemageMag = _Other.HitDemageMag;
	FixHitDemage = _Other.FixHitDemage;

	HitEffectIndex = _Other.HitEffectIndex;
	HitEffectCustomSize = _Other.HitEffectCustomSize;
	HitEffectImageMag = _Other.HitEffectImageMag;
	HitEffectPivot = _Other.HitEffectPivot;

	HitSoundIndex = _Other.HitSoundIndex;
	ReActionIndex = _Other.ReActionIndex;
	
	PosOption = _Other.PosOption;
	Apply = _Other.Apply;
}

void LEVELDATA::operator=(const LEVELDATA& _Other)
{
	SkillNo = _Other.SkillNo;
	SkillCount = _Other.SkillCount;
	DemageStyle = _Other.DemageStyle;
	Option = _Other.Option;

	memset(SkillCost, 0, sizeof(UINT)*MAXSKILLCOST);

	for (size_t i = 0; i < MAXSKILLCOST; i++)
	{
		SkillCost[i] = _Other.SkillCost[i];
	}

	ClearHitData();

	for (size_t i = 0; i < _Other.HitDataCount; ++i)
		AddHitData(_Other.HitData[i]);
	
}
void LEVELDATA::ClearHitData()
{
	//for (size_t i = 0; i < MAXHITCOUNT; i++)
	//	HitData[i] = HITDATA();

	//HitDataCount = 0;

	while (HitDataCount > 0)
		DelHitData(0);
	
}
void SKILLDATA::operator=(const SKILLDATA& _Other)
{
	SkillNo = _Other.SkillNo;
	SkillType = _Other.SkillType;
	TeamTargetType = _Other.TeamTargetType;
	EnemyTargetType = _Other.EnemyTargetType;
	CharacterSkill = _Other.CharacterSkill;
	SkillBuffType = _Other.SkillBuffType;
	InitPosMode = _Other.InitPosMode;
	InitFixPos = _Other.InitFixPos;
	InitPosPivot = _Other.InitPosPivot;
	
}

bool LEVELDATA::AddHitData(const HITDATA& _Data)
{
	if (HitDataCount >= MAXHITCOUNT) 
		return false;
	

	HitData[HitDataCount++] = _Data;
	return true;
}
bool LEVELDATA::DelHitData(const int& _Index)
{
	if ((UINT)_Index >= HitDataCount)
		return false;

	HitData[_Index] = HITDATA();

	for (UINT i = (UINT)_Index; i < HitDataCount - 1; ++i)
		SwapHitData(i, i + 1);

	--HitDataCount;
	return true;
}
bool LEVELDATA::SwapHitData(const int& _a, const int& _b)
{
	if ((UINT)_a >= HitDataCount || (UINT)_b >= HitDataCount)
		return false;

	HITDATA Temp(HitData[_a]);

	HitData[_a] = HitData[_b];
	HitData[_b] = Temp;
	return true;
}
void LEVELDATA::SortHitData()
{
	std::vector<HITDATA> Temp;

	Temp.clear();

	for (size_t i = 0; i < HitDataCount; i++)
		Temp.push_back(HitData[i]);
	
	sort(Temp.begin(), Temp.end());

	ClearHitData();

	for (size_t i = 0; i < HitDataCount; i++)
		AddHitData(Temp[i]);

	Temp.clear();
}
bool LEVELDATA::SetSkillCost(const size_t& _CostType, const size_t& _CostValue)
{
	if (_CostType >= MAXSKILLCOST)
	{
		return false;
	}

	SkillCost[_CostType] = (unsigned int)_CostValue;
	return true;
}
bool HITDATA::SetHitEffect(const size_t& _EffectIndex)
{
	HitEffectIndex = (unsigned int)_EffectIndex;
	return true;
}
void HITDATA::InitHitEffect()
{
	HitEffectIndex = -1;
}
void HITDATA::InitHitSound() 
{
	HitSoundIndex = -1;
}
void HITDATA::InitReAction() 
{
	ReActionIndex = -1;
}
bool HITDATA::SetHitSound(const size_t& _SoundIndex) 
{
	if (_SoundIndex >= MAXHITSOUND) 
		return false;
	
	HitSoundIndex = (unsigned int)_SoundIndex;
	return true;
}
bool HITDATA::SetHitReAction(const size_t& _ReActionIndex) 
{
	if (_ReActionIndex >= MAXREACTION)
		return false;
	
	ReActionIndex = (unsigned int)_ReActionIndex;
	return true;
}
bool HITDATA::operator>(const HITDATA& _Other) 
{
	return StartTime > _Other.StartTime;
}
bool HITDATA::operator>=(const HITDATA& _Other) 
{
	return StartTime >= _Other.StartTime;
}
bool HITDATA::operator<(const HITDATA& _Other) 
{
	return StartTime < _Other.StartTime;
}
bool HITDATA::operator<=(const HITDATA& _Other) 
{
	return StartTime <= _Other.StartTime;
}
void BUFFDATA::operator=(const BUFFDATA& _Other) 
{
	BuffType = _Other.BuffType;
	BuffValue = _Other.BuffValue;
	BuffCount = _Other.BuffCount;
	BuffIconIndex = _Other.BuffIconIndex;
	BuffEffectIndex = _Other.BuffEffectIndex;
}
void SKILLUIDATA::operator=(const SKILLUIDATA& _Other)
{
	SkillNo = _Other.SkillNo;
	SkillIconIndex = _Other.SkillIconIndex;
	SkillName = _Other.SkillName;
	SkillKName = _Other.SkillKName;
	SkillDescription = _Other.SkillDescription;

	memset(SkillUISaveData, 0, sizeof(CHAR)*MAXSTRING);
	strcpy_s(SkillUISaveData, _Other.SkillUISaveData);
}
void SKILLUIDATA::SetSkillSaveData()
{
	string SName = CW2A(SkillName.c_str(), CP_ACP);
	string KName = CW2A(SkillKName.c_str(), CP_ACP);
	string SDesc = CW2A(SkillDescription.c_str(), CP_ACP);

	sprintf_s(SkillUISaveData, "[%u][%u]%s$%s$%s"
		, SkillNo, SkillIconIndex
		, SName.c_str()
		, KName.c_str()
		, SDesc.c_str()
	);

	return;
}
void SKILLUIDATA::ApplySaveData()
{
	tstring UISaveData = CA2W(SkillUISaveData);
	tstring Temp = _T("");
	size_t Start = 0;
	size_t End = 0;

	Start = UISaveData.find_first_of('[');
	End = UISaveData.find_first_of(']');

	Temp.assign(UISaveData, Start + 1, End - Start - 1);
	SkillNo = _wtoi(Temp.c_str());
	
	UISaveData.replace(Start, End - Start + 1,_T(""));
	
	Start = UISaveData.find_first_of('[');
	End = UISaveData.find_first_of(']');

	Temp.assign(UISaveData, Start + 1, End - Start - 1);
	SkillIconIndex = _wtoi(Temp.c_str());

	UISaveData.replace(Start, End - Start + 1, _T(""));

	Start = 0;
	End = UISaveData.find_first_of('$');
	SkillName.clear();
	SkillName.assign(UISaveData, Start, End );
	UISaveData.replace(Start, End - Start + 1, _T(""));

	End = UISaveData.find_first_of('$');
	SkillKName.clear();
	SkillKName.assign(UISaveData, Start, End);
	UISaveData.replace(Start, End - Start + 1, _T(""));

	SkillDescription = UISaveData;

}
void SKILLUIDATA::ClearData()
{
	SkillNo = -1;
	SkillIconIndex = -1;
	SkillName = _T("");
	SkillKName = _T("");
	SkillDescription = _T("");
}
BOOL SKILLUIDATA::SaveDataToFile(FILE* _WriteFile)
{
	if (nullptr == _WriteFile) 
	{
		return FALSE;
	}


	fwrite(SkillUISaveData, sizeof(SkillUISaveData), 1, _WriteFile);
	return TRUE;
}
BOOL  SKILLUIDATA::LoadTextFile(FILE* _ReadFile, std::list<SKILLUIDATA>* _TempVec)
{
	if (nullptr == _ReadFile || nullptr == _TempVec)
		return FALSE;


	CHAR Buf[MAXSTRING];

	while (!feof(_ReadFile)) 
	{
		fgets(Buf, MAXSTRING, _ReadFile);
		SKILLUIDATA TempData(Buf);

		TempData.ApplySaveData();
		_TempVec->push_back(TempData);
	}

	return TRUE;
}
void SKILL::operator=(const SKILL& _Other)
{
	SkillNo = _Other.SkillNo;
	SkillActionScript = _Other.SkillActionScript;
	SkillData = _Other.SkillData;
	SkillUIData = _Other.SkillUIData;

	SetSkillName(_Other.SkillName);
}
SKILL::SKILL(const size_t& _SkillNo, const std::tstring& _SkillName
	, const ACTIONSCRIPT& _Action
	, const SKILLDATA& _SkillData
	, const SKILLUIDATA& _UIData)
	:SkillNo(0), SkillName(_T(""))
	, SkillActionScript(_Action)
	, SkillData(_SkillData)
	, SkillUIData(_UIData)
	, CurSkillLevel(0)
	, CurSkillExp(0)
{
	SkillNo = (unsigned int)_SkillNo;
	SetSkillName(_SkillName);
}
void SKILL::SetSkillName(const std::tstring& _SkillName)
{
	_tcscpy_s(SkillName, _countof(SkillName), _SkillName.c_str());
}
const unsigned int	 SKILL::GetUnitIndex()
{
	return SkillActionScript.CharIndex;
}
void UNITSKILLDATA::operator=(const UNITSKILLDATA& _Other)
{
	SkillNo = _Other.SkillNo;
	SkillLevel = _Other.SkillLevel;
}

bool SCENEEFFECTDATA::SetEarthEffect(const DWORD& _Flag, const float& _StartTime, const float& _EffectTime, const  Vec2& _SpaceSize) 
{
	if (nullptr == &_Flag || nullptr == &_StartTime || nullptr == &_EffectTime || nullptr == &_SpaceSize)
		return false;

	DWORD Flag = _Flag;

	if ((Flag & 0xFF000000L) != SCENE_EFFECT_EARTHQUAKE && _EffectTime <= 0.0f)
		return false;

	SceneEffectFlag = _Flag;
	SceneEffectStartTime = _StartTime;
	SceneEffectTime = _EffectTime;
	Earthquake_SpaceSize = _SpaceSize;
	SubTime = 0.0f;
	ZoomPosMode = 0;
	ZoomPos = Vec3{0.0f,0.0f,0.0f};
	ZoomRatio = Vec2{0.0f,0.0f};

	return true;
}
bool SCENEEFFECTDATA::SetBlackOutEffect(const DWORD& _Flag, const float& _StartTime, const float& _EffectTime) 
{
	if (nullptr == &_Flag || nullptr == &_StartTime || nullptr == &_EffectTime )
		return false;

	DWORD Flag = _Flag;

	if ( (Flag & 0xFF000000L) != SCENE_EFFECT_BLACKOUT &&  (Flag & 0xFF000000L) != SCENE_EFFECT_FLASH)
		return false;

	if(_EffectTime <=  0.0f)
		return false;

	SceneEffectFlag = _Flag;
	SceneEffectStartTime = _StartTime;
	SceneEffectTime = _EffectTime;
	Earthquake_SpaceSize = Vec2{0.0f,0.0f};
	SubTime = 0.0f;
	ZoomPosMode = 0;
	ZoomPos = Vec3{ 0.0f,0.0f,0.0f };
	ZoomRatio = Vec2{ 0.0f,0.0f };

	return true;
}
bool SCENEEFFECTDATA::SetFadeEffect(const DWORD& _Flag, const float& _StartTime, const float& _EffectTime, const float& _SubTime) 
{
	if (nullptr == &_Flag || nullptr == &_StartTime || nullptr == &_EffectTime || nullptr == &_SubTime)
		return false;

	DWORD Flag = _Flag;
	if ((Flag & 0xFF000000L) != SCENE_EFFECT_FADE_IN && (Flag & 0xFF000000L) != SCENE_EFFECT_FADE_OUT)
		return false;

	if (_EffectTime <= 0.0f || _SubTime <= 0.0f)
		return false;

	SceneEffectFlag = _Flag;
	SceneEffectStartTime = _StartTime;
	SceneEffectTime = _EffectTime;
	SubTime = _SubTime;
	Earthquake_SpaceSize = Vec2{ 0.0f,0.0f };
	ZoomPosMode = 0;
	ZoomPos = Vec3{ 0.0f,0.0f,0.0f };
	ZoomRatio = Vec2{ 0.0f,0.0f };

	return true;
}
bool SCENEEFFECTDATA::SetZoomEffect(const DWORD& _Flag, const float& _StartTime, const float& _EffectTime, const float& _SubTime, const UINT& _ZoomMode, const Vec3& _ZoomPos, const Vec2& _ZoomRatio)
{
	if (nullptr == &_Flag || nullptr == &_StartTime || nullptr == &_EffectTime || nullptr == &_SubTime)
		return false;

	DWORD Flag = _Flag;
	if ((Flag & 0xFF000000L) != SCENE_EFFECT_ZOOM_IN && (Flag & 0xFF000000L) != SCENE_EFFECT_ZOOM_OUT)
		return false;

	if (_EffectTime <= 0.0f || _SubTime <= 0.0f)
		return false;

	SceneEffectFlag = _Flag;
	SceneEffectStartTime = _StartTime;
	SceneEffectTime = _EffectTime;
	SubTime = _SubTime;
	ZoomPosMode = _ZoomMode;
	ZoomPos = _ZoomPos;
	ZoomRatio = _ZoomRatio;
	Earthquake_SpaceSize = Vec2{0.0f,0.0f};

	return true;
}
void SKILLSAVEDATA::operator=(const SKILLSAVEDATA& _Other)
{
	ActionScript = _Other.ActionScript;
	SkillData = _Other.SkillData;
}
size_t SKILLSAVEDATA::WriteData(FILE* _WriteFile)
{
	if (nullptr == _WriteFile)
		return -1;

	return fwrite(this, sizeof(SKILLSAVEDATA), 1, _WriteFile);
}
SKILLSAVEDATA::SKILLSAVEDATA(const SKILL& _Skill)
{
	ActionScript = _Skill.SkillActionScript;
	SkillData = _Skill.SkillData;
}

SAVELEVELDATA::SAVELEVELDATA(std::vector<LEVELDATA>* _LevelData)
{
	if (nullptr == _LevelData)
		return;

	if (5 != _LevelData->size())
		return;

	SkillNo = (*_LevelData)[0].SkillNo;

	for (size_t i = 0; i < LEVELDATA::MAXSKILLLEVEL; i++)
		LevelData[i] = (*_LevelData)[i];
}
void SAVELEVELDATA::operator=(const SAVELEVELDATA& _Other)
{
	SkillNo = _Other.SkillNo;

	for (size_t i = 0; i < LEVELDATA::MAXSKILLLEVEL; i++)
		LevelData[i] = _Other.LevelData[i];
}
SAVELEVELDATA::SAVELEVELDATA(const SAVELEVELDATA& _Other)
{
	SkillNo = _Other.SkillNo;
	for (size_t i = 0; i < LEVELDATA::MAXSKILLLEVEL; i++)
		LevelData[i] = _Other.LevelData[i];

}
SAVELEVELDATA::SAVELEVELDATA()
	:SkillNo(0)
{
	for (size_t i = 0; i < LEVELDATA::MAXSKILLLEVEL; i++)
		LevelData[i] = LEVELDATA();
}

size_t SAVELEVELDATA::LoadDataFile(FILE* _ReadFile)
{
	if (nullptr == _ReadFile)
		return 0;

	size_t ReadCount =  fread_s(LevelData, sizeof(LevelData), sizeof(LEVELDATA), LEVELDATA::MAXSKILLLEVEL, _ReadFile);
	SkillNo = LevelData[0].SkillNo;
	
	return ReadCount;
}
size_t SAVELEVELDATA::SaveDataFile(FILE* _WriteFile)
{
	if (nullptr == _WriteFile)
		return 0;

	return fwrite(LevelData,sizeof(LEVELDATA) , LEVELDATA::MAXSKILLLEVEL, _WriteFile);
}
void DIRSET::operator=(const DIRSET& _Other)
{
	Dir = _Other.Dir;
	DirString = _Other.DirString;
}
void TILEMAPDATA::SetTileMapName(const std::tstring& _Name)
{
	_tcscpy_s(TileMapName, _countof(TileMapName), _Name.c_str());
}
size_t TILEMAPDATA::WriteTileMapData(FILE* _WriteFile)
{
	if (nullptr == _WriteFile)
		return 0;

	return fwrite(this, sizeof(TILEMAPDATA), 1, _WriteFile);
}
size_t TILEMAPDATA::ReadTileMapData(FILE* _ReadFile)
{
	if (nullptr == _ReadFile )
		return 0;

	return fread_s(this, sizeof(*this), sizeof(TILEMAPDATA), 1, _ReadFile);
}
void TILEMAPDATA::operator=(const TILEMAPDATA& _Other)
{
	TileMapNo = _Other.TileMapNo;
	_tcscpy_s(TileMapName, _countof(TileMapName), _Other.TileMapName);
	TileSizeX = _Other.TileSizeX;
	TileSizeY = _Other.TileSizeY;
}
TILEMAPDATA::TILEMAPDATA(const TILEMAPDATA& _Other)
{
	TileMapNo = _Other.TileMapNo;
	_tcscpy_s(TileMapName, _countof(TileMapName), _Other.TileMapName);
	TileSizeX = _Other.TileSizeX;
	TileSizeY = _Other.TileSizeY;
}
bool MONSTERDATA::LoadMonsterScriptFile(FILE* _ReadFile, std::list<MONSTERDATA>* _TempList)
{
	if (nullptr == _ReadFile || nullptr == _TempList)
		return FALSE;


	CHAR Buf[MAXSTRING];
	//처음 한줄은 주석용이므로 제외한다.
	fgets(Buf, MAXSTRING, _ReadFile);

	while (!feof(_ReadFile))
	{
		fgets(Buf, MAXSTRING, _ReadFile);

		MONSTERDATA TempData(Buf);

		TempData.DecodingStringData();
		_TempList->push_back(TempData);
	}

	return TRUE;
}
void MONSTERDATA::operator=(const MONSTERDATA& _Other)
{
	MonsterName = _Other.MonsterName;
	MonsterKName = _Other.MonsterKName;
	MonsterStat = _Other.MonsterStat;
	MonsterSkillCount = _Other.MonsterSkillCount;
	memcpy_s(MonsterSkill, sizeof(UINT)*MAXMONSTERSKILL, _Other.MonsterSkill, sizeof(UINT)*MAXMONSTERSKILL);
	strcpy_s(MonsterScriptData, _Other.MonsterScriptData);
}
void MONSTERDATA::DecodingStringData()
{
	ClearData();
	//string Temp = MonsterScriptData;
	//
	//tstring Test(Temp.begin(), Temp.end());

	tstring MonsterScript = CA2W(MonsterScriptData);
	tstring Temp = _T("");

	size_t Start = 0;
	size_t End = 0;

	Start = MonsterScript.find_first_of('[');
	End = MonsterScript.find_first_of(']');

	// 1. 몬스터 영어명
	Temp.assign(MonsterScript, Start + 1, End - Start - 1);
	MonsterName = Temp;


	MonsterScript.replace(Start, End - Start + 1, _T(""));

	Start = MonsterScript.find_first_of('[');
	End = MonsterScript.find_first_of(']');

	// 2. 몬스터 한글명
	Temp.assign(MonsterScript, Start + 1, End - Start - 1);
	MonsterKName = Temp;

	MonsterScript.replace(Start, End - Start + 1, _T(""));

	Start = MonsterScript.find_first_of('[');
	End = MonsterScript.find_first_of(']');

	Temp.assign(MonsterScript, Start + 1, End - Start - 1);
	MonsterScript.replace(Start, End - Start + 1, _T(""));

	// 3. 몬스터 스텟정보
	size_t Find = -1;
	tstring Str;
	UINT No;

	for (size_t i = 0; i < MAXMONSTAT; i++)
	{
		Find = Temp.find(',');

		if (-1 == Find)
		{
			Str.assign(Temp, 0, Temp.size());
			No = _wtoi(Str.c_str());
			MonsterStat.Level = No;
			break;
		}

		Str.assign(Temp, 0, Find);
		No = _wtoi(Str.c_str());
		Temp.replace(0, Find + 1, _T(""));
		switch (i)
		{
		case MONSTERDATA::MON_STR:MonsterStat.Str = No;	break;
		case MONSTERDATA::MON_DEX:MonsterStat.Dex = No;	break;
		case MONSTERDATA::MON_INT:MonsterStat.Int = No;	break;
		case MONSTERDATA::MON_LUK:MonsterStat.Luck = No;break;
		case MONSTERDATA::MON_HP:MonsterStat.Heath = No;break;
		case MONSTERDATA::MON_MP:MonsterStat.Mana = No;	break;
		case MONSTERDATA::MON_DMG:MonsterStat.Damage = No;break;
		case MONSTERDATA::MON_MINDMG:MonsterStat.MinDamage = No;break;
		case MONSTERDATA::MON_MAXDMG:MonsterStat.MaxDamage = No;break;
		case MONSTERDATA::MON_DEF:MonsterStat.Defense = No;break;
		case MONSTERDATA::MON_CRI:MonsterStat.Critical = No;break;
		case MONSTERDATA::MON_CRIDMG:MonsterStat.CriticalDamage = No;break;
		case MONSTERDATA::MON_AVD:MonsterStat.Avoidability = No;break;
		case MONSTERDATA::MON_EXP:MonsterStat.Exp = No;	break;
		case MONSTERDATA::MON_LEVEL:MonsterStat.Level = No;	break;
		}
	}
	
	Start = MonsterScript.find_first_of('[');
	End = MonsterScript.find_first_of(']');

	// 4. 몬스터 스킬
	Temp.assign(MonsterScript, Start + 1, End - Start - 1);
	MonsterScript.replace(Start, End - Start + 1, _T(""));

	Find = 0;

	/*if (-1 == Temp.find(','))
		return;
*/
	while (true) 
	{
		Find = Find = Temp.find(',');
		if (-1 == Find) 
		{
			Str.assign(Temp, 0, Temp.size());
			No = _wtoi(Str.c_str());
			MonsterSkill[MonsterSkillCount++] = No;
			break;
		}

		Str.assign(Temp, 0, Find);
		No = _wtoi(Str.c_str());
		Temp.replace(0, Find + 1, _T(""));
		MonsterSkill[MonsterSkillCount++] = No;
	}

	if (MonsterScript == _T("\r\n"))
		return;

	Start = MonsterScript.find_first_of('[');
	End = MonsterScript.find_first_of(']');

	// 5. Boss Monster 
	Temp.assign(MonsterScript, Start + 1, End - Start - 1);
	
	if (Temp == _T("Boss"))
		bBoss = true;

	//SkillDescription = UISaveData;
	return;
}
void MONSTERDATA::ClearData() 
{
	MonsterName = _T("");
	MonsterKName = _T("");
	MonsterStat = STAT();
	memset(MonsterSkill, -1, sizeof(UINT)*MAXMONSTERSKILL);
	MonsterSkillCount = 0;
}
void TILEDATA::operator=(const TILEDATA& _Other)
{
	TileType = _Other.TileType;

	TileXIndex = _Other.TileXIndex;
	TileYIndex = _Other.TileYIndex;

	TileColorNo = _Other.TileColorNo;

	ChangeTileMapNo = _Other.ChangeTileMapNo;
	SpawnTileXIndex = _Other.SpawnTileXIndex;
	SpawnTileYIndex = _Other.SpawnTileYIndex;

	WorldMapOption = _Other.WorldMapOption;
}
size_t TILEDATA::ReadTileMapData(FILE* _ReadFile, TILEDATA* _Buf, const size_t& _ReadCount)
{
	if (nullptr == _ReadFile || nullptr == _Buf)
		return 0;

	size_t DataSize = sizeof(TILEDATA);
	
	return fread_s(_Buf, DataSize*_ReadCount, DataSize, _ReadCount, _ReadFile);
}
size_t TILEDATA::WriteTileData(FILE* _WriteFile, TILEDATA* _Buf, const size_t& _WriteCount)
{
	if (nullptr == _WriteFile || nullptr == _Buf)
		return -1;

	return fwrite(_Buf, sizeof(TILEDATA), _WriteCount, _WriteFile);
}
TILEDATA::TILEDATA(const TILEDATA& _Other)
{
	TileType = _Other.TileType;

	TileXIndex = _Other.TileXIndex;
	TileYIndex = _Other.TileYIndex;

	TileColorNo = _Other.TileColorNo;

	ChangeTileMapNo = _Other.ChangeTileMapNo;
	SpawnTileXIndex = _Other.SpawnTileXIndex;
	SpawnTileYIndex = _Other.SpawnTileYIndex;

	WorldMapOption = _Other.WorldMapOption;
}
void MAPOBJECTDATA::operator=(const MAPOBJECTDATA& _Other)
{
	TileMapNo = _Other.TileMapNo;
	MapObjectNo = _Other.MapObjectNo;
	LeftIndex = _Other.LeftIndex;
	BottomIndex = _Other.BottomIndex;
}
size_t MAPOBJECTDATA::WriteData(FILE* _WriteFile, MAPOBJECTDATA* _Buf, const UINT& _WriteCount)
{
	if (nullptr == _WriteFile || nullptr == _Buf)
		return 0;

	return fwrite(_Buf, sizeof(MAPOBJECTDATA) ,_WriteCount, _WriteFile);
}
size_t MAPOBJECTDATA::ReadData(FILE* _ReadFile, MAPOBJECTDATA* _Buf, const size_t& BufSize,const UINT& _ReadCount)
{
	if (nullptr == _ReadFile || nullptr == _Buf)
		return 0;

	return  fread_s(_Buf, BufSize, sizeof(MAPOBJECTDATA), _ReadCount, _ReadFile);
}
