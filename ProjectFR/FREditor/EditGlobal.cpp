#include "stdafx.h"
#include "EditGlobal.h"
#include <GameMultiSprite.h>
#include "EditEffectMgr.h"
#include "SkillMgr.h"
#include "StatSceneBuilder.h"
#include "EditTileMap.h"




SPTR<CGameWindow>		 CEditGlobal::MainWindow = nullptr;
SPTR<CGameScene>		 CEditGlobal::SkillScene = nullptr;
SPTR<CStatSceneBuilder>	 CEditGlobal::StatBuilder = nullptr;

BOOL					 CEditGlobal::bEditInit = FALSE;
BOOL					 CEditGlobal::bAllLoading = FALSE;

int						 CEditGlobal::CurThreadCount = 0;
int						 CEditGlobal::LimitThreadCount = 4;

vector<CString>			 CEditGlobal::vecSaveDataFileName;

SPTR<CResourceMgr>		 CEditGlobal::EditResourceMgr = nullptr;
vector<SPTR<CActObject>> CEditGlobal::vecSpriteDlgModel;
SPTR<CActObject>		 CEditGlobal::EditMainCameraMen = nullptr;
SPTR<CCamera>			 CEditGlobal::EditMainCamera;
SPTR<CActObject>		 CEditGlobal::MTexModel = nullptr;
SPTR<CFixRenderer>		 CEditGlobal::MTexCom = nullptr;

SPTR<CActObject>		 CEditGlobal::MAniSpriteModel = nullptr;
SPTR<CFixRenderer>		 CEditGlobal::MAniSpriteCom = nullptr;

SPTR<CActObject>		 CEditGlobal::MBaseAniModel = nullptr;
SPTR<CAniRenderer>		 CEditGlobal::MBaseAniCom = nullptr;



SPTR<CActObject>		CEditGlobal::AAnimationModel = nullptr;
SPTR<CAniRenderer>		CEditGlobal::AAnimationCom = nullptr;

SPTR<CAniRenderer>		CEditGlobal::SStatIdleModel = nullptr;
CParameterRen*			CEditGlobal::SParameter = nullptr;
CDataRenderer*			CEditGlobal::SDataRenderer = nullptr;

CRootDlg*				CEditGlobal::RootDlg = nullptr;
CCharacterDlg*			CEditGlobal::CharacterDlg = nullptr;

CAniSpriteDlg*			CEditGlobal::AniSpriteDlg = nullptr;
CTextureListDlg*		CEditGlobal::TextureList = nullptr;
CMultiSpriteListDlg*	CEditGlobal::MultiSpriteList = nullptr;

CAnimationDlg*			CEditGlobal::AnimationDlg = nullptr;
CAniSpriteList*			CEditGlobal::AniSpriteList = nullptr;
CAnimationList*			CEditGlobal::AnimationList = nullptr;

CStatDlg*				CEditGlobal::StatDlg = nullptr;
CCharStatDlg*			CEditGlobal::CharStatDlg = nullptr;
CMonStatDlg*			CEditGlobal::MonStatDlg = nullptr;

CEffectDlg*				CEditGlobal::EffectDlg = nullptr;

vector<FolderData>		CEditGlobal::vecFolderData;



vector<CString>						CEditGlobal::CharacterName;
set<CString>						CEditGlobal::MonsterName;

map<tstring, STAT>					CEditGlobal::CharacterBaseStat;
map<tstring, vector<STAT>>			CEditGlobal::CharacterStatTable;
map<tstring, STATPARAMETERDATA>		CEditGlobal::CharacterParameter;
vector<CString>						CEditGlobal::SkillHitSound;

SPTR<CAniRenderer>					CEditGlobal::EEffectAniModel = nullptr;
SPTR<CSoundPlayer>					CEditGlobal::ESoundPlayer = nullptr;
SPTR<CEditEffectMgr>				CEditGlobal::EffectMgr = nullptr;
map<tstring, ANIMATIONDATA*>		CEditGlobal::AnimationDataMap;
map<tstring, size_t>				CEditGlobal::EffectIndexMap;
map<tstring, ANIMATIONNAME>			CEditGlobal::CharacterAniIndex;

vector<EFFECTDATA>					CEditGlobal::EffectData;

CSkillDlg*							CEditGlobal::SkillDlg = nullptr;
CActionScriptDlg*					CEditGlobal::ActionScriptDlg = nullptr;
CSkillDataDlg*						CEditGlobal::SkillDataDlg = nullptr;

SPTR<CSkillMgr>						CEditGlobal::EditSkillMgr = nullptr;
vector<SKILL>						CEditGlobal::vecSkill;

vector<CString>						CEditGlobal::EditDemageFont;

CMapDlg*							CEditGlobal::MapDlg = nullptr;
SPTR<CEditTileMap>					CEditGlobal::CurTileMap = nullptr;
CEditMapMgr*						CEditGlobal::EditMapMgr = nullptr;

void CEditGlobal::GlobalInit()
{
	
	vecFolderData.push_back(FolderData(_T("Character"),CHARACTER));
	vecFolderData.push_back(FolderData(_T("Monster"),MONSTER));
	vecFolderData.push_back(FolderData(_T("Effect"),EFFECT));
	vecFolderData.push_back(FolderData(_T("UI"),UI));
	vecFolderData.push_back(FolderData(_T("Map"), MAP));


	vecSaveDataFileName.push_back(_T("Character Data File"));
	vecSaveDataFileName.push_back(_T("Monster Data File"));
	vecSaveDataFileName.push_back(_T("Effect Data File"));
	vecSaveDataFileName.push_back(_T("ETC Data File"));

	LoadUnitName();
	SettingCharacterBaseStat();

	CharacterAniIndex.clear();
	CharacterAniIndex.insert(map<tstring, ANIMATIONNAME>::value_type(_T("_idle"), ANINAME_IDLE));
	CharacterAniIndex.insert(map<tstring, ANIMATIONNAME>::value_type(_T("_standby"), ANINAME_STAND));
	CharacterAniIndex.insert(map<tstring, ANIMATIONNAME>::value_type(_T("_move"), ANINAME_MOVE));
	CharacterAniIndex.insert(map<tstring, ANIMATIONNAME>::value_type(_T("_atk"), ANINAME_ATTACK));
	CharacterAniIndex.insert(map<tstring, ANIMATIONNAME>::value_type(_T("_atk1"), ANINAME_ATTACK));
	CharacterAniIndex.insert(map<tstring, ANIMATIONNAME>::value_type(_T("_limit_atk"), ANINAME_LIMIT));
	CharacterAniIndex.insert(map<tstring, ANIMATIONNAME>::value_type(_T("_dead"), ANINAME_DEAD));
	CharacterAniIndex.insert(map<tstring, ANIMATIONNAME>::value_type(_T("_dying"), ANINAME_DYING));
	CharacterAniIndex.insert(map<tstring, ANIMATIONNAME>::value_type(_T("_magic_atk"), ANINAME_MAGICATTACK));
	CharacterAniIndex.insert(map<tstring, ANIMATIONNAME>::value_type(_T("_magic_standby"), ANINAME_MAGICSTAND));
	CharacterAniIndex.insert(map<tstring, ANIMATIONNAME>::value_type(_T("_win"), ANINAME_WIN));
	CharacterAniIndex.insert(map<tstring, ANIMATIONNAME>::value_type(_T("_win_before"), ANINAME_WIN_BEFORE));
	CharacterAniIndex.insert(map<tstring, ANIMATIONNAME>::value_type(_T("_atk2"), ANINAME_ATTACK2));
	CharacterAniIndex.insert(map<tstring, ANIMATIONNAME>::value_type(_T("_DoubleAtk"), ANINAME_DOUBLEATTACK));


	int Count = 0;


	CString HitSound;
	
	for (; Count < MAXHITSOUND; ++Count)
	{
		HitSound.Format(_T("Hit%d"), Count);
		SkillHitSound.push_back(HitSound);
	}

	EditDemageFont.push_back(_T("Number_Black"));
	EditDemageFont.push_back(_T("Number_White"));
	EditDemageFont.push_back(_T("Number_Sp"));
	EditDemageFont.push_back(_T("Number_Red"));
	EditDemageFont.push_back(_T("Number_Blue"));

}
void CEditGlobal::GlobalRelease()
{
	ClearAnimationDataMap();
}
void CEditGlobal::SettingCharacterBaseStat()
{
	STAT BaseStat = STAT(17, 44, 15, 25, 70, 20, 87, 150);
	CharacterBaseStat.insert(map<tstring, STAT>::value_type(_T("Chizuru1"), BaseStat));
	BaseStat = STAT(70, 134, 67, 93, 204, 62, 245, 200);
	CharacterBaseStat.insert(map<tstring, STAT>::value_type(_T("Chizuru3"), BaseStat));
	BaseStat = STAT( 50, 121, 63, 72, 210, 53, 204, 150);
	CharacterBaseStat.insert(map<tstring, STAT>::value_type(_T("CupidArtemios"), BaseStat));
	BaseStat = STAT( 12, 52, 10, 10, 65, 17, 64, 150);
	CharacterBaseStat.insert(map<tstring, STAT>::value_type(_T("CupidLuna1"), BaseStat));
	BaseStat = STAT( 43, 144, 80, 80, 220, 47, 192, 200);
	CharacterBaseStat.insert(map<tstring, STAT>::value_type(_T("CupidLuna3"), BaseStat));
	BaseStat = STAT( 180, 67, 52, 42, 160, 84, 315, 150);
	CharacterBaseStat.insert(map<tstring, STAT>::value_type(_T("Dean"), BaseStat));
	BaseStat = STAT( 200, 170, 100, 90, 300, 70, 300, 200);
	CharacterBaseStat.insert(map<tstring, STAT>::value_type(_T("DemonRain"), BaseStat));
	BaseStat = STAT( 43, 92, 78, 102, 220, 55, 200, 200);
	CharacterBaseStat.insert(map<tstring, STAT>::value_type(_T("Lid"), BaseStat));
	BaseStat = STAT( 41, 58, 130, 72, 163, 65, 190, 150);
	CharacterBaseStat.insert(map<tstring, STAT>::value_type(_T("Ling"), BaseStat));
	BaseStat = STAT( 42, 20, 12, 20, 55, 30, 102, 150);
	CharacterBaseStat.insert(map<tstring, STAT>::value_type(_T("Rain2"), BaseStat));
	BaseStat = STAT( 120, 72, 53, 67, 150, 65, 280, 150);
	CharacterBaseStat.insert(map<tstring, STAT>::value_type(_T("Rain5"), BaseStat));
	BaseStat = STAT( 41, 88, 127, 100, 240, 40, 180, 200);
	CharacterBaseStat.insert(map<tstring, STAT>::value_type(_T("Sakura"), BaseStat));
	BaseStat = STAT( 135, 60, 55, 37, 190, 50, 220, 150);
	CharacterBaseStat.insert(map<tstring, STAT>::value_type(_T("Vargas"), BaseStat));
	BaseStat = STAT( 10, 21, 57, 30, 120, 12, 58, 150);
	CharacterBaseStat.insert(map<tstring, STAT>::value_type(_T("Zyrus1"), BaseStat));
	BaseStat = STAT( 33, 49, 144, 102, 280, 35, 170, 200);
	CharacterBaseStat.insert(map<tstring, STAT>::value_type(_T("Zyrus3"), BaseStat));


}
void CEditGlobal::ChangeFloatString(CString& _String)
{
	char Zero = '0';
	char Nine = '9';

	for (int i = 0; i < _String.GetLength(); ++i)
	{
		// 부호는 항상 문자열의 시작 부분에만 존재 가능
		if (_String[i] == '-' && 0 == i)
		{
			continue;
		}
		if (_String[i] == '.')
		{
			// 첫번째 자리에 .이 들어간경우 . 제거
			if (i == 0)
			{
				_String.Delete(i, 1);
				continue;
			}
			// .앞에 -가 존재할경우 . 제거
			if (_String[i - 1] == '-')
			{
				_String.Delete(i, 1);
				continue;
			}
			// 자기 앞에 .이 이미 존재한다면 .제거
			for (int j = 0; j < i; j++)
			{
				if (_String[j] == '.')
				{
					_String.Delete(i, 1);
					break;
				}
			}
			continue;
		}
		if (_String[i] < Zero || _String[i] > Nine)
		{
			_String.Delete(i, 1);
		}
	}

	return;
}
void CEditGlobal::ChangeFloatString(CString* _String)
{
	if (nullptr == _String)
		return;

	char Zero = '0';
	char Nine = '9';

	for (int i = 0; i < _String->GetLength(); ++i)
	{
		// 부호는 항상 문자열의 시작 부분에만 존재 가능
		if ((*_String)[i] == '-' && 0 == i)
		{
			continue;
		}
		if ((*_String)[i] == '.')
		{
			// 첫번째 자리에 .이 들어간경우 . 제거
			if (i == 0)
			{
				_String->Delete(i, 1);
				continue;
			}
			// .앞에 -가 존재할경우 . 제거
			if ((*_String)[i - 1] == '-')
			{
				_String->Delete(i, 1);
				continue;
			}
			// 자기 앞에 .이 이미 존재한다면 .제거
			for (int j = 0; j < i; j++)
			{
				if ((*_String)[j] == '.')
				{
					_String->Delete(i, 1);
					break;
				}
			}
			continue;
		}
		if ((*_String)[i] < Zero || (*_String)[i] > Nine)
		{
			_String->Delete(i, 1);
		}
	}

	return;
}
BOOL CEditGlobal::CheckFloatString(const CString& _String)
{
	char Zero = '0';
	char Nine = '9';

	for (int i = 0; i < _String.GetLength(); i++)
	{
		if (_String[i] == '-' || _String[i] == '.')
		{
			continue;
		}
		if (_String[i] < Zero || _String[i] > Nine)
		{
			return FALSE;
		}
	}


	return TRUE;
}
const float CEditGlobal::ChangeFloatValue(const CString& _String)
{
	CString FloatString = _String;

	float Sign = 1.0f;
	float Value = 0.0f;

	if (FloatString == _T("")) 
	{
		return 0.0f;
	}

	if (FloatString[0] == '-')
	{
		Sign = -1.0f;
		FloatString.Delete(0, 1);
	}

	int LastIndex = FloatString.GetLength() - 1;
	if (FloatString[LastIndex] == '.')
	{
		FloatString.Delete(LastIndex, 1);
	}

	int Pow = FloatString.Find('.');

	if (-1 == Pow)
	{
		Pow = FloatString.GetLength();
	}
	else
	{
		FloatString.Delete(Pow, 1);
	}

	float Base = powf(10.0f, (float)(Pow - 1));

	for (int i = 0; i < FloatString.GetLength(); ++i, Base /= 10.0f)
	{
		int value = (int)FloatString[i];
		value -= (int)'0';
		Value += value * Base;
	}

	return Value * Sign;
}
const int CEditGlobal::ChangeIntValue(const CString& _String)
{
	CString IntString = _String;
	int Sign = 1;
	int Value = 0;
	char Zero = '0';
	char Nine = '9';

	if (IntString == _T("")) 
	{
		return 0;
	}
	if (IntString[0] == '-')
	{
		Sign = -1;
		IntString.Delete(0, 1);
	}

	int Pow = IntString.Find(_T("."));

	// -,.처리후 숫자가 아닌 글자가 들어오면 나올수 없는 수를 리턴하도록 수정
	for (int i = 0; i < IntString.GetLength(); i++)
	{
		if (Zero > IntString[i] || Nine < IntString[i])
		{
			IntString.Delete(i, 1);
		}
	}
	if (-1 == Pow )
	{
		Pow = IntString.GetLength();
	}

	int Base = (int)pow(10.0f, (Pow - 1));

	for (int i = 0; i < IntString.GetLength(), Base > 0; i++, Base /= 10)
	{
		int value = (int)IntString[i];
		value -= '0';
		Value += value * Base;
	}

	return Value*Sign;

}
BOOL CEditGlobal::AvailableValue(const float& _Value, const tstring& ErrorMsg)
{
	if (0.0f > _Value)
	{
		AfxMessageBox(ErrorMsg.c_str());
		return FALSE;
	}
	return TRUE;
}
BOOL CEditGlobal::AvailableValue(const float& _Value, const float& _LimitValue, const tstring& ErrorMsg)
{
	if (0.0f > _Value || _LimitValue < _Value)
	{
		AfxMessageBox(ErrorMsg.c_str());
		return FALSE;
	}
	return TRUE;
}

BOOL CEditGlobal::AvailableValue(const int& _Value, const tstring& ErrorMsg)
{
	if (0 > _Value)
	{
		AfxMessageBox(ErrorMsg.c_str());
		return FALSE;
	}
	return TRUE;
}
BOOL CEditGlobal::AvailableValue(const int& _Value, const int& _LimitValue, const tstring& ErrorMsg)
{

	if (0.0f > _Value || _LimitValue < _Value)
	{
		AfxMessageBox(ErrorMsg.c_str());
		return FALSE;
	}
	return TRUE;
}

void CEditGlobal::InitAniSpriteModel()
{
	for (size_t i = 0; i < vecSpriteDlgModel.size(); i++)
	{
		vecSpriteDlgModel[i]->Off();
	}
	return;

}
void CEditGlobal::InitAnimationModel()
{
	AAnimationModel->Off();
	return;
}
void CEditGlobal::InitStatModel()
{
	SStatIdleModel->Off();
}



///////////////////////////////////////   Loading & Save Logic  ////////////////////////////////////////////////////////

void CEditGlobal::LoadingImage(const tstring& _Path, FolderData* _FolderData)
{

	CFileFind Finder;

	CString Path;
	BOOL bWorking;
	CString FileName;


	for (int i = 0; i < FILEMAX; i++)
	{
		Path = _Path.c_str();
		Path += CFilePathMgr::ImageFile[i].c_str();
		bWorking = Finder.FindFile(Path);

		while (TRUE == bWorking)
		{
			bWorking = Finder.FindNextFile();
			if (Finder.IsArchived())
			{
				CString _filename = Finder.GetFileName();

				if (_filename == _T(".") ||
					_filename == _T("..") ||
					_filename == _T("Thumbs.db"))
				{
					continue;
				}
				FileName = Finder.GetFileTitle();
				
				FileName = Finder.GetFileName();
				tstring TexPath = _Path + FileName.GetString();
				
				CGameTexture* TempTex = CEditGlobal::EditResourceMgr->LoadGameTexture(TexPath, &_FolderData->mapLoadTex);
				if (nullptr == TempTex) 
				{
					continue;
				}
				FileName = Finder.GetFileTitle();
				TempTex->SetFolderIndex(_FolderData->FolderIndex);
				CEditGlobal::EditResourceMgr->CreateGameSprite(TempTex, (_T("_Tex") + FileName).GetString(), &_FolderData->mapLoadSprite);
			}
		}
	}

	_FolderData->CheckLoading = TRUE;
	
}

BOOL CEditGlobal::CheckUseThread()
{
	CCriSectionObject<CEditGlobal>();
	if (CurThreadCount < LimitThreadCount) 
	{
		++CurThreadCount;
		return TRUE;
	}
	return FALSE;
}
BOOL CEditGlobal::ReturnThread()
{
	CCriSectionObject<CEditGlobal>();
	if (CurThreadCount < 0 ) 
	{
		return FALSE;
	}
	--CurThreadCount;
	return TRUE;
}



//////////////////////////////////		ListBox Update(리스트 박스를 대신 갱신 시켜주는 함수)	//////////////////////////////////////////////
void CEditGlobal::TexSpriteListUpdate(CListBox* _ListBox, FOLDER _FolderIndex)
{
	if (nullptr == EditResourceMgr)
	{
		return;
	}

	unordered_map<tstring, SPTR<CGameSprite>>* MapSprite = EditResourceMgr->GetSpriteMap();
	if (nullptr == MapSprite) 
	{
		return;
	}
	unordered_map<tstring, SPTR<CGameSprite>>::iterator Iter = MapSprite->begin();

	if (0 >= MapSprite->size())
	{
		return;
	}

	_ListBox->ResetContent();
	
	for (; Iter != MapSprite->end(); ++Iter)
	{
		if (_FolderIndex == Iter->second->GetFolderIndex())
		{
			tstring AddListString = Iter->first.c_str();
			tstring StringHead = AddListString.substr(0, 4);
			if (StringHead == _T("_Tex"))
			{
				AddListString.replace(0, 4, _T(""));
				_ListBox->AddString(AddListString.c_str());
			}
		}
	}
}

void CEditGlobal::AniSpriteListUpdate(CListBox* _ListBox,ANISPRITETYPE _Type)
{
	if (nullptr == EditResourceMgr) 
	{
		return;
	}

	unordered_map<tstring, SPTR<CGameMultiSprite>>* MapAniSprite = CEditGlobal::EditResourceMgr->GetAniSpriteMap();
	if (nullptr == MapAniSprite)
	{
		return;
	}
	unordered_map<tstring, SPTR<CGameMultiSprite>>::iterator Iter = MapAniSprite->begin();
	
	if (0 >= MapAniSprite->size()) 
	{
		return;
	}

	_ListBox->ResetContent();
	

	for (; Iter != MapAniSprite->end(); ++Iter)
	{
		if (_Type == Iter->second->GetSpriteType())
		{
			_ListBox->AddString(Iter->first.c_str());
		}
		
	}

}
void CEditGlobal::AnimationListUpdate(CListBox* _ListBox, SPTR<CAniRenderer> _AniRenderer, ANISPRITETYPE _Type)
{
	if (nullptr == _AniRenderer) 
	{
		return;
	}

	map<tstring, SPTR<CAniRenderer::CAnimation>>* MapAni =_AniRenderer->GetAniMap();
	map<tstring, SPTR<CAniRenderer::CAnimation>>::iterator Iter = MapAni->begin();

	for (; Iter != MapAni->end(); ++Iter)
	{
	
		if (_Type == Iter->second->GetAniSpriteType())
		{
			_ListBox->AddString(Iter->first.c_str());
		}
	}
}


void CEditGlobal::Update()
{
	SKILLMGRSTATE SkillMgrState = STATE_INIT;

	if( nullptr !=  CEditGlobal::EditSkillMgr)
		SkillMgrState = CEditGlobal::EditSkillMgr->GetSkillMgrState();

	if (TRUE == bAllLoading || SkillMgrState == STATE_LOADING)
	{
		return;
	}

	for (int i = 0; i < (int)FOLDERMAX; i++)
	{
		if (TRUE == vecFolderData[i].CheckLoading && FALSE == vecFolderData[i].CheckFinish) 
		{
			EditResourceMgr->MergeTextureMap(&vecFolderData[i].mapLoadTex);
			EditResourceMgr->MergeSpriteMap(&vecFolderData[i].mapLoadSprite);
			vecFolderData[i].mapLoadTex.clear();
			vecFolderData[i].mapLoadSprite.clear();
			vecFolderData[i].CheckFinish = TRUE;
			ReturnThread();
		}
	}
	int Count = 0;

	for (int i = 0; i < (int)MAP; i++)
	{
		if (TRUE == vecFolderData[i].CheckFinish) 
		{
			++Count;
		}
	}

	if (MAP == Count) 
	{
		bAllLoading = TRUE;
	}

}
void CEditGlobal::CurLoadTexMap()
{
	for (int i = 0; i < (int)MAP; i++)
	{
		if (TRUE == vecFolderData[i].CheckLoading && FALSE == vecFolderData[i].CheckFinish)
		{
			EditResourceMgr->MergeTextureMap(&vecFolderData[i].mapLoadTex);
			EditResourceMgr->MergeSpriteMap(&vecFolderData[i].mapLoadSprite);
			vecFolderData[i].mapLoadTex.clear();
			vecFolderData[i].mapLoadSprite.clear();
			vecFolderData[i].CheckFinish = TRUE;

		}
	}
	int Count = 0;

	for (int i = 0; i < (int)MAP; i++)
	{
		if (TRUE == vecFolderData[i].CheckFinish)
		{
			++Count;
		}
	}

	if (MAP == Count)
	{
		bAllLoading = TRUE;
	}

}
void CEditGlobal::LoadUnitName()
{
	CharacterName.push_back(_T("DemonRain"));
	CharacterName.push_back(_T("Rain2"));
	CharacterName.push_back(_T("Rain5"));
	CharacterName.push_back(_T("Vargas"));
	CharacterName.push_back(_T("Chizuru1"));
	CharacterName.push_back(_T("Chizuru3"));
	CharacterName.push_back(_T("CupidArtemios"));
	CharacterName.push_back(_T("CupidLuna1"));
	CharacterName.push_back(_T("CupidLuna3"));
	CharacterName.push_back(_T("Dean"));
	CharacterName.push_back(_T("Lid"));
	CharacterName.push_back(_T("Ling"));
	CharacterName.push_back(_T("Sakura"));
	CharacterName.push_back(_T("Zyrus1"));
	CharacterName.push_back(_T("Zyrus3"));


	CFileFind Finder;

	CString Path;
	BOOL bWorking;
	CString FileName;


	for (int i = 0; i < FILEMAX; i++)
	{
		Path = CFilePathMgr::GetPath(_T("Monster")).c_str();
		Path += CFilePathMgr::ImageFile[i].c_str();
		bWorking = Finder.FindFile(Path);

		while (TRUE == bWorking)
		{
			bWorking = Finder.FindNextFile();
			
			if (Finder.IsArchived())
			{
				CString _filename = Finder.GetFileName();

				if (_filename == _T(".") ||
					_filename == _T("..") ||
					_filename == _T("Thumbs.db"))
				{
					continue;
				}
				
				_filename = Finder.GetFileTitle();
				int Index = _filename.Find(_T("_atk"));
				
				if (-1 != Index) 
				{
					_filename.Delete(Index, 4);

					set<CString>::iterator FindIter =  CEditGlobal::MonsterName.find(_filename);

					if (FindIter == CEditGlobal::MonsterName.end()) 
					{
						CEditGlobal::MonsterName.insert(_filename);
					}
				}
				
			}
		}
	}
	
	return;
}

void CEditGlobal::LoadMultiSpriteData(vector<MULTISPRITEDATA>& _TempVec, FILE* _LoadFile)
{
	size_t LoadCount;

	MULTISPRITEDATA LoadSpriteData;
	while (0 < (LoadCount = fread_s(&LoadSpriteData, sizeof(LoadSpriteData), sizeof(MULTISPRITEDATA), 1, _LoadFile)))
	{
		_TempVec.push_back(LoadSpriteData);
	}


}
void CEditGlobal::SoundLoading()
{
	CFileFind Finder;

	CString SoundFolder;
	CString SoundPath;
	BOOL bWorking;
	
	SoundFolder = CFilePathMgr::GetPath(_T("Sound")).c_str();

	for (int i = 0; i < MAXSOUND; i++)
	{
		SoundPath = SoundFolder + CFilePathMgr::SoundFile[i].c_str();
		bWorking = Finder.FindFile(SoundPath);

		while (TRUE == bWorking)
		{
			bWorking = Finder.FindNextFile();

			if (Finder.IsArchived())
			{
				CString _filename = Finder.GetFileName();

				if (_filename == _T(".") ||
					_filename == _T("..") ||
					_filename == _T("Thumbs.db"))
				{
					continue;
				}

				CGameSound* NewSound = CEditGlobal::EditResourceMgr->SoundLoad((SoundFolder + _filename).GetString());
				
				if (nullptr == NewSound) 
				{
					assert(nullptr);
				}
			}
		}
	}

}
void CEditGlobal::SkillSoundLoading(vector<tstring>& _SkillSoundNameVec)
{
	if (nullptr == &_SkillSoundNameVec )
		return;

	_SkillSoundNameVec.clear();

	CFileFind Finder;

	CString SoundFolder;
	CString SoundPath;
	BOOL bWorking;

	SoundFolder = CFilePathMgr::GetPath(_T("SkillSound")).c_str();

	for (int i = 0; i < MAXSOUND; i++)
	{
		SoundPath = SoundFolder + CFilePathMgr::SoundFile[i].c_str();
		bWorking = Finder.FindFile(SoundPath);

		while (TRUE == bWorking)
		{
			bWorking = Finder.FindNextFile();

			if (Finder.IsArchived())
			{
				CString _filename = Finder.GetFileName();
				CString _filetitle = Finder.GetFileTitle();

				if (_filename == _T(".") ||
					_filename == _T("..") ||
					_filename == _T("Thumbs.db"))
				{
					continue;
				}

				CGameSound* NewSound = CEditGlobal::EditResourceMgr->SoundLoad((SoundFolder + _filename).GetString());

				if (nullptr == NewSound)
				{
					assert(nullptr);
				}

				if (_T("") != _filetitle)
					_SkillSoundNameVec.push_back(_filetitle.GetString());
			}

		}
	}
}
void CEditGlobal::InitEffectModel()
{
	EEffectAniModel->Off();
}
void CEditGlobal::ClearAnimationDataMap()
{
	if (0 >= AnimationDataMap.size()) 
	{
		return;
	}

	map<tstring, ANIMATIONDATA*>::iterator	Start = AnimationDataMap.begin();
	map<tstring, ANIMATIONDATA*>::iterator	End = AnimationDataMap.end();

	for (;Start!= End; ++Start)
	{
		SAFE_DELETE(Start->second);
	}

	AnimationDataMap.clear();
}
ANIMATIONDATA* CEditGlobal::GetAniData(const tstring& _DataName)
{
	map<tstring,ANIMATIONDATA*>::iterator Find =  CEditGlobal::AnimationDataMap.find(_DataName);

	if (CEditGlobal::AnimationDataMap.end() == Find) 
	{
		return nullptr;
	}

	return Find->second;
}
BOOL CEditGlobal::ExistEffectIndex(const tstring& _EffectName) 
{
	map<tstring, size_t>::iterator Find = EffectIndexMap.find(_EffectName);

	if (Find == EffectIndexMap.end()) 
	{
		return FALSE;
	}
	return TRUE;
}
size_t CEditGlobal::FindEffectIndex(const tstring& _EffectName)
{
	map<tstring, size_t>::iterator Find = EffectIndexMap.find(_EffectName);

	if (Find == EffectIndexMap.end())
	{
		return -1;
	}
	return Find->second;
}
const size_t CEditGlobal::GetEffectIndex(const tstring& _EffectName)
{
	map<tstring, size_t>::iterator Find = EffectIndexMap.find(_EffectName);

	if (Find == EffectIndexMap.end())
	{
		return -1;
	}

	return Find->second;
}
BOOL CEditGlobal::isDoubleAttackChar(const int& _CharIndex)
{
	if (_CharIndex <= CharacterIndex::Vargas)
		return TRUE;

	return FALSE;
}
bool CEditGlobal::CheckDoubleAtkCharacter(const tstring& _Name)
{
	for (size_t i = 0; i < CharacterName.size(); i++)
	{
		if (_Name == CharacterName[i].GetString())
		{
			if (i <= Vargas)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
	}

	return false;
}
bool CEditGlobal::CheckCharName(const tstring& _Name)
{
	for (size_t i = 0; i < CharacterName.size(); i++)
	{
		if (CharacterName[i].GetString() == _Name)
		{
			return true;
		}
	}

	return false;
}
bool CEditGlobal::IsMonsterName(const tstring& _MonName)
{
	set<CString>::iterator Find = MonsterName.find(_MonName.c_str());

	if (Find != MonsterName.end())
	{
		return true;
	}

	return false;

}

CString CEditGlobal::GetHitSound(const int& _Index)
{
	 if (_Index >= SkillHitSound.size())
		 return _T("");

	 return SkillHitSound[_Index];
}
const CString CEditGlobal::GetDemageFont(const size_t& _DemageFontIndex)
{
	if (_DemageFontIndex >= EditDemageFont.size())
	{
		return _T("");
	}

	return EditDemageFont[_DemageFontIndex];
}