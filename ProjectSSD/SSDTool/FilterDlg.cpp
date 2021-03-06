// FilterDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "SSDTool.h"
#include "FilterDlg.h"
#include "afxdialogex.h"
#include "SSDToolGlobal.h"

#include <BloomFilter.h>
#include <GaussianBlur.h>

// FilterDlg 대화 상자

IMPLEMENT_DYNAMIC(FilterDlg, CDialogEx)

FilterDlg::FilterDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_FILTER, pParent)
	, m_GauOnValue(FALSE)
	, m_BloomOnValue(FALSE)
{

}

FilterDlg::~FilterDlg()
{
}

void FilterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUFFERSIZE, BufferSizeList);
	DDX_Control(pDX, IDC_RANGESLIDER, m_RangeSlider);
	DDX_Control(pDX, IDC_ON, m_GauOn);
	DDX_Radio(pDX, IDC_ON, m_GauOnValue);
	DDX_Control(pDX, IDC_ON2, m_BloomOn);
	DDX_Radio(pDX, IDC_ON2, m_BloomOnValue);
}


BEGIN_MESSAGE_MAP(FilterDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &FilterDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &FilterDlg::OnBnClickedCancel)
	ON_EN_CHANGE(IDC_SIGMA, &FilterDlg::OnEnChangeSigma)
	ON_EN_CHANGE(IDC_RANGE, &FilterDlg::OnEnChangeRange)
	ON_EN_CHANGE(IDC_AMOUNT, &FilterDlg::OnEnChangeAmount)
	ON_EN_CHANGE(IDC_MULTIPLE, &FilterDlg::OnEnChangeMultiple)
	ON_CBN_SELCHANGE(IDC_BUFFERSIZE, &FilterDlg::OnCbnSelchangeCombo1)
	ON_EN_CHANGE(IDC_LUX, &FilterDlg::OnEnChangeLux)
	ON_EN_CHANGE(IDC_LUY, &FilterDlg::OnEnChangeLuy)
	ON_EN_CHANGE(IDC_LUZ, &FilterDlg::OnEnChangeLuz)
	ON_EN_CHANGE(IDC_SIGMA2, &FilterDlg::OnEnChangeSigma2)
	ON_EN_CHANGE(IDC_RANGE2, &FilterDlg::OnEnChangeRange2)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_RANGESLIDER, &FilterDlg::OnNMCustomdrawRangeslider)
	ON_BN_CLICKED(IDC_ON, &FilterDlg::OnBnClickedOn)
	ON_BN_CLICKED(IDC_ON2, &FilterDlg::OnBnClickedOn2)
	ON_BN_CLICKED(IDC_OFF2, &FilterDlg::OnBnClickedOff2)
	ON_BN_CLICKED(IDC_OFF, &FilterDlg::OnBnClickedOff)
END_MESSAGE_MAP()


// FilterDlg 메시지 처리기


void FilterDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//CDialogEx::OnOK();
}


void FilterDlg::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//CDialogEx::OnCancel();
}

std::wstring FilterDlg::GetWinChar(float _Value)
{
	char Temp[20];
	sprintf_s(Temp, "%f", _Value);
	std::string TempKey = std::string(Temp);
	std::wstring KeyName;
	KeyName.assign(TempKey.begin(), TempKey.end());
	return KeyName;
}


void FilterDlg::SetCurBloomInfo()
{
	if (nullptr == SSDToolGlobal::ToolSelectActor)
	{
		return;
	}
	if (nullptr == SSDToolGlobal::ToolSelectBloom)
	{
		return;
	}
	if (nullptr == SSDToolGlobal::ToolFilterDlg)
	{
		return;
	}

	int Type = (int)SSDToolGlobal::ToolSelectBloom->GetBloomMiniBufferType();
	float Amount = SSDToolGlobal::ToolSelectBloom->GetMiniBufferAmount();
	Vec3 Luminance = SSDToolGlobal::ToolSelectBloom->GetMiniBufferLuminance();
	float Multiple = SSDToolGlobal::ToolSelectBloom->GetMiniBufferMultiple();
	float Sigma = SSDToolGlobal::ToolSelectBloom->GetSigma();
	int Range = SSDToolGlobal::ToolSelectBloom->GetRange();

	BufferSizeList.SetCurSel(Type);

	GetDlgItem(IDC_AMOUNT)->SetWindowTextW(GetWinChar(Amount).c_str());
	GetDlgItem(IDC_MULTIPLE)->SetWindowTextW(GetWinChar(Multiple).c_str());
	GetDlgItem(IDC_SIGMA2)->SetWindowTextW(GetWinChar(Sigma).c_str());
	GetDlgItem(IDC_RANGE2)->SetWindowTextW(GetWinChar((float)Range).c_str());

	GetDlgItem(IDC_LUX)->SetWindowTextW(GetWinChar(Luminance.x).c_str());
	GetDlgItem(IDC_LUY)->SetWindowTextW(GetWinChar(Luminance.y).c_str());
	GetDlgItem(IDC_LUZ)->SetWindowTextW(GetWinChar(Luminance.z).c_str());

	if (true == SSDToolGlobal::ToolSelectBloom->IsFilterOn())
	{
		m_BloomOnValue = true;
	}
	else
	{
		m_BloomOnValue = false;
	}
}

void FilterDlg::SetNewBloomInfo()
{
	if (nullptr == SSDToolGlobal::ToolSelectActor)
	{
		return;
	}
	if (nullptr == SSDToolGlobal::ToolSelectBloom)
	{
		return;
	}
	if (nullptr == SSDToolGlobal::ToolFilterDlg)
	{
		return;
	}

	float Amount = 0.5f;
	Vec3 Luminance = Vec3(0.2125f, 0.7154f, 0.0721f);
	float Multiple = 2.f;

	BufferSizeList.SetCurSel(0);
	GetDlgItem(IDC_AMOUNT)->SetWindowTextW(L"0.5f");
	GetDlgItem(IDC_MULTIPLE)->SetWindowTextW(L"2.f");

	GetDlgItem(IDC_LUX)->SetWindowTextW(L"0.2125f");
	GetDlgItem(IDC_LUY)->SetWindowTextW(L"0.7154f");
	GetDlgItem(IDC_LUZ)->SetWindowTextW(L"0.0721f");
}

void FilterDlg::SetCurGaussianInfo()
{
	if (nullptr == SSDToolGlobal::ToolSelectActor)
	{
		return;
	}
	if (nullptr == SSDToolGlobal::ToolSelectGaussian)
	{
		return;
	}
	if (nullptr == SSDToolGlobal::ToolFilterDlg)
	{
		return;
	}
	float Sigma = SSDToolGlobal::ToolSelectGaussian->GetSigma();
	int Range = SSDToolGlobal::ToolSelectGaussian->GetRange();

	GetDlgItem(IDC_SIGMA)->SetWindowTextW(GetWinChar(Sigma).c_str());
	GetDlgItem(IDC_RANGE)->SetWindowTextW(GetWinChar((float)Range).c_str());
	m_RangeSlider.SetPos(Range);

	if (true == SSDToolGlobal::ToolSelectGaussian->IsFilterOn())
	{
		m_GauOnValue = true;
	}
	else
	{
		m_GauOnValue = false;
	}
}

void FilterDlg::SetNewGaussianInfo()
{
	if (nullptr == SSDToolGlobal::ToolSelectActor)
	{
		return;
	}
	if (nullptr == SSDToolGlobal::ToolSelectGaussian)
	{
		return;
	}
	if (nullptr == SSDToolGlobal::ToolFilterDlg)
	{
		return;
	}
}

void FilterDlg::OnEnChangeSigma()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.
	if (nullptr == SSDToolGlobal::ToolSelectActor)
	{
		return;
	}
	if (nullptr == SSDToolGlobal::ToolFilterDlg)
	{
		return;
	}
	UpdateData(TRUE);
	CString TempValue;
	GetDlgItemText(IDC_SIGMA, TempValue);

	if (nullptr != SSDToolGlobal::ToolSelectGaussian)
	{
		SSDToolGlobal::ToolSelectGaussian->SetSigma((float)_ttof(TempValue));
	}
	UpdateData(FALSE);
	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void FilterDlg::OnEnChangeRange()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.
	if (nullptr == SSDToolGlobal::ToolSelectActor)
	{
		return;
	}
	if (nullptr == SSDToolGlobal::ToolFilterDlg)
	{
		return;
	}
	UpdateData(TRUE);
	CString TempValue;
	GetDlgItemText(IDC_RANGE, TempValue);

	if (nullptr != SSDToolGlobal::ToolSelectGaussian)
	{
		SSDToolGlobal::ToolSelectGaussian->SetRange(_ttoi(TempValue));
	}
	UpdateData(FALSE);
	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void FilterDlg::OnEnChangeAmount()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.
	if (nullptr == SSDToolGlobal::ToolSelectActor)
	{
		return;
	}
	if (nullptr == SSDToolGlobal::ToolFilterDlg)
	{
		return;
	}
	UpdateData(TRUE);
	CString TempValue;
	GetDlgItemText(IDC_AMOUNT, TempValue);

	if (nullptr != SSDToolGlobal::ToolSelectBloom)
	{
		SSDToolGlobal::ToolSelectBloom->SetMiniBufferAmount((float)_ttof(TempValue));
	}
	UpdateData(FALSE);

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void FilterDlg::OnEnChangeMultiple()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.
	if (nullptr == SSDToolGlobal::ToolSelectActor)
	{
		return;
	}
	if (nullptr == SSDToolGlobal::ToolFilterDlg)
	{
		return;
	}
	UpdateData(TRUE);
	CString TempValue;
	GetDlgItemText(IDC_AMOUNT, TempValue);

	if (nullptr != SSDToolGlobal::ToolSelectBloom)
	{
		SSDToolGlobal::ToolSelectBloom->SetMiniBufferMultiple((float)_ttof(TempValue));
	}
	UpdateData(FALSE);
	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void FilterDlg::OnCbnSelchangeCombo1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (nullptr == SSDToolGlobal::ToolFilterDlg)
	{
		return;
	}
	int FilterType = BufferSizeList.GetCurSel();

	if (nullptr != SSDToolGlobal::ToolSelectBloom)
	{
		switch (FilterType)
		{
		case MINI_64:
			SSDToolGlobal::ToolSelectBloom->SetMiniBufferType(MINI_64);
			break;
		case MINI_128:
			SSDToolGlobal::ToolSelectBloom->SetMiniBufferType(MINI_128);
			break;
		case MINI_256:
			SSDToolGlobal::ToolSelectBloom->SetMiniBufferType(MINI_256);
			break;
		case MINI_512:
			SSDToolGlobal::ToolSelectBloom->SetMiniBufferType(MINI_512);
			break;
		default:
			break;
		}
	}

}

BOOL FilterDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	// 수지 컴퓨터 해상도
	//SetWindowPos(this, 0 - 2, 265, 331 - 4 * 0, 961 + 10, SWP_NOZORDER);

	// 석재 컴퓨터 해상도
	SetWindowPos(this, 0 - 2, 330, 331, 971, SWP_NOZORDER);

	BufferSizeList.ResetContent();
	BufferSizeList.AddString(L"MINI_64");
	BufferSizeList.AddString(L"MINI_128");
	BufferSizeList.AddString(L"MINI_256");
	BufferSizeList.AddString(L"MINI_512");
	BufferSizeList.UpdateData();

	m_RangeSlider.SetRange(1, 16);
	m_RangeSlider.SetTicFreq(2);
	m_RangeSlider.SetPageSize(2);

	((CEdit*)GetDlgItem(IDC_SIGMA))->SetLimitText(6);
	((CEdit*)GetDlgItem(IDC_RANGE))->SetLimitText(6);
	((CEdit*)GetDlgItem(IDC_AMOUNT))->SetLimitText(6);
	((CEdit*)GetDlgItem(IDC_MULTIPLE))->SetLimitText(6);
	((CEdit*)GetDlgItem(IDC_LUX))->SetLimitText(6);
	((CEdit*)GetDlgItem(IDC_LUY))->SetLimitText(6);
	((CEdit*)GetDlgItem(IDC_LUZ))->SetLimitText(6);
	((CEdit*)GetDlgItem(IDC_SIGMA2))->SetLimitText(6);
	((CEdit*)GetDlgItem(IDC_RANGE2))->SetLimitText(6);


	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void FilterDlg::OnEnChangeLux()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	if (nullptr == SSDToolGlobal::ToolSelectActor)
	{
		return;
	}
	if (nullptr == SSDToolGlobal::ToolFilterDlg)
	{
		return;
	}
	UpdateData(TRUE);
	CString TempValue;
	GetDlgItemText(IDC_LUX, TempValue);

	if (nullptr != SSDToolGlobal::ToolSelectBloom)
	{
		Vec3 Lumi = SSDToolGlobal::ToolSelectBloom->GetMiniBufferLuminance();
		SSDToolGlobal::ToolSelectBloom->SetMiniBufferLuminance(Vec3((float)_ttof(TempValue), Lumi.y, Lumi.z));
	}
	UpdateData(FALSE);
	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void FilterDlg::OnEnChangeLuy()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.
	if (nullptr == SSDToolGlobal::ToolSelectActor)
	{
		return;
	}
	if (nullptr == SSDToolGlobal::ToolFilterDlg)
	{
		return;
	}
	UpdateData(TRUE);
	CString TempValue;
	GetDlgItemText(IDC_LUY, TempValue);

	if (nullptr != SSDToolGlobal::ToolSelectBloom)
	{
		Vec3 Lumi = SSDToolGlobal::ToolSelectBloom->GetMiniBufferLuminance();
		SSDToolGlobal::ToolSelectBloom->SetMiniBufferLuminance(Vec3(Lumi.x, (float)_ttof(TempValue), Lumi.z));
	}
	UpdateData(FALSE);
	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void FilterDlg::OnEnChangeLuz()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.
	if (nullptr == SSDToolGlobal::ToolSelectActor)
	{
		return;
	}
	if (nullptr == SSDToolGlobal::ToolFilterDlg)
	{
		return;
	}
	UpdateData(TRUE);
	CString TempValue;
	GetDlgItemText(IDC_LUZ, TempValue);

	if (nullptr != SSDToolGlobal::ToolSelectBloom)
	{
		Vec3 Lumi = SSDToolGlobal::ToolSelectBloom->GetMiniBufferLuminance();
		SSDToolGlobal::ToolSelectBloom->SetMiniBufferLuminance(Vec3(Lumi.x, Lumi.y, (float)_ttof(TempValue)));
	}
	UpdateData(FALSE);
	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void FilterDlg::OnEnChangeSigma2()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.
	if (nullptr == SSDToolGlobal::ToolSelectActor)
	{
		return;
	}
	if (nullptr == SSDToolGlobal::ToolFilterDlg)
	{
		return;
	}
	UpdateData(TRUE);
	CString TempValue;
	GetDlgItemText(IDC_SIGMA2, TempValue);

	if (nullptr != SSDToolGlobal::ToolSelectBloom)
	{
		SSDToolGlobal::ToolSelectBloom->SetSigma((float)_ttof(TempValue));
	}
	UpdateData(FALSE);
	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void FilterDlg::OnEnChangeRange2()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.
	if (nullptr == SSDToolGlobal::ToolSelectActor)
	{
		return;
	}
	if (nullptr == SSDToolGlobal::ToolFilterDlg)
	{
		return;
	}
	UpdateData(TRUE);
	CString TempValue;
	GetDlgItemText(IDC_RANGE2, TempValue);

	if (nullptr != SSDToolGlobal::ToolSelectBloom)
	{
		SSDToolGlobal::ToolSelectBloom->SetRange(_ttoi(TempValue));
	}
	UpdateData(FALSE);
	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


BOOL FilterDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	CEdit *webmacrotext = (CEdit *)GetDlgItem(IDC_SIGMA);
	if (pMsg->message == WM_LBUTTONDOWN && pMsg->hwnd == webmacrotext->m_hWnd &&
		GetFocus()->m_hWnd != webmacrotext->m_hWnd)
	{
		webmacrotext->SetFocus();
		webmacrotext->SetSel(0, -1, true);
		return true;
	}

	webmacrotext = (CEdit *)GetDlgItem(IDC_RANGE);
	if (pMsg->message == WM_LBUTTONDOWN && pMsg->hwnd == webmacrotext->m_hWnd &&
		GetFocus()->m_hWnd != webmacrotext->m_hWnd)
	{
		webmacrotext->SetFocus();
		webmacrotext->SetSel(0, -1, true);
		return true;
	}

	webmacrotext = (CEdit *)GetDlgItem(IDC_AMOUNT);
	if (pMsg->message == WM_LBUTTONDOWN && pMsg->hwnd == webmacrotext->m_hWnd &&
		GetFocus()->m_hWnd != webmacrotext->m_hWnd)
	{
		webmacrotext->SetFocus();
		webmacrotext->SetSel(0, -1, true);
		return true;
	}

	webmacrotext = (CEdit *)GetDlgItem(IDC_MULTIPLE);
	if (pMsg->message == WM_LBUTTONDOWN && pMsg->hwnd == webmacrotext->m_hWnd &&
		GetFocus()->m_hWnd != webmacrotext->m_hWnd)
	{
		webmacrotext->SetFocus();
		webmacrotext->SetSel(0, -1, true);
		return true;
	}

	webmacrotext = (CEdit *)GetDlgItem(IDC_LUX);
	if (pMsg->message == WM_LBUTTONDOWN && pMsg->hwnd == webmacrotext->m_hWnd &&
		GetFocus()->m_hWnd != webmacrotext->m_hWnd)
	{
		webmacrotext->SetFocus();
		webmacrotext->SetSel(0, -1, true);
		return true;
	}

	webmacrotext = (CEdit *)GetDlgItem(IDC_LUY);
	if (pMsg->message == WM_LBUTTONDOWN && pMsg->hwnd == webmacrotext->m_hWnd &&
		GetFocus()->m_hWnd != webmacrotext->m_hWnd)
	{
		webmacrotext->SetFocus();
		webmacrotext->SetSel(0, -1, true);
		return true;
	}

	webmacrotext = (CEdit *)GetDlgItem(IDC_LUZ);
	if (pMsg->message == WM_LBUTTONDOWN && pMsg->hwnd == webmacrotext->m_hWnd &&
		GetFocus()->m_hWnd != webmacrotext->m_hWnd)
	{
		webmacrotext->SetFocus();
		webmacrotext->SetSel(0, -1, true);
		return true;
	}

	webmacrotext = (CEdit *)GetDlgItem(IDC_SIGMA2);
	if (pMsg->message == WM_LBUTTONDOWN && pMsg->hwnd == webmacrotext->m_hWnd &&
		GetFocus()->m_hWnd != webmacrotext->m_hWnd)
	{
		webmacrotext->SetFocus();
		webmacrotext->SetSel(0, -1, true);
		return true;
	}

	webmacrotext = (CEdit *)GetDlgItem(IDC_RANGE2);
	if (pMsg->message == WM_LBUTTONDOWN && pMsg->hwnd == webmacrotext->m_hWnd &&
		GetFocus()->m_hWnd != webmacrotext->m_hWnd)
	{
		webmacrotext->SetFocus();
		webmacrotext->SetSel(0, -1, true);
		return true;
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}

void FilterDlg::OnNMCustomdrawRangeslider(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
	int nPos;
	CString strPos;
	nPos = m_RangeSlider.GetPos();

	strPos.Format(_T("%d"), nPos);
	GetDlgItem(IDC_RANGE)->SetWindowTextW(strPos);
}

void FilterDlg::OnBnClickedOn()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (nullptr == SSDToolGlobal::ToolSelectActor)
	{
		return;
	}
	if (nullptr == SSDToolGlobal::ToolFilterDlg)
	{
		return;
	}
	if (nullptr == SSDToolGlobal::ToolSelectGaussian)
	{
		return;
	}
	SSDToolGlobal::ToolSelectGaussian->FilterOn();
}

void FilterDlg::OnBnClickedOff()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (nullptr == SSDToolGlobal::ToolSelectActor)
	{
		return;
	}
	if (nullptr == SSDToolGlobal::ToolFilterDlg)
	{
		return;
	}
	if (nullptr == SSDToolGlobal::ToolSelectGaussian)
	{
		return;
	}
	SSDToolGlobal::ToolSelectGaussian->FilterOff();
}

void FilterDlg::OnBnClickedOn2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (nullptr == SSDToolGlobal::ToolSelectActor)
	{
		return;
	}
	if (nullptr == SSDToolGlobal::ToolFilterDlg)
	{
		return;
	}
	if (nullptr == SSDToolGlobal::ToolSelectBloom)
	{
		return;
	}
	SSDToolGlobal::ToolSelectBloom->FilterOn();
}

void FilterDlg::OnBnClickedOff2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	if (nullptr == SSDToolGlobal::ToolSelectActor)
	{
		return;
	}
	if (nullptr == SSDToolGlobal::ToolFilterDlg)
	{
		return;
	}
	if (nullptr == SSDToolGlobal::ToolSelectBloom)
	{
		return;
	}
	SSDToolGlobal::ToolSelectBloom->FilterOff();
}

