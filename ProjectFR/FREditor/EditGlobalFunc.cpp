#include "stdafx.h"
#include "EditGlobalFunc.h"
#include "StatSceneBuilder.h"
#include "RootDlg.h"
#include "SkillDlg.h"
#include "SkillMgr.h"
#include "ActionScriptDlg.h"
#include "CharStatDlg.h"

tstring Idle(const tstring& _UnitName)
{
	return _UnitName + _T("_idle");
}
tstring Atk(const tstring& _UnitName)
{
	if (true == CEditGlobal::CheckDoubleAtkCharacter(_UnitName))
	{
		return _UnitName + _T("_atk1");
	}
	return _UnitName + _T("_atk");

}
tstring Atk2(const tstring& _UnitName)
{
	if (true == CEditGlobal::CheckDoubleAtkCharacter(_UnitName))
	{
		return _UnitName + _T("_atk2");
	}
	return _UnitName + _T("_atk");
}
tstring DAtk(const tstring& _UnitName)
{
	if (true == CEditGlobal::CheckDoubleAtkCharacter(_UnitName))
	{
		return _UnitName + _T("_DoubleAtk");
	}
	return _UnitName + _T("_atk");

}
tstring Dead(const tstring& _UnitName)
{
	return _UnitName + _T("_dead");
}
tstring Dying(const tstring& _UnitName)
{
	return _UnitName + _T("_dying");
}
tstring Jump(const tstring& _UnitName)
{
	return _UnitName + _T("_jump");
}
tstring Move(const tstring& _UnitName)
{
	return _UnitName + _T("_move");
}
tstring Limit(const tstring& _UnitName)
{
	return _UnitName + _T("_limit_atk");
}
tstring MAtk(const tstring& _UnitName)
{
	return _UnitName + _T("_magic_atk");
}
tstring MStand(const tstring& _UnitName)
{
	return _UnitName + _T("_magic_standby");
}
tstring Stand(const tstring& _UnitName)
{
	return _UnitName + _T("_standby");
}
tstring Win(const tstring& _UnitName)
{
	return _UnitName + _T("_win");
}
tstring WinBe(const tstring& _UnitName)
{
	return _UnitName + _T("_win_before");
}
tstring Icon(const tstring& _UnitName)
{
	return _T("unit_icon_") + _UnitName;
}
tstring Frame(const tstring& _UnitName)
{
	return _T("Frame_") + _UnitName;
}
tstring Illust(const tstring& _UnitName)
{
	return _T("unit_ills_") + _UnitName;
}

ANIMATIONDATA* ANI(const tstring& _AniKey)
{
	return CEditGlobal::GetAniData(_AniKey);
}
ANIMATIONDATA* Ani_Idle(const tstring& _Name)
{
	return CEditGlobal::GetAniData(_Name + _T("_idle"));
}
ANIMATIONDATA* Ani_Atk(const tstring& _Name)
{
	return CEditGlobal::GetAniData(_Name + _T("_atk"));
}
ANIMATIONDATA* Ani_Atk1(const tstring& _Name)
{
	return CEditGlobal::GetAniData(_Name + _T("_atk1"));
}
ANIMATIONDATA* Ani_Atk2(const tstring& _Name)
{
	return CEditGlobal::GetAniData(_Name + _T("_atk2"));
}
ANIMATIONDATA* Ani_DAtk(const tstring& _Name)
{
	return CEditGlobal::GetAniData(_Name + _T("_DoubleAtk"));
}
ANIMATIONDATA* Ani_Limit(const tstring& _Name)
{
	return CEditGlobal::GetAniData(_Name + _T("_limit_atk"));
}
ANIMATIONDATA* Ani_Dead(const tstring& _Name)
{
	return CEditGlobal::GetAniData(_Name + _T("_dead"));
}
ANIMATIONDATA* Ani_Dying(const tstring& _Name)
{
	return CEditGlobal::GetAniData(_Name + _T("_dying"));
}
ANIMATIONDATA* Ani_Move(const tstring& _Name)
{
	return CEditGlobal::GetAniData(_Name + _T("_move"));
}
ANIMATIONDATA* Ani_Jump(const tstring& _Name)
{
	return CEditGlobal::GetAniData(_Name + _T("_jump"));
}
ANIMATIONDATA* Ani_MAtk(const tstring& _Name)
{
	return CEditGlobal::GetAniData(_Name + _T("_magic_atk"));
}
ANIMATIONDATA* Ani_MStand(const tstring& _Name)
{
	return CEditGlobal::GetAniData(_Name + _T("_magic_standby"));
}
ANIMATIONDATA* Ani_Stand(const tstring& _Name)
{
	return CEditGlobal::GetAniData(_Name + _T("_standby"));
}
ANIMATIONDATA* Ani_Win(const tstring& _Name)
{
	return CEditGlobal::GetAniData(_Name + _T("_win"));
}
ANIMATIONDATA* Ani_WinBe(const tstring& _Name)
{
	return CEditGlobal::GetAniData(_Name + _T("_win_before"));
}
POINT operator+(const POINT& _Left, const POINT& _Right)
{
	LONG X = _Left.x + _Right.x;
	LONG Y = _Left.y + _Right.y;
	return POINT{ X, Y };
}
POINT operator-(const POINT& _Left, const POINT& _Right)
{
	LONG X = _Left.x - _Right.x;
	LONG Y = _Left.y - _Right.y;
	return POINT{ X, Y };
}

tstring GetAnimationName(const ANIMATIONNAME& _AniNameIndex, const tstring& _CharName)
{
	if (false == CEditGlobal::CheckCharName(_CharName))
	{
		return _T("");
	}

	switch (_AniNameIndex)
	{
	case ANINAME_IDLE:
		return Idle(_CharName);
	case ANINAME_STAND:
		return Stand(_CharName);
	case ANINAME_MOVE:
		return Move(_CharName);
	case ANINAME_ATTACK:
		return Atk(_CharName);
	case ANINAME_LIMIT:
		return Limit(_CharName);
	case ANINAME_DEAD:
		return Dead(_CharName);
	case ANINAME_DYING:
		return Dying(_CharName);
	case ANINAME_MAGICATTACK:
		return MAtk(_CharName);
	case ANINAME_MAGICSTAND:
		return MStand(_CharName);
	case ANINAME_WIN:
		return Win(_CharName);
	case ANINAME_WIN_BEFORE:
		return WinBe(_CharName);
	case ANINAME_ATTACK2:
		return Atk2(_CharName);
	case ANINAME_DOUBLEATTACK:
		return DAtk(_CharName);
	}
	return _T("");
}

ANIMATIONNAME GetAniNameIndex(const tstring& _CharAniName)
{
	size_t Index = _CharAniName.find_first_of(_T("_"));

	if (-1 == Index)
	{
		return MAXANINAME;
	}

	tstring AniName = _CharAniName;
	AniName.replace(0, Index + 1, _T(""));

	map<tstring, ANIMATIONNAME>::iterator Find = CEditGlobal::CharacterAniIndex.find(AniName);

	if (Find != CEditGlobal::CharacterAniIndex.end())
	{
		return MAXANINAME;
	}

	return Find->second;
}
tstring GetAnimationName(const int& _AniNameIndex, const tstring& _CharName)
{
	return GetAnimationName((ANIMATIONNAME)_AniNameIndex, _CharName);
}
tstring GetAnimationName(const int& _AniNameIndex, const int& _CharIndex)
{
	if ((int)CEditGlobal::CharacterName.size() <= _CharIndex)
	{
		return _T("");
	}

	return GetAnimationName((ANIMATIONNAME)_AniNameIndex, CEditGlobal::CharacterName[_CharIndex].GetString());
}

void LoadingTexture(FolderData* _FolderData)
{
	tstring DirPath = CFilePathMgr::GetPath(_FolderData->FolderKey);
	CEditGlobal::LoadingImage(DirPath, _FolderData);
}
void SkillMgrOn(void* _ptr)
{
	CCriSectionObject<CKeyMgr>();
	CKeyMgr::UpdateOff();
	CEditGlobal::EditSkillMgr->InitSkillMgr();
	CEditGlobal::StatBuilder->SettingStatModel();
	CEditGlobal::CharStatDlg->DataLoadButtonEvent();
	CEditGlobal::RootDlg->ShowSkillDlg();
	//CEditGlobal::RootDlg->ShowDlg(CRootDlg::CHARACTER);
	CEditGlobal::ActionScriptDlg->GetSkillCharCombo()->EnableWindow(TRUE);
	CEditGlobal::ActionScriptDlg->GetSoundNameCombo()->EnableWindow(TRUE);
	//CEditGlobal::SkillDlg->SendMessage(WM_SHOWWINDOW);
	//CEditGlobal::EditSkillMgr->CreateBattleUnit();
	//CEditGlobal::EditSkillMgr->MgrOn();
	CKeyMgr::UpdateOn();
	
}
bool operator==(const POINT& _Left, const POINT& _Right)
{
	if (_Left.x != _Right.x)
		return false;

	if (_Left.y != _Right.y)
		return false;

	return  true;
}