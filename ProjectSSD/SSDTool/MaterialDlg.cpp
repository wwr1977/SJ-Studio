// CMaterialDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "SSDTool.h"
#include "MaterialDlg.h"
#include "afxdialogex.h"
#include <FbxRenderer.h>
#include <Fbx.h>
#include <atlimage.h>
#include <GameWindow.h>
#include "NaviDlg.h"
// MaterialDlg 대화 상자

IMPLEMENT_DYNAMIC(MaterialDlg, CDialogEx)

MaterialDlg::MaterialDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MATDLG, pParent)
{

}

MaterialDlg::~MaterialDlg()
{
}

void MaterialDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_INDEX, m_IndexList);
}


BEGIN_MESSAGE_MAP(MaterialDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &MaterialDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &MaterialDlg::OnBnClickedCancel)
	ON_EN_CHANGE(IDC_PATH1, &MaterialDlg::OnEnChangePath1)
	ON_EN_CHANGE(IDC_NAME1, &MaterialDlg::OnEnChangeName1)
	ON_EN_CHANGE(IDC_PATH2, &MaterialDlg::OnEnChangePath2)
	ON_EN_CHANGE(IDC_NAME2, &MaterialDlg::OnEnChangeName2)
	ON_EN_CHANGE(IDC_PATH3, &MaterialDlg::OnEnChangePath3)
	ON_EN_CHANGE(IDC_NAME3, &MaterialDlg::OnEnChangeName3)
	ON_EN_CHANGE(IDC_PATH4, &MaterialDlg::OnEnChangePath4)
	ON_EN_CHANGE(IDC_NAME4, &MaterialDlg::OnEnChangeName4)
	ON_CBN_SELCHANGE(IDC_INDEX, &MaterialDlg::OnCbnSelchangeIndex)
	ON_BN_CLICKED(IDC_BUTTON2, &MaterialDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON4, &MaterialDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON6, &MaterialDlg::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON8, &MaterialDlg::OnBnClickedButton8)
	ON_BN_CLICKED(IDC_BUTTON1, &MaterialDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// MaterialDlg 메시지 처리기


void MaterialDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//CDialogEx::OnOK();
}


void MaterialDlg::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//CDialogEx::OnCancel();
}


BOOL MaterialDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	// 수지 컴퓨터 해상도
	//SetWindowPos(this, 0 - 2, 265, 331 - 4 * 0, 961 + 10, SWP_NOZORDER);

	// 석재 컴퓨터 해상도
	SetWindowPos(this, 0 - 2, 330, 331, 971, SWP_NOZORDER);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void MaterialDlg::SetMaterialValue(FbxData* _Data, int _Subset, bool _AllCheck)
{
	if (nullptr == _Data)
	{
		SSDToolGlobal::ToolBoneMesh = nullptr;
		SSDToolGlobal::ToolSelectFbx = nullptr;
		SSDToolGlobal::ToolSelectSubset = -1;
		return;
	}
	if (nullptr == _Data->FbxPtr)
	{
		SSDToolGlobal::ToolBoneMesh = nullptr;
		SSDToolGlobal::ToolSelectFbx = nullptr;
		SSDToolGlobal::ToolSelectSubset = -1;
		return;
	}
	if (0 > _Subset)
	{
		return;
	}
	if (true == _AllCheck)
	{
		_Subset = 0;
	}

	SSDToolGlobal::ToolSelectSubset = _Subset;

	SPTR<CBoneMesh> TempMesh = _Data->FbxPtr->GetBoneMesh(SSDToolGlobal::ToolSelectSubset);
	SSDToolGlobal::ToolFbxMesh = _Data->FbxPtr->GetSingleFbxMesh(SSDToolGlobal::ToolSelectSubset);

	size_t MatSize = TempMesh->MaterialVec.size();

	SSDToolGlobal::ToolSelectFbx = _Data->FbxPtr;
	SSDToolGlobal::ToolBoneMesh = TempMesh;

	m_IndexList.ResetContent();
	for (unsigned int i = 0; i < MatSize; ++i)
	{
		char Buf[256];
		_itoa_s(i, Buf, 10);
		std::string TempKey = std::string(Buf);
		std::wstring KeyName;
		KeyName.assign(TempKey.begin(), TempKey.end());

		m_IndexList.AddString(KeyName.c_str());
	}
	m_IndexList.UpdateData();
}


void MaterialDlg::OnEnChangePath1()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void MaterialDlg::OnEnChangeName1()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void MaterialDlg::OnEnChangePath2()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void MaterialDlg::OnEnChangeName2()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void MaterialDlg::OnEnChangePath3()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void MaterialDlg::OnEnChangeName3()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void MaterialDlg::OnEnChangePath4()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void MaterialDlg::OnEnChangeName4()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

//선택하면 업데이트해야함
void MaterialDlg::OnCbnSelchangeIndex()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (nullptr == SSDToolGlobal::ToolBoneMesh)
	{
		SSDToolGlobal::ToolSelectMeshIndex = -1;
		SSDToolGlobal::ToolFbxMesh = nullptr;
		return;
	}
	if (nullptr == SSDToolGlobal::ToolSelectFbx)
	{
		SSDToolGlobal::ToolSelectMeshIndex = -1;
		SSDToolGlobal::ToolFbxMesh = nullptr;
		return;
	}
	if (nullptr == SSDToolGlobal::ToolMatDlg)
	{
		return;
	}

	SSDToolGlobal::ToolSelectMeshIndex = m_IndexList.GetCurSel();
	SPTR<CBoneMaterial> TempBoneMat = SSDToolGlobal::ToolBoneMesh->GetMaterialValue(SSDToolGlobal::ToolSelectMeshIndex);

	std::string Path;
	std::wstring WPath;
	std::wstring WName;
	std::string FileName;

	if (true != TempBoneMat->DiffTexName.empty())
	{
		Path = TempBoneMat->DiffTexName;
		WPath.assign(Path.begin(), Path.end());
		LPCTSTR Temp = WPath.c_str();

		FileName = Path;
		size_t Last_Index = FileName.find_last_of("/");
		Last_Index += 1;
		FileName.replace(0, Last_Index, "");
		size_t First_Index = FileName.find_last_of(".");
		size_t Count = FileName.size() - First_Index;
		FileName.replace(First_Index, Count, "");
		
		WName.assign(FileName.begin(), FileName.end());

		//패스 이름 넣어주기
		GetDlgItem(IDC_PATH1)->SetWindowTextW(Temp);
		GetDlgItem(IDC_NAME1)->SetWindowTextW(WName.c_str());

		Path.clear();
		WPath.clear();
		WName.clear();
		FileName.clear();
	}
	if (true != TempBoneMat->BumpTexName.empty())
	{
		Path = TempBoneMat->DiffTexName;
		WPath.assign(Path.begin(), Path.end());
		LPCTSTR Temp = WPath.c_str();

		FileName = Path;
		size_t Last_Index = FileName.find_last_of("/");
		Last_Index += 1;
		FileName.replace(0, Last_Index, "");
		size_t First_Index = FileName.find_last_of(".");
		size_t Count = FileName.size() - First_Index;
		FileName.replace(First_Index, Count, "");

		WName.assign(FileName.begin(), FileName.end());

		//패스 이름 넣어주기
		GetDlgItem(IDC_PATH2)->SetWindowTextW(Temp);
		GetDlgItem(IDC_NAME2)->SetWindowTextW(WName.c_str());

		Path.clear();
		WPath.clear();
		WName.clear();
		FileName.clear();
	}
	if (true != TempBoneMat->SpecTexName.empty())
	{
		Path = TempBoneMat->SpecTexName;
		WPath.assign(Path.begin(), Path.end());
		LPCTSTR Temp = WPath.c_str();

		FileName = Path;
		size_t Last_Index = FileName.find_last_of("/");
		Last_Index += 1;
		FileName.replace(0, Last_Index, "");
		size_t First_Index = FileName.find_last_of(".");
		size_t Count = FileName.size() - First_Index;
		FileName.replace(First_Index, Count, "");

		WName.assign(FileName.begin(), FileName.end());

		//패스 이름 넣어주기
		GetDlgItem(IDC_PATH3)->SetWindowTextW(Temp);
		GetDlgItem(IDC_NAME3)->SetWindowTextW(WName.c_str());

		Path.clear();
		WPath.clear();
		WName.clear();
		FileName.clear();
	}
	if (true != TempBoneMat->EmivTexName.empty())
	{
		Path = TempBoneMat->EmivTexName;
		WPath.assign(Path.begin(), Path.end());
		LPCTSTR Temp = WPath.c_str();

		FileName = Path;
		size_t Last_Index = FileName.find_last_of("/");
		Last_Index += 1;
		FileName.replace(0, Last_Index, "");
		size_t First_Index = FileName.find_last_of(".");
		size_t Count = FileName.size() - First_Index;
		FileName.replace(First_Index, Count, "");

		WName.assign(FileName.begin(), FileName.end());

		//패스 이름 넣어주기
		GetDlgItem(IDC_PATH4)->SetWindowTextW(Temp);
		GetDlgItem(IDC_NAME4)->SetWindowTextW(WName.c_str());

		Path.clear();
		WPath.clear();
		WName.clear();
		FileName.clear();
	}
}


void MaterialDlg::OnBnClickedButton2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CFileDialog fDlg(TRUE, NULL, NULL, OFN_NOCHANGEDIR | OFN_NOCHANGEDIR, L"All Files(*.*)|*.*||");
	if (fDlg.DoModal() == IDOK)
	{
		CString PathName = fDlg.GetPathName();
		GetDlgItem(IDC_PATH1)->SetWindowTextW(PathName);

		std::wstring FileName = PathName.GetString();
		size_t Last_Index = FileName.find_last_of(L"\\");
		Last_Index += 1;
		FileName.replace(0, Last_Index, L"");
		size_t First_Index = FileName.find_last_of(L".");
		size_t Count = FileName.size() - First_Index;
		FileName.replace(First_Index, Count, L"");

		GetDlgItem(IDC_NAME1)->SetWindowTextW(FileName.c_str());

		if (nullptr == SSDToolGlobal::ToolSelectRenderer)
		{
			return;
		}
		if (nullptr == SSDToolGlobal::ToolBoneMesh)
		{
			return;
		}
		if (nullptr == SSDToolGlobal::ToolFbxMesh)
		{
			return;
		}

		std::wstring SaveFileName = PathName.GetString();
		size_t Save_Last_Index = SaveFileName.find_last_of(L"\\");
		std::wstring TempName = L"/";
		std::wstring ExpName = L".tga";
		SaveFileName.replace(Save_Last_Index, 1, L"/");

		SPTR<CBoneMaterial> TempBoneMat = SSDToolGlobal::ToolBoneMesh->GetMaterialValue(SSDToolGlobal::ToolSelectMeshIndex);
		TempBoneMat->SetDiffName(CW2A(SaveFileName.c_str(), CP_UTF8).m_psz);
		CFbxRenderer* TempStaticRender = (CFbxRenderer*)SSDToolGlobal::ToolSelectRenderer;
		if (nullptr == TempStaticRender)
		{
			return;
		}

		CResourceMgr<CTexture>::Load(SaveFileName);
		TempStaticRender->Clear();
		TempStaticRender->SettingFbxMesh(SSDToolGlobal::ToolSelectSubset);
	}
}

void MaterialDlg::OnBnClickedButton4()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CFileDialog fDlg(TRUE, NULL, NULL, OFN_NOCHANGEDIR | OFN_NOCHANGEDIR, L"All Files(*.*)|*.*||");
	if (fDlg.DoModal() == IDOK)
	{
		CString PathName = fDlg.GetPathName();
		GetDlgItem(IDC_PATH2)->SetWindowTextW(PathName);

		std::wstring FileName = PathName.GetString();
		size_t Last_Index = FileName.find_last_of(L"\\");
		Last_Index += 1;
		FileName.replace(0, Last_Index, L"");
		size_t First_Index = FileName.find_last_of(L".");
		size_t Count = FileName.size() - First_Index;
		FileName.replace(First_Index, Count, L"");

		GetDlgItem(IDC_NAME2)->SetWindowTextW(FileName.c_str());

		if (nullptr == SSDToolGlobal::ToolSelectRenderer)
		{
			return;
		}
		if (nullptr == SSDToolGlobal::ToolBoneMesh)
		{
			return;
		}
		if (nullptr == SSDToolGlobal::ToolFbxMesh)
		{
			return;
		}
  
		std::wstring SaveFileName = PathName.GetString();
		size_t Save_Last_Index = SaveFileName.find_last_of(L"\\");
		std::wstring TempName = L"/";
		std::wstring ExpName = L".tga";
		SaveFileName.replace(Save_Last_Index, 1, L"/");

 		SPTR<CBoneMaterial> TempBoneMat = SSDToolGlobal::ToolBoneMesh->GetMaterialValue(SSDToolGlobal::ToolSelectMeshIndex);
		TempBoneMat->SetBumpName(CW2A(SaveFileName.c_str(), CP_UTF8).m_psz);
		CFbxRenderer* TempStaticRender = (CFbxRenderer*)SSDToolGlobal::ToolSelectRenderer;
		if (nullptr == TempStaticRender)
		{
			return;
		}

		CResourceMgr<CTexture>::Load(SaveFileName);
		TempStaticRender->Clear();
		TempStaticRender->SettingFbxMesh(SSDToolGlobal::ToolSelectSubset);
	}
}

void MaterialDlg::OnBnClickedButton6()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void MaterialDlg::OnBnClickedButton8()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void MaterialDlg::OnBnClickedButton1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (nullptr == SSDToolGlobal::ToolSelectRenderer)
	{
		return;
	}
	CFbxRenderer* TempStaticRender = (CFbxRenderer*)SSDToolGlobal::ToolSelectRenderer;
	TempStaticRender->GetRenderingFbx()->BinarySave(SSDToolGlobal::ToolSelectFbx->GetPath().c_str());
}
