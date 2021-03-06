// FogDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "SSDTool.h"
#include "FogDlg.h"
#include "afxdialogex.h"
#include "LightDlg.h"

#include "SSDToolGlobal.h"
#include <DXStruct.h>
#include <FogFilter.h>

// FogDlg 대화 상자

IMPLEMENT_DYNAMIC(FogDlg, CDialogEx)

FogDlg::FogDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_FOGDLG, pParent)
{

}

FogDlg::~FogDlg()
{
}

void FogDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDER1, m_FogColorR);
	DDX_Control(pDX, IDC_SLIDER2, m_FogColorG);
	DDX_Control(pDX, IDC_SLIDER3, m_FogColorB);
	DDX_Control(pDX, IDC_SLIDER4, m_FogColorA);
	DDX_Control(pDX, IDC_SLIDER5, m_HighLightColorR);
	DDX_Control(pDX, IDC_SLIDER6, m_HighLightColorG);
	DDX_Control(pDX, IDC_SLIDER7, m_HighLightColorB);
	DDX_Control(pDX, IDC_SLIDER8, m_HighLightColorA);
}


BEGIN_MESSAGE_MAP(FogDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &FogDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &FogDlg::OnBnClickedCancel)
	ON_EN_CHANGE(IDC_FOGCOLORR, &FogDlg::OnEnChangeColorr)
	ON_EN_CHANGE(IDC_FOGCOLORG, &FogDlg::OnEnChangeColorg)
	ON_EN_CHANGE(IDC_FOGCOLORB, &FogDlg::OnEnChangeColorb)
	ON_EN_CHANGE(IDC_FOGCOLORA, &FogDlg::OnEnChangeColora)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER1, &FogDlg::OnNMCustomdrawSlider1)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER2, &FogDlg::OnNMCustomdrawSlider2)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER3, &FogDlg::OnNMCustomdrawSlider3)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER4, &FogDlg::OnNMCustomdrawSlider4)
	ON_EN_CHANGE(IDC_FOGCOLORR2, &FogDlg::OnEnChangeColorr2)
	ON_EN_CHANGE(IDC_FOGCOLORG2, &FogDlg::OnEnChangeColorg2)
	ON_EN_CHANGE(IDC_FOGCOLORB2, &FogDlg::OnEnChangeColorb2)
	ON_EN_CHANGE(IDC_FOGCOLORA3, &FogDlg::OnEnChangeColora3)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER5, &FogDlg::OnNMCustomdrawSlider5)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER6, &FogDlg::OnNMCustomdrawSlider6)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER7, &FogDlg::OnNMCustomdrawSlider7)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER8, &FogDlg::OnNMCustomdrawSlider8)
	ON_EN_CHANGE(IDC_STARTDEPTH, &FogDlg::OnEnChangeStartdepth)
	ON_EN_CHANGE(IDC_DENSITY, &FogDlg::OnEnChangeDensity)
	ON_EN_CHANGE(IDC_FALLOFF, &FogDlg::OnEnChangeFalloff)
	ON_EN_CHANGE(IDC_SUNDIRX, &FogDlg::OnEnChangeSundirx)
	ON_EN_CHANGE(IDC_SUNDIRY, &FogDlg::OnEnChangeSundiry)
	ON_EN_CHANGE(IDC_SUNDIRZ, &FogDlg::OnEnChangeSundirz)
END_MESSAGE_MAP()


BOOL FogDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 수지 컴퓨터 해상도
	SetWindowPos(this, 0 - 2, 265, 331 - 4 * 0, 961 + 10, SWP_NOZORDER);

	// 석재 컴퓨터 해상도
	//SetWindowPos(this, 0 - 2, 330, 331, 971, SWP_NOZORDER);

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	((CEdit*)GetDlgItem(IDC_FOGCOLORR))->SetLimitText(6);
	((CEdit*)GetDlgItem(IDC_FOGCOLORG))->SetLimitText(6);
	((CEdit*)GetDlgItem(IDC_FOGCOLORB))->SetLimitText(6);
	((CEdit*)GetDlgItem(IDC_FOGCOLORA))->SetLimitText(6);
	((CEdit*)GetDlgItem(IDC_FOGCOLORR2))->SetLimitText(6);
	((CEdit*)GetDlgItem(IDC_FOGCOLORG2))->SetLimitText(6);
	((CEdit*)GetDlgItem(IDC_FOGCOLORB2))->SetLimitText(6);
	((CEdit*)GetDlgItem(IDC_FOGCOLORA3))->SetLimitText(6);
	((CEdit*)GetDlgItem(IDC_STARTDEPTH))->SetLimitText(6);
	((CEdit*)GetDlgItem(IDC_DENSITY))->SetLimitText(6);
	((CEdit*)GetDlgItem(IDC_FALLOFF))->SetLimitText(6);
	((CEdit*)GetDlgItem(IDC_SUNDIRX))->SetLimitText(6);
	((CEdit*)GetDlgItem(IDC_SUNDIRY))->SetLimitText(6);
	((CEdit*)GetDlgItem(IDC_SUNDIRZ))->SetLimitText(6);

	m_HighLightColorR.SetRange(0, 255);
	m_HighLightColorR.SetPageSize(1);
	m_HighLightColorR.SetTicFreq(10);

	m_HighLightColorG.SetRange(0, 255);
	m_HighLightColorG.SetPageSize(1);
	m_HighLightColorG.SetTicFreq(10);

	m_HighLightColorB.SetRange(0, 255);
	m_HighLightColorB.SetPageSize(1);
	m_HighLightColorB.SetTicFreq(10);

	m_HighLightColorA.SetRange(0, 255);
	m_HighLightColorA.SetPageSize(1);
	m_HighLightColorA.SetTicFreq(10);

	m_FogColorR.SetRange(0, 255);
	m_FogColorR.SetPageSize(1);
	m_FogColorR.SetTicFreq(10);

	m_FogColorG.SetRange(0, 255);
	m_FogColorG.SetPageSize(1);
	m_FogColorG.SetTicFreq(10);

	m_FogColorB.SetRange(0, 255);
	m_FogColorB.SetPageSize(1);
	m_FogColorB.SetTicFreq(10);

	m_FogColorA.SetRange(0, 255);
	m_FogColorA.SetPageSize(1);
	m_FogColorA.SetTicFreq(10);

	//현재 씬에 있는 가장 첫번째 포그 가져오기
	//if (nullptr != )
	//{

	//}


	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void FogDlg::SetCurFog()
{


}

// FogDlg 메시지 처리기


void FogDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//CDialogEx::OnOK();
}


void FogDlg::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//CDialogEx::OnCancel();
}


void FogDlg::OnEnChangeColorr()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.

	if (nullptr == SSDToolGlobal::ToolSelectFog)
	{
		return;
	}
	if (nullptr == SSDToolGlobal::ToolFogDlg)
	{
		return;
	}

	UpdateData(TRUE);
	CString TempValue;
	GetDlgItemText(IDC_FOGCOLORR, TempValue);

	if (nullptr != SSDToolGlobal::ToolSelectFog)
	{
		float Value = (float)_ttof(TempValue);
		CBUFFER::FogBuffer TempFogBuffer = SSDToolGlobal::ToolSelectFog->GetFogBuffer();
		SSDToolGlobal::ToolSelectFog->SetFogColor(Vec3(Value, TempFogBuffer.FogColor.g, TempFogBuffer.FogColor.b));
	}

	UpdateData(FALSE);
}


void FogDlg::OnEnChangeColorg()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.

	if (nullptr == SSDToolGlobal::ToolSelectFog)
	{
		return;
	}
	if (nullptr == SSDToolGlobal::ToolFogDlg)
	{
		return;
	}
	UpdateData(TRUE);
	CString TempValue;
	GetDlgItemText(IDC_FOGCOLORR, TempValue);

	if (nullptr != SSDToolGlobal::ToolSelectFog)
	{
		float Value = (float)_ttof(TempValue);
		CBUFFER::FogBuffer TempFogBuffer = SSDToolGlobal::ToolSelectFog->GetFogBuffer();
		SSDToolGlobal::ToolSelectFog->SetFogColor(Vec3(TempFogBuffer.FogColor.r, Value, TempFogBuffer.FogColor.b));
	}

	UpdateData(FALSE);
}


void FogDlg::OnEnChangeColorb()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (nullptr == SSDToolGlobal::ToolSelectFog)
	{
		return;
	}
	if (nullptr == SSDToolGlobal::ToolFogDlg)
	{
		return;
	}
	UpdateData(TRUE);
	CString TempValue;
	GetDlgItemText(IDC_FOGCOLORR, TempValue);

	if (nullptr != SSDToolGlobal::ToolSelectFog)
	{
		float Value = (float)_ttof(TempValue);
		CBUFFER::FogBuffer TempFogBuffer = SSDToolGlobal::ToolSelectFog->GetFogBuffer();
		SSDToolGlobal::ToolSelectFog->SetFogColor(Vec3(TempFogBuffer.FogColor.r, TempFogBuffer.FogColor.g, Value));
	}

	UpdateData(FALSE);

}


void FogDlg::OnEnChangeColora()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.

}


void FogDlg::OnNMCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	UpdateData(TRUE);
	int nPos;
	CString strPos;
	nPos = m_HighLightColorR.GetPos();

	float Value = LightDlg::LightDlg::Rgbtofloat(nPos);
	strPos.Format(_T("%f"), Value);

	GetDlgItem(IDC_FOGCOLORR)->SetWindowTextW(strPos);
	UpdateData(FALSE);
}


void FogDlg::OnNMCustomdrawSlider2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
	UpdateData(TRUE);
	int nPos;
	CString strPos;
	nPos = m_FogColorG.GetPos();

	float Value = LightDlg::LightDlg::Rgbtofloat(nPos);
	strPos.Format(_T("%f"), Value);

	GetDlgItem(IDC_FOGCOLORG)->SetWindowTextW(strPos);
	UpdateData(FALSE);
}


void FogDlg::OnNMCustomdrawSlider3(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
	UpdateData(TRUE);
	int nPos;
	CString strPos;
	nPos = m_FogColorB.GetPos();

	float Value = LightDlg::LightDlg::Rgbtofloat(nPos);
	strPos.Format(_T("%f"), Value);

	GetDlgItem(IDC_FOGCOLORB)->SetWindowTextW(strPos);
	UpdateData(FALSE);
}


void FogDlg::OnNMCustomdrawSlider4(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

}


void FogDlg::OnEnChangeColorr2()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.

	if (nullptr == SSDToolGlobal::ToolSelectFog)
	{
		return;
	}
	if (nullptr == SSDToolGlobal::ToolFogDlg)
	{
		return;
	}
	UpdateData(TRUE);
	CString TempValue;
	GetDlgItemText(IDC_FOGCOLORR, TempValue);

	if (nullptr != SSDToolGlobal::ToolSelectFog)
	{
		float Value = (float)_ttof(TempValue);
		CBUFFER::FogBuffer TempFogBuffer = SSDToolGlobal::ToolSelectFog->GetFogBuffer();
		SSDToolGlobal::ToolSelectFog->SetFogHighlightColor(Vec3(Value, TempFogBuffer.FogColor.g, TempFogBuffer.FogColor.b));
	}

	UpdateData(FALSE);
}


void FogDlg::OnEnChangeColorg2()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.

	if (nullptr == SSDToolGlobal::ToolSelectFog)
	{
		return;
	}
	if (nullptr == SSDToolGlobal::ToolFogDlg)
	{
		return;
	}
	UpdateData(TRUE);
	CString TempValue;
	GetDlgItemText(IDC_FOGCOLORR, TempValue);

	if (nullptr != SSDToolGlobal::ToolSelectFog)
	{
		float Value = (float)_ttof(TempValue);
		CBUFFER::FogBuffer TempFogBuffer = SSDToolGlobal::ToolSelectFog->GetFogBuffer();
		SSDToolGlobal::ToolSelectFog->SetFogHighlightColor(Vec3(TempFogBuffer.FogColor.r, Value, TempFogBuffer.FogColor.b));
	}

	UpdateData(FALSE);
}


void FogDlg::OnEnChangeColorb2()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (nullptr == SSDToolGlobal::ToolSelectFog)
	{
		return;
	}
	if (nullptr == SSDToolGlobal::ToolFogDlg)
	{
		return;
	}
	UpdateData(TRUE);
	CString TempValue;
	GetDlgItemText(IDC_FOGCOLORR, TempValue);

	if (nullptr != SSDToolGlobal::ToolSelectFog)
	{
		float Value = (float)_ttof(TempValue);
		CBUFFER::FogBuffer TempFogBuffer = SSDToolGlobal::ToolSelectFog->GetFogBuffer();
		SSDToolGlobal::ToolSelectFog->SetFogHighlightColor(Vec3(TempFogBuffer.FogColor.r, TempFogBuffer.FogColor.g, Value));
	}

	UpdateData(FALSE);
}


void FogDlg::OnEnChangeColora3()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void FogDlg::OnNMCustomdrawSlider5(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
	UpdateData(TRUE);
	int nPos;
	CString strPos;
	nPos = m_HighLightColorR.GetPos();

	float Value = LightDlg::LightDlg::Rgbtofloat(nPos);
	strPos.Format(_T("%f"), Value);

	GetDlgItem(IDC_FOGCOLORR2)->SetWindowTextW(strPos);
	UpdateData(FALSE);
}


void FogDlg::OnNMCustomdrawSlider6(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
	UpdateData(TRUE);
	int nPos;
	CString strPos;
	nPos = m_HighLightColorG.GetPos();

	float Value = LightDlg::LightDlg::Rgbtofloat(nPos);
	strPos.Format(_T("%f"), Value);

	GetDlgItem(IDC_FOGCOLORG2)->SetWindowTextW(strPos);
	UpdateData(FALSE);
}


void FogDlg::OnNMCustomdrawSlider7(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
	UpdateData(TRUE);
	int nPos;
	CString strPos;
	nPos = m_HighLightColorB.GetPos();

	float Value = LightDlg::LightDlg::Rgbtofloat(nPos);
	strPos.Format(_T("%f"), Value);

	GetDlgItem(IDC_FOGCOLORB2)->SetWindowTextW(strPos);
	UpdateData(FALSE);
}


void FogDlg::OnNMCustomdrawSlider8(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

}


void FogDlg::OnEnChangeStartdepth()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (nullptr == SSDToolGlobal::ToolSelectFog)
	{
		return;
	}
	if (nullptr == SSDToolGlobal::ToolFogDlg)
	{
		return;
	}
	UpdateData(TRUE);
	CString TempValue;
	GetDlgItemText(IDC_FOGCOLORR, TempValue);

	if (nullptr != SSDToolGlobal::ToolSelectFog)
	{
		float Value = (float)_ttof(TempValue);
		SSDToolGlobal::ToolSelectFog->SetFogStartDepth(Value);
	}

	UpdateData(FALSE);
}


void FogDlg::OnEnChangeDensity()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (nullptr == SSDToolGlobal::ToolSelectFog)
	{
		return;
	}
	if (nullptr == SSDToolGlobal::ToolFogDlg)
	{
		return;
	}
	UpdateData(TRUE);
	CString TempValue;
	GetDlgItemText(IDC_FOGCOLORR, TempValue);

	if (nullptr != SSDToolGlobal::ToolSelectFog)
	{
		float Value = (float)_ttof(TempValue);
		SSDToolGlobal::ToolSelectFog->SetFogGlobalDensity(Value);
	}

	UpdateData(FALSE);
}


void FogDlg::OnEnChangeFalloff()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (nullptr == SSDToolGlobal::ToolSelectFog)
	{
		return;
	}
	if (nullptr == SSDToolGlobal::ToolFogDlg)
	{
		return;
	}
	UpdateData(TRUE);
	CString TempValue;
	GetDlgItemText(IDC_FOGCOLORR, TempValue);

	if (nullptr != SSDToolGlobal::ToolSelectFog)
	{
		float Value = (float)_ttof(TempValue);
		SSDToolGlobal::ToolSelectFog->SetFogHeightFallOff(Value);
	}

	UpdateData(FALSE);
}


void FogDlg::OnEnChangeSundirx()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (nullptr == SSDToolGlobal::ToolSelectFog)
	{
		return;
	}
	if (nullptr == SSDToolGlobal::ToolFogDlg)
	{
		return;
	}
	UpdateData(TRUE);
	CString TempValue;
	GetDlgItemText(IDC_FOGCOLORR, TempValue);

	if (nullptr != SSDToolGlobal::ToolSelectFog)
	{
		float Value = (float)_ttof(TempValue);
		CBUFFER::FogBuffer TempFogBuffer = SSDToolGlobal::ToolSelectFog->GetFogBuffer();
		SSDToolGlobal::ToolSelectFog->SetFogSunDir(Vec3(Value, TempFogBuffer.FogSunDir.y, TempFogBuffer.FogSunDir.z));
	}

	UpdateData(FALSE);
}


void FogDlg::OnEnChangeSundiry()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.

	if (nullptr == SSDToolGlobal::ToolSelectFog)
	{
		return;
	}
	if (nullptr == SSDToolGlobal::ToolFogDlg)
	{
		return;
	}
	UpdateData(TRUE);
	CString TempValue;
	GetDlgItemText(IDC_FOGCOLORR, TempValue);

	if (nullptr != SSDToolGlobal::ToolSelectFog)
	{
		float Value = (float)_ttof(TempValue);
		CBUFFER::FogBuffer TempFogBuffer = SSDToolGlobal::ToolSelectFog->GetFogBuffer();
		SSDToolGlobal::ToolSelectFog->SetFogSunDir(Vec3(TempFogBuffer.FogSunDir.x, Value, TempFogBuffer.FogSunDir.z));
	}

	UpdateData(FALSE);
}


void FogDlg::OnEnChangeSundirz()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (nullptr == SSDToolGlobal::ToolSelectFog)
	{
		return;
	}
	if (nullptr == SSDToolGlobal::ToolFogDlg)
	{
		return;
	}
	UpdateData(TRUE);
	CString TempValue;
	GetDlgItemText(IDC_FOGCOLORR, TempValue);

	if (nullptr != SSDToolGlobal::ToolSelectFog)
	{
		float Value = (float)_ttof(TempValue);
		CBUFFER::FogBuffer TempFogBuffer = SSDToolGlobal::ToolSelectFog->GetFogBuffer();
		SSDToolGlobal::ToolSelectFog->SetFogSunDir(Vec3(TempFogBuffer.FogSunDir.x, TempFogBuffer.FogSunDir.y, Value));
	}

	UpdateData(FALSE);
}

