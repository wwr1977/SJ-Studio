// LightDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "SSDTool.h"
#include "LightDlg.h"
#include "afxdialogex.h"
#include "SSDToolGlobal.h"
#include <Light.h>
#include <TransForm.h>

// LightDlg 대화 상자

IMPLEMENT_DYNAMIC(LightDlg, CDialogEx)

LightDlg::LightDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_LIGHT, pParent)
	, m_VolumeMeshCheck(FALSE)
	, m_VolumeMeshType(_T(""))
	, m_LightTypeValue(_T(""))
{

}

LightDlg::~LightDlg()
{
}

void LightDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO1, m_VolumeMeshCheck);
	DDX_Control(pDX, IDC_VOLUMEMESH, m_VolumeList);
	DDX_Control(pDX, IDC_LIGHTTYPE, m_TypeList);
	DDX_CBString(pDX, IDC_VOLUMEMESH, m_VolumeMeshType);
	DDX_CBString(pDX, IDC_LIGHTTYPE, m_LightTypeValue);
	DDX_Control(pDX, IDC_SLIDER1, m_RSlider);
	DDX_Control(pDX, IDC_SLIDER2, m_GSlider);
	DDX_Control(pDX, IDC_SLIDER3, m_BSlider);
	DDX_Control(pDX, IDC_SLIDER4, m_ASlider);
	DDX_Control(pDX, IDC_SLIDER5, m_DistanceSlider);
	DDX_Control(pDX, IDC_SLIDER6, m_ConstantSlider);
	DDX_Control(pDX, IDC_SLIDER7, m_LinearSlider);
	DDX_Control(pDX, IDC_SLIDER8, m_QuadraticSlider);
}


BEGIN_MESSAGE_MAP(LightDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &LightDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &LightDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_RADIO1, &LightDlg::OnBnClickedRadio1)
	ON_EN_CHANGE(IDC_COLORR, &LightDlg::OnEnChangeColorr)
	ON_EN_CHANGE(IDC_COLORG, &LightDlg::OnEnChangeColorg)
	ON_EN_CHANGE(IDC_COLORB, &LightDlg::OnEnChangeColorb)
	ON_EN_CHANGE(IDC_COLORA, &LightDlg::OnEnChangeColora)
	ON_EN_CHANGE(IDC_DIRX, &LightDlg::OnEnChangeDirx)
	ON_EN_CHANGE(IDC_DIRY, &LightDlg::OnEnChangeDiry)
	ON_EN_CHANGE(IDC_DIRZ, &LightDlg::OnEnChangeDirz)
	ON_EN_CHANGE(IDC_ATENW, &LightDlg::OnEnChangeAtenw)
	ON_EN_CHANGE(IDC_ATENZ, &LightDlg::OnEnChangeAtenz)
	ON_EN_CHANGE(IDC_ATENY, &LightDlg::OnEnChangeAteny)
	ON_EN_CHANGE(IDC_ATENIX, &LightDlg::OnEnChangeAtenix)
	ON_EN_CHANGE(IDC_SPECPOWER, &LightDlg::OnEnChangeSpecpower)
	ON_EN_CHANGE(IDC_AMBIPOWER, &LightDlg::OnEnChangeAmbipower)
	ON_CBN_SELCHANGE(IDC_VOLUMEMESH, &LightDlg::OnCbnSelchangeVolumemesh)
	ON_CBN_SELCHANGE(IDC_LIGHTTYPE, &LightDlg::OnCbnSelchangeLighttype)
	ON_EN_CHANGE(IDC_POWER, &LightDlg::OnEnChangePower)
	ON_EN_CHANGE(IDC_SPOTPOWER, &LightDlg::OnEnChangeRange)
	ON_CBN_SETFOCUS(IDC_VOLUMEMESH, &LightDlg::OnCbnSetfocusVolumemesh)
	ON_BN_CLICKED(IDC_BUTTON1, &LightDlg::OnBnClickedButton1)
	ON_EN_CHANGE(IDC_POSX3, &LightDlg::OnEnChangePosx3)
	ON_EN_CHANGE(IDC_POSY3, &LightDlg::OnEnChangePosy3)
	ON_EN_CHANGE(IDC_POSZ3, &LightDlg::OnEnChangePosz3)
	ON_EN_CHANGE(IDC_SCALEX3, &LightDlg::OnEnChangeScalex3)
	ON_EN_CHANGE(IDC_SCALEY3, &LightDlg::OnEnChangeScaley3)
	ON_EN_CHANGE(IDC_SCALEZ3, &LightDlg::OnEnChangeScalez3)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER1, &LightDlg::OnNMCustomdrawSlider1)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER2, &LightDlg::OnNMCustomdrawSlider2)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER3, &LightDlg::OnNMCustomdrawSlider3)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER4, &LightDlg::OnNMCustomdrawSlider4)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER5, &LightDlg::OnNMCustomdrawSlider5)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER6, &LightDlg::OnNMCustomdrawSlider6)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER7, &LightDlg::OnNMCustomdrawSlider7)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER8, &LightDlg::OnNMCustomdrawSlider8)
END_MESSAGE_MAP()


// LightDlg 메시지 처리기



void LightDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//CDialogEx::OnOK();
}


void LightDlg::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//CDialogEx::OnCancel();
}


BOOL LightDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	// 수지 컴퓨터 해상도
	//SetWindowPos(this, 0 - 2, 265, 331 - 4 * 0, 961 + 10, SWP_NOZORDER);

	// 석재 컴퓨터 해상도
	SetWindowPos(this, 0 - 2, 330, 331, 971, SWP_NOZORDER);

	//ResetLightValue();

	m_TypeList.ResetContent();
	m_TypeList.AddString(L"DIRECTION_LIGHT");
	m_TypeList.AddString(L"POINT_LIGHT");
	m_TypeList.AddString(L"SPOT_LIGHT");
	m_TypeList.AddString(L"NONE");
	m_TypeList.UpdateData();

	m_VolumeList.ResetContent();
	m_VolumeList.AddString(L"Cube");
	m_VolumeList.AddString(L"Cone_Lv360");
	m_VolumeList.AddString(L"Sphere_Lv8");
	m_VolumeList.AddString(L"Sphere_Lv16");
	m_VolumeList.AddString(L"Sphere_Lv32");
	m_VolumeList.AddString(L"Sphere_Lv64");
	m_VolumeList.AddString(L"None");
	m_VolumeList.UpdateData();

	((CEdit*)GetDlgItem(IDC_COLORR))->SetLimitText(6);
	((CEdit*)GetDlgItem(IDC_COLORG))->SetLimitText(6);
	((CEdit*)GetDlgItem(IDC_COLORB))->SetLimitText(6);
	((CEdit*)GetDlgItem(IDC_COLORA))->SetLimitText(6);
	((CEdit*)GetDlgItem(IDC_DIRX))->SetLimitText(6);
	((CEdit*)GetDlgItem(IDC_DIRY))->SetLimitText(6);
	((CEdit*)GetDlgItem(IDC_DIRZ))->SetLimitText(6);
	((CEdit*)GetDlgItem(IDC_ATENW))->SetLimitText(6);
	((CEdit*)GetDlgItem(IDC_ATENZ))->SetLimitText(6);
	((CEdit*)GetDlgItem(IDC_ATENY))->SetLimitText(6);
	((CEdit*)GetDlgItem(IDC_ATENIX))->SetLimitText(6);
	((CEdit*)GetDlgItem(IDC_SPECPOWER))->SetLimitText(6);
	((CEdit*)GetDlgItem(IDC_AMBIPOWER))->SetLimitText(6);
	((CEdit*)GetDlgItem(IDC_POWER))->SetLimitText(6);
	((CEdit*)GetDlgItem(IDC_SPOTPOWER))->SetLimitText(6);
	((CEdit*)GetDlgItem(IDC_POSX3))->SetLimitText(6);
	((CEdit*)GetDlgItem(IDC_POSY3))->SetLimitText(6);
	((CEdit*)GetDlgItem(IDC_POSZ3))->SetLimitText(6);
	((CEdit*)GetDlgItem(IDC_SCALEX3))->SetLimitText(6);
	((CEdit*)GetDlgItem(IDC_SCALEY3))->SetLimitText(6);
	((CEdit*)GetDlgItem(IDC_SCALEZ3))->SetLimitText(6);

	m_RSlider.SetRange(0, 255);
	m_RSlider.SetPageSize(1);
	m_RSlider.SetTicFreq(10);

	m_GSlider.SetRange(0, 255);
	m_GSlider.SetPageSize(1);
	m_GSlider.SetTicFreq(10);

	m_BSlider.SetRange(0, 255);
	m_BSlider.SetPageSize(1);
	m_BSlider.SetTicFreq(10);

	m_ASlider.SetRange(0, 255);
	m_ASlider.SetPageSize(1);
	m_ASlider.SetTicFreq(10);

	m_DistanceSlider.SetRange(0, 1000);
	m_DistanceSlider.SetPageSize(10);
	m_DistanceSlider.SetTicFreq(10);

	m_ConstantSlider.SetRange(0, 1000);
	m_ConstantSlider.SetPageSize(10);
	m_ConstantSlider.SetTicFreq(10);

	m_LinearSlider.SetRange(0, 200);
	m_LinearSlider.SetPageSize(10);
	m_LinearSlider.SetTicFreq(10);

	m_QuadraticSlider.SetRange(0, 200);
	m_QuadraticSlider.SetPageSize(10);
	m_QuadraticSlider.SetTicFreq(10);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void LightDlg::OnBnClickedRadio1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void LightDlg::OnEnChangeColorr()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	if (nullptr == SSDToolGlobal::ToolLightDlg) 
	{
		return;
	}

	UpdateData(TRUE);
	CString TempValue;
	GetDlgItemText(IDC_COLORR, TempValue);

	if (nullptr != SSDToolGlobal::ToolSelectLight)
	{
		float Value = (float)_ttof(TempValue);
		CBUFFER::LightBuffer TempBuffer = SSDToolGlobal::ToolSelectLight->GetLightData();
		SSDToolGlobal::ToolSelectLight->SetLightColor(Vec4(Value, TempBuffer.LightColor.g, TempBuffer.LightColor.b, TempBuffer.LightColor.a));
	}

	UpdateData(FALSE);

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void LightDlg::OnEnChangeColorg()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.
	if (nullptr == SSDToolGlobal::ToolLightDlg)
	{
		return;
	}
	UpdateData(TRUE);
	CString TempValue;
	GetDlgItemText(IDC_COLORG, TempValue);

	if (nullptr != SSDToolGlobal::ToolSelectLight)
	{
		CBUFFER::LightBuffer TempBuffer = SSDToolGlobal::ToolSelectLight->GetLightData();
		SSDToolGlobal::ToolSelectLight->SetLightColor(Vec4(TempBuffer.LightColor.r, (float)_ttof(TempValue), TempBuffer.LightColor.b, TempBuffer.LightColor.a));
	}
	UpdateData(FALSE);
	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void LightDlg::OnEnChangeColorb()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.
	if (nullptr == SSDToolGlobal::ToolLightDlg)
	{
		return;
	}
	UpdateData(TRUE);
	CString TempValue;
	GetDlgItemText(IDC_COLORB, TempValue);

	if (nullptr != SSDToolGlobal::ToolSelectLight)
	{
		CBUFFER::LightBuffer TempBuffer = SSDToolGlobal::ToolSelectLight->GetLightData();
		SSDToolGlobal::ToolSelectLight->SetLightColor(Vec4(TempBuffer.LightColor.r, TempBuffer.LightColor.g, (float)_ttof(TempValue), TempBuffer.LightColor.a));
	}
	UpdateData(FALSE);
	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void LightDlg::OnEnChangeColora()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.
	if (nullptr == SSDToolGlobal::ToolLightDlg)
	{
		return;
	}
	UpdateData(TRUE);
	CString TempValue;
	GetDlgItemText(IDC_COLORA, TempValue);

	if (nullptr != SSDToolGlobal::ToolSelectLight)
	{
		CBUFFER::LightBuffer TempBuffer = SSDToolGlobal::ToolSelectLight->GetLightData();
		SSDToolGlobal::ToolSelectLight->SetLightColor(Vec4(TempBuffer.LightColor.r, TempBuffer.LightColor.g, TempBuffer.LightColor.b, (float)_ttof(TempValue)));
	}
	UpdateData(FALSE);
	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void LightDlg::OnEnChangeDirx()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.
	if (nullptr == SSDToolGlobal::ToolLightDlg)
	{
		return;
	}
	UpdateData(TRUE);
	CString TempValue;
	GetDlgItemText(IDC_DIRX, TempValue);

	if (nullptr != SSDToolGlobal::ToolSelectLight)
	{
		CBUFFER::LightBuffer TempBuffer = SSDToolGlobal::ToolSelectLight->GetLightData();
		SSDToolGlobal::ToolSelectLight->SetLightDir(Vec3((float)_ttof(TempValue), TempBuffer.LightDir.y, TempBuffer.LightDir.z));
	}
	UpdateData(FALSE);
	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void LightDlg::OnEnChangeDiry()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.
	if (nullptr == SSDToolGlobal::ToolLightDlg)
	{
		return;
	}
	UpdateData(TRUE);
	CString TempValue;
	GetDlgItemText(IDC_DIRY, TempValue);

	if (nullptr != SSDToolGlobal::ToolSelectLight)
	{
		CBUFFER::LightBuffer TempBuffer = SSDToolGlobal::ToolSelectLight->GetLightData();
		SSDToolGlobal::ToolSelectLight->SetLightDir(Vec3(TempBuffer.LightDir.x, (float)_ttof(TempValue), TempBuffer.LightDir.z));
	}
	UpdateData(FALSE); 
	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void LightDlg::OnEnChangeDirz()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.
	if (nullptr == SSDToolGlobal::ToolLightDlg)
	{
		return;
	}
	UpdateData(TRUE);
	CString TempValue;
	GetDlgItemText(IDC_DIRZ, TempValue);

	if (nullptr != SSDToolGlobal::ToolSelectLight)
	{
		CBUFFER::LightBuffer TempBuffer = SSDToolGlobal::ToolSelectLight->GetLightData();
		SSDToolGlobal::ToolSelectLight->SetLightDir(Vec3(TempBuffer.LightDir.x, TempBuffer.LightDir.y, (float)_ttof(TempValue)));
	}
	UpdateData(FALSE);
	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void LightDlg::OnEnChangeAtenw()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.
	if (nullptr == SSDToolGlobal::ToolLightDlg)
	{
		return;
	}
	UpdateData(TRUE);
	CString TempValue;
	GetDlgItemText(IDC_ATENW, TempValue);

	if (nullptr != SSDToolGlobal::ToolSelectLight)
	{
		CBUFFER::LightBuffer TempBuffer = SSDToolGlobal::ToolSelectLight->GetLightData();
		SSDToolGlobal::ToolSelectLight->SetLightAtenuation(Vec4(TempBuffer.LightAtenuation.x, TempBuffer.LightAtenuation.y, TempBuffer.LightAtenuation.z, (float)_ttof(TempValue)));
	}
	UpdateData(FALSE);
	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void LightDlg::OnEnChangeAtenz()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.
	if (nullptr == SSDToolGlobal::ToolLightDlg)
	{
		return;
	}
	UpdateData(TRUE);
	CString TempValue;
	GetDlgItemText(IDC_ATENZ, TempValue);

	if (nullptr != SSDToolGlobal::ToolSelectLight)
	{
		CBUFFER::LightBuffer TempBuffer = SSDToolGlobal::ToolSelectLight->GetLightData();
		SSDToolGlobal::ToolSelectLight->SetLightAtenuation(Vec4(TempBuffer.LightAtenuation.x, TempBuffer.LightAtenuation.y, (float)_ttof(TempValue), TempBuffer.LightAtenuation.w));
	}
	UpdateData(FALSE);
	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void LightDlg::OnEnChangeAteny()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.
	if (nullptr == SSDToolGlobal::ToolLightDlg)
	{
		return;
	}
	UpdateData(TRUE);
	CString TempValue;
	GetDlgItemText(IDC_ATENY, TempValue);

	if (nullptr != SSDToolGlobal::ToolSelectLight)
	{
		CBUFFER::LightBuffer TempBuffer = SSDToolGlobal::ToolSelectLight->GetLightData();
		SSDToolGlobal::ToolSelectLight->SetLightAtenuation(Vec4(TempBuffer.LightAtenuation.x, (float)_ttof(TempValue), TempBuffer.LightAtenuation.z, TempBuffer.LightAtenuation.w));
	}
	UpdateData(FALSE);
	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void LightDlg::OnEnChangeAtenix()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.
	if (nullptr == SSDToolGlobal::ToolLightDlg)
	{
		return;
	}
	UpdateData(TRUE);
	CString TempValue;
	GetDlgItemText(IDC_ATENIX, TempValue);

	if (nullptr != SSDToolGlobal::ToolSelectLight)
	{
		CBUFFER::LightBuffer TempBuffer = SSDToolGlobal::ToolSelectLight->GetLightData();

		SSDToolGlobal::ToolSelectLight->SetLightAtenuation(Vec4((float)_ttof(TempValue), TempBuffer.LightAtenuation.y, TempBuffer.LightAtenuation.z, TempBuffer.LightAtenuation.w));
	}
	UpdateData(FALSE);
	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void LightDlg::OnEnChangeSpecpower()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.
	if (nullptr == SSDToolGlobal::ToolLightDlg)
	{
		return;
	}
	UpdateData(TRUE);
	CString TempValue;
	GetDlgItemText(IDC_SPECPOWER, TempValue);

	if (nullptr != SSDToolGlobal::ToolSelectLight)
	{
		SSDToolGlobal::ToolSelectLight->SetSpecPower((float)_ttof(TempValue));
	}
	UpdateData(FALSE);
	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void LightDlg::OnEnChangeAmbipower()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.
	if (nullptr == SSDToolGlobal::ToolLightDlg)
	{
		return;
	}
	UpdateData(TRUE);
	CString TempValue;
	GetDlgItemText(IDC_AMBIPOWER, TempValue);

	if (nullptr != SSDToolGlobal::ToolSelectLight)
	{
		SSDToolGlobal::ToolSelectLight->SetAmbiFactor((float)_ttof(TempValue));
	}
	UpdateData(FALSE);
	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void LightDlg::OnCbnSelchangeVolumemesh()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (nullptr == SSDToolGlobal::ToolLightDlg)
	{
		return;
	}
	if (true != SSDToolGlobal::ToolSelectLight->GetIsVolume())
	{
		return;
	}

	int LightType = m_VolumeList.GetCurSel();
	CString TempName = L"";
	m_VolumeList.GetLBText(LightType, TempName);

	if (nullptr != SSDToolGlobal::ToolSelectLight)
	{
		if (false == TempName.IsEmpty())
		{
			SSDToolGlobal::ToolSelectLight->SetVolumeLightMesh(TempName.GetString());
		}
	}
}


void LightDlg::OnCbnSelchangeLighttype()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (nullptr == SSDToolGlobal::ToolLightDlg)
	{
		return;
	}
	int LightType = m_TypeList.GetCurSel();

	if (nullptr != SSDToolGlobal::ToolSelectLight)
	{
		SSDToolGlobal::ToolSelectLight->SetLightType((LIGHT_TYPE)LightType);
	}
	if (2 == LightType)
	{
		Vec3 Dir = -SSDToolGlobal::ToolSelectLight->GetSubTransWorld().Row[AXIS_Y].GetNormalize_Vec3();

		GetDlgItem(IDC_DIRX)->SetWindowTextW(LightDlg::GetWinChar(Dir.x).c_str());
		GetDlgItem(IDC_DIRY)->SetWindowTextW(LightDlg::GetWinChar(Dir.y).c_str());
		GetDlgItem(IDC_DIRZ)->SetWindowTextW(LightDlg::GetWinChar(Dir.z).c_str());

		if (nullptr == SSDToolGlobal::ToolLightDlg)
		{
			return;
		}
		if (nullptr != SSDToolGlobal::ToolSelectLight)
		{
			SSDToolGlobal::ToolSelectLight->SetLightDir(Dir);
		}
	}
}


void LightDlg::OnEnChangePower()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.
	if (nullptr == SSDToolGlobal::ToolLightDlg)
	{
		return;
	}
	UpdateData(TRUE);
	CString TempValue;
	GetDlgItemText(IDC_POWER, TempValue);

	if (nullptr != SSDToolGlobal::ToolSelectLight)
	{
		SSDToolGlobal::ToolSelectLight->SetLightPower((float)_ttof(TempValue));
	}
	UpdateData(FALSE);
	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

//SPOT POWER
void LightDlg::OnEnChangeRange()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.
	//if (nullptr == SSDToolGlobal::ToolLightDlg)
	//{
	//	return;
	//}
	//UpdateData(TRUE);
	//CString TempValue;
	//GetDlgItemText(IDC_RANGE, TempValue);

	//if (nullptr != SSDToolGlobal::ToolSelectLight)
	//{
	//	SSDToolGlobal::ToolSelectLight->SetSpotPower((float)_ttof(TempValue));
	//}
	//UpdateData(FALSE);
	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void LightDlg::SetCurLightInfo()
{
	if (nullptr == SSDToolGlobal::ToolSelectActor)
	{
		return;
	}
	if (nullptr == SSDToolGlobal::ToolLightDlg)
	{
		return;
	}
	if (nullptr != SSDToolGlobal::ToolSelectLight)
	{
		CBUFFER::LightBuffer TempBuffer = SSDToolGlobal::ToolSelectLight->GetLightData();
		Vec4 TempColor = TempBuffer.LightColor;
		Vec3 TempDir = TempBuffer.LightDir;
		Vec4 TempAten = TempBuffer.LightAtenuation;
		Vec3 TempPos = SSDToolGlobal::ToolSelectLight->GetLightPivot();
		Vec3 TempSize = SSDToolGlobal::ToolSelectLight->GetLightSize();

		GetDlgItem(IDC_POSX3)->SetWindowTextW(LightDlg::GetWinChar(TempPos.x).c_str());
		GetDlgItem(IDC_POSY3)->SetWindowTextW(LightDlg::GetWinChar(TempPos.y).c_str());
		GetDlgItem(IDC_POSZ3)->SetWindowTextW(LightDlg::GetWinChar(TempPos.z).c_str());
		GetDlgItem(IDC_SCALEX3)->SetWindowTextW(LightDlg::GetWinChar(TempSize.x).c_str());
		GetDlgItem(IDC_SCALEY3)->SetWindowTextW(LightDlg::GetWinChar(TempSize.y).c_str());
		GetDlgItem(IDC_SCALEZ3)->SetWindowTextW(LightDlg::GetWinChar(TempSize.z).c_str());
		GetDlgItem(IDC_COLORR)->SetWindowTextW(LightDlg::GetWinChar(TempColor.r).c_str());
		GetDlgItem(IDC_COLORG)->SetWindowTextW(LightDlg::GetWinChar(TempColor.g).c_str());
		GetDlgItem(IDC_COLORB)->SetWindowTextW(LightDlg::GetWinChar(TempColor.b).c_str());
		GetDlgItem(IDC_COLORA)->SetWindowTextW(LightDlg::GetWinChar(TempColor.a).c_str());
		GetDlgItem(IDC_DIRX)->SetWindowTextW(LightDlg::GetWinChar(TempDir.x).c_str());
		GetDlgItem(IDC_DIRY)->SetWindowTextW(LightDlg::GetWinChar(TempDir.y).c_str());
		GetDlgItem(IDC_DIRZ)->SetWindowTextW(LightDlg::GetWinChar(TempDir.z).c_str());
		GetDlgItem(IDC_ATENIX)->SetWindowTextW(LightDlg::GetWinChar(TempAten.x).c_str());
		GetDlgItem(IDC_ATENY)->SetWindowTextW(LightDlg::GetWinChar(TempAten.y).c_str());
		GetDlgItem(IDC_ATENZ)->SetWindowTextW(LightDlg::GetWinChar(TempAten.z).c_str());
		GetDlgItem(IDC_ATENW)->SetWindowTextW(LightDlg::GetWinChar(TempAten.w).c_str());
		GetDlgItem(IDC_POWER)->SetWindowTextW(LightDlg::GetWinChar(TempBuffer.LightPower).c_str());
		GetDlgItem(IDC_SPOTPOWER)->SetWindowTextW(LightDlg::GetWinChar(TempBuffer.SpotPower).c_str());
		GetDlgItem(IDC_SPECPOWER)->SetWindowTextW(LightDlg::GetWinChar(TempBuffer.SpecularPower).c_str());
		GetDlgItem(IDC_AMBIPOWER)->SetWindowTextW(LightDlg::GetWinChar(TempBuffer.AmbientFactor).c_str());

		m_RSlider.SetPos(floattoRgb(TempColor.r));
		m_GSlider.SetPos(floattoRgb(TempColor.g));
		m_BSlider.SetPos(floattoRgb(TempColor.b));
		m_ASlider.SetPos(floattoRgb(TempColor.a));

		m_DistanceSlider.SetPos((int)TempAten.x);
		m_ConstantSlider.SetPos((int)TempAten.y);
		m_LinearSlider.SetPos(FloattoInt(TempAten.z));
		m_QuadraticSlider.SetPos(FloattoInt(TempAten.w));

		bool TempIsVolume = SSDToolGlobal::ToolSelectLight->GetIsVolume();
		m_VolumeMeshCheck = TempIsVolume;

		if (true == TempIsVolume)
		{
			std::wstring TempMeshName = SSDToolGlobal::ToolSelectLight->GetVolumeMeshName();
			if (L"Cube" == TempMeshName)
			{
				m_VolumeList.SetCurSel(0);
			}
			else if (L"Cone_Lv360" == TempMeshName)
			{
				m_VolumeList.SetCurSel(1);
			}
			else if (L"Sphere_Lv8" == TempMeshName)
			{
				m_VolumeList.SetCurSel(2);
			}
			else if (L"Sphere_Lv16" == TempMeshName)
			{
				m_VolumeList.SetCurSel(3);
			}
			else if (L"Sphere_Lv32" == TempMeshName)
			{
				m_VolumeList.SetCurSel(4);
			}
			else if (L"Sphere_Lv64" == TempMeshName)
			{
				m_VolumeList.SetCurSel(5);
			}
			else
			{
				m_VolumeList.SetCurSel(6);

			}
		}
		UpdateData(FALSE);

		int TempTypeNum = TempBuffer.LightType;
		if (0 != TempTypeNum && 1 != TempTypeNum && 2 != TempTypeNum)
		{
			m_TypeList.SetCurSel(3);
		}
		m_TypeList.SetCurSel(TempTypeNum);
	}
}

void LightDlg::SetNewLightInfo(bool _VolumeCheck)
{
	if (nullptr == SSDToolGlobal::ToolSelectActor)
	{
		return;
	}
	if (nullptr == SSDToolGlobal::ToolLightDlg)
	{
		return;
	}
	if (nullptr != SSDToolGlobal::ToolSelectLight)
	{
		Vec4 TempColor = Vec4::White;
		Vec3 TempDir = Vec3(1.f, 0.f, 0.f);
		Vec4 TempAten = Vec4(2000.f, 1.f, 1.f, 0.f);
		Vec3 TempPos = Vec3(0.f, 0.f, 0.f);
		Vec3 TempSize = Vec3(300.f, 500.f, 300.f);
		float SpecPower = 40.f;
		float LightPower = 1.f;
		float LightRange = TempSize.y;
		float AmbientFactor = 0.05f;

		GetDlgItem(IDC_POSX3)->SetWindowTextW(LightDlg::GetWinChar(TempPos.x).c_str());
		GetDlgItem(IDC_POSY3)->SetWindowTextW(LightDlg::GetWinChar(TempPos.y).c_str());
		GetDlgItem(IDC_POSZ3)->SetWindowTextW(LightDlg::GetWinChar(TempPos.z).c_str());
		GetDlgItem(IDC_SCALEX3)->SetWindowTextW(LightDlg::GetWinChar(TempSize.x).c_str());
		GetDlgItem(IDC_SCALEY3)->SetWindowTextW(LightDlg::GetWinChar(TempSize.y).c_str());
		GetDlgItem(IDC_SCALEZ3)->SetWindowTextW(LightDlg::GetWinChar(TempSize.z).c_str());
		GetDlgItem(IDC_COLORR)->SetWindowTextW(LightDlg::GetWinChar(TempColor.r).c_str());
		GetDlgItem(IDC_COLORG)->SetWindowTextW(LightDlg::GetWinChar(TempColor.g).c_str());
		GetDlgItem(IDC_COLORB)->SetWindowTextW(LightDlg::GetWinChar(TempColor.b).c_str());
		GetDlgItem(IDC_COLORA)->SetWindowTextW(LightDlg::GetWinChar(TempColor.a).c_str());
		GetDlgItem(IDC_DIRX)->SetWindowTextW(LightDlg::GetWinChar(TempDir.x).c_str());
		GetDlgItem(IDC_DIRY)->SetWindowTextW(LightDlg::GetWinChar(TempDir.y).c_str());
		GetDlgItem(IDC_DIRZ)->SetWindowTextW(LightDlg::GetWinChar(TempDir.z).c_str());
		GetDlgItem(IDC_ATENIX)->SetWindowTextW(LightDlg::GetWinChar(TempAten.x).c_str());
		GetDlgItem(IDC_ATENY)->SetWindowTextW(LightDlg::GetWinChar(TempAten.y).c_str());
		GetDlgItem(IDC_ATENZ)->SetWindowTextW(LightDlg::GetWinChar(TempAten.z).c_str());
		GetDlgItem(IDC_ATENW)->SetWindowTextW(LightDlg::GetWinChar(TempAten.w).c_str());
		GetDlgItem(IDC_POWER)->SetWindowTextW(LightDlg::GetWinChar(LightPower).c_str());
		GetDlgItem(IDC_SPECPOWER)->SetWindowTextW(LightDlg::GetWinChar(SpecPower).c_str());
		GetDlgItem(IDC_AMBIPOWER)->SetWindowTextW(LightDlg::GetWinChar(AmbientFactor).c_str());

		m_RSlider.SetPos(floattoRgb(TempColor.r));
		m_GSlider.SetPos(floattoRgb(TempColor.g));
		m_BSlider.SetPos(floattoRgb(TempColor.b));
		m_ASlider.SetPos(floattoRgb(TempColor.a));

		m_DistanceSlider.SetPos((int)TempAten.x);
		m_ConstantSlider.SetPos((int)TempAten.y);
		m_LinearSlider.SetPos(FloattoInt(TempAten.z));
		m_QuadraticSlider.SetPos(FloattoInt(TempAten.w));

		m_VolumeMeshCheck = _VolumeCheck;
		if (TRUE == m_VolumeMeshCheck)
		{
			m_VolumeList.SetCurSel(1);
			m_TypeList.SetCurSel(1);
			GetDlgItem(IDC_SPOTPOWER)->SetWindowTextW(LightDlg::GetWinChar(0).c_str());
		}
		else
		{
			m_TypeList.SetCurSel(3);
		}
		UpdateData(FALSE);
	}
}

std::wstring LightDlg::GetWinChar(float _Value)
{
	char Temp[20];
	sprintf_s(Temp, "%f", _Value);
	std::string TempKey = std::string(Temp);
	std::wstring KeyName;
	KeyName.assign(TempKey.begin(), TempKey.end());
	return KeyName;
}

void LightDlg::ResetLightValue()
{
	GetDlgItem(IDC_POSX3)->SetWindowTextW(L"0.f");
	GetDlgItem(IDC_POSY3)->SetWindowTextW(L"0.f");
	GetDlgItem(IDC_POSZ3)->SetWindowTextW(L"0.f");
	GetDlgItem(IDC_SCALEX3)->SetWindowTextW(L"1.f");
	GetDlgItem(IDC_SCALEY3)->SetWindowTextW(L"1.f");
	GetDlgItem(IDC_SCALEZ3)->SetWindowTextW(L"1.f");
	GetDlgItem(IDC_COLORR)->SetWindowTextW(L"1.f");
	GetDlgItem(IDC_COLORG)->SetWindowTextW(L"1.f");
	GetDlgItem(IDC_COLORB)->SetWindowTextW(L"1.f");
	GetDlgItem(IDC_COLORA)->SetWindowTextW(L"1.f");
	GetDlgItem(IDC_DIRX)->SetWindowTextW(L"0.f");
	GetDlgItem(IDC_DIRY)->SetWindowTextW(L"0.f");
	GetDlgItem(IDC_DIRZ)->SetWindowTextW(L"0.f");
	GetDlgItem(IDC_ATENW)->SetWindowTextW(L"1.f");
	GetDlgItem(IDC_ATENZ)->SetWindowTextW(L"1.f");
	GetDlgItem(IDC_ATENY)->SetWindowTextW(L"1.f");
	GetDlgItem(IDC_ATENIX)->SetWindowTextW(L"1.f");
	GetDlgItem(IDC_POWER)->SetWindowTextW(L"1.f");
	GetDlgItem(IDC_SPOTPOWER)->SetWindowTextW(L"0.f");
	GetDlgItem(IDC_SPECPOWER)->SetWindowTextW(L"32.f");
	GetDlgItem(IDC_AMBIPOWER)->SetWindowTextW(L"0.05f");

	m_VolumeList.ResetContent();
	m_VolumeList.AddString(L"DIRECTION_LIGHT");
	m_VolumeList.AddString(L"POINT_LIGHT");
	m_VolumeList.AddString(L"SPOT_LIGHT");
	m_VolumeList.UpdateData();

	m_TypeList.ResetContent();
	m_TypeList.AddString(L"Cube");
	m_TypeList.AddString(L"Cone_Lv360");
	m_TypeList.AddString(L"Sphere_Lv8");
	m_TypeList.AddString(L"Sphere_Lv16");
	m_TypeList.AddString(L"Sphere_Lv32");
	m_TypeList.AddString(L"Sphere_Lv64");
	m_VolumeList.UpdateData();
}

void LightDlg::OnCbnSetfocusVolumemesh()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

//라이트 생성 후 디폴트 라이트에서 볼륨메쉬로 변경하는 셋 버튼
void LightDlg::OnBnClickedButton1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (nullptr == SSDToolGlobal::ToolSelectLight)
	{
		return;
	}
}

//씬 리스트에서 라이트 선택하면 dlg 생성 - 값 바꾸면 바뀐다
//엑터 추가하고 라이트 추가하면 dlg 생성 - 값 바꾸면 바뀐다 - 볼륨라이트인지 일반 라이트인지는 셋해줘야함

void LightDlg::OnEnChangePosx3()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.
	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void LightDlg::OnEnChangePosy3()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void LightDlg::OnEnChangePosz3()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void LightDlg::OnEnChangeScalex3()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	UpdateData(TRUE);

	CString TempValue;
	GetDlgItemText(IDC_SCALEX3, TempValue);

	if (nullptr != SSDToolGlobal::ToolSelectLight)
	{
		Vec3 TempPos = SSDToolGlobal::ToolSelectLight->GetLightSize();
		SSDToolGlobal::ToolSelectLight->SetLightSize(Vec3((float)_ttof(TempValue), TempPos.y, TempPos.z));
		GetDlgItem(IDC_SPOTPOWER)->SetWindowTextW(LightDlg::GetWinChar(SSDToolGlobal::ToolSelectLight->GetLightData().SpotPower).c_str());
	}
	UpdateData(FALSE);

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void LightDlg::OnEnChangeScaley3()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.
	UpdateData(TRUE);

	CString TempValue;
	GetDlgItemText(IDC_SCALEY3, TempValue);

	if (nullptr != SSDToolGlobal::ToolSelectLight)
	{
		Vec3 TempPos = SSDToolGlobal::ToolSelectLight->GetLightSize();
		SSDToolGlobal::ToolSelectLight->SetLightSize(Vec3(TempPos.x, (float)_ttof(TempValue), TempPos.z));
		GetDlgItem(IDC_SPOTPOWER)->SetWindowTextW(LightDlg::GetWinChar(SSDToolGlobal::ToolSelectLight->GetLightData().SpotPower).c_str());
	}
	UpdateData(FALSE);
	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void LightDlg::OnEnChangeScalez3()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.
	UpdateData(TRUE);

	CString TempValue;
	GetDlgItemText(IDC_SCALEZ3, TempValue);

	if (nullptr != SSDToolGlobal::ToolSelectLight)
	{
		Vec3 TempPos = SSDToolGlobal::ToolSelectLight->GetLightSize();
		SSDToolGlobal::ToolSelectLight->SetLightSize(Vec3(TempPos.x, TempPos.y, (float)_ttof(TempValue)));
		GetDlgItem(IDC_SPOTPOWER)->SetWindowTextW(LightDlg::GetWinChar(SSDToolGlobal::ToolSelectLight->GetLightData().SpotPower).c_str());
	}
	UpdateData(FALSE);
	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


BOOL LightDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	CEdit *webmacrotext = (CEdit *)GetDlgItem(IDC_COLORR);
	if (pMsg->message == WM_LBUTTONDOWN && pMsg->hwnd == webmacrotext->m_hWnd &&
		GetFocus()->m_hWnd != webmacrotext->m_hWnd)
	{
		webmacrotext->SetFocus();
		webmacrotext->SetSel(0, -1, true);
		return true;
	}

	webmacrotext = (CEdit *)GetDlgItem(IDC_COLORG);
	if (pMsg->message == WM_LBUTTONDOWN && pMsg->hwnd == webmacrotext->m_hWnd &&
		GetFocus()->m_hWnd != webmacrotext->m_hWnd)
	{
		webmacrotext->SetFocus();
		webmacrotext->SetSel(0, -1, true);
		return true;
	}

	webmacrotext = (CEdit *)GetDlgItem(IDC_COLORB);
	if (pMsg->message == WM_LBUTTONDOWN && pMsg->hwnd == webmacrotext->m_hWnd &&
		GetFocus()->m_hWnd != webmacrotext->m_hWnd)
	{
		webmacrotext->SetFocus();
		webmacrotext->SetSel(0, -1, true);
		return true;
	}

	webmacrotext = (CEdit *)GetDlgItem(IDC_COLORA);
	if (pMsg->message == WM_LBUTTONDOWN && pMsg->hwnd == webmacrotext->m_hWnd &&
		GetFocus()->m_hWnd != webmacrotext->m_hWnd)
	{
		webmacrotext->SetFocus();
		webmacrotext->SetSel(0, -1, true);
		return true;
	}

	webmacrotext = (CEdit *)GetDlgItem(IDC_DIRX);
	if (pMsg->message == WM_LBUTTONDOWN && pMsg->hwnd == webmacrotext->m_hWnd &&
		GetFocus()->m_hWnd != webmacrotext->m_hWnd)
	{
		webmacrotext->SetFocus();
		webmacrotext->SetSel(0, -1, true);
		return true;
	}

	webmacrotext = (CEdit *)GetDlgItem(IDC_DIRY);
	if (pMsg->message == WM_LBUTTONDOWN && pMsg->hwnd == webmacrotext->m_hWnd &&
		GetFocus()->m_hWnd != webmacrotext->m_hWnd)
	{
		webmacrotext->SetFocus();
		webmacrotext->SetSel(0, -1, true);
		return true;
	}

	webmacrotext = (CEdit *)GetDlgItem(IDC_DIRZ);
	if (pMsg->message == WM_LBUTTONDOWN && pMsg->hwnd == webmacrotext->m_hWnd &&
		GetFocus()->m_hWnd != webmacrotext->m_hWnd)
	{
		webmacrotext->SetFocus();
		webmacrotext->SetSel(0, -1, true);
		return true;
	}

	webmacrotext = (CEdit *)GetDlgItem(IDC_ATENW);
	if (pMsg->message == WM_LBUTTONDOWN && pMsg->hwnd == webmacrotext->m_hWnd &&
		GetFocus()->m_hWnd != webmacrotext->m_hWnd)
	{
		webmacrotext->SetFocus();
		webmacrotext->SetSel(0, -1, true);
		return true;
	}

	webmacrotext = (CEdit *)GetDlgItem(IDC_ATENZ);
	if (pMsg->message == WM_LBUTTONDOWN && pMsg->hwnd == webmacrotext->m_hWnd &&
		GetFocus()->m_hWnd != webmacrotext->m_hWnd)
	{
		webmacrotext->SetFocus();
		webmacrotext->SetSel(0, -1, true);
		return true;
	}

	webmacrotext = (CEdit *)GetDlgItem(IDC_ATENY);
	if (pMsg->message == WM_LBUTTONDOWN && pMsg->hwnd == webmacrotext->m_hWnd &&
		GetFocus()->m_hWnd != webmacrotext->m_hWnd)
	{
		webmacrotext->SetFocus();
		webmacrotext->SetSel(0, -1, true);
		return true;
	}

	webmacrotext = (CEdit *)GetDlgItem(IDC_ATENIX);
	if (pMsg->message == WM_LBUTTONDOWN && pMsg->hwnd == webmacrotext->m_hWnd &&
		GetFocus()->m_hWnd != webmacrotext->m_hWnd)
	{
		webmacrotext->SetFocus();
		webmacrotext->SetSel(0, -1, true);
		return true;
	}

	webmacrotext = (CEdit *)GetDlgItem(IDC_SPECPOWER);
	if (pMsg->message == WM_LBUTTONDOWN && pMsg->hwnd == webmacrotext->m_hWnd &&
		GetFocus()->m_hWnd != webmacrotext->m_hWnd)
	{
		webmacrotext->SetFocus();
		webmacrotext->SetSel(0, -1, true);
		return true;
	}

	webmacrotext = (CEdit *)GetDlgItem(IDC_AMBIPOWER);
	if (pMsg->message == WM_LBUTTONDOWN && pMsg->hwnd == webmacrotext->m_hWnd &&
		GetFocus()->m_hWnd != webmacrotext->m_hWnd)
	{
		webmacrotext->SetFocus();
		webmacrotext->SetSel(0, -1, true);
		return true;
	}

	webmacrotext = (CEdit *)GetDlgItem(IDC_POWER);
	if (pMsg->message == WM_LBUTTONDOWN && pMsg->hwnd == webmacrotext->m_hWnd &&
		GetFocus()->m_hWnd != webmacrotext->m_hWnd)
	{
		webmacrotext->SetFocus();
		webmacrotext->SetSel(0, -1, true);
		return true;
	}

	webmacrotext = (CEdit *)GetDlgItem(IDC_SPOTPOWER);
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


void LightDlg::OnNMCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	int nPos;
	CString strPos;
	nPos = m_RSlider.GetPos();

	float Value = Rgbtofloat(nPos);
	strPos.Format(_T("%f"), Value);

	GetDlgItem(IDC_COLORR)->SetWindowTextW(strPos);
	UpdateData(FALSE);
	*pResult = 0;
}


void LightDlg::OnNMCustomdrawSlider2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	UpdateData(TRUE);
	int nPos;
	CString strPos;
	nPos = m_GSlider.GetPos();

	float Value = Rgbtofloat(nPos);
	strPos.Format(_T("%f"), Value);

	GetDlgItem(IDC_COLORG)->SetWindowTextW(strPos);
	UpdateData(FALSE);

	*pResult = 0;
}


void LightDlg::OnNMCustomdrawSlider3(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	int nPos;
	CString strPos;
	nPos = m_BSlider.GetPos();

	float Value = Rgbtofloat(nPos);
	strPos.Format(_T("%f"), Value);

	GetDlgItem(IDC_COLORB)->SetWindowTextW(strPos);
	UpdateData(FALSE);

	*pResult = 0;
}


void LightDlg::OnNMCustomdrawSlider4(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
}


void LightDlg::OnNMCustomdrawSlider5(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
	UpdateData(TRUE);
	int nPos;
	CString strPos;
	nPos = m_DistanceSlider.GetPos();

	strPos.Format(_T("%d"), nPos);
	GetDlgItem(IDC_ATENIX)->SetWindowTextW(strPos);

	UpdateData(FALSE);
}


void LightDlg::OnNMCustomdrawSlider6(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
	UpdateData(TRUE);
	int nPos;
	CString strPos;
	nPos = m_ConstantSlider.GetPos();

	strPos.Format(_T("%d"), nPos);
	GetDlgItem(IDC_ATENY)->SetWindowTextW(strPos);

	UpdateData(FALSE);
}


void LightDlg::OnNMCustomdrawSlider7(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
	UpdateData(TRUE);
	int nPos;
	CString strPos;
	nPos = m_LinearSlider.GetPos();

	float Value = InttoFloat(nPos);
	strPos.Format(_T("%f"), Value);

	GetDlgItem(IDC_ATENZ)->SetWindowTextW(strPos);
	UpdateData(FALSE);
}


void LightDlg::OnNMCustomdrawSlider8(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
	UpdateData(TRUE);
	int nPos;
	CString strPos;
	nPos = m_QuadraticSlider.GetPos();

	float Value = InttoFloat(nPos);
	strPos.Format(_T("%f"), Value);

	GetDlgItem(IDC_ATENW)->SetWindowTextW(strPos);
	UpdateData(FALSE);
}

void LightDlg::SpotLightDirUpdate()
{
	if (nullptr == SSDToolGlobal::ToolLightDlg)
	{
		return;
	}
	if (nullptr == SSDToolGlobal::ToolSelectLight)
	{
		return;
	}
	if (nullptr == SSDToolGlobal::ToolSelectActor)
	{
		return;
	}
	Vec3 LightDir = SSDToolGlobal::ToolSelectLight->GetLightDir();
	//SSDToolGlobal::ToolSelectLight->SetLightDir(LightDir);

	CString strPos;
	strPos.Format(_T("%f"), LightDir.x);
	GetDlgItem(IDC_DIRX)->SetWindowTextW(strPos);

	strPos.Format(_T("%f"), LightDir.y);
	GetDlgItem(IDC_DIRY)->SetWindowTextW(strPos);

	strPos.Format(_T("%f"), LightDir.z);
	GetDlgItem(IDC_DIRZ)->SetWindowTextW(strPos);
}

float LightDlg::Rgbtofloat(int _Rgb)
{
	if (_Rgb > 255)
	{
		return 0.f;
	}
	if (_Rgb < 0)
	{
		return 0.f;
	}
	return (float)_Rgb / 255.f;
}

int LightDlg::floattoRgb(float _Value)
{
	if (_Value > 1.f)
	{
		return 0;
	}
	if (_Value < 0.f)
	{
		return 0;
	}
	return (int)(_Value * 255);
}

float LightDlg::InttoFloat(int _Int)
{
	if (_Int > 200)
	{
		return 0.f;
	}
	if (_Int < 0)
	{
		return 0.f;
	}
	return (float)_Int / 100.f;
}

int LightDlg::FloattoInt(float _Float)
{
	if (_Float > 2.f)
	{
		return 0;
	}
	if (_Float < 0.f)
	{
		return 0;
	}
	return (int)(_Float * 100.f);
}
