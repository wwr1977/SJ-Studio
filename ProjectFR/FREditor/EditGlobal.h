#pragma once
#include<UserHeader.h>
#include<ResourceMgr.h>
#include<GameTexture.h>
#include<GameSprite.h>

struct EditSet
{
	CEdit*				EditCtrl;
	CString*			EditString;
	float*				EditData;


	void operator=(const EditSet& _Other)
	{
		EditCtrl = _Other.EditCtrl;
		EditString = _Other.EditString;
		EditData = _Other.EditData;
	}

	EditSet()
		:EditCtrl(nullptr), EditString(nullptr), EditData(nullptr)
	{}
	EditSet(CEdit* _EditCtrl, CString* _EditString, float* EditData)
		:EditCtrl(_EditCtrl), EditString(_EditString), EditData(EditData)
	{
	}
	EditSet(const EditSet& _Other)
		:EditCtrl(_Other.EditCtrl), EditString(_Other.EditString), EditData(_Other.EditData)
	{}
	~EditSet() {}
};

struct FolderData 
{
	BOOL													CheckLoadingStart;
	BOOL													CheckLoading;
	BOOL													CheckFinish;
	
	tstring													FolderKey;
	FOLDER													FolderIndex;
	unordered_map<tstring, SPTR<CGameTexture>>				mapLoadTex;
	unordered_map<tstring, SPTR<CGameSprite>>				mapLoadSprite;

public:
	FolderData(const tstring& _FolderKey, FOLDER _FolderIndex)
		:CheckLoading(FALSE), CheckFinish(FALSE), CheckLoadingStart(FALSE)
		,FolderKey(_FolderKey),FolderIndex(_FolderIndex)
	{}
	~FolderData() 
	{
		mapLoadTex.clear();
		mapLoadSprite.clear();
	}
};

class CRootDlg;
class CCharacterDlg;
class CStatDlg;
class CAniSpriteDlg;
class CTextureListDlg;
class CMultiSpriteListDlg;
class CAnimationDlg;
class CAniSpriteList;
class CAnimationList;
class CStatDlg;
class CCharStatDlg;
class CMonStatDlg;
class CParameterRen;
class CDataRenderer;
class CEditEffectMgr;
class CEffectDlg;
class CSkillMgr;
class CStatSceneBuilder;
class CSkillDlg;
class CActionScriptDlg;
class CSkillDataDlg;
class CSkillDlg;
class CEditGlobal
{
public:

	enum CHARDLG
	{
		ANISPRITEDLG,
		ANIMATIONDLG,
		DLGMAX
	};
	


public:
	static SPTR<CGameWindow>						 MainWindow;
	static SPTR<CStatSceneBuilder>					 StatBuilder;
	static SPTR<CGameScene>							 SkillScene;
	static BOOL										 bEditInit;
	static BOOL										 bAllLoading;

	static int								 		 CurThreadCount;
	static int							  	 		 LimitThreadCount;

	static vector<CString>							 vecSaveDataFileName;

	static SPTR<CResourceMgr>						 EditResourceMgr;
	static vector<SPTR<CActObject>>					 vecSpriteDlgModel;

	static SPTR<CActObject>							 EditMainCameraMen;
	static SPTR<CCamera>							 EditMainCamera;


	static vector<CString>							CharacterName;
	static set<CString>								MonsterName;

	static map<tstring, STAT>						CharacterBaseStat;
	static map<tstring, vector<STAT>>				CharacterStatTable;
	static map<tstring, STATPARAMETERDATA>			CharacterParameter;

	static vector<CString>							SkillHitSound;

	// MultiSpriteEditor에서 사용되어지는 모델
	static SPTR<CActObject>			MTexModel;
	static SPTR<CFixRenderer>		MTexCom;


	static SPTR<CActObject>			MAniSpriteModel;
	static SPTR<CFixRenderer>		MAniSpriteCom;


	static SPTR<CActObject>			MBaseAniModel;
	static SPTR<CAniRenderer>		MBaseAniCom;
	
	// Root Dlg;
	static CRootDlg*				RootDlg;
	static CCharacterDlg*			CharacterDlg;

	// 멀티스프라이트 DLG 와 하위 List DLG의 포인터
	static CAniSpriteDlg*			AniSpriteDlg;
	static CTextureListDlg*			TextureList;
	static CMultiSpriteListDlg*		MultiSpriteList;


	// 애니메이션 DLG와 List DLG의 포인터
	static CAnimationDlg*			AnimationDlg;
	static CAniSpriteList*			AniSpriteList;
	static CAnimationList*			AnimationList;

	// Stat Dlg와 하위 Dlg들
	static CStatDlg*				StatDlg;
	static CCharStatDlg*			CharStatDlg;
	static CMonStatDlg*				MonStatDlg;

	// Effect Dlg
	static CEffectDlg*				EffectDlg;


	//Skill Dlg
	static CSkillDlg*				SkillDlg;
	static CActionScriptDlg*		ActionScriptDlg;
	static CSkillDataDlg*			SkillDataDlg;
	// AnimationEditor에서 사용되어지는 모델
	static SPTR<CActObject>			AAnimationModel;
	static SPTR<CAniRenderer>		AAnimationCom;



	// 스탯 Editor에서 사용되는 모델 과 Logic
	static SPTR<CAniRenderer>		SStatIdleModel;
	static CParameterRen*			SParameter;
	static CDataRenderer*			SDataRenderer;


	// Effect Editor에서 사용되는 모델과 Logic;
	static SPTR<CAniRenderer>			EEffectAniModel;
	static SPTR<CSoundPlayer>			ESoundPlayer;
	static SPTR<CEditEffectMgr>			EffectMgr;
	
	static map<tstring, ANIMATIONDATA*>	AnimationDataMap;
	static map<tstring, size_t>			EffectIndexMap;
	static map<tstring, ANIMATIONNAME>	CharacterAniIndex;
	/*static vector<EFFECTDATA>			EffectDataVector;
	static map<tstring, EFFECTDATA>		EffectDataMap;*/

	static vector<EFFECTDATA>			EffectData;

	// Skill Editor 관련 변수들
	static SPTR<CSkillMgr>				EditSkillMgr;
	static vector<SKILL>				vecSkill;

	static vector<CString>				EditDemageFont;

	// Map Edit 관련 변수들
	static class CMapDlg*				MapDlg;
	static SPTR<class CEditTileMap>		CurTileMap;
	static class CEditMapMgr*			EditMapMgr;
public:
	static void GlobalInit();
	static void GlobalRelease();
	static void SettingCharacterBaseStat();
	static void LoadUnitName();
	static void ChangeFloatString(CString& _String);
	static void ChangeFloatString(CString* _String);
	static BOOL CheckFloatString(const CString& _String);
	static const float ChangeFloatValue(const CString& _String);
	static const int ChangeIntValue(const CString& _String);
	static BOOL AvailableValue(const float& _Value, const tstring& ErrorMsg);
	static BOOL AvailableValue(const float& _Value, const float& _LimitValue, const tstring& ErrorMsg);
	static BOOL AvailableValue(const int& _Value, const tstring& ErrorMsg);
	static BOOL AvailableValue(const int& _Value, const int& _LimitValue, const tstring& ErrorMsg);

public:
	static void InitAniSpriteModel();
	static void InitAnimationModel();
	static void InitEffectModel();
	static void InitStatModel();


///////////////////////////////////////   Loading & Save Logic  ////////////////////////////////////////////////////////

	static vector<FolderData>					vecFolderData;


	static void LoadingImage(const tstring& _Path, FolderData* _FolderData);
	static void SoundLoading();
	static void SkillSoundLoading(vector<tstring>& _SkillSoundNameVec);
	static BOOL CheckUseThread();
	static BOOL ReturnThread();

//////////////////////////////////		ListBox Update(리스트 박스를 대신 갱신 시켜주는 함수)	//////////////////////////////////////////////
	static void TexSpriteListUpdate(CListBox* _ListBox, FOLDER _FolderIndex);
	static void AniSpriteListUpdate(CListBox* _ListBox, ANISPRITETYPE _Type);
	static void AnimationListUpdate(CListBox* _ListBox,SPTR<CAniRenderer> _AniRenderer, ANISPRITETYPE _Type);

	static void LoadMultiSpriteData(vector<MULTISPRITEDATA>& _TempVec, FILE* _LoadFile);
	
	static void ClearAnimationDataMap();
	static ANIMATIONDATA* GetAniData(const tstring& _DataName);

	//////////////////////////////
	static void Update();
	static void CurLoadTexMap();

	// EffectData 관련 탐색 함수
	static  BOOL ExistEffectIndex(const tstring& _EffectName);
	static size_t FindEffectIndex(const tstring& _EffectName);
	static const size_t GetEffectIndex(const tstring& _EffectName);

	// 캐릭터 관련 함수
	static BOOL isDoubleAttackChar(const int& _CharIndex);
	static bool CheckDoubleAtkCharacter(const tstring& _Name);
	static bool CheckCharName(const tstring& _Name);
	static bool IsMonsterName(const tstring& _MonName);

	static CString GetHitSound(const int& _Index);
	static const CString GetDemageFont(const size_t& _DemageFontIndex);


public:
	CEditGlobal() {}
	~CEditGlobal() {}
};

