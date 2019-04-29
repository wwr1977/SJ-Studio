#include "stdafx.h"
#include "StatSceneBuilder.h"
#include "EditBackGround.h"
#include "ParameterRen.h"
#include "StatDlg.h"
#include "CharStatDlg.h"
#include "MonStatDlg.h"
#include"DataRenderer.h"
#include"SkillMgr.h"

CStatSceneBuilder::CStatSceneBuilder()
	:m_StatIdleModel(nullptr), m_DataRenderer(nullptr)
{
	CEditGlobal::StatBuilder = this;
}


CStatSceneBuilder::~CStatSceneBuilder()
{
}
void CStatSceneBuilder::WakeUpScene()
{
	CEditGlobal::EditMainCamera = GetMainCam();
}
bool CStatSceneBuilder::SceneBuild() 
{
	POINT WndSize = CEditGlobal::MainWindow->WndSize();
	Vec2 ModelPos = Vec2{ WndSize.x* -0.25f,WndSize.y*0.15f };

	CKeyMgr::CreateKey(_T("NextMonster"), 'M');
	CKeyMgr::CreateKey(_T("PrevMonster"), 'N');

	CreateMainCamera();
	CreateMainLight();
	LoadResource();

	CEditBackGround* Back = CreateActObject(_T("StatBack"))->CreateCom<CEditBackGround>();
	Back->SetSprite(_T("StatBack"));
	

	CParameterRen* Para = CreateActObject(_T("Parameter"))->CreateCom<CParameterRen>();
	

	CActObject* Charater = CreateActObject(_T("CharModel"));


	Charater->SetImageScale(2.5f);
	Charater->SetPos(ModelPos);
	
	m_StatIdleModel = Charater->CreateCom<CAniRenderer>(RENDATA(LAYER_PLAYER, 0.8f));
	
	/*
	if (STATE_COMPLETE != CEditGlobal::EditSkillMgr->GetSkillMgrState())
	{
		LoadingIdleImage(CHARACTER);
		LoadingIdleImage(MONSTER);

		LoadingIdleAnimation(CHARACTER);
		LoadingIdleAnimation(MONSTER);
	}
	else 
	{
		SettingStatModel();
	}
	*/

	m_StatIdleModel->Off();


	CEditGlobal::SDataRenderer = CreateActObject(_T("DataRenderer"))->CreateCom<CDataRenderer>();
	CEditGlobal::SDataRenderer->ConnectParameterArray(Para->GetParameterArr());
	CEditGlobal::SStatIdleModel = m_StatIdleModel;
	
	

	return true;
}
void CStatSceneBuilder::SceneUpdate()
{
	
}
void CStatSceneBuilder::LoadResource()
{
	for (size_t i = 0; i < CEditGlobal::CharacterName.size(); i++)
	{
		tstring UnitIllust = _T("unit_ills_") + CEditGlobal::CharacterName[i];
		RESMGR->LoadGameTexture(CFilePathMgr::GetPath(_T("UI")) + UnitIllust + _T(".png"));
		RESMGR->CreateGameSprite(UnitIllust);
	}

	set<CString>::iterator Start = CEditGlobal::MonsterName.begin();
	set<CString>::iterator End = CEditGlobal::MonsterName.end();

	for (; Start != End; ++Start)
	{
		tstring UnitIllust = _T("unit_icon_") + (*Start);
		RESMGR->LoadGameTexture(CFilePathMgr::GetPath(_T("UI")) + UnitIllust + _T(".png"));
		RESMGR->CreateGameSprite(UnitIllust);
	}

	RESMGR->LoadGameTexture(CFilePathMgr::GetPath(_T("Texture")) + _T("StatBack.jpg"));
	RESMGR->CreateGameSprite(_T("StatBack"));

	RESMGR->LoadGameTexture(CFilePathMgr::GetPath(_T("UI")) + _T("ParameterBack.png"));
	RESMGR->CreateGameSprite(_T("ParameterBack"));

}
void CStatSceneBuilder::LoadingIdleImage(FOLDER _FolderIndex)
{
	if (FOLDERMAX <= _FolderIndex)
	{
		return;
	}

	if (TRUE == CEditGlobal::vecFolderData[_FolderIndex].CheckFinish)
	{
		return;
	}

	CFileFind Finder;

	CString Path = CFilePathMgr::GetPath(CEditGlobal::vecFolderData[_FolderIndex].FolderKey).c_str();
	BOOL bWorking;
	CString FileName;

	for (int i = 0; i < FILEMAX; i++)
	{
		CString TempPath;
		TempPath = Path + CFilePathMgr::ImageFile[i].c_str();
		bWorking = Finder.FindFile(TempPath);

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

				FileName = Finder.GetFileName();

				int Index = FileName.Find(_T("_idle"));
				if (-1 == Index)
				{
					continue;
				}

				tstring TexPath = Path + FileName;

				CGameTexture* TempTex = CEditGlobal::EditResourceMgr->LoadGameTexture(TexPath);

				if (nullptr == TempTex)
				{
					continue;
				}
				
				TempTex->SetFolderIndex(_FolderIndex);
			}
		}
	}
}
void CStatSceneBuilder::LoadingIdleAnimation(FOLDER _FolderIndex)
{
	if (EFFECT <= _FolderIndex) 
	{
		return;
	}

	CGameFile RFile = CGameFile(_T("EditData"), CGameFile::vecSpriteDataFileName[_FolderIndex]);

	if (FALSE == RFile.Open(_T("rb")))
	{
		return;
	}

	vector<MULTISPRITEDATA> vecRegData;
	vecRegData.reserve(64);

	CEditGlobal::LoadMultiSpriteData(vecRegData, RFile.GetFile());

	for (size_t i = 0; i < vecRegData.size(); i++)
	{
		tstring MultiSpriteKey = vecRegData[i].MultiSpriteKey;
		size_t Index = MultiSpriteKey.find(_T("_idle"));
	
		if (-1 == Index) 
		{
			continue;
		}

		RESMGR->LoadMultiSpriteToData(&vecRegData[i], (ANISPRITETYPE)_FolderIndex);
	
		ANIDATA AniData(vecRegData[i].MultiSpriteKey, vecRegData[i].MultiSpriteKey);
		AniData.SettingFullRage({ 0,vecRegData[i].ImageCount - 1 });

	
		m_StatIdleModel->CreateAnimation(AniData);
		

	}

}
void CStatSceneBuilder::SettingStatModel()
{
	for (size_t i = 0; i < MAXBATTLECHARACTER; i++)
	{
		CCriSectionObject<CGameScene>();
		CCriSectionObject<CActObject>();
		ANIMATIONDATA* CharIdle = Ani_Idle(CEditGlobal::CharacterName[i].GetString());

		if (nullptr != CharIdle)
			m_StatIdleModel->CreateAnimation(CharIdle);
	}
}