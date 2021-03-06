// EditDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "SSDTool.h"
#include "EditDlg.h"
#include "afxdialogex.h"
#include "SSDToolGlobal.h"

#include <Actor.h>
#include <TransForm.h>
// EditDlg 대화 상자

#include "FilterDlg.h"
#include "LightDlg.h"
#include "MaterialDlg.h"
#include "NaviDlg.h"
#include "ToolMainScene.h"
#include "FogDlg.h"

IMPLEMENT_DYNAMIC(EditDlg, CDialogEx)


EditDlg::EditDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_EDITDLG, pParent)
{

}

EditDlg::~EditDlg()
{
	if (nullptr != SSDToolGlobal::ToolLightDlg)
	{
		SSDToolGlobal::ToolLightDlg->DestroyWindow();
		delete SSDToolGlobal::ToolLightDlg;
	}

	if (nullptr != SSDToolGlobal::ToolMatDlg)
	{
		SSDToolGlobal::ToolMatDlg->DestroyWindow();
		delete SSDToolGlobal::ToolMatDlg;
	}

	if (nullptr != SSDToolGlobal::ToolFilterDlg)
	{
		SSDToolGlobal::ToolFilterDlg->DestroyWindow();
		delete SSDToolGlobal::ToolFilterDlg;
	}
}

void EditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_POSX, m_Posx);
	/*DDX_Control(pDX, IDC_PLAYBUTTON, m_PlayButton);
	DDX_Control(pDX, IDC_PAUSEBUTTON, m_PauseButton);
	DDX_Control(pDX, IDC_STOPBUTTON, m_StopButton);*/
}


BEGIN_MESSAGE_MAP(EditDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &EditDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &EditDlg::OnBnClickedCancel)
	ON_EN_CHANGE(IDC_POSX, &EditDlg::OnEnChangePosx)
	ON_EN_CHANGE(IDC_POSY, &EditDlg::OnEnChangePosy)
	ON_EN_CHANGE(IDC_POSZ, &EditDlg::OnEnChangePosz)
	ON_EN_CHANGE(IDC_SCALEX, &EditDlg::OnEnChangeScalex)
	ON_EN_CHANGE(IDC_SCALEY, &EditDlg::OnEnChangeScaley)
	ON_EN_CHANGE(IDC_SCALEZ, &EditDlg::OnEnChangeScalez)
	ON_EN_CHANGE(IDC_ROTX, &EditDlg::OnEnChangeRotx)
	ON_EN_CHANGE(IDC_ROTY, &EditDlg::OnEnChangeRoty)
	ON_EN_CHANGE(IDC_ROTZ, &EditDlg::OnEnChangeRotz)
	ON_EN_CHANGE(IDC_POSX2, &EditDlg::OnEnChangePosx2)
	ON_EN_CHANGE(IDC_POSY2, &EditDlg::OnEnChangePosy2)
	ON_EN_CHANGE(IDC_POSZ2, &EditDlg::OnEnChangePosz2)
	ON_EN_CHANGE(IDC_SCALEX2, &EditDlg::OnEnChangeScalex2)
	ON_EN_CHANGE(IDC_SCALEY2, &EditDlg::OnEnChangeScaley2)
	ON_EN_CHANGE(IDC_SCALEZ2, &EditDlg::OnEnChangeScalez2)
	ON_EN_CHANGE(IDC_POSX3, &EditDlg::OnEnChangePosx3)
	ON_EN_CHANGE(IDC_POSY3, &EditDlg::OnEnChangePosy3)
	ON_EN_CHANGE(IDC_POSZ3, &EditDlg::OnEnChangePosz3)
	ON_EN_CHANGE(IDC_SCALEX3, &EditDlg::OnEnChangeScalex3)
	ON_EN_CHANGE(IDC_SCALEY3, &EditDlg::OnEnChangeScaley3)
	ON_EN_CHANGE(IDC_SCALEZ3, &EditDlg::OnEnChangeScalez3)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// EditDlg 메시지 처리기


void EditDlg::CreateLightDlg()
{
	SSDToolGlobal::ResetDynamicDlg();
	if (nullptr == SSDToolGlobal::ToolLightDlg)
	{
		SSDToolGlobal::ToolLightDlg = new LightDlg;
		SSDToolGlobal::ToolLightDlg->Create(IDD_LIGHT, this);
		SSDToolGlobal::ToolLightDlg->ShowWindow(SW_SHOW);
	}
}

void EditDlg::DeleteLightDlg()
{
	if (nullptr != SSDToolGlobal::ToolLightDlg)
	{
		SSDToolGlobal::ToolLightDlg->DestroyWindow();
		delete SSDToolGlobal::ToolLightDlg;
		SSDToolGlobal::ToolLightDlg = nullptr;
	}
}


void EditDlg::CreateMatDlg()
{
	SSDToolGlobal::ResetDynamicDlg();
	if (nullptr == SSDToolGlobal::ToolMatDlg)
	{
		SSDToolGlobal::ToolMatDlg = new MaterialDlg;
		SSDToolGlobal::ToolMatDlg->Create(IDD_MATDLG, this);
		SSDToolGlobal::ToolMatDlg->ShowWindow(SW_SHOW);
	}
}

void EditDlg::DeleteMatDlg()
{
	if (nullptr != SSDToolGlobal::ToolMatDlg)
	{
		SSDToolGlobal::ToolMatDlg->DestroyWindow();
		delete SSDToolGlobal::ToolMatDlg;
		SSDToolGlobal::ToolMatDlg = nullptr;
	}
}

void EditDlg::CreateFilterDlg()
{
	SSDToolGlobal::ResetDynamicDlg();
	if (nullptr == SSDToolGlobal::ToolFilterDlg)
	{
		SSDToolGlobal::ToolFilterDlg = new FilterDlg;
		SSDToolGlobal::ToolFilterDlg->Create(IDD_FILTER, this);
		SSDToolGlobal::ToolFilterDlg->ShowWindow(SW_SHOW);
	}
}

void EditDlg::DeleteFilterDlg()
{
	if (nullptr != SSDToolGlobal::ToolFilterDlg)
	{
		SSDToolGlobal::ToolFilterDlg->DestroyWindow();
		delete SSDToolGlobal::ToolFilterDlg;
		SSDToolGlobal::ToolFilterDlg = nullptr;
	}
}


void EditDlg::CreateNaviDlg()
{
	SSDToolGlobal::ResetDynamicDlg();
	if (nullptr == SSDToolGlobal::ToolNaviDlg)
	{
		SSDToolGlobal::ToolNaviDlg = new NaviDlg;
		SSDToolGlobal::ToolNaviDlg->Create(IDD_NAVIDLG, this);
		SSDToolGlobal::ToolNaviDlg->ShowWindow(SW_SHOW);
	}
}

void EditDlg::DeleteNaviDlg()
{
	if (nullptr != SSDToolGlobal::ToolNaviDlg)
	{
		SSDToolGlobal::ToolNaviDlg->DestroyWindow();
		delete SSDToolGlobal::ToolNaviDlg;
		SSDToolGlobal::ToolNaviDlg = nullptr;
	}
}

void EditDlg::CreateFogDlg()
{
	SSDToolGlobal::ResetDynamicDlg();
	if (nullptr == SSDToolGlobal::ToolFogDlg)
	{
		SSDToolGlobal::ToolFogDlg = new FogDlg;
		SSDToolGlobal::ToolFogDlg->Create(IDD_FOGDLG, this);
		SSDToolGlobal::ToolFogDlg->ShowWindow(SW_SHOW);
	}
}

void EditDlg::DeleteFogDlg()
{
	if (nullptr != SSDToolGlobal::ToolFogDlg)
	{
		SSDToolGlobal::ToolFogDlg->DestroyWindow();
		delete SSDToolGlobal::ToolFogDlg;
		SSDToolGlobal::ToolFogDlg = nullptr;
	}
}


void EditDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//CDialogEx::OnOK();
}


void EditDlg::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//CDialogEx::OnCancel();
}


BOOL EditDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	SSDToolGlobal::ToolEditDlg = this;

	GetDlgItem(IDC_POSX)->SetWindowTextW(L"0.f");
	GetDlgItem(IDC_POSY)->SetWindowTextW(L"0.f");
	GetDlgItem(IDC_POSZ)->SetWindowTextW(L"0.f");
	GetDlgItem(IDC_SCALEX)->SetWindowTextW(L"0.f");
	GetDlgItem(IDC_SCALEY)->SetWindowTextW(L"0.f");
	GetDlgItem(IDC_SCALEZ)->SetWindowTextW(L"0.f");
	GetDlgItem(IDC_ROTX)->SetWindowTextW(L"0.f");
	GetDlgItem(IDC_ROTY)->SetWindowTextW(L"0.f");
	GetDlgItem(IDC_ROTZ)->SetWindowTextW(L"0.f");

	GetDlgItem(IDC_POSX2)->SetWindowTextW(L"0.f");
	GetDlgItem(IDC_POSY2)->SetWindowTextW(L"0.f");
	GetDlgItem(IDC_POSZ2)->SetWindowTextW(L"0.f");
	GetDlgItem(IDC_SCALEX2)->SetWindowTextW(L"0.f");
	GetDlgItem(IDC_SCALEY2)->SetWindowTextW(L"0.f");
	GetDlgItem(IDC_SCALEZ2)->SetWindowTextW(L"0.f");

	GetDlgItem(IDC_POSX3)->SetWindowTextW(L"0.f");
	GetDlgItem(IDC_POSY3)->SetWindowTextW(L"0.f");
	GetDlgItem(IDC_POSZ3)->SetWindowTextW(L"0.f");
	GetDlgItem(IDC_SCALEX3)->SetWindowTextW(L"0.f");
	GetDlgItem(IDC_SCALEY3)->SetWindowTextW(L"0.f");
	GetDlgItem(IDC_SCALEZ3)->SetWindowTextW(L"0.f");

	((CEdit*)GetDlgItem(IDC_POSX))->SetLimitText(6);
	((CEdit*)GetDlgItem(IDC_POSY))->SetLimitText(6);
	((CEdit*)GetDlgItem(IDC_POSZ))->SetLimitText(6);
	((CEdit*)GetDlgItem(IDC_SCALEX))->SetLimitText(6);
	((CEdit*)GetDlgItem(IDC_SCALEY))->SetLimitText(6);
	((CEdit*)GetDlgItem(IDC_SCALEZ))->SetLimitText(6);
	((CEdit*)GetDlgItem(IDC_ROTX))->SetLimitText(6);
	((CEdit*)GetDlgItem(IDC_ROTY))->SetLimitText(6);
	((CEdit*)GetDlgItem(IDC_ROTZ))->SetLimitText(6);
	((CEdit*)GetDlgItem(IDC_POSX2))->SetLimitText(6);
	((CEdit*)GetDlgItem(IDC_POSY2))->SetLimitText(6);
	((CEdit*)GetDlgItem(IDC_POSZ2))->SetLimitText(6);
	((CEdit*)GetDlgItem(IDC_SCALEX2))->SetLimitText(6);
	((CEdit*)GetDlgItem(IDC_SCALEY2))->SetLimitText(6);
	((CEdit*)GetDlgItem(IDC_SCALEZ2))->SetLimitText(6);
	((CEdit*)GetDlgItem(IDC_POSX3))->SetLimitText(6);
	((CEdit*)GetDlgItem(IDC_POSY3))->SetLimitText(6);
	((CEdit*)GetDlgItem(IDC_POSZ3))->SetLimitText(6);
	((CEdit*)GetDlgItem(IDC_SCALEX3))->SetLimitText(6);
	((CEdit*)GetDlgItem(IDC_SCALEY3))->SetLimitText(6);
	((CEdit*)GetDlgItem(IDC_SCALEZ3))->SetLimitText(6);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void EditDlg::SetRenderTransValue()
{
	if (nullptr != SSDToolGlobal::ToolSelectRenderer)
	{
		Vec3 Pos = SSDToolGlobal::ToolSelectRenderer->GetRenPivot();
		Vec3 Scale = SSDToolGlobal::ToolSelectRenderer->GetRenSize();



		char Temp[20];
		sprintf_s(Temp, "%f", Pos.x);
		std::string TempKey = std::string(Temp);
		std::wstring KeyName;
		KeyName.assign(TempKey.begin(), TempKey.end());
		LPCTSTR Key = KeyName.c_str();
		GetDlgItem(IDC_POSX2)->SetWindowTextW(Key);
		Temp[0] = '\n';
		sprintf_s(Temp, "%f", Pos.y);
		TempKey = std::string(Temp);
		KeyName = std::wstring();
		KeyName.assign(TempKey.begin(), TempKey.end());
		Key = KeyName.c_str();
		GetDlgItem(IDC_POSY2)->SetWindowTextW(Key);
		Temp[0] = '\n';
		sprintf_s(Temp, "%f", Pos.z);
		TempKey = std::string(Temp);
		KeyName = std::wstring();
		KeyName.assign(TempKey.begin(), TempKey.end());
		Key = KeyName.c_str();
		GetDlgItem(IDC_POSZ2)->SetWindowTextW(Key);

		Temp[0] = '\n';
		sprintf_s(Temp, "%f", Scale.x);
		TempKey = std::string(Temp);
		KeyName = std::wstring();
		KeyName.assign(TempKey.begin(), TempKey.end());
		Key = KeyName.c_str();
		GetDlgItem(IDC_SCALEX2)->SetWindowTextW(Key);
		Temp[0] = '\n';
		sprintf_s(Temp, "%f", Scale.y);
		TempKey = std::string(Temp);
		KeyName = std::wstring();
		KeyName.assign(TempKey.begin(), TempKey.end());
		Key = KeyName.c_str();
		GetDlgItem(IDC_SCALEY2)->SetWindowTextW(Key);
		Temp[0] = '\n';
		sprintf_s(Temp, "%f", Scale.z);
		TempKey = std::string(Temp);
		KeyName = std::wstring();
		KeyName.assign(TempKey.begin(), TempKey.end());
		Key = KeyName.c_str();
		GetDlgItem(IDC_SCALEZ2)->SetWindowTextW(Key);
	}
}

void EditDlg::SetCollTransValue()
{
	if (nullptr != SSDToolGlobal::ToolSelectCollider)
	{
		Vec3 Pos = SSDToolGlobal::ToolSelectCollider->GetColliderPivot();
		Vec3 Scale = SSDToolGlobal::ToolSelectCollider->GetColliderSize();

		char Temp[20];
		sprintf_s(Temp, "%f", Pos.x);
		std::string TempKey = std::string(Temp);
		std::wstring KeyName;
		KeyName.assign(TempKey.begin(), TempKey.end());
		LPCTSTR Key = KeyName.c_str();
		GetDlgItem(IDC_POSX3)->SetWindowTextW(Key);
		Temp[0] = '\n';
		sprintf_s(Temp, "%f", Pos.y);
		TempKey = std::string(Temp);
		KeyName = std::wstring();
		KeyName.assign(TempKey.begin(), TempKey.end());
		Key = KeyName.c_str();
		GetDlgItem(IDC_POSY3)->SetWindowTextW(Key);
		Temp[0] = '\n';
		sprintf_s(Temp, "%f", Pos.z);
		TempKey = std::string(Temp);
		KeyName = std::wstring();
		KeyName.assign(TempKey.begin(), TempKey.end());
		Key = KeyName.c_str();
		GetDlgItem(IDC_POSZ3)->SetWindowTextW(Key);

		Temp[0] = '\n';
		sprintf_s(Temp, "%f", Scale.x);
		TempKey = std::string(Temp);
		KeyName = std::wstring();
		KeyName.assign(TempKey.begin(), TempKey.end());
		Key = KeyName.c_str();
		GetDlgItem(IDC_SCALEX3)->SetWindowTextW(Key);
		Temp[0] = '\n';
		sprintf_s(Temp, "%f", Scale.y);
		TempKey = std::string(Temp);
		KeyName = std::wstring();
		KeyName.assign(TempKey.begin(), TempKey.end());
		Key = KeyName.c_str();
		GetDlgItem(IDC_SCALEY3)->SetWindowTextW(Key);
		Temp[0] = '\n';
		sprintf_s(Temp, "%f", Scale.z);
		TempKey = std::string(Temp);
		KeyName = std::wstring();
		KeyName.assign(TempKey.begin(), TempKey.end());
		Key = KeyName.c_str();
		GetDlgItem(IDC_SCALEZ3)->SetWindowTextW(Key);
	}
}

void EditDlg::SetActorTransValue()
{
	if (nullptr != SSDToolGlobal::ToolSelectActor)
	{
		Vec3 Pos = SSDToolGlobal::ToolSelectActor->TRANS->GetWorldPos();
		Vec3 Scale = SSDToolGlobal::ToolSelectActor->TRANS->GetScale();
		Vec3 Rot = SSDToolGlobal::ToolSelectActor->TRANS->GetLocalRotate();

		char Temp[20];
		sprintf_s(Temp, "%f", Pos.x);
		std::string TempKey = std::string(Temp);
		std::wstring KeyName;
		KeyName.assign(TempKey.begin(), TempKey.end());
		LPCTSTR Key = KeyName.c_str();
		GetDlgItem(IDC_POSX)->SetWindowTextW(Key);
		Temp[0] = '\n';
		sprintf_s(Temp, "%f", Pos.y);
		TempKey = std::string(Temp);
		KeyName = std::wstring();
		KeyName.assign(TempKey.begin(), TempKey.end());
		Key = KeyName.c_str();
		GetDlgItem(IDC_POSY)->SetWindowTextW(Key);
		Temp[0] = '\n';
		sprintf_s(Temp, "%f", Pos.z);
		TempKey = std::string(Temp);
		KeyName = std::wstring();
		KeyName.assign(TempKey.begin(), TempKey.end());
		Key = KeyName.c_str();
		GetDlgItem(IDC_POSZ)->SetWindowTextW(Key);

		Temp[0] = '\n';
		sprintf_s(Temp, "%f", Scale.x);
		TempKey = std::string(Temp);
		KeyName = std::wstring();
		KeyName.assign(TempKey.begin(), TempKey.end());
		Key = KeyName.c_str();
		GetDlgItem(IDC_SCALEX)->SetWindowTextW(Key);
		Temp[0] = '\n';
		sprintf_s(Temp, "%f", Scale.y);
		TempKey = std::string(Temp);
		KeyName = std::wstring();
		KeyName.assign(TempKey.begin(), TempKey.end());
		Key = KeyName.c_str();
		GetDlgItem(IDC_SCALEY)->SetWindowTextW(Key);
		Temp[0] = '\n';
		sprintf_s(Temp, "%f", Scale.z);
		TempKey = std::string(Temp);
		KeyName = std::wstring();
		KeyName.assign(TempKey.begin(), TempKey.end());
		Key = KeyName.c_str();
		GetDlgItem(IDC_SCALEZ)->SetWindowTextW(Key);

		Temp[0] = '\n';
		sprintf_s(Temp, "%f", Rot.x);
		TempKey = std::string(Temp);
		KeyName = std::wstring();
		KeyName.assign(TempKey.begin(), TempKey.end());
		Key = KeyName.c_str();
		GetDlgItem(IDC_ROTX)->SetWindowTextW(Key);
		Temp[0] = '\n';
		sprintf_s(Temp, "%f", Rot.y);
		TempKey = std::string(Temp);
		KeyName = std::wstring();
		KeyName.assign(TempKey.begin(), TempKey.end());
		Key = KeyName.c_str();
		GetDlgItem(IDC_ROTY)->SetWindowTextW(Key);
		Temp[0] = '\n';
		sprintf_s(Temp, "%f", Rot.z);
		TempKey = std::string(Temp);
		KeyName = std::wstring();
		KeyName.assign(TempKey.begin(), TempKey.end());
		Key = KeyName.c_str();
		GetDlgItem(IDC_ROTZ)->SetWindowTextW(Key);
	}
}


void EditDlg::OnEnChangePosx()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.
	UpdateData(TRUE);
	CString TempValue;
	GetDlgItemText(IDC_POSX, TempValue);

	if (nullptr != SSDToolGlobal::ToolSelectActor)
	{
		SSDToolGlobal::ToolSelectActor->TRANS->SetPos(Vec3((float)_ttof(TempValue)
			, SSDToolGlobal::ToolSelectActor->TRANS->GetWorldPos().y
			, SSDToolGlobal::ToolSelectActor->TRANS->GetWorldPos().z));
	}
	UpdateData(FALSE);
	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void EditDlg::OnEnChangePosy()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.
	UpdateData(TRUE);
	CString TempValue;
	GetDlgItemText(IDC_POSY, TempValue);

	if (nullptr != SSDToolGlobal::ToolSelectActor)
	{
		SSDToolGlobal::ToolSelectActor->TRANS->SetPos(Vec3(SSDToolGlobal::ToolSelectActor->TRANS->GetWorldPos().x
			, (float)_ttof(TempValue)
			, SSDToolGlobal::ToolSelectActor->TRANS->GetWorldPos().z));
	}
	UpdateData(FALSE);
	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void EditDlg::OnEnChangePosz()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.
	UpdateData(TRUE);
	CString TempValue;
	GetDlgItemText(IDC_POSZ, TempValue);

	if (nullptr != SSDToolGlobal::ToolSelectActor)
	{
		SSDToolGlobal::ToolSelectActor->TRANS->SetPos(Vec3(SSDToolGlobal::ToolSelectActor->TRANS->GetWorldPos().x
			, SSDToolGlobal::ToolSelectActor->TRANS->GetWorldPos().y
			, (float)_ttof(TempValue)));
	}
	UpdateData(FALSE);
	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void EditDlg::OnEnChangeScalex()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.
	UpdateData(TRUE);
	CString TempValue;
	GetDlgItemText(IDC_SCALEX, TempValue);

	if (nullptr != SSDToolGlobal::ToolSelectActor)
	{
		SSDToolGlobal::ToolSelectActor->TRANS->SetScale(Vec3((float)_ttof(TempValue)
			, SSDToolGlobal::ToolSelectActor->TRANS->GetScale().y
			, SSDToolGlobal::ToolSelectActor->TRANS->GetScale().z));
	}
	UpdateData(FALSE);
	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void EditDlg::OnEnChangeScaley()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.
	UpdateData(TRUE);
	CString TempValue;
	GetDlgItemText(IDC_SCALEY, TempValue);

	if (nullptr != SSDToolGlobal::ToolSelectActor)
	{
		SSDToolGlobal::ToolSelectActor->TRANS->SetScale(Vec3(SSDToolGlobal::ToolSelectActor->TRANS->GetScale().x
			, (float)_ttof(TempValue)
			, SSDToolGlobal::ToolSelectActor->TRANS->GetScale().z));
	}
	UpdateData(FALSE);
	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void EditDlg::OnEnChangeScalez()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.
	UpdateData(TRUE);
	CString TempValue;
	GetDlgItemText(IDC_SCALEZ, TempValue);

	if (nullptr != SSDToolGlobal::ToolSelectActor)
	{
		SSDToolGlobal::ToolSelectActor->TRANS->SetScale(Vec3(SSDToolGlobal::ToolSelectActor->TRANS->GetScale().x
			, SSDToolGlobal::ToolSelectActor->TRANS->GetScale().y
			, (float)_ttof(TempValue)));
	}
	UpdateData(FALSE);
	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void EditDlg::OnEnChangeRotx()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.
	UpdateData(TRUE);
	CString TempValue;
	GetDlgItemText(IDC_ROTX, TempValue);

	if (nullptr != SSDToolGlobal::ToolSelectActor)
	{
		SSDToolGlobal::ToolSelectActor->TRANS->SetRotate(Vec3((float)_ttof(TempValue)
			, SSDToolGlobal::ToolSelectActor->TRANS->GetLocalRotate().y
			, SSDToolGlobal::ToolSelectActor->TRANS->GetLocalRotate().z));
	}
	UpdateData(FALSE);
	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void EditDlg::OnEnChangeRoty()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.
	UpdateData(TRUE);
	CString TempValue;
	GetDlgItemText(IDC_ROTY, TempValue);

	if (nullptr != SSDToolGlobal::ToolSelectActor)
	{
		SSDToolGlobal::ToolSelectActor->TRANS->SetRotate(Vec3(SSDToolGlobal::ToolSelectActor->TRANS->GetLocalRotate().x
			, (float)_ttof(TempValue)
			, SSDToolGlobal::ToolSelectActor->TRANS->GetLocalRotate().z));
	}
	UpdateData(FALSE);
	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void EditDlg::OnEnChangeRotz()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.
	UpdateData(TRUE);
	CString TempValue;
	GetDlgItemText(IDC_ROTZ, TempValue);

	if (nullptr != SSDToolGlobal::ToolSelectActor)
	{
		SSDToolGlobal::ToolSelectActor->TRANS->SetRotate(Vec3(SSDToolGlobal::ToolSelectActor->TRANS->GetLocalRotate().x
			, SSDToolGlobal::ToolSelectActor->TRANS->GetLocalRotate().y
			, (float)_ttof(TempValue)));
	}
	UpdateData(FALSE);
	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

////////////////////////////////////////////// Renderer

void EditDlg::OnEnChangePosx2()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.
	UpdateData(TRUE);
	CString TempValue;
	GetDlgItemText(IDC_POSX2, TempValue);

	if (nullptr != SSDToolGlobal::ToolSelectRenderer)
	{
		SSDToolGlobal::ToolSelectRenderer->SetRenPivot(Vec3((float)_ttof(TempValue)
			, SSDToolGlobal::ToolSelectRenderer->GetRenPivot().y
			, SSDToolGlobal::ToolSelectRenderer->GetRenPivot().z));
	}
	UpdateData(FALSE);
	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void EditDlg::OnEnChangePosy2()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.
	UpdateData(TRUE);
	CString TempValue;
	GetDlgItemText(IDC_POSY2, TempValue);

	if (nullptr != SSDToolGlobal::ToolSelectRenderer)
	{
		SSDToolGlobal::ToolSelectRenderer->SetRenPivot(Vec3(SSDToolGlobal::ToolSelectRenderer->GetRenPivot().x
			, (float)_ttof(TempValue)
			, SSDToolGlobal::ToolSelectRenderer->GetRenPivot().z));
	}
	UpdateData(FALSE);
	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void EditDlg::OnEnChangePosz2()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.
	UpdateData(TRUE);
	CString TempValue;
	GetDlgItemText(IDC_POSZ2, TempValue);

	if (nullptr != SSDToolGlobal::ToolSelectRenderer)
	{
		SSDToolGlobal::ToolSelectRenderer->SetRenPivot(Vec3(SSDToolGlobal::ToolSelectRenderer->GetRenPivot().x
			, SSDToolGlobal::ToolSelectRenderer->GetRenPivot().y
			, (float)_ttof(TempValue)));
	}
	UpdateData(FALSE);
	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void EditDlg::OnEnChangeScalex2()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.
	UpdateData(TRUE);
	CString TempValue;
	GetDlgItemText(IDC_SCALEX2, TempValue);

	if (nullptr != SSDToolGlobal::ToolSelectRenderer)
	{
		SSDToolGlobal::ToolSelectRenderer->SetRenSize(Vec3((float)_ttof(TempValue)
			, SSDToolGlobal::ToolSelectRenderer->GetRenSize().y
			, SSDToolGlobal::ToolSelectRenderer->GetRenSize().z));
	}
	UpdateData(FALSE);
	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void EditDlg::OnEnChangeScaley2()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.
	UpdateData(TRUE);
	CString TempValue;
	GetDlgItemText(IDC_SCALEY2, TempValue);

	if (nullptr != SSDToolGlobal::ToolSelectRenderer)
	{
		SSDToolGlobal::ToolSelectRenderer->SetRenSize(Vec3(SSDToolGlobal::ToolSelectRenderer->GetRenSize().x
			, (float)_ttof(TempValue)
			, SSDToolGlobal::ToolSelectRenderer->GetRenSize().z));
	}
	UpdateData(FALSE);
	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void EditDlg::OnEnChangeScalez2()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.
	UpdateData(TRUE);
	CString TempValue;
	GetDlgItemText(IDC_SCALEZ2, TempValue);

	if (nullptr != SSDToolGlobal::ToolSelectRenderer)
	{
		SSDToolGlobal::ToolSelectRenderer->SetRenSize(Vec3(SSDToolGlobal::ToolSelectRenderer->GetRenSize().x
			, SSDToolGlobal::ToolSelectRenderer->GetRenSize().y
			, (float)_ttof(TempValue)));
	}
	UpdateData(FALSE);
	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

////////////////////////////////////////////// Collider


void EditDlg::OnEnChangePosx3()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.
	UpdateData(TRUE);
	CString TempValue;
	GetDlgItemText(IDC_POSX3, TempValue);

	if (nullptr != SSDToolGlobal::ToolSelectCollider)
	{
		SSDToolGlobal::ToolSelectCollider->SetColPivot(Vec3((float)_ttof(TempValue)
			, SSDToolGlobal::ToolSelectCollider->GetColliderPivot().y
			, SSDToolGlobal::ToolSelectCollider->GetColliderPivot().z));
	}
	UpdateData(FALSE);
	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void EditDlg::OnEnChangePosy3()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.
	UpdateData(TRUE);
	CString TempValue;
	GetDlgItemText(IDC_POSY3, TempValue);

	if (nullptr != SSDToolGlobal::ToolSelectCollider)
	{
		SSDToolGlobal::ToolSelectCollider->SetColPivot(Vec3(SSDToolGlobal::ToolSelectCollider->GetColliderPivot().x
			, (float)_ttof(TempValue)
			, SSDToolGlobal::ToolSelectCollider->GetColliderPivot().z));
	}
	UpdateData(FALSE);
	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void EditDlg::OnEnChangePosz3()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.
	UpdateData(TRUE);
	CString TempValue;
	GetDlgItemText(IDC_POSZ3, TempValue);

	if (nullptr != SSDToolGlobal::ToolSelectCollider)
	{
		SSDToolGlobal::ToolSelectCollider->SetColPivot(Vec3(SSDToolGlobal::ToolSelectCollider->GetColliderPivot().x
			, SSDToolGlobal::ToolSelectCollider->GetColliderPivot().y
			, (float)_ttof(TempValue)));
	}
	UpdateData(FALSE);
	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void EditDlg::OnEnChangeScalex3()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.
	UpdateData(TRUE);
	CString TempValue;
	GetDlgItemText(IDC_SCALEX3, TempValue);

	if (nullptr != SSDToolGlobal::ToolSelectCollider)
	{
		SSDToolGlobal::ToolSelectCollider->SetColSize(Vec3((float)_ttof(TempValue)
			, SSDToolGlobal::ToolSelectCollider->GetColliderSize().y
			, SSDToolGlobal::ToolSelectCollider->GetColliderSize().z));
	}
	UpdateData(FALSE);
	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void EditDlg::OnEnChangeScaley3()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.
	UpdateData(TRUE);
	CString TempValue;
	GetDlgItemText(IDC_SCALEY3, TempValue);

	if (nullptr != SSDToolGlobal::ToolSelectCollider)
	{
		SSDToolGlobal::ToolSelectCollider->SetColSize(Vec3(SSDToolGlobal::ToolSelectCollider->GetColliderSize().x
			, (float)_ttof(TempValue)
			, SSDToolGlobal::ToolSelectCollider->GetColliderSize().z));
	}
	UpdateData(FALSE);
	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void EditDlg::OnEnChangeScalez3()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.
	UpdateData(TRUE);
	CString TempValue;
	GetDlgItemText(IDC_SCALEZ3, TempValue);

	if (nullptr != SSDToolGlobal::ToolSelectCollider)
	{
		SSDToolGlobal::ToolSelectCollider->SetColSize(Vec3(SSDToolGlobal::ToolSelectCollider->GetColliderSize().x
			, SSDToolGlobal::ToolSelectCollider->GetColliderSize().y
			, (float)_ttof(TempValue)));
	}
	UpdateData(FALSE);
	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


BOOL EditDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	CEdit *webmacrotext = (CEdit *)GetDlgItem(IDC_POSX);
	if (pMsg->message == WM_LBUTTONDOWN && pMsg->hwnd == webmacrotext->m_hWnd &&
		GetFocus()->m_hWnd != webmacrotext->m_hWnd)
	{
		webmacrotext->SetFocus();
		webmacrotext->SetSel(0, -1, true);
		return true;
	}

	webmacrotext = (CEdit *)GetDlgItem(IDC_POSY);
	if (pMsg->message == WM_LBUTTONDOWN && pMsg->hwnd == webmacrotext->m_hWnd &&
		GetFocus()->m_hWnd != webmacrotext->m_hWnd)
	{
		webmacrotext->SetFocus();
		webmacrotext->SetSel(0, -1, true);
		return true;
	}

	webmacrotext = (CEdit *)GetDlgItem(IDC_POSZ);
	if (pMsg->message == WM_LBUTTONDOWN && pMsg->hwnd == webmacrotext->m_hWnd &&
		GetFocus()->m_hWnd != webmacrotext->m_hWnd)
	{
		webmacrotext->SetFocus();
		webmacrotext->SetSel(0, -1, true);
		return true;
	}

	webmacrotext = (CEdit *)GetDlgItem(IDC_SCALEX);
	if (pMsg->message == WM_LBUTTONDOWN && pMsg->hwnd == webmacrotext->m_hWnd &&
		GetFocus()->m_hWnd != webmacrotext->m_hWnd)
	{
		webmacrotext->SetFocus();
		webmacrotext->SetSel(0, -1, true);
		return true;
	}

	webmacrotext = (CEdit *)GetDlgItem(IDC_SCALEY);
	if (pMsg->message == WM_LBUTTONDOWN && pMsg->hwnd == webmacrotext->m_hWnd &&
		GetFocus()->m_hWnd != webmacrotext->m_hWnd)
	{
		webmacrotext->SetFocus();
		webmacrotext->SetSel(0, -1, true);
		return true;
	}

	webmacrotext = (CEdit *)GetDlgItem(IDC_SCALEZ);
	if (pMsg->message == WM_LBUTTONDOWN && pMsg->hwnd == webmacrotext->m_hWnd &&
		GetFocus()->m_hWnd != webmacrotext->m_hWnd)
	{
		webmacrotext->SetFocus();
		webmacrotext->SetSel(0, -1, true);
		return true;
	}

	webmacrotext = (CEdit *)GetDlgItem(IDC_ROTX);
	if (pMsg->message == WM_LBUTTONDOWN && pMsg->hwnd == webmacrotext->m_hWnd &&
		GetFocus()->m_hWnd != webmacrotext->m_hWnd)
	{
		webmacrotext->SetFocus();
		webmacrotext->SetSel(0, -1, true);
		return true;
	}

	webmacrotext = (CEdit *)GetDlgItem(IDC_ROTY);
	if (pMsg->message == WM_LBUTTONDOWN && pMsg->hwnd == webmacrotext->m_hWnd &&
		GetFocus()->m_hWnd != webmacrotext->m_hWnd)
	{
		webmacrotext->SetFocus();
		webmacrotext->SetSel(0, -1, true);
		return true;
	}

	webmacrotext = (CEdit *)GetDlgItem(IDC_ROTZ);
	if (pMsg->message == WM_LBUTTONDOWN && pMsg->hwnd == webmacrotext->m_hWnd &&
		GetFocus()->m_hWnd != webmacrotext->m_hWnd)
	{
		webmacrotext->SetFocus();
		webmacrotext->SetSel(0, -1, true);
		return true;
	}

	webmacrotext = (CEdit *)GetDlgItem(IDC_POSX2);
	if (pMsg->message == WM_LBUTTONDOWN && pMsg->hwnd == webmacrotext->m_hWnd &&
		GetFocus()->m_hWnd != webmacrotext->m_hWnd)
	{
		webmacrotext->SetFocus();
		webmacrotext->SetSel(0, -1, true);
		return true;
	}

	webmacrotext = (CEdit *)GetDlgItem(IDC_POSY2);
	if (pMsg->message == WM_LBUTTONDOWN && pMsg->hwnd == webmacrotext->m_hWnd &&
		GetFocus()->m_hWnd != webmacrotext->m_hWnd)
	{
		webmacrotext->SetFocus();
		webmacrotext->SetSel(0, -1, true);
		return true;
	}

	webmacrotext = (CEdit *)GetDlgItem(IDC_POSZ2);
	if (pMsg->message == WM_LBUTTONDOWN && pMsg->hwnd == webmacrotext->m_hWnd &&
		GetFocus()->m_hWnd != webmacrotext->m_hWnd)
	{
		webmacrotext->SetFocus();
		webmacrotext->SetSel(0, -1, true);
		return true;
	}

	webmacrotext = (CEdit *)GetDlgItem(IDC_SCALEX2);
	if (pMsg->message == WM_LBUTTONDOWN && pMsg->hwnd == webmacrotext->m_hWnd &&
		GetFocus()->m_hWnd != webmacrotext->m_hWnd)
	{
		webmacrotext->SetFocus();
		webmacrotext->SetSel(0, -1, true);
		return true;
	}

	webmacrotext = (CEdit *)GetDlgItem(IDC_SCALEY2);
	if (pMsg->message == WM_LBUTTONDOWN && pMsg->hwnd == webmacrotext->m_hWnd &&
		GetFocus()->m_hWnd != webmacrotext->m_hWnd)
	{
		webmacrotext->SetFocus();
		webmacrotext->SetSel(0, -1, true);
		return true;
	}

	webmacrotext = (CEdit *)GetDlgItem(IDC_SCALEZ2);
	if (pMsg->message == WM_LBUTTONDOWN && pMsg->hwnd == webmacrotext->m_hWnd &&
		GetFocus()->m_hWnd != webmacrotext->m_hWnd)
	{
		webmacrotext->SetFocus();
		webmacrotext->SetSel(0, -1, true);
		return true;
	}

	webmacrotext = (CEdit *)GetDlgItem(IDC_POSX3);
	if (pMsg->message == WM_LBUTTONDOWN && pMsg->hwnd == webmacrotext->m_hWnd &&
		GetFocus()->m_hWnd != webmacrotext->m_hWnd)
	{
		webmacrotext->SetFocus();
		webmacrotext->SetSel(0, -1, true);
		return true;
	}

	webmacrotext = (CEdit *)GetDlgItem(IDC_POSY3);
	if (pMsg->message == WM_LBUTTONDOWN && pMsg->hwnd == webmacrotext->m_hWnd &&
		GetFocus()->m_hWnd != webmacrotext->m_hWnd)
	{
		webmacrotext->SetFocus();
		webmacrotext->SetSel(0, -1, true);
		return true;
	}

	webmacrotext = (CEdit *)GetDlgItem(IDC_POSZ3);
	if (pMsg->message == WM_LBUTTONDOWN && pMsg->hwnd == webmacrotext->m_hWnd &&
		GetFocus()->m_hWnd != webmacrotext->m_hWnd)
	{
		webmacrotext->SetFocus();
		webmacrotext->SetSel(0, -1, true);
		return true;
	}

	webmacrotext = (CEdit *)GetDlgItem(IDC_SCALEX3);
	if (pMsg->message == WM_LBUTTONDOWN && pMsg->hwnd == webmacrotext->m_hWnd &&
		GetFocus()->m_hWnd != webmacrotext->m_hWnd)
	{
		webmacrotext->SetFocus();
		webmacrotext->SetSel(0, -1, true);
		return true;
	}

	webmacrotext = (CEdit *)GetDlgItem(IDC_SCALEY3);
	if (pMsg->message == WM_LBUTTONDOWN && pMsg->hwnd == webmacrotext->m_hWnd &&
		GetFocus()->m_hWnd != webmacrotext->m_hWnd)
	{
		webmacrotext->SetFocus();
		webmacrotext->SetSel(0, -1, true);
		return true;
	}

	webmacrotext = (CEdit *)GetDlgItem(IDC_SCALEZ3);
	if (pMsg->message == WM_LBUTTONDOWN && pMsg->hwnd == webmacrotext->m_hWnd &&
		GetFocus()->m_hWnd != webmacrotext->m_hWnd)
	{
		webmacrotext->SetFocus();
		webmacrotext->SetSel(0, -1, true);
		return true;
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}

void EditDlg::ChangeCollPivotX(float _x)
{
	char Temp[20];
	sprintf_s(Temp, "%f", _x);
	std::string TempKey = std::string(Temp);
	std::wstring KeyName;
	KeyName.assign(TempKey.begin(), TempKey.end());
	LPCTSTR Key = KeyName.c_str();
	GetDlgItem(IDC_POSX3)->SetWindowTextW(Key);
}

void EditDlg::ChangeCollPivotY(float _y)
{
	char Temp[20];
	sprintf_s(Temp, "%f", _y);
	std::string TempKey = std::string(Temp);
	std::wstring KeyName;
	KeyName.assign(TempKey.begin(), TempKey.end());
	LPCTSTR Key = KeyName.c_str();
	GetDlgItem(IDC_POSY3)->SetWindowTextW(Key);
}

void EditDlg::ChangeCollPivotZ(float _z)
{
	char Temp[20];
	sprintf_s(Temp, "%f", _z);
	std::string TempKey = std::string(Temp);
	std::wstring KeyName;
	KeyName.assign(TempKey.begin(), TempKey.end());
	LPCTSTR Key = KeyName.c_str();
	GetDlgItem(IDC_POSZ3)->SetWindowTextW(Key);
}

void EditDlg::ChangeCollSizeX(float _x)
{
	char Temp[20];
	sprintf_s(Temp, "%f", _x);
	std::string TempKey = std::string(Temp);
	std::wstring KeyName;
	KeyName.assign(TempKey.begin(), TempKey.end());
	LPCTSTR Key = KeyName.c_str();
	GetDlgItem(IDC_SCALEX3)->SetWindowTextW(Key);
}

void EditDlg::ChangeCollSizeY(float _y)
{
	char Temp[20];
	sprintf_s(Temp, "%f", _y);
	std::string TempKey = std::string(Temp);
	std::wstring KeyName;
	KeyName.assign(TempKey.begin(), TempKey.end());
	LPCTSTR Key = KeyName.c_str();
	GetDlgItem(IDC_SCALEY3)->SetWindowTextW(Key);
}

void EditDlg::ChangeCollSizeZ(float _z)
{
	char Temp[20];
	sprintf_s(Temp, "%f", _z);
	std::string TempKey = std::string(Temp);
	std::wstring KeyName;
	KeyName.assign(TempKey.begin(), TempKey.end());
	LPCTSTR Key = KeyName.c_str();
	GetDlgItem(IDC_SCALEZ3)->SetWindowTextW(Key);
}

void EditDlg::ChangePosX()
{
	if (nullptr == SSDToolGlobal::ToolSelectActor)
	{
		return;
	}
	char Temp[20];
	sprintf_s(Temp, "%f", SSDToolGlobal::ToolSelectActor->GetWorldPos().x);
	std::string TempKey = std::string(Temp);
	std::wstring KeyName;
	KeyName.assign(TempKey.begin(), TempKey.end());
	LPCTSTR Key = KeyName.c_str();
	GetDlgItem(IDC_POSX)->SetWindowTextW(Key);
}

void EditDlg::ChangePosY()
{
	if (nullptr == SSDToolGlobal::ToolSelectActor)
	{
		return;
	}
	char Temp[20];
	sprintf_s(Temp, "%f", SSDToolGlobal::ToolSelectActor->GetWorldPos().y);
	std::string TempKey = std::string(Temp);
	std::wstring KeyName;
	KeyName.assign(TempKey.begin(), TempKey.end());
	LPCTSTR Key = KeyName.c_str();
	GetDlgItem(IDC_POSY)->SetWindowTextW(Key);
}

void EditDlg::ChangePosZ()
{
	if (nullptr == SSDToolGlobal::ToolSelectActor)
	{
		return;
	}
	char Temp[20];
	sprintf_s(Temp, "%f", SSDToolGlobal::ToolSelectActor->GetWorldPos().z);
	std::string TempKey = std::string(Temp);
	std::wstring KeyName;
	KeyName.assign(TempKey.begin(), TempKey.end());
	LPCTSTR Key = KeyName.c_str();
	GetDlgItem(IDC_POSZ)->SetWindowTextW(Key);
}

void EditDlg::ChangeRotX()
{
	if (nullptr == SSDToolGlobal::ToolSelectActor)
	{
		return;
	}
	char Temp[20];
	sprintf_s(Temp, "%f", SSDToolGlobal::ToolSelectActor->TRANS->GetRotate().x);
	std::string TempKey = std::string(Temp);
	std::wstring KeyName;
	KeyName.assign(TempKey.begin(), TempKey.end());
	LPCTSTR Key = KeyName.c_str();
	GetDlgItem(IDC_ROTX)->SetWindowTextW(Key);
}

void EditDlg::ChangeRotY()
{
	if (nullptr == SSDToolGlobal::ToolSelectActor)
	{
		return;
	}
	char Temp[20];
	sprintf_s(Temp, "%f", SSDToolGlobal::ToolSelectActor->TRANS->GetRotate().y);
	std::string TempKey = std::string(Temp);
	std::wstring KeyName;
	KeyName.assign(TempKey.begin(), TempKey.end());
	LPCTSTR Key = KeyName.c_str();
	GetDlgItem(IDC_ROTY)->SetWindowTextW(Key);
}

void EditDlg::ChangeRotZ()
{
	if (nullptr == SSDToolGlobal::ToolSelectActor)
	{
		return;
	}
	char Temp[20];
	sprintf_s(Temp, "%f", SSDToolGlobal::ToolSelectActor->TRANS->GetRotate().z);
	std::string TempKey = std::string(Temp);
	std::wstring KeyName;
	KeyName.assign(TempKey.begin(), TempKey.end());
	LPCTSTR Key = KeyName.c_str();
	GetDlgItem(IDC_ROTZ)->SetWindowTextW(Key);
}

//void EditDlg::ChangeRenPosX()
//{
//	if (nullptr == SSDToolGlobal::ToolSelectActor)
//	{
//		return;
//	}
//	char Temp[20];
//	sprintf_s(Temp, "%f", SSDToolGlobal::ToolSelectActor->GetWorldPos().x);
//	std::string TempKey = std::string(Temp);
//	std::wstring KeyName;
//	KeyName.assign(TempKey.begin(), TempKey.end());
//	LPCTSTR Key = KeyName.c_str();
//	GetDlgItem(IDC_POSX2)->SetWindowTextW(Key);
//}
//
//void EditDlg::ChangeRenPosY()
//{
//	if (nullptr == SSDToolGlobal::ToolSelectActor)
//	{
//		return;
//	}
//	char Temp[20];
//	sprintf_s(Temp, "%f", SSDToolGlobal::ToolSelectActor->GetWorldPos().x);
//	std::string TempKey = std::string(Temp);
//	std::wstring KeyName;
//	KeyName.assign(TempKey.begin(), TempKey.end());
//	LPCTSTR Key = KeyName.c_str();
//	GetDlgItem(IDC_POSY2)->SetWindowTextW(Key);
//}
//
//void EditDlg::ChangeRenPosZ()
//{
//	if (nullptr == SSDToolGlobal::ToolSelectActor)
//	{
//		return;
//	}
//	char Temp[20];
//	sprintf_s(Temp, "%f", SSDToolGlobal::ToolSelectActor->GetWorldPos().x);
//	std::string TempKey = std::string(Temp);
//	std::wstring KeyName;
//	KeyName.assign(TempKey.begin(), TempKey.end());
//	LPCTSTR Key = KeyName.c_str();
//	GetDlgItem(IDC_POSZ2)->SetWindowTextW(Key);
//}
//
//void EditDlg::ChangeColPosX()
//{
//	if (nullptr == SSDToolGlobal::ToolSelectActor)
//	{
//		return;
//	}
//	char Temp[20];
//	sprintf_s(Temp, "%f", SSDToolGlobal::ToolSelectActor->GetWorldPos().x);
//	std::string TempKey = std::string(Temp);
//	std::wstring KeyName;
//	KeyName.assign(TempKey.begin(), TempKey.end());
//	LPCTSTR Key = KeyName.c_str();
//	GetDlgItem(IDC_POSX)->SetWindowTextW(Key);
//}
//
//void EditDlg::ChangeColPosY()
//{
//	if (nullptr == SSDToolGlobal::ToolSelectActor)
//	{
//		return;
//	}
//	char Temp[20];
//	sprintf_s(Temp, "%f", SSDToolGlobal::ToolSelectActor->GetWorldPos().x);
//	std::string TempKey = std::string(Temp);
//	std::wstring KeyName;
//	KeyName.assign(TempKey.begin(), TempKey.end());
//	LPCTSTR Key = KeyName.c_str();
//	GetDlgItem(IDC_POSX)->SetWindowTextW(Key);
//}
//
//void EditDlg::ChangeColPosZ()
//{
//	if (nullptr == SSDToolGlobal::ToolSelectActor)
//	{
//		return;
//	}
//	char Temp[20];
//	sprintf_s(Temp, "%f", SSDToolGlobal::ToolSelectActor->GetWorldPos().x);
//	std::string TempKey = std::string(Temp);
//	std::wstring KeyName;
//	KeyName.assign(TempKey.begin(), TempKey.end());
//	LPCTSTR Key = KeyName.c_str();
//	GetDlgItem(IDC_POSX)->SetWindowTextW(Key);
//}

void EditDlg::ChangeActorTrans()
{
	ChangePosX();
	ChangePosY();
	ChangePosZ();
	ChangeRotX();
	ChangeRotY();
	ChangeRotZ();
}




void EditDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	//for (size_t i = 0; i < MAX_BUTTON_IMAGE_TYPE; i++)
	//{
	//	m_PlayImage[i].Destroy();
	//	m_PauseImage[i].Destroy();
	//	m_StopImage[i].Destroy();
	//}
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}
