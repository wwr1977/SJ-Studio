// EffectDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "FREditor.h"
#include "EffectDlg.h"
#include "afxdialogex.h"
#include"EditEffectMgr.h"
#include"EditEffect.h"


// CEffectDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CEffectDlg, CDialogEx)

CEffectDlg::CEffectDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_EFFECT, pParent)
	, m_EffectName(_T(""))
	, m_SoundStartTime(_T(""))
	, m_LifeTime(_T(""))
	, m_ImageMag(_T(""))
	, m_RenderStartTime(_T(""))
	, m_bLoop(FALSE)
	, m_bBlackBack(FALSE)
	, m_bFixPos(FALSE)
	, m_bInit(false)
	, m_bMultiSpriteInit(false)
	, m_bModelSetting(false)
	, m_iListMode(-1)
	, m_iPrevIndex(-1)
	, m_iCustomSizeX(0)
	, m_iCustomSizeY(0)
	, m_fSoundStartTime(0.0f)
	, m_fLifeTime(0.0f)
	, m_fImageMag(0.0f)
	, m_fRenderStartTime(0.0f)
	, m_iCurLayer(PLAYERBACK)
	, m_EffectNumber(-1)
{

}

CEffectDlg::~CEffectDlg()
{
}

void CEffectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EFFECTLIST, m_EffectListCtrl);
	DDX_Control(pDX, IDC_EFFECTLISTCOM, m_ListModeCom);
	DDX_Control(pDX, IDC_EFFECTNAME, m_EffectNameCtrl);
	DDX_Text(pDX, IDC_EFFECTNAME, m_EffectName);
	DDX_Control(pDX, IDC_SOUNDCOMBO, m_SoundListComCtrl);
	DDX_Control(pDX, IDC_STARTTIME, m_SoundStartTimeCtrl);
	DDX_Text(pDX, IDC_STARTTIME, m_SoundStartTime);
	DDX_Control(pDX, IDC_EFFECTLIFETIME, m_LifeTimeCtrl);
	DDX_Text(pDX, IDC_EFFECTLIFETIME, m_LifeTime);
	DDX_Control(pDX, IDC_CUSTONSIZEX, m_CustomSizeXCtrl);
	DDX_Control(pDX, IDC_CUSTONSIZEY, m_CustomSizeYCtrl);
	DDX_Control(pDX, IDC_IMAGEMAG, m_ImageMagCtrl);
	DDX_Text(pDX, IDC_IMAGEMAG, m_ImageMag);
	DDX_Control(pDX, IDC_LAYERCOMBO, m_LayerComCtrl);
	DDX_Control(pDX, IDC_IMAGETIME, m_RenderStartTimeCtrl);
	DDX_Text(pDX, IDC_IMAGETIME, m_RenderStartTime);
	DDX_Check(pDX, IDC_LOOPCHECK, m_bLoop);
	DDX_Check(pDX, IDC_BLACKBACKCHECK, m_bBlackBack);
	DDX_Check(pDX, IDC_FIXPOS, m_bFixPos);
	DDX_Control(pDX, IDC_EFFECTANINAME, m_EffectAniNameCtrl);
	DDX_Text(pDX, IDC_CUSTONSIZEX, m_iCustomSizeX);
	DDX_Text(pDX, IDC_CUSTONSIZEY, m_iCustomSizeY);
	DDX_Control(pDX, IDC_SELECTSOUND, m_CurSoundCtrl);
	DDX_Text(pDX, IDC_EFFECTNUMBER, m_EffectNumber);
}


BEGIN_MESSAGE_MAP(CEffectDlg, CDialogEx)
	ON_WM_SHOWWINDOW()
	ON_CBN_SELCHANGE(IDC_EFFECTLISTCOM, &CEffectDlg::OnCbnSelchangeEffectlistcom)
	ON_LBN_SELCHANGE(IDC_EFFECTLIST, &CEffectDlg::OnLbnSelchangeEffectlist)
	ON_EN_KILLFOCUS(IDC_STARTTIME, &CEffectDlg::OnEnKillfocusStarttime)
	ON_EN_KILLFOCUS(IDC_EFFECTLIFETIME, &CEffectDlg::OnEnKillfocusEffectlifetime)
	ON_EN_KILLFOCUS(IDC_IMAGEMAG, &CEffectDlg::OnEnKillfocusImagemag)
	ON_EN_KILLFOCUS(IDC_IMAGETIME, &CEffectDlg::OnEnKillfocusImagetime)
	ON_CBN_SELCHANGE(IDC_SOUNDCOMBO, &CEffectDlg::OnCbnSelchangeSoundcombo)
	ON_CBN_SELCHANGE(IDC_LAYERCOMBO, &CEffectDlg::OnCbnSelchangeLayercombo)
END_MESSAGE_MAP()


// CEffectDlg 메시지 처리기입니다.
void CEffectDlg::InitEffectDlg()
{
	if (true == m_bInit) 
	{
		return;
	}

	LoadEffectTexture();
	LoadEffectMultiSprite();
	LoadEffectAnimation();
	m_bInit = true;
	m_bModelSetting = true;
}
void CEffectDlg::CompleteEffectSetting()
{
	m_bInit = true;
	m_bModelSetting = true;
}
void CEffectDlg::LoadEffectTexture()
{
	if (FALSE == CEditGlobal::vecFolderData[EFFECT].CheckLoadingStart)
	{
		tstring DirPath = CFilePathMgr::GetPath(CEditGlobal::vecFolderData[EFFECT].FolderKey);
		CEditGlobal::vecFolderData[EFFECT].CheckLoadingStart = TRUE;
		CEditGlobal::LoadingImage(DirPath, &CEditGlobal::vecFolderData[EFFECT]);
	}

	if (TRUE == CEditGlobal::vecFolderData[EFFECT].CheckLoading && FALSE == CEditGlobal::vecFolderData[EFFECT].CheckFinish)
		CEditGlobal::CurLoadTexMap();

}
BOOL CEffectDlg::LoadEffectMultiSprite()
{
	if (FALSE == CEditGlobal::vecFolderData[EFFECT].CheckFinish) 
	{
		return FALSE;
	}

	CGameFile RFile = CGameFile(_T("EditData"), CGameFile::vecSpriteDataFileName[EFFECT]);

	if (FALSE == RFile.Open(_T("rb")))
	{
		MessageBox(CEditGlobal::vecSaveDataFileName[EFFECT] + _T("을 열지 못하였습니다."), _T("MultiSpriteDlg"), MB_ICONERROR);
		return FALSE;
	}

	vector<MULTISPRITEDATA> vecRegData;
	vecRegData.reserve(128);

	if (FALSE == CEditGlobal::EditResourceMgr->LoadSpriteData(RFile.GetFile(), (ANISPRITETYPE)ANI_EFFECT, &vecRegData))
	{
		MessageBox(CEditGlobal::vecSaveDataFileName[EFFECT] + _T("의 데이터 로드에 실패하였습니다."), _T("EffectDlg"), MB_ICONERROR);
		return FALSE;
	}

	for (size_t i = 0; i < vecRegData.size(); i++)
	{
		CEditGlobal::EditResourceMgr->CreateEditGameSprite(
			vecRegData[i].TexKey
			, vecRegData[i].MultiSpriteKey
			, { vecRegData[i].StartPosX,vecRegData[i].StartPosY }
			, { vecRegData[i].SizeX,vecRegData[i].SizeY }
		);

		//Texture Model에서 사용하는 애들의 정보를 셋팅을 대신해준다.
		ANIDATA AniData(vecRegData[i].MultiSpriteKey, vecRegData[i].MultiSpriteKey);
		AniData.SettingFullRage({ 0,vecRegData[i].ImageCount - 1 });

		CEditGlobal::MBaseAniCom->CreateEditAnimation(AniData);
	}
	return TRUE;
}
void CEffectDlg::LoadEffectAnimation()
{

	CGameFile RFile = CGameFile(_T("EditData"), CGameFile::vecAniDataFileName[EFFECT]);

	if (FALSE == RFile.Open(_T("rb")))
	{
		MessageBox(CEditGlobal::vecSaveDataFileName[EFFECT] + _T("을 열지 못하였습니다."), _T("MultiSpriteDlg"), MB_ICONERROR);
		return;
	}

	map<tstring, ANIMATIONDATA*> TempMap;

	ANIMATIONDATA::ReadAnimationData(RFile.GetFile(), &TempMap);

	if (0 >= TempMap.size())
	{
		MessageBox(_T("데이터 파일이 비어있습니다!"), _T("EffectDlg"), MB_ICONERROR);
		return ;
	}

	map<tstring, ANIMATIONDATA*>::iterator Iter = TempMap.begin();

	int RegAniCount = 0;

	CEditGlobal::EEffectAniModel->ClearAnimationMap();

	for (; Iter != TempMap.end(); ++Iter)
	{
		CAniRenderer::CAnimation* NewAni;
		if (nullptr != (NewAni = CEditGlobal::EEffectAniModel->CreateEditAnimation(Iter->second->GetAniData())))
		{
			NewAni->SetAniStyle((ANISTYLE)Iter->second->AniStyle);
			++RegAniCount;
		}
	}

	//CEditGlobal::ClearAnimationDataMap();
	CEditGlobal::AnimationDataMap.insert(TempMap.begin(), TempMap.end());

	if (RegAniCount == TempMap.size())
	{
		return;
	}

	MessageBox(CEditGlobal::vecSaveDataFileName[EFFECT] + _T("의 데이터 일부분을 Model에 적용하지 못했습니다."));
	return;

}
void CEffectDlg::UpdateEffectAniList()
{
	m_CurSoundCtrl.ResetContent();
	map<tstring, SPTR<CAniRenderer::CAnimation>>* AniMap = CEditGlobal::EEffectAniModel->GetAniMap();

	if (nullptr == AniMap) 
	{
		return;
	}

	m_EffectListCtrl.ResetContent();

	map<tstring, SPTR<CAniRenderer::CAnimation>>::iterator Start = AniMap->begin();
	map<tstring, SPTR<CAniRenderer::CAnimation>>::iterator End = AniMap->end();


	for (;Start!= End; ++Start)
	{
		CString EffectAniName = Start->first.c_str();
		m_EffectListCtrl.AddString(EffectAniName);
	}

	return;
}
void CEffectDlg::UpdateSoundList() 
{
	
	unordered_map<tstring, SPTR<CGameSound>>* SoundMap = CEditGlobal::EditResourceMgr->GetSoundMap();

	if (nullptr == SoundMap) 
	{
		assert(nullptr);
		return;
	}

	m_EffectListCtrl.ResetContent();

	unordered_map<tstring, SPTR<CGameSound>>::iterator Start = SoundMap->begin();
	unordered_map<tstring, SPTR<CGameSound>>::iterator End = SoundMap->end();

	for (;Start!=End; ++Start)
	{
		CString SoundName = Start->first.c_str();
		m_EffectListCtrl.AddString(SoundName);
	}

}
void CEffectDlg::UpdateEffectList() 
{
	UpdateData(TRUE);
	m_EffectName = _T("");
	UpdateData(FALSE);

	m_EffectAniNameCtrl.ResetContent();
	m_EffectListCtrl.ResetContent();
	m_CurSoundCtrl.ResetContent();
	m_SoundListComCtrl.ResetContent();
	m_listSoundData.clear();

	vector<SPTR<CEditEffect>>* EffectVec = CEditGlobal::EffectMgr->GetEffectVector();

	if (nullptr == EffectVec)
	{
		assert(nullptr);
		return;
	}

	for (size_t i = 0; i < EffectVec->size(); i++)
	{
		if (nullptr != (*EffectVec)[i]) 
		{
			m_EffectListCtrl.AddString((*EffectVec)[i]->GetEffectName().c_str());
		}
	}
	CEditGlobal::InitEffectModel();

	InitEffectData();

	return;
}

void CEffectDlg::UpdateList(const int& _Index)
{
	m_iPrevIndex = -1;
	CEditGlobal::EffectMgr->EffectAllOff();

	switch (_Index)
	{
	case 0: UpdateEffectAniList();break;
	case 1: UpdateSoundList(); break;
	case 2: UpdateEffectList();break;
	default:
		break;
	}

	return;
}

void CEffectDlg::SelectEffectAniList() 
{
	int Index = m_EffectListCtrl.GetCurSel();

	if (Index == m_iPrevIndex)
	{
		return;
	}

	CString SelectEffectAniName;  
	m_EffectListCtrl.GetText(Index, SelectEffectAniName);

	BOOL Check = CEditGlobal::EEffectAniModel->ExistAnimation(SelectEffectAniName.GetString());

	if (FALSE == Check) 
	{
		assert(nullptr);
		return;
	}

	m_EffectAniNameCtrl.ResetContent();

	m_EffectAniNameCtrl.AddString(SelectEffectAniName);

	CEditGlobal::EEffectAniModel->ChangeAni(SelectEffectAniName.GetString());
	CEditGlobal::EEffectAniModel->On();
	
	InitEffectData();
	UpdateData(TRUE);
	m_EffectName = SelectEffectAniName;
	m_fLifeTime = CEditGlobal::EEffectAniModel->GetFullAnimationTime();
	m_LifeTime.Format(_T("%.2f"), m_fLifeTime);
	UpdateData(FALSE);


	m_listSoundData.clear();
	m_SoundListComCtrl.ResetContent();
	
	m_iPrevIndex = Index;
}
void CEffectDlg::SelectSoundList() 
{

	CString Sound;
	int Index = m_EffectListCtrl.GetCurSel();

	if (Index == m_iPrevIndex)
	{
		return;
	}

	m_EffectListCtrl.GetText(Index, Sound);

	BOOL Check = CEditGlobal::EditResourceMgr->ExistGameSound(Sound.GetString());

	if (FALSE == Check) 
	{
		assert(nullptr);
		return;
	}

	m_CurSoundCtrl.ResetContent();
	m_CurSoundCtrl.AddString(Sound);
	CEditGlobal::ESoundPlayer->CurPlaySoundStop();
	CEditGlobal::ESoundPlayer->PlaySound(Sound.GetString());

	m_iPrevIndex = Index;
}
void CEffectDlg::SelectEffectList() 
{
	CString EffectName;
	int Index = m_EffectListCtrl.GetCurSel();

	m_EffectListCtrl.GetText(Index, EffectName);

	SPTR<CEditEffect> Effect = CEditGlobal::EffectMgr->FindEffect(EffectName.GetString());

	if (nullptr == Effect) 
	{
		assert(nullptr);
		return;
	}

	CEditGlobal::EffectMgr->EffectAllOff();
	Effect->EffectOn();
	
	InitEffectData(Effect->GetEffectData());

	m_iPrevIndex = Index;
}
void CEffectDlg::InitEffectData(const EFFECTDATA& _EffectData /*= EFFECTDATA()*/)
{
	int Mode = m_ListModeCom.GetCurSel();

	tstring EffectName = _EffectData.EffectName;

	UpdateData(TRUE);
	if (_T("") == EffectName) 
	{
		m_EffectName = _T("");
		m_SoundStartTime = _T("0.0");
		m_fSoundStartTime = 0.0f;

		
		m_LifeTime = _T("0.0");
		m_fLifeTime = 0.0f;	
		

		m_iCustomSizeX = 0;
		m_iCustomSizeY = 0;
		
		m_fImageMag = 1.0f;
		m_ImageMag.Format(_T("%.2f"),m_fImageMag);

		m_RenderStartTime = _T("0.0");
		m_fRenderStartTime = 0.0f;

		if(nullptr != CEditGlobal::EffectMgr)
			m_EffectNumber =  CEditGlobal::EffectMgr->SearchNextEffectIndex();

	}
	else 
	{
		// 이팩트 매니져 안에 등록된 이팩트를 클릭한 경우 
		m_EffectName = _EffectData.EffectName;
		m_EffectAniNameCtrl.ResetContent();
		m_EffectAniNameCtrl.AddString(_EffectData.EffectAniName);

		m_fSoundStartTime = 0.0f;
		m_SoundStartTime = _T("0.0");

		m_fLifeTime = _EffectData.LifeTime;
		m_LifeTime.Format(_T("%.2f"), m_fLifeTime);

		m_iCustomSizeX = (int)_EffectData.CustomSize.x;
		m_iCustomSizeY = (int)_EffectData.CustomSize.y;

		m_fImageMag = _EffectData.ImageScale;
		m_ImageMag.Format(_T("%.2f"),m_fImageMag);

		m_fRenderStartTime = _EffectData.PlayRender;;
		m_RenderStartTime.Format(_T("%.2f"), m_fRenderStartTime);

		m_bFixPos = _EffectData.NonCameraAffect;
		m_bLoop = _EffectData.Loop;
		m_bBlackBack = _EffectData.BlackBack;

		m_EffectNumber = _EffectData.EffectIndex;

		switch (_EffectData.RenderLayer)
		{
		case LAYER_MONBACK:
			m_LayerComCtrl.SetCurSel(0);
			break;
		case LAYER_BACKEFFECT:
			m_LayerComCtrl.SetCurSel(1);
			break;
		case LAYER_FRONTEFFECT:
			m_LayerComCtrl.SetCurSel(2);
			break;
		default:
			break;
		}

		m_SoundListComCtrl.ResetContent();
		m_listSoundData.clear();

		for (int i = 0; i < _EffectData.SoundCount; i++)
		{
			m_listSoundData.push_back(SoundData(_EffectData.SoundName[i], _EffectData.SoundStartTime[i]));
			m_SoundListComCtrl.AddString(_EffectData.SoundName[i]);
		}

		
	}
	UpdateData(FALSE);

}
// 이펙트 에디터를 처음키거나 애니메이션 데이터 파일이 변경되었을 경우
// 이 함수를 호출한다
void CEffectDlg::SettingEffectModel()
{
	// 이 함수가 false 상태에서 EffectEditor의 애니메이션 관련 콤보박스를 클릭하면
	// 변경된 데이터 파일을 다시 로딩하고 애니메이션 모델의 모든 애니메이션을 
	// 변경된 데이터로 바꾸워 준다.

	m_bModelSetting = false;
}
BOOL CEffectDlg::SoundPlayEvent()
{
	CString Sound;
	int Count = m_CurSoundCtrl.GetCount();
	
	if (Count == 0)
	{
		return FALSE;
	}


	m_CurSoundCtrl.GetText(0, Sound);

	CEditGlobal::ESoundPlayer->CurPlaySoundStop();
	CEditGlobal::ESoundPlayer->PlaySound(Sound.GetString());


	return TRUE;
}
BOOL CEffectDlg::SoundAddButtonEvent()
{
	if (SOUND != m_iListMode) 
	{
		return FALSE;
	}

	CString SelectSoundName;
	int Index = m_EffectListCtrl.GetCurSel();
	if (-1 == Index) 
	{	
		return FALSE;
	}
	m_EffectListCtrl.GetText(Index, SelectSoundName);

	if (FALSE == CEditGlobal::EditResourceMgr->ExistGameSound(SelectSoundName.GetString())) 
	{
		assert(nullptr);
		return FALSE;
	}

	int SoundCount = m_SoundListComCtrl.GetCount();

	if (EFFECTDATA::MAXSOUNDCOUNT <= SoundCount) 
	{
		MessageBox(_T("더 이상 사운드를 지정할 수 없습니다."), _T("EffectDlg"), MB_ICONERROR);
		return FALSE;
	}

	for (int i = 0; i < SoundCount; i++)
	{
		CString SoundName;
		m_SoundListComCtrl.GetLBText(i, SoundName);
		/*if (SelectSoundName == SoundName) 
		{
			MessageBox(_T("이미 지정된 사운드 입니다."), _T("EffectDlg"), MB_ICONERROR);
			return FALSE;
		}*/

	}

	m_SoundListComCtrl.AddString(SelectSoundName);
	m_SoundListComCtrl.SetCurSel(SoundCount);

	m_listSoundData.push_back(SoundData(SelectSoundName.GetString(), 0.0f));

	return TRUE;
}
BOOL CEffectDlg::SoundDeleteButtonEvent() 
{
	if (EFFECTANIMATION == m_iListMode)
	{
		return FALSE;
	}

	int Index = m_SoundListComCtrl.GetCurSel();

	if (0 > Index) 
	{
		return FALSE;
	}

	if (EFFECTDATA::MAXSOUNDCOUNT <= Index)
	{
		assert(nullptr);
	}

	CString CurSoundName;
	m_SoundListComCtrl.GetLBText(Index, CurSoundName);

	list<SoundData>::iterator Start = m_listSoundData.begin();
	list<SoundData>::iterator End = m_listSoundData.end();

	for (;Start!= End; ++Start)
	{
		if (Start->SoundName == CurSoundName.GetString()) 
		{
			Start = m_listSoundData.erase(Start);
			break;
		}
	}

	m_SoundListComCtrl.ResetContent();
	
	Start = m_listSoundData.begin();
	End = m_listSoundData.end();

	for (;Start!= End; ++Start)
	{
		m_SoundListComCtrl.AddString(Start->SoundName.c_str());
	}

	return TRUE;
}
BOOL CEffectDlg::SoundTimeButtonEvent()
{
	int Index = m_SoundListComCtrl.GetCurSel();

	if (-1 >= Index) 
	{
		return FALSE;
	}

	if (Index >= (int)m_listSoundData.size())
	{
		assert(nullptr);
		return FALSE;
	}

	list<SoundData>::iterator Start = m_listSoundData.begin();
	list<SoundData>::iterator End = m_listSoundData.end();

	for (; Start!= End ; ++Start)
	{
		if (Index <= 0) 
		{
			Start->StartTime = m_fSoundStartTime;
			return TRUE;
		}
		--Index;
	}

	return FALSE;
}
BOOL CEffectDlg::CreateButtonEvent() 
{
	if (FALSE == CheckEffectDataValue()) 
	{
		return FALSE;
	}

	CString EffectAni;
	UpdateData(TRUE);
	m_EffectAniNameCtrl.GetText(0, EffectAni);
	EFFECTDATA EffectData = EFFECTDATA(m_EffectName.GetString(),EffectAni.GetString());
	
	list<SoundData>::iterator Start = m_listSoundData.begin();
	list<SoundData>::iterator End = m_listSoundData.end();

	for (;Start!= End; ++Start)
	{
		EffectData.AddSoundData(Start->SoundName, Start->StartTime);
	}

	EffectData.LifeTime = m_fLifeTime;
	EffectData.CustomSize = Vec2{ (float)m_iCustomSizeX,(float)m_iCustomSizeY };
	EffectData.ImageScale = m_fImageMag;
	EffectData.PlayRender = m_fRenderStartTime;
	EffectData.Loop = m_bLoop;
	EffectData.BlackBack = m_bBlackBack;
	// 카메라 위치에 영향을 받지 않는 경우 이 값이 true 가 된다(UI 같은 랜더러 레이어에 포함된다)
	EffectData.NonCameraAffect = m_bFixPos;

	EffectData.EffectIndex = (UINT)m_EffectNumber;

	switch (m_iCurLayer)
	{
	case CEffectDlg::MONBACK:
		EffectData.RenderLayer = LAYER_MONBACK;
		break;
	case CEffectDlg::PLAYERBACK:
		EffectData.RenderLayer = LAYER_BACKEFFECT;
		break;
	case CEffectDlg::PLAYERFRONT:
		EffectData.RenderLayer = LAYER_FRONTEFFECT;
		break;
	default:
		break;
	}
	UpdateData(FALSE);

	int Index = m_ListModeCom.GetCurSel();



	if (EFFECT == Index)
	{
		if (nullptr == CEditGlobal::EffectMgr->ChangeEffect(EffectData)) 
			MessageBox(_T("Effect 생성에 실패하였습니다."));
		
		MessageBox(_T("Effect를 변경하였습니다."));
		return FALSE;
	}

	if (nullptr == CEditGlobal::EffectMgr->CreateEffect(EffectData)) 
	{
		MessageBox(_T("Effect 생성에 실패하였습니다."));
		return FALSE;
	}
	

	return TRUE;
}
BOOL CEffectDlg::DeleteButtonEvent() 
{
	if (EFFECT != m_ListModeCom.GetCurSel()) 
	{
		return FALSE;
	}

	CString EffectName;
	int Index = m_EffectListCtrl.GetCurSel();
	m_EffectListCtrl.GetText(Index, EffectName);

	m_EffectListCtrl.DeleteString(Index);

	return CEditGlobal::EffectMgr->DeleteEffect(EffectName.GetString());

}
BOOL CEffectDlg::DataLoadButtonEvent() 
{
	/*if (false == m_bMultiSpriteInit && true == m_bModelSetting )
	{
		if (FALSE == LoadEffectMultiSprite()) 
		{
			return FALSE;
		}
		LoadEffectAnimation();
		m_bMultiSpriteInit = true;
		m_bModelSetting = true;
	}*/

	if (false == m_bInit) 
	{
		LoadEffectTexture();
		LoadEffectMultiSprite();
		LoadEffectAnimation();
		m_bMultiSpriteInit = true;
		m_bModelSetting = true;
	}

	CGameFile LoadFile(_T("EditData"), _T("EffectData.bin"));

	if (FALSE == LoadFile.Open(_T("rb")))
	{
		MessageBox(_T("EffectDataFile을 열지 못하였습니다."));
		return FALSE;

	}

	vector<EFFECTDATA> TempVec; 
	TempVec.reserve(64);
	EFFECTDATA::LoadDataToFile(LoadFile.GetFile(), &TempVec);
	BOOL Check  = CEditGlobal::EffectMgr->LoadEffectData(LoadFile.GetFile(), &TempVec);

	

	

	if (FALSE == Check)
	{
		MessageBox(_T("Effect Data File의 정상적으로 읽지 못하였습니다."));
	}
	else 
	{
		MessageBox(_T("Effect Data File의 정상적으로 읽어들었습니다."));
	}
	
	m_EffectListCtrl.ResetContent();
	m_iListMode = EFFECT;
	m_ListModeCom.SetCurSel(m_iListMode);
	UpdateList(m_iListMode);


	return Check;

}
BOOL CEffectDlg::DataSaveButtonEvent()
{
	if (CEditGlobal::EffectMgr->EmptyEffectMap()) 
	{
		return FALSE;
	}

	CGameFile EffectFile(_T("EditData"), _T("EffectData.bin"));

	if (FALSE == EffectFile.Open(_T("wb"))) 
	{
		MessageBox(_T("EffectDataFile을 열지 못하였습니다."));
		return FALSE;
	}

	BOOL Check = CEditGlobal::EffectMgr->SaveEffectData(EffectFile.GetFile());
	
	if (TRUE == Check) 
	{
		MessageBox(_T("EffectData를 정상적으로 저장하였습니다."));
	}
	else 
	{
		MessageBox(_T("일부 EffectData 저장에 실패하였습니다."));
	}

	return Check;
}
BOOL CEffectDlg::CheckEffectDataValue()
{
	UpdateData(TRUE);

	if(0 == m_EffectAniNameCtrl.GetCount())
	{
		MessageBox(_T("이팩트 애니메이션을 선택하십시오"));
		return FALSE;
	}
	if (_T("") == m_EffectName) 
	{
		MessageBox(_T("이팩트 이름을 지정하십시오"));
		return FALSE;
	}

	if (0.0f > m_fSoundStartTime) 
	{
		m_fSoundStartTime = 0.0f;
		m_SoundStartTime.Format(_T("%.2f"), m_fSoundStartTime);
	}
	if (0.0f > m_fLifeTime ) 
	{
		MessageBox(_T("올바르지 않은 값이 들어왔습니다.(LifeTime)"));
		m_fLifeTime = 0.0f;
		m_LifeTime.Format(_T("%.2f"), m_fLifeTime);
		return FALSE;
	}

	if (0 > m_iCustomSizeX || 0 > m_iCustomSizeY) 
	{
		m_iCustomSizeX = 0;
		m_iCustomSizeY = 0;
	}

	if (0.0f > m_fImageMag) 
	{
		m_fImageMag = 0.0f;
		m_ImageMag.Format(_T("%.2f"), m_fImageMag);
	}
	if (0.0f > m_fRenderStartTime) 
	{
		m_fRenderStartTime = 0.0f;
		m_RenderStartTime.Format(_T("%.2f"), m_fRenderStartTime);
	}

	int Layer = m_LayerComCtrl.GetCurSel();

	if (0 > Layer) 
	{
		MessageBox(_T("랜더링 레이어를 설정하십시오"));
		return FALSE;
	}
	UpdateData(FALSE);
	return TRUE;
}
BOOL CEffectDlg::EditBoxClickCheck(MSG* pMsg)
{	
	for (size_t i = 0; i < m_vecEditBox.size(); i++)
	{
		if (pMsg->hwnd == m_vecEditBox[i]->m_hWnd
			&& GetFocus()->m_hWnd != m_vecEditBox[i]->m_hWnd) 
		{
			m_vecEditBox[i]->SetFocus();
			m_vecEditBox[i]->SetSel(0, -1,true);
			return TRUE;
		}
	}
	return FALSE;
}
void CEffectDlg::OnOK()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	return;
}


BOOL CEffectDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{
		if (GetFocus()->m_hWnd == GetDlgItem(IDC_SOUNDPLAY)->m_hWnd)
		{
			SoundPlayEvent();
			return TRUE;
		}
		if (GetFocus()->m_hWnd == GetDlgItem(IDC_SOUNDADDBUTTON)->m_hWnd)
		{
			SoundAddButtonEvent();
			return TRUE;
		}
		if (GetFocus()->m_hWnd == GetDlgItem(IDC_SOUNDDELBUTTON)->m_hWnd)
		{
			SoundDeleteButtonEvent();
			return TRUE;
		}
		if (GetFocus()->m_hWnd == GetDlgItem(IDC_SOUNDTIMEBUTTON)->m_hWnd)
		{
			SoundTimeButtonEvent();
			return TRUE;
		}
		if (GetFocus()->m_hWnd == GetDlgItem(IDC_EFFECTCREATEBUTTON)->m_hWnd)
		{
			CreateButtonEvent();
			return TRUE;
		}
		if (GetFocus()->m_hWnd == GetDlgItem(IDC_EFFECTDELETEBUTTON)->m_hWnd)
		{
			DeleteButtonEvent();
			return TRUE;
		}
		if (GetFocus()->m_hWnd == GetDlgItem(IDC_EFFECTLOADBUTTON)->m_hWnd)
		{
			DataLoadButtonEvent();
			return TRUE;
		}
		if (GetFocus()->m_hWnd == GetDlgItem(IDC_EFFECTSAVEBUTTON)->m_hWnd)
		{
			DataSaveButtonEvent();
			return TRUE;
		}
	}

	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE) {
			// VK_RETURN  : Enter Key
			// VK_ESCAPE  : ESC key

			return TRUE;
		}
	}

	if (pMsg->message == WM_LBUTTONDOWN)
	{

		SendMessageA(this->m_hWnd, WM_SETFOCUS, NULL, NULL);

		if (pMsg->hwnd == GetDlgItem(IDC_SOUNDPLAY)->m_hWnd)
		{
			SoundPlayEvent();
			return TRUE;
		}
		if (pMsg->hwnd == GetDlgItem(IDC_SOUNDADDBUTTON)->m_hWnd)
		{
			SoundAddButtonEvent();
			return TRUE;
		}
		if (pMsg->hwnd == GetDlgItem(IDC_SOUNDDELBUTTON)->m_hWnd)
		{
			SoundDeleteButtonEvent();
			return TRUE;
		}
		if (pMsg->hwnd == GetDlgItem(IDC_SOUNDTIMEBUTTON)->m_hWnd)
		{
			SoundTimeButtonEvent();
			return TRUE;
		}
		if (pMsg->hwnd == GetDlgItem(IDC_EFFECTCREATEBUTTON)->m_hWnd)
		{
			CreateButtonEvent();
			return TRUE;
		}
		if (pMsg->hwnd == GetDlgItem(IDC_EFFECTDELETEBUTTON)->m_hWnd)
		{
			DeleteButtonEvent();
			return TRUE;
		}
		if (pMsg->hwnd == GetDlgItem(IDC_EFFECTLOADBUTTON)->m_hWnd)
		{
			DataLoadButtonEvent();
			return TRUE;
		}
		if (pMsg->hwnd == GetDlgItem(IDC_EFFECTSAVEBUTTON)->m_hWnd)
		{
			DataSaveButtonEvent();
			return TRUE;
		}

		if (TRUE == EditBoxClickCheck(pMsg)) 
		{
			return TRUE;
		}

		
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}


BOOL CEffectDlg::OnInitDialog()
{

	CDialogEx::OnInitDialog();
	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_ListModeCom.AddString(_T("Effect Animation"));
	m_ListModeCom.AddString(_T("Sound"));
	m_ListModeCom.AddString(_T("Effect"));

	
	m_LayerComCtrl.AddString(_T("Moster Back"));
	m_LayerComCtrl.AddString(_T("Player Back"));
	m_LayerComCtrl.AddString(_T("Player Front"));

	
	m_vecEditBox.push_back(&m_EffectNameCtrl);
	m_vecEditBox.push_back(&m_SoundStartTimeCtrl);
	m_vecEditBox.push_back(&m_LifeTimeCtrl);
	m_vecEditBox.push_back(&m_CustomSizeXCtrl);
	m_vecEditBox.push_back(&m_CustomSizeYCtrl);
	m_vecEditBox.push_back(&m_ImageMagCtrl);
	m_vecEditBox.push_back(&m_RenderStartTimeCtrl);

	m_LayerComCtrl.SetCurSel(PLAYERBACK);
	InitEffectData();
	CEditGlobal::EffectDlg = this;

	
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CEffectDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	if (TRUE  == bShow)
	{
		if (false == m_bInit) 
		{
			LoadEffectTexture();
			LoadEffectMultiSprite();
			LoadEffectAnimation();
			m_bInit = true;
			m_bModelSetting = true;
			return;
		}
		/*if (true == m_bInit && false == m_bModelSetting) 
		{	
			LoadEffectMultiSprite();
			LoadEffectAnimation();
			m_bModelSetting = true;
			return;
		}*/

	}
	else 
	{
		// Hide 할 때 불러올 함수
		CEditGlobal::InitEffectModel();
		m_iPrevIndex = -1;
	}

	CDialogEx::OnShowWindow(bShow, nStatus);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	
}


void CEffectDlg::OnCbnSelchangeEffectlistcom()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (false == m_bMultiSpriteInit) 
	{
		if (FALSE == LoadEffectMultiSprite()) 
		{
			return;
		}
		m_bMultiSpriteInit = true;
	}


	int ListIndex = m_ListModeCom.GetCurSel();

	if (false == m_bModelSetting) 
	{
		LoadEffectAnimation();
		m_bModelSetting = true;
	}
	

	m_iListMode = ListIndex;

	UpdateList(ListIndex);
}


void CEffectDlg::OnLbnSelchangeEffectlist()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	if (m_iListMode > 2) 
	{
		assert(nullptr);
	}

	switch (m_iListMode)
	{
	case EFFECTANIMATION: SelectEffectAniList(); break;
	case SOUND: SelectSoundList(); break;
	case EFFECT: SelectEffectList(); break;
	default:
		break;
	}
}



BOOL CEffectDlg::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (nID == IDC_STARTTIME) 
	{
		UpdateData(TRUE);
		int Sel = m_SoundStartTimeCtrl.GetSel();
		UpdateData(TRUE);
		CEditGlobal::ChangeFloatString(m_SoundStartTime);
		UpdateData(FALSE);
		m_SoundStartTimeCtrl.SetSel(Sel);
		UpdateData(FALSE);
	}
	else if (nID == IDC_EFFECTLIFETIME)
	{
		UpdateData(TRUE);
		int Sel = m_LifeTimeCtrl.GetSel();
		UpdateData(TRUE);
		CEditGlobal::ChangeFloatString(m_LifeTime);
		UpdateData(FALSE);
		m_LifeTimeCtrl.SetSel(Sel);
		UpdateData(FALSE);
	}
	else if (nID == IDC_IMAGETIME)
	{
		UpdateData(TRUE);
		int Sel = m_RenderStartTimeCtrl.GetSel();
		UpdateData(TRUE);
		CEditGlobal::ChangeFloatString(m_RenderStartTime);
		UpdateData(FALSE);
		m_RenderStartTimeCtrl.SetSel(Sel);
		UpdateData(FALSE);
	}
	else if (nID == IDC_IMAGEMAG)
	{
		UpdateData(TRUE);
		int Sel = m_ImageMagCtrl.GetSel();
		UpdateData(TRUE);
		CEditGlobal::ChangeFloatString(m_ImageMag);
		UpdateData(FALSE);
		m_ImageMagCtrl.SetSel(Sel);
		UpdateData(FALSE);
	}

	return CDialogEx::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

// 사운드 재생 시간 애디터가 포커스 아웃됬을때 
// 실수 변수에 문자열실수값 적용
void CEffectDlg::OnEnKillfocusStarttime()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	float Time = CGameString::StringToFloat(m_SoundStartTime.GetString());
	if (Time < 0.0f) 
	{
		MessageBox(_T("입력한 값이 음수 입니다."));
		m_SoundStartTimeCtrl.SetFocus();
		m_SoundStartTimeCtrl.SetSel(0, -1, true);
		return;
	}
	else 
	{
		m_fSoundStartTime = Time;
		m_SoundStartTime.Format(_T("%.2f"), m_fSoundStartTime);
	}
	UpdateData(FALSE);

}


void CEffectDlg::OnEnKillfocusEffectlifetime()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	float Time = CGameString::StringToFloat(m_LifeTime.GetString());
	if (Time < 0.0f)
	{
		MessageBox(_T("입력한 값이 음수 입니다."));
		m_LifeTimeCtrl.SetFocus();
		m_LifeTimeCtrl.SetSel(0, -1, true);
		return;
	}
	else
	{
		m_fLifeTime = Time;
		m_LifeTime.Format(_T("%.2f"), m_fLifeTime);
	}
	UpdateData(FALSE);

}


void CEffectDlg::OnEnKillfocusImagemag()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	float ImageMag = CGameString::StringToFloat(m_ImageMag.GetString());
	if (ImageMag < 0.0f)
	{
		MessageBox(_T("입력한 값이 음수 입니다."));
		m_ImageMagCtrl.SetFocus();
		m_ImageMagCtrl.SetSel(0, -1, true);
		return;
	}
	else
	{
		m_fImageMag = ImageMag;
		m_ImageMag.Format(_T("%.2f"), m_fImageMag);
	}
	UpdateData(FALSE);
}

// 이미지 재생시간 에디터 박스가 포커스 아웃되었을때
// 실수 변수에 문자열실수 값을 채우는 작업
void CEffectDlg::OnEnKillfocusImagetime()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	float Time = CGameString::StringToFloat(m_RenderStartTime.GetString());
	if (Time < 0.0f)
	{
		MessageBox(_T("입력한 값이 음수 입니다."));
		m_RenderStartTimeCtrl.SetFocus();
		m_RenderStartTimeCtrl.SetSel(0, -1, true);
		return;
	}
	else
	{
		m_fRenderStartTime = Time;
		m_RenderStartTime.Format(_T("%.2f"), m_fRenderStartTime);
		
		m_fLifeTime += Time;
		m_LifeTime.Format(_T("%.2f"), m_fLifeTime);
		
	}
	UpdateData(FALSE);
}


void CEffectDlg::OnCbnSelchangeSoundcombo()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	int Index = m_SoundListComCtrl.GetCurSel();

	if (-1 >= Index)
	{
		return;
	}

	if (Index >= (int)m_listSoundData.size())
	{
		assert(nullptr);
		return;
	}

	list<SoundData>::iterator Start = m_listSoundData.begin();
	list<SoundData>::iterator End = m_listSoundData.end();

	for (; Start != End; ++Start)
	{
		if (Index <= 0)
		{
			UpdateData(TRUE);
			m_fSoundStartTime = Start->StartTime;
			m_SoundStartTime.Format(_T("%.2f"), m_fSoundStartTime);
			UpdateData(FALSE);
			return;
		}
		--Index;
	}


}


void CEffectDlg::OnCbnSelchangeLayercombo()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int Layer = m_LayerComCtrl.GetCurSel();

	if (MAXLAYERINDEX <= Layer) 
	{
		assert(nullptr);
	}
	m_iCurLayer = (LAYERINDEX)Layer;
}

