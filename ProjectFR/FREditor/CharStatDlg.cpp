// CharStatDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "FREditor.h"
#include "CharStatDlg.h"
#include "afxdialogex.h"
#include"ParameterRen.h"
#include"DataRenderer.h"


// CCharStatDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CCharStatDlg, CDialogEx)

CCharStatDlg::CCharStatDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_CHARSTAT, pParent)
	, m_iPrevIndex(-1), m_MainStat(STRENGTH), m_bInit(false)
	, m_Str(_T(""))
	, m_Dex(_T(""))
	, m_Int(_T(""))
	, m_Luk(_T(""))
	, m_Damage(_T(""))
	, m_Def(_T(""))
	, m_Hp(_T(""))
	, m_Mp(_T(""))
	, m_iCritical(0)
	, m_iCriDamage(0)
	, m_iDamageRange(0)
	, m_iAvd(0)
	, m_fCritical(0.0f)
	, m_fCriDamage(0.0f)
	, m_fAvd(0.0f)
	, m_fStr(0.0f)
	, m_fDex(0.0f)
	, m_fInt(0.0f)
	, m_fLuk(0.0f)
	, m_fDamage(0.0f)
	, m_fDef(0.0f)
	, m_fHp(0.0f)
	, m_fMp(0.0f)
	, m_iLevel(0)
{

}

CCharStatDlg::~CCharStatDlg()
{
}

void CCharStatDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATCHARLIST, m_CharacterListCtrl);
	DDX_Control(pDX, IDC_MAINSTAT, m_ComboCtrl);
	DDX_Control(pDX, IDC_STR, m_StrCtrl);
	DDX_Text(pDX, IDC_STR, m_Str);
	DDV_MaxChars(pDX, m_Str, 3);
	DDX_Control(pDX, IDC_DEX, m_DexCtrl);
	DDX_Text(pDX, IDC_DEX, m_Dex);
	DDV_MaxChars(pDX, m_Dex, 3);
	DDX_Control(pDX, IDC_INT, m_IntCtrl);
	DDX_Text(pDX, IDC_INT, m_Int);
	DDV_MaxChars(pDX, m_Int, 3);
	DDX_Control(pDX, IDC_LUK, m_LukCtrl);
	DDX_Text(pDX, IDC_LUK, m_Luk);
	DDV_MaxChars(pDX, m_Luk, 3);
	DDX_Control(pDX, IDC_DEMAGE, m_DamageCtrl);
	DDX_Text(pDX, IDC_DEMAGE, m_Damage);
	DDV_MaxChars(pDX, m_Damage, 3);
	DDX_Control(pDX, IDC_DEF, m_DefCtrl);
	DDX_Text(pDX, IDC_DEF, m_Def);
	DDV_MaxChars(pDX, m_Def, 3);
	DDX_Control(pDX, IDC_HP, m_HpCtrl);
	DDX_Text(pDX, IDC_HP, m_Hp);
	DDV_MaxChars(pDX, m_Hp, 3);
	DDX_Control(pDX, IDC_MP, m_MpCtrl);
	DDX_Text(pDX, IDC_MP, m_Mp);
	DDV_MaxChars(pDX, m_Mp, 3);
	DDX_Control(pDX, IDC_CRITICAL, m_CriticalCtrl);
	DDX_Text(pDX, IDC_CRITICAL, m_iCritical);
	DDX_Control(pDX, IDC_CRITICALDEMAGE, m_CriDamageCtrl);
	DDX_Text(pDX, IDC_CRITICALDEMAGE, m_iCriDamage);
	DDX_Control(pDX, IDC_DEMAGERANGE, m_DamageRangeCtrl);
	DDX_Text(pDX, IDC_DEMAGERANGE, m_iDamageRange);
	DDX_Control(pDX, IDC_AVOIDABILITY, m_AvdCtrl);
	DDX_Text(pDX, IDC_AVOIDABILITY, m_iAvd);
	DDV_MinMaxInt(pDX, m_iCritical, 0, 100);
	DDX_Control(pDX, IDC_LEVELSLIDER, m_LevelCtrl);
	DDX_Slider(pDX, IDC_LEVELSLIDER, m_iLevel);
}


BEGIN_MESSAGE_MAP(CCharStatDlg, CDialogEx)
	ON_LBN_SELCHANGE(IDC_STATCHARLIST, &CCharStatDlg::OnLbnSelchangeStatcharlist)
	ON_WM_SHOWWINDOW()
	ON_CBN_SELCHANGE(IDC_MAINSTAT, &CCharStatDlg::OnCbnSelchangeMainstat)
	ON_EN_KILLFOCUS(IDC_STR, &CCharStatDlg::OnEnKillfocusStr)
	ON_EN_KILLFOCUS(IDC_DEX, &CCharStatDlg::OnEnKillfocusDex)
	ON_EN_KILLFOCUS(IDC_INT, &CCharStatDlg::OnEnKillfocusInt)
	ON_EN_KILLFOCUS(IDC_LUK, &CCharStatDlg::OnEnKillfocusLuk)
	ON_EN_KILLFOCUS(IDC_DEMAGE, &CCharStatDlg::OnEnKillfocusDemage)
	ON_EN_KILLFOCUS(IDC_DEF, &CCharStatDlg::OnEnKillfocusDef)
	ON_EN_KILLFOCUS(IDC_HP, &CCharStatDlg::OnEnKillfocusHp)
	ON_EN_KILLFOCUS(IDC_MP, &CCharStatDlg::OnEnKillfocusMp)
	ON_WM_HSCROLL()
	ON_EN_KILLFOCUS(IDC_CRITICAL, &CCharStatDlg::OnEnKillfocusCritical)
	ON_EN_KILLFOCUS(IDC_CRITICALDEMAGE, &CCharStatDlg::OnEnKillfocusCriticaldemage)
	ON_EN_KILLFOCUS(IDC_DEMAGERANGE, &CCharStatDlg::OnEnKillfocusDemagerange)
	ON_EN_KILLFOCUS(IDC_AVOIDABILITY, &CCharStatDlg::OnEnKillfocusAvoidability)
END_MESSAGE_MAP()


// CCharStatDlg 메시지 처리기입니다.

void CCharStatDlg::SettingUnitName()
{
	for (size_t i = 0; i < CEditGlobal::CharacterName.size(); i++)
	{
		m_CharacterListCtrl.AddString(CEditGlobal::CharacterName[i]);
	}
	m_bInit = true;
}
void CCharStatDlg::InitStatData(const STATPARAMETERDATA& _InitData)
{
	UpdateData(TRUE);
	
	if (MAXMAINSTAT  == _InitData.MainStat)
	{
		m_Str = _T("1.0");
		m_fStr = 1.0f;
		m_Dex = _T("1.0");
		m_fDex = 1.0f;
		m_Int = _T("1.0");
		m_fInt = 1.0f;
		m_Luk = _T("1.0");
		m_fLuk = 1.0f;
		m_Damage = _T("1.0");
		m_fDamage = 1.0f;
		m_Def = _T("1.0");
		m_fDef = 1.0f;
		m_Hp = _T("1.0");
		m_fHp = 1.0f;
		m_Mp = _T("1.0");
		m_fMp = 1.0f;


		m_iCritical = 0;
		m_iCriDamage = 0;
		m_iDamageRange = 0;
		m_iAvd = 0;

		if (true == m_bInit) 
		{
			CEditGlobal::SParameter->InitParameterValue();
		}

	}
	else 
	{
		m_MainStat = _InitData.MainStat;
		m_ComboCtrl.SetCurSel(m_MainStat);
		CEditGlobal::SDataRenderer->SetCurMainStat(m_MainStat);
		m_fStr = _InitData.StrPara;
		m_Str.Format(_T("%.1f"), m_fStr);
		m_fDex = _InitData.DexPara;
		m_Dex.Format(_T("%.1f"), m_fDex);
		m_fInt = _InitData.IntPara;
		m_Int.Format(_T("%.1f"), m_fInt);
		m_fLuk = _InitData.LuckPara;
		m_Luk.Format(_T("%.1f"), m_fLuk);
		m_fDamage = _InitData.DamagePara;
		m_Damage.Format(_T("%.1f"), m_fDamage);
		m_fDef = _InitData.DefPara;
		m_Def.Format(_T("%.1f"), m_fDef);
		m_fHp = _InitData.HpPara;;
		m_Hp.Format(_T("%.1f"), m_fHp);
		m_fMp = _InitData.MpPara;
		m_Mp.Format(_T("%.1f"), m_fMp);



		m_iCritical = _InitData.Critical;
		m_iCriDamage = _InitData.CriticalDamage;
		m_iDamageRange = _InitData.DamageRange;
		m_iAvd = _InitData.Avoidability;

		if (true == m_bInit)
		{
			CEditGlobal::SParameter->InitParameterValue(_InitData);
		}
	}

	UpdateData(FALSE);
}
void CCharStatDlg::CheckParameterData(CString& _BaseData, float& _ApplyData)
{
	float fValue = CEditGlobal::ChangeFloatValue(_BaseData);

	UpdateData(TRUE);

	if (fValue < 1.0f) 
	{
		_BaseData = _T("1.0");
		_ApplyData = 1.0f;
		UpdateData(FALSE);
		return;
	}
	
	if (fValue > 5.0f) 
	{
		_BaseData = _T("5.0");
		_ApplyData = 5.0f;
		UpdateData(FALSE);
		return;
	}

	// 소수점 첫째 자리수의 반올림하는 과정 
	int Check = (int)(fValue *10.0f) % 10;

	if (Check > 5) 
	{
		_ApplyData = ceilf(fValue);
		_BaseData.Format(_T("%.1f"), _ApplyData);
	}
	else if(Check < 5)
	{
		_ApplyData = floorf(fValue);
		_BaseData.Format(_T("%.1f"), _ApplyData);
	}
	else 
	{
		_ApplyData = fValue;
		_BaseData.Format(_T("%.1f"), _ApplyData);
	}
	UpdateData(FALSE);

}
void CCharStatDlg::ChangeCharacterEvent()
{
	int Index = m_CharacterListCtrl.GetCurSel();
	CString Character;
	m_CharacterListCtrl.GetText(Index, Character);

	map<tstring, vector<STAT>>::iterator  Find = CEditGlobal::CharacterStatTable.find(Character.GetString());

	if (Find == CEditGlobal::CharacterStatTable.end()) 
	{
		// 초기 스탯 셋팅 가져오기
		map<tstring, STAT>::iterator Find = CEditGlobal::CharacterBaseStat.find(Character.GetString());
		if (Find == CEditGlobal::CharacterBaseStat.end()) 
		{
			assert(nullptr);
			return;
		}
		
		//애디터 박스 초기화
		InitStatData();

		SettingInitData(Find->second);
	}
	else 
	{
		//Editor Box에 ParameterValue로 초기화
		CString UnitName;
		int Index = m_CharacterListCtrl.GetCurSel();
		
		m_CharacterListCtrl.GetText(Index, UnitName);

		map<tstring, STATPARAMETERDATA>::iterator FindPara = CEditGlobal::CharacterParameter.find(UnitName.GetString());
		if (FindPara == CEditGlobal::CharacterParameter.end()) 
		{
			InitStatData();
		}
		STATPARAMETERDATA  LoadData = FindPara->second;
		InitStatData(LoadData);
		//스탯 테이블 가져오기
		SettingTableData(&Find->second);
	}

	

}
// 테이블에 해당 캐릭터의 스탯테이블이 존재하지 않을경우 Level 1 의 정보 반영
void CCharStatDlg::SettingInitData(const STAT& _BaseStat)
{
	CEditGlobal::SDataRenderer->SettingInitData(_BaseStat);
	CEditGlobal::SDataRenderer->UpdateStat();
}
// 테이블에 해당 캐릭터의 스탯테이블이 존재하는 경우 테이블정보 반영
void CCharStatDlg::SettingTableData(vector<STAT>* _Table)
{
	CEditGlobal::SDataRenderer->SettingTableData(_Table);
	CEditGlobal::SDataRenderer->UpdateStat();
}
void CCharStatDlg::CreateButtonEvent() 
{
	CString SelectChar;
	int Index = m_CharacterListCtrl.GetCurSel();
	m_CharacterListCtrl.GetText(Index, SelectChar);

	

	vector<STAT> TempTable = vector<STAT>();
	TempTable.reserve(100);

	STATPARAMETERDATA ParameterData;
	UpdateData(TRUE);

	ParameterData = STATPARAMETERDATA(SelectChar.GetString()
		, m_MainStat, m_fStr, m_fDex, m_fInt, m_fLuk
		, m_fDamage, m_fDef, m_fHp, m_fMp
	,m_iCritical,m_iCriDamage,m_iAvd,m_iDamageRange);
	
	map<tstring, STATPARAMETERDATA>::iterator FPara = CEditGlobal::CharacterParameter.find(ParameterData.UnitName);
	if (CEditGlobal::CharacterParameter.end() == FPara) 
	{
		CEditGlobal::CharacterParameter.insert(map<tstring, STATPARAMETERDATA>::value_type(ParameterData.UnitName, ParameterData));
	}
	else 
	{
		FPara->second = ParameterData;
	}


	for (int K = 1; K <= MAXLEVEL; ++K)
	{
		STATPARAMETERDATA	SavePara;
		STAT Base = CEditGlobal::CharacterBaseStat.find(SelectChar.GetString())->second;

		int Str, Dex, Int, Luck, HP, MP, Min, Max, Def,Damage;
		int MainStat = 0;

		Str = Base.Str + (int)((K - 1)* m_fStr);
		Dex = Base.Dex + (int)((K - 1)* m_fDex);
		Int = Base.Int + (int)((K - 1)* m_fInt);
		Luck = Base.Luck + (int)((K - 1)* m_fLuk);

	/*	switch (SavePara.MainStat)
		{
		case STRENGTH: MainStat = Str; break;
		case DEXERITY:MainStat = Dex; break;
		case INTELIGENCE:MainStat = Int; break;
		case LUCK:MainStat = Luck; break;
		}*/

		HP = Base.Heath + (int)((K - 1)* m_fHp * 2.0f);
		MP = Base.Mana;
		Def = Base.Defense + (int)((K - 1) * m_fDef * 0.5f);
		Damage = Base.Damage + (int)((K - 1) * m_fDamage*0.5f);
		float HalfRange = m_iDamageRange*0.01f;
		Min = (int)(Damage * (1.0f - HalfRange));
		Max = (int)(Damage * (1.0f + HalfRange));

		STAT SaveStat = STAT(Str,Dex,Int,Luck
		,Damage,Def,HP,MP);

		SaveStat.MinDamage = Min;
		SaveStat.MaxDamage = Max;
		SaveStat.Critical = m_iCritical;
		SaveStat.CriticalDamage = m_iCriDamage;
		SaveStat.Avoidability = m_iAvd;
	
		
		TempTable.push_back(SaveStat);
	}


	map<tstring,vector<STAT>>::iterator Find = CEditGlobal::CharacterStatTable.find(SelectChar.GetString());
	
	if (Find == CEditGlobal::CharacterStatTable.end()) 
	{
		CEditGlobal::CharacterStatTable.insert(map<tstring, vector<STAT>>::value_type(SelectChar.GetString(), TempTable));
	}
	else 
	{
		MessageBox(_T("새로운 테이블로 교체 되었습니다"));
		Find->second = TempTable;
	}

	UpdateData(FALSE);
	return;
}
void CCharStatDlg::DeleteButtonEvent() 
{
	CString UnitName = _T("");
	int Index = m_CharacterListCtrl.GetCurSel();

	m_CharacterListCtrl.GetText(Index, UnitName);

	map<tstring, vector<STAT>>::iterator Find = CEditGlobal::CharacterStatTable.find(UnitName.GetString());
	map<tstring, STATPARAMETERDATA>::iterator ParaFind = CEditGlobal::CharacterParameter.find(UnitName.GetString());

	if (Find == CEditGlobal::CharacterStatTable.end() 
		|| ParaFind == CEditGlobal::CharacterParameter.end())
	{
		return;
	}


	Find->second.clear();
	CEditGlobal::CharacterStatTable.erase(Find);
	CEditGlobal::CharacterParameter.erase(ParaFind);

	MessageBox(UnitName + _T("의 스탯 테이블과 파라미터 정보를 제거하였습니다."));
	
}
void CCharStatDlg::DataLoadButtonEvent() 
{
	CGameFile LoadFile = CGameFile(_T("EditData"), _T("StatTable.bin"));

	if (FALSE == LoadFile.Open(_T("rb")))
	{
		MessageBox(_T("StatTable 을 열지 못하였습니다."), _T("CharStatDlg"), MB_ICONERROR);
		return;
	}

	size_t LoadCount;
	STATPARAMETERDATA LoadParaData;

	while (0 < (LoadCount = fread_s(&LoadParaData, sizeof(LoadParaData), sizeof(STATPARAMETERDATA), 1, LoadFile.GetFile() ))) 
	{
		
		vector<STAT> TempTable = vector<STAT>();
		
		STAT StatBuf[READCOUNT];

		for (size_t i = 0; i < MAXLEVEL / READCOUNT; i++)
		{
			fread_s(&StatBuf, sizeof(StatBuf), sizeof(STAT), READCOUNT, LoadFile.GetFile());

			for (size_t j = 0; j < READCOUNT; ++j)
			{
				TempTable.push_back(StatBuf[j]);
			}
		}
			
		map<tstring, STATPARAMETERDATA>::iterator ParaFind = CEditGlobal::CharacterParameter.find(LoadParaData.UnitName);

		if (ParaFind == CEditGlobal::CharacterParameter.end())
		{
			CEditGlobal::CharacterParameter.insert(map<tstring, STATPARAMETERDATA>::value_type(LoadParaData.UnitName, LoadParaData));
		}
		else 
		{
			ParaFind->second = LoadParaData;
		}

		map<tstring, vector<STAT>>::iterator StatFind = CEditGlobal::CharacterStatTable.find(LoadParaData.UnitName);

		if (StatFind == CEditGlobal::CharacterStatTable.end()) 
		{
			CEditGlobal::CharacterStatTable.insert(map<tstring, vector<STAT>>::value_type(LoadParaData.UnitName, TempTable));
		}
		else 
		{
			StatFind->second = TempTable;
		}

	}


}
void CCharStatDlg::DataSaveButtonEvent() 
{
	CGameFile WriteFile = CGameFile(_T("EditData"), _T("StatTable.bin"));
	CGameFile ParaFile = CGameFile(_T("EditData"), _T("StatParameter.bin"));


	if (FALSE == WriteFile.Open(_T("wb")))
	{
		MessageBox(_T("StatTable 을 열지 못하였습니다."), _T("CharStatDlg"), MB_ICONERROR);
		return ;
	}
		
	if (FALSE == ParaFile.Open(_T("wb")))
	{
		MessageBox(_T("실패"), _T("CharStatDlg"), MB_ICONERROR);
		return;
	}

	
	map<tstring, STATPARAMETERDATA>::iterator ParaStart = CEditGlobal::CharacterParameter.begin();
	map<tstring, STATPARAMETERDATA>::iterator ParaEnd = CEditGlobal::CharacterParameter.end();

	for (;ParaStart != ParaEnd; ++ParaStart)
	{
		map<tstring, vector<STAT>>::iterator Find = CEditGlobal::CharacterStatTable.find(ParaStart->second.UnitName);

		if (Find == CEditGlobal::CharacterStatTable.end()) 
		{
			continue;
		}

		if (MAXLEVEL != Find->second.size()) 
		{
			continue;
		}

		// 추가 코드(2018년 1월1일 이후) Stat Parameter 데이터를 다른파일에 따로 저장
		ParaStart->second.SaveDataToFile(ParaFile.GetFile());
		
		// 기존 코드(2018년 1월1일 이전)
		ParaStart->second.SaveDataToFile(WriteFile.GetFile());

		for (size_t i = 0; i < Find->second.size() ; ++i)
		{
			Find->second[i].SaveDataToFile(WriteFile.GetFile());
		}
	}

	return ;
}
int CCharStatDlg::GetDetailStat(const PARA& _Para)
{
	UpdateData(TRUE);
	int Value = 0;
	switch (_Para)
	{
		case CCharStatDlg::DAMAGEPARA:		 Value = m_iDamageRange; break;
		case CCharStatDlg::CRIPARA:			 Value = m_iCritical; break;
		case CCharStatDlg::CRIDAMAGEPARA:	 Value = m_iCriDamage; break;
		case CCharStatDlg::AVOIDPARA:		 Value = m_iAvd; break;
		default:
			break;
	}
	UpdateData(FALSE);
	return Value;
}
int CCharStatDlg::GetDetailStat(const int& _Para)
{
	return GetDetailStat((PARA)_Para);
}
BOOL CCharStatDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	CEditGlobal::CharStatDlg = this;

	m_ComboCtrl.AddString(_T("체력"));
	m_ComboCtrl.AddString(_T("민첩"));
	m_ComboCtrl.AddString(_T("지력"));
	m_ComboCtrl.AddString(_T("운"));

	m_ComboCtrl.SetCurSel(0);

	UpdateData(TRUE);
	InitStatData();
	UpdateData(FALSE);

	((CEdit*)GetDlgItem(IDC_CRITICAL))->SetLimitText(2);
	((CEdit*)GetDlgItem(IDC_CRITICALDEMAGE))->SetLimitText(3);
	((CEdit*)GetDlgItem(IDC_DEMAGERANGE))->SetLimitText(2);
	((CEdit*)GetDlgItem(IDC_AVOIDABILITY))->SetLimitText(2);

	m_LevelCtrl.SetRange(1, 100);
	m_LevelCtrl.SetRangeMin(1);
	m_LevelCtrl.SetRangeMax(100);
	m_LevelCtrl.SetLineSize(1);

	SettingUnitName();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CCharStatDlg::OnLbnSelchangeStatcharlist()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int Index = m_CharacterListCtrl.GetCurSel();

	if (m_iPrevIndex == Index)
	{
		return;
	}

	CString SelectCharName;
	m_CharacterListCtrl.GetText(Index, SelectCharName);

	//CEditGlobal::SDataRenderer->SetCurCharacter(SelectCharName);
	SelectCharName += _T("_idle");
	CEditGlobal::SStatIdleModel->ChangeAni(SelectCharName.GetString());
	CEditGlobal::SStatIdleModel->On();

	ChangeCharacterEvent();
	
	m_iPrevIndex = Index;
}


void CCharStatDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);
	if (false == m_bInit) 
	{
		return;
	}

	if (TRUE == bShow) 
	{
		UpdateData(TRUE);
		InitStatData();
		UpdateData(FALSE);
	}

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


void CCharStatDlg::OnCbnSelchangeMainstat()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	int  Index = m_ComboCtrl.GetCurSel();

	if (MAXMAINSTAT <= (MAINSTAT)Index) 
	{
		assert(nullptr);
		return;
	}

	m_MainStat = (MAINSTAT)Index;
	CEditGlobal::SDataRenderer->SetCurMainStat(m_MainStat);
}


BOOL CCharStatDlg::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	if (nID == IDC_STR)
	{
		int Sel = m_StrCtrl.GetSel();
		UpdateData(TRUE);
		CEditGlobal::ChangeFloatString(m_Str);
		UpdateData(FALSE);
		m_StrCtrl.SetSel(Sel);
	}
	if (nID == IDC_DEX)
	{
		int Sel = m_DexCtrl.GetSel();
		UpdateData(TRUE);
		CEditGlobal::ChangeFloatString(m_Dex);
		UpdateData(FALSE);
		m_DexCtrl.SetSel(Sel);
	}
	if (nID == IDC_INT)
	{
		int Sel = m_IntCtrl.GetSel();
		UpdateData(TRUE);
		CEditGlobal::ChangeFloatString(m_Int);
		UpdateData(FALSE);
		m_IntCtrl.SetSel(Sel);
	}
	if (nID == IDC_LUK)
	{
		int Sel = m_LukCtrl.GetSel();
		UpdateData(TRUE);
		CEditGlobal::ChangeFloatString(m_Luk);
		UpdateData(FALSE);
		m_LukCtrl.SetSel(Sel);
	}
	if (nID == IDC_DEMAGE)
	{
		int Sel = m_DamageCtrl.GetSel();
		UpdateData(TRUE);
		CEditGlobal::ChangeFloatString(m_Damage);
		UpdateData(FALSE);
		m_DamageCtrl.SetSel(Sel);
	}
	if (nID == IDC_DEF)
	{
		int Sel = m_DefCtrl.GetSel();
		UpdateData(TRUE);
		CEditGlobal::ChangeFloatString(m_Def);
		UpdateData(FALSE);
		m_DefCtrl.SetSel(Sel);
	}
	if (nID == IDC_HP)
	{
		int Sel = m_HpCtrl.GetSel();
		UpdateData(TRUE);
		CEditGlobal::ChangeFloatString(m_Hp);
		UpdateData(FALSE);
		m_HpCtrl.SetSel(Sel);
	}
	if (nID == IDC_MP)
	{
		int Sel = m_MpCtrl.GetSel();
		UpdateData(TRUE);
		CEditGlobal::ChangeFloatString(m_Mp);
		UpdateData(FALSE);
		m_MpCtrl.SetSel(Sel);
	}

	return CDialogEx::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}


BOOL CCharStatDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
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
		if (pMsg->hwnd == GetDlgItem(IDC_CSTATCREATEBUTTON)->m_hWnd)
		{
			SendMessageA(GetFocus()->m_hWnd, WM_KILLFOCUS, NULL, NULL);
			CreateButtonEvent();
			return TRUE;
		}
		if (pMsg->hwnd == GetDlgItem(IDC_CSTATDELETEBUTTON)->m_hWnd)
		{
			DeleteButtonEvent();
			return TRUE;
		}
		if (pMsg->hwnd == GetDlgItem(IDC_CSTATLOADBUTTON)->m_hWnd)
		{
			DataLoadButtonEvent();
			return TRUE;
		}
		if (pMsg->hwnd == GetDlgItem(IDC_CSTATSAVEBUTTON)->m_hWnd)
		{
			DataSaveButtonEvent();
			return TRUE;
		}

		int nID = IDC_STR;
		while (nID <= IDC_AVOIDABILITY)
		{
			if (pMsg->hwnd == GetDlgItem(nID)->m_hWnd &&
				GetFocus()->m_hWnd != GetDlgItem(nID)->m_hWnd)
			{
				GetDlgItem(nID)->SetFocus();

				switch (nID)
				{
				case IDC_STR:			m_StrCtrl.SetSel(0, -1, true); return TRUE;
				case IDC_DEX:			m_DexCtrl.SetSel(0, -1, true); return TRUE;
				case IDC_INT:			m_IntCtrl.SetSel(0, -1, true); return TRUE;
				case IDC_LUK:			m_LukCtrl.SetSel(0, -1, true); return TRUE;
				case IDC_DEMAGE:		m_DamageCtrl.SetSel(0, -1, true); return TRUE;
				case IDC_DEF:			m_DefCtrl.SetSel(0, -1, true); return TRUE;
				case IDC_HP:			m_HpCtrl.SetSel(0, -1, true); return TRUE;
				case IDC_MP:			m_MpCtrl.SetSel(0, -1, true); return TRUE;
				case IDC_CRITICAL:		m_CriticalCtrl.SetSel(0, -1, true); return TRUE;
				case IDC_CRITICALDEMAGE:m_CriDamageCtrl.SetSel(0, -1, true); return TRUE;
				case IDC_DEMAGERANGE :	m_DamageRangeCtrl.SetSel(0, -1, true); return TRUE;
				case IDC_AVOIDABILITY : m_AvdCtrl.SetSel(0, -1, true); return TRUE;
				default:
					break;
				}
			}
			else
			{
				++nID;
			}
		}


		SendMessageA(this->m_hWnd, WM_SETFOCUS, NULL, NULL);

		/*if (GetFocus()->m_hWnd != this->m_hWnd) 
		{
			SendMessageA(GetFocus()->m_hWnd, WM_KILLFOCUS, NULL, NULL);
		}*/
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}


void CCharStatDlg::OnEnKillfocusStr()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CheckParameterData(m_Str, m_fStr);
	CEditGlobal::SParameter->SetParameterValue(2, m_fStr);
	CEditGlobal::SDataRenderer->UpdateStat(STRPARA);
}


void CCharStatDlg::OnEnKillfocusDex()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CheckParameterData(m_Dex, m_fDex);
	CEditGlobal::SParameter->SetParameterValue(3, m_fDex);
	CEditGlobal::SDataRenderer->UpdateStat(DEXPARA);
}


void CCharStatDlg::OnEnKillfocusInt()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CheckParameterData(m_Int, m_fInt);
	CEditGlobal::SParameter->SetParameterValue(4, m_fInt);
	CEditGlobal::SDataRenderer->UpdateStat(INTPARA);
}


void CCharStatDlg::OnEnKillfocusLuk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CheckParameterData(m_Luk, m_fLuk);
	CEditGlobal::SParameter->SetParameterValue(5, m_fLuk);
	CEditGlobal::SDataRenderer->UpdateStat(LUCKPARA);
}


void CCharStatDlg::OnEnKillfocusDemage()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CheckParameterData(m_Damage, m_fDamage);
	CEditGlobal::SParameter->SetParameterValue(6, m_fDamage);
	CEditGlobal::SDataRenderer->UpdateStat(DAMAGEPARA);
}


void CCharStatDlg::OnEnKillfocusDef()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CheckParameterData(m_Def, m_fDef);
	CEditGlobal::SParameter->SetParameterValue(7, m_fDef);
	CEditGlobal::SDataRenderer->UpdateStat(DEFPARA);
}


void CCharStatDlg::OnEnKillfocusHp()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CheckParameterData(m_Hp, m_fHp);
	CEditGlobal::SParameter->SetParameterValue(0, m_fHp);
	CEditGlobal::SDataRenderer->UpdateStat(HPPARA);
}


void CCharStatDlg::OnEnKillfocusMp()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CheckParameterData(m_Mp, m_fMp);
	CEditGlobal::SParameter->SetParameterValue(1, m_fMp);
	CEditGlobal::SDataRenderer->UpdateStat(MPPARA);
}




void CCharStatDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (pScrollBar == (CScrollBar*)&m_LevelCtrl) 
	{
		m_iLevel = m_LevelCtrl.GetPos();
		CEditGlobal::SDataRenderer->SetLevel(m_iLevel);
	}


	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}


void CCharStatDlg::OnEnKillfocusCritical()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	m_fCritical = m_iCritical * 0.01f;
	UpdateData(FALSE);
	CEditGlobal::SDataRenderer->UpdateStat(CRIPARA);
}


void CCharStatDlg::OnEnKillfocusCriticaldemage()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	m_fCriDamage = m_iCriDamage * 0.01f;
	UpdateData(FALSE);
	CEditGlobal::SDataRenderer->UpdateStat(CRIDAMAGEPARA);
}


void CCharStatDlg::OnEnKillfocusDemagerange()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CEditGlobal::SDataRenderer->UpdateStat(DAMAGEPARA);
}


void CCharStatDlg::OnEnKillfocusAvoidability()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	m_fAvd = m_iAvd * 0.01f;
	UpdateData(FALSE);
	CEditGlobal::SDataRenderer->UpdateStat(AVOIDPARA);
}
