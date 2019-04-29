#include "stdafx.h"
#include "DataRenderer.h"
#include"Gauge.h"
#include"CharStatDlg.h"

CDataRenderer::CDataRenderer()
	:m_iCurLevel(1), m_bTableData(false), m_pCurParameterData(nullptr)
	, m_CurMainStat(STRENGTH), m_CurCharacter(_T(""))
{

}


CDataRenderer::~CDataRenderer()
{
	m_vecGauge.clear();
}
void CDataRenderer::Init()
{
	POINT WndSize = CEditGlobal::MainWindow->WndSize();
	m_Pos = Vec2{ WndSize.x*-0.16f,WndSize.x*-0.1f };

	CGauge* StrGauge = CActObject::CreateActObject(_T(""), ACTOR->GetScene())->CreateCom<CGauge>();
	StrGauge->SetGaugeFrame(_T("StatGaugeFrame"));
	StrGauge->SetFrameSize(Vec2{ 300.0f,40.0f });
	StrGauge->SetGaugeSize(Vec2{ 290.0f,30.0f });
	StrGauge->SetGaugeColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.9f));
	StrGauge->SetMaxGauge(1000);
	StrGauge->SetCurGauge(0);

	CGauge* DexGauge = CActObject::CreateActObject(_T(""), ACTOR->GetScene())->CreateCom<CGauge>();
	DexGauge->SetGaugeFrame(_T("StatGaugeFrame"));
	DexGauge->SetFrameSize(Vec2{ 300.0f,40.0f });
	DexGauge->SetGaugeSize(Vec2{ 290.0f,30.0f });
	DexGauge->SetGaugeColor(D3DXCOLOR(0.0f, 1.0f, 0.0f, 0.9f));
	DexGauge->SetMaxGauge(1000);
	DexGauge->SetCurGauge(0);

	CGauge* IntGauge = CActObject::CreateActObject(_T(""), ACTOR->GetScene())->CreateCom<CGauge>();
	IntGauge->SetGaugeFrame(_T("StatGaugeFrame"));
	IntGauge->SetFrameSize(Vec2{ 300.0f,40.0f });
	IntGauge->SetGaugeSize(Vec2{ 290.0f,30.0f });
	IntGauge->SetGaugeColor(D3DXCOLOR(0.0f, 0.0f, 1.0f, 0.9f));
	IntGauge->SetMaxGauge(1000);
	IntGauge->SetCurGauge(0);

	CGauge* LukGauge = CActObject::CreateActObject(_T(""), ACTOR->GetScene())->CreateCom<CGauge>();
	LukGauge->SetGaugeFrame(_T("StatGaugeFrame"));
	LukGauge->SetFrameSize(Vec2{ 300.0f,40.0f });
	LukGauge->SetGaugeSize(Vec2{ 290.0f,30.0f });
	LukGauge->SetGaugeColor(D3DXCOLOR(1.0f, 1.0f, 0.0f, 0.9f));
	LukGauge->SetMaxGauge(1000);
	LukGauge->SetCurGauge(0);

	StrGauge->SetPos(m_Pos - Vec2{ 0.0f,50.0f });
	DexGauge->SetPos(m_Pos - Vec2{ 0.0f,100.0f });
	IntGauge->SetPos(m_Pos - Vec2{ 0.0f,150.0f });
	LukGauge->SetPos(m_Pos - Vec2{ 0.0f,200.0f });


	m_vecGauge.push_back(StrGauge);
	m_vecGauge.push_back(DexGauge);
	m_vecGauge.push_back(IntGauge);
	m_vecGauge.push_back(LukGauge);

	m_vecStatString.reserve(MAXPARA);

	m_vecStatString.push_back(_T("HP : "));
	m_vecStatString.push_back(_T("MP : "));
	m_vecStatString.push_back(_T("체력 : "));
	m_vecStatString.push_back(_T("민첩 : "));
	m_vecStatString.push_back(_T("지력 : "));
	m_vecStatString.push_back(_T("행운 : "));
	m_vecStatString.push_back(_T("공격력 : "));
	m_vecStatString.push_back(_T("방어력 : "));
	m_vecStatString.push_back(_T("크리티컬 확률 : "));
	m_vecStatString.push_back(_T("크리티컬 데미지 : "));
	m_vecStatString.push_back(_T("회피율 : "));
	m_vecStatString.push_back(_T("Level "));
	

}
void CDataRenderer::Update()
{

}
void CDataRenderer::FontRender()
{

	POINT Pos = POINT{ (LONG)m_Pos.x,(LONG)m_Pos.y };
	Vec3 Highlight = Vec3{ m_Pos.x-80.0f,m_Pos.y ,30.0f };

	for (size_t i = 0; i <= MAXMAINSTAT; i++)
	{
		Highlight -= Vec3{ 0.0f,50.0f,0.0f };
		
		if ((MAINSTAT)i == m_CurMainStat) 
		{
			break;
		}
	}
	
	GetComDebugMgr()->RectRender(D3DCOLOR_ARGB(255, 255, 0, 0), Highlight, { 490.0,44.0f,1.0f });
	COMRESMGR->DrawFont(_T("돋움"), m_vecStatString[LEVELPARA].GetString(), Pos - POINT{ 240,13 }, 2.3f, D3DCOLOR_ARGB(255, 255, 255, 255));
	COMRESMGR->DrawFont(_T("돋움"), m_vecStatString[STRPARA].GetString(), Pos - POINT{ 310,63 }, 2.3f, D3DCOLOR_ARGB(255, 255, 255, 255));
	COMRESMGR->DrawFont(_T("돋움"), m_vecStatString[DEXPARA].GetString(), Pos - POINT{ 310,113 }, 2.3f, D3DCOLOR_ARGB(255, 255, 255, 255));
	COMRESMGR->DrawFont(_T("돋움"), m_vecStatString[INTPARA].GetString(), Pos - POINT{ 310,163 }, 2.3f, D3DCOLOR_ARGB(255, 255, 255, 255));
	COMRESMGR->DrawFont(_T("돋움"), m_vecStatString[LUCKPARA].GetString(), Pos - POINT{ 310,213 }, 2.3f, D3DCOLOR_ARGB(255, 255, 255, 255));

	COMRESMGR->DrawFont(_T("돋움"), m_vecStatString[HPPARA].GetString(), Pos + POINT{ 200, -13 }, 2.3f, D3DCOLOR_ARGB(255, 255, 255, 255));
	COMRESMGR->DrawFont(_T("돋움"), m_vecStatString[MPPARA].GetString(), Pos + POINT{ 400, -13 }, 2.3f, D3DCOLOR_ARGB(255, 255, 255, 255));
	COMRESMGR->DrawFont(_T("돋움"), m_vecStatString[DAMAGEPARA].GetString(), Pos + POINT{ 200,-63 }, 2.3f, D3DCOLOR_ARGB(255, 255, 255, 255));
	COMRESMGR->DrawFont(_T("돋움"), m_vecStatString[DEFPARA].GetString(), Pos + POINT{ 200,-113 }, 2.3f, D3DCOLOR_ARGB(255, 255, 255, 255));
	COMRESMGR->DrawFont(_T("돋움"), m_vecStatString[CRIPARA].GetString(), Pos + POINT{ 200,-163 }, 2.3f, D3DCOLOR_ARGB(255, 255, 255, 255));
	COMRESMGR->DrawFont(_T("돋움"), m_vecStatString[CRIDAMAGEPARA].GetString(), Pos + POINT{ 200,-213 }, 2.3f, D3DCOLOR_ARGB(255, 255, 255, 255));
	COMRESMGR->DrawFont(_T("돋움"), m_vecStatString[AVOIDPARA].GetString(), Pos + POINT{ 200,-263 }, 2.3f, D3DCOLOR_ARGB(255, 255, 255, 255));


	/*if (true == m_bTableData) 
	{
		TableDataRender();
	}
	else 
	{
		InitDataRender();
	}*/
	
}
void CDataRenderer::TableDataRender()
{

}
void CDataRenderer::InitDataRender() 
{
	
}
void CDataRenderer::ConnectParameterArray(float* _ParameterArr)
{
	if (nullptr == _ParameterArr) 
	{
		assert(nullptr);
		return;
	}
	m_pCurParameterData = _ParameterArr;

}
void CDataRenderer::SetLevel(const int& _Level)
{
	m_iCurLevel = _Level;

	m_vecStatString[LEVELPARA].Format(_T("Level %d"), m_iCurLevel);
	UpdateStat(STRPARA);
	UpdateStat(DEXPARA);
	UpdateStat(INTPARA);
	UpdateStat(LUCKPARA);

	UpdateStat(HPPARA);
	UpdateStat(MPPARA);
	UpdateStat(DAMAGEPARA);
	UpdateStat(DEFPARA);


}
void CDataRenderer::SettingTableData(vector<STAT>* _Table)
{
	if (nullptr == _Table) 
	{
		assert(nullptr);
		return;
	}

	m_bTableData = true;
	m_mapCurTable = _Table;
}
void CDataRenderer::SettingTableData(const tstring& _UnitName)
{
	m_bTableData = true;

	m_mapCurTable = &(CEditGlobal::CharacterStatTable.find(_UnitName)->second);

}
void CDataRenderer::SettingInitData(const STAT& _InitStat)
{
	m_CurRenStat = _InitStat;
	m_bTableData = false;

}
void CDataRenderer::UpdateStat(const PARA& _Para)
{
	UpdateStatString(_Para);
	UpdateStatGauge(_Para);
}
void CDataRenderer::UpdateStat(const int& _Para)
{
	UpdateStat((PARA)_Para);
}
void CDataRenderer::UpdateStatGauge(const PARA& _Para)
{
	if (_Para < STRPARA || _Para > LUCKPARA) 
	{
		return;
	}


	/*int CurStat = 0;

	switch (_Para)
	{
	case CDataRenderer::STRPARA: CurStat = m_CurRenStat.Str; break;
	case CDataRenderer::DEXPARA:CurStat = m_CurRenStat.Dex; break;
	case CDataRenderer::INTPARA:CurStat = m_CurRenStat.Int; break;
	case CDataRenderer::LUCKPARA:CurStat = m_CurRenStat.Luck; break;
	default:
		return;
	}

	int K = m_iCurLevel - 1;
	m_vecGauge[_Para - STRPARA]->SetCurGauge((int)(K*m_pCurParameterData[_Para]) + CurStat);
*/
	// 새로 지정되어야 하는 경우 
	if (false == m_bTableData)
	{
		int CurStat = 0;
		switch (_Para)
		{
		case CDataRenderer::STRPARA: CurStat = m_CurRenStat.Str; break;
		case CDataRenderer::DEXPARA:CurStat = m_CurRenStat.Dex; break;
		case CDataRenderer::INTPARA:CurStat = m_CurRenStat.Int;break;
		case CDataRenderer::LUCKPARA:CurStat = m_CurRenStat.Luck; break;
		default:
			return;
		}

		int K = m_iCurLevel - 1;
		m_vecGauge[_Para - STRPARA]->SetCurGauge((int)(K*m_pCurParameterData[_Para]) + CurStat);
	}
	// 테이블에서 읽어야하는 경우
	else 
	{
		if (nullptr == m_mapCurTable) 
		{
			assert(nullptr);
			return;
		}
	
		STAT Stat = (*m_mapCurTable)[m_iCurLevel - 1];
		
		int CurStat = 0;

		switch (_Para)
		{
		case CDataRenderer::STRPARA: CurStat = Stat.Str; break;
		case CDataRenderer::DEXPARA: CurStat = Stat.Dex; break;
		case CDataRenderer::INTPARA: CurStat = Stat.Int; break;
		case CDataRenderer::LUCKPARA:CurStat = Stat.Luck; break;
		default:
			return;
		}
		m_vecGauge[_Para - STRPARA]->SetCurGauge(CurStat);

	
	}

}
void CDataRenderer::UpdateStatString(const PARA& _Para)
{
	if (_Para >= MAXPARA) 
	{
		assert(nullptr);
		return;
	}

	/*float Para = m_pCurParameterData[_Para];
	int K = m_iCurLevel - 1;

	switch (_Para)
	{
	case CDataRenderer::HPPARA:			m_vecStatString[_Para].Format(_T("HP : %d"), (int)(K*Para*2.0f) + m_CurRenStat.Heath); break;
	case CDataRenderer::MPPARA:			m_vecStatString[_Para].Format(_T("MP : %d"), (int)m_CurRenStat.Mana); break;
	case CDataRenderer::STRPARA:		m_vecStatString[_Para].Format(_T("체력 : %d"), (int)(K*Para) + m_CurRenStat.Str); break;
	case CDataRenderer::DEXPARA:		m_vecStatString[_Para].Format(_T("민첩 : %d"), (int)(K*Para) + m_CurRenStat.Dex); break;
	case CDataRenderer::INTPARA:		m_vecStatString[_Para].Format(_T("지력 : %d"), (int)(K*Para) + m_CurRenStat.Int); break;
	case CDataRenderer::LUCKPARA:		m_vecStatString[_Para].Format(_T("행운 : %d"), (int)(K*Para) + m_CurRenStat.Luck); break;
	case CDataRenderer::DAMAGEPARA:
	{
		float HalfRange = (float)CEditGlobal::CharStatDlg->GetDetailStat(_Para) * 0.01f;
		int Damage = (int)(K*Para*0.5f) + m_CurRenStat.Damage;
		float Min = Damage * (1.0f - HalfRange);
		float Max = Damage * (1.0f + HalfRange);;

		m_vecStatString[_Para].Format(_T("공격력 : %d ~ %d"), (int)Min, (int)Max);
	}
	break;
	case CDataRenderer::DEFPARA:		m_vecStatString[_Para].Format(_T("방어력 : %d"), (int)(K*Para*0.5f) + m_CurRenStat.Defense); break;
	case CDataRenderer::CRIPARA:		m_vecStatString[_Para].Format(_T("크리티컬 확률 : %d%%"), GetDetailStat(_Para)); break;
	case CDataRenderer::CRIDAMAGEPARA:	m_vecStatString[_Para].Format(_T("크리티컬 데미지: %d%%"), GetDetailStat(_Para)); break;
	case CDataRenderer::AVOIDPARA:		m_vecStatString[_Para].Format(_T("회피율 : %d%%"), GetDetailStat(_Para)); break;
	case CDataRenderer::LEVELPARA:		m_vecStatString[_Para].Format(_T("Level %d"), m_iCurLevel); break;
	default:
		return;
	}
*/

	if (true == m_bTableData) 
	{
		if (nullptr == m_mapCurTable) 
		{
			assert(nullptr);
			return;
		}

		STAT LoadStat = (*m_mapCurTable)[m_iCurLevel - 1];

		
		switch (_Para)
		{
		case CDataRenderer::HPPARA:			m_vecStatString[_Para].Format(_T("HP : %d"), LoadStat.Heath); break;
		case CDataRenderer::MPPARA:			m_vecStatString[_Para].Format(_T("MP : %d"), LoadStat.Mana); break;
		case CDataRenderer::STRPARA:		m_vecStatString[_Para].Format(_T("체력 : %d"), LoadStat.Str); break;
		case CDataRenderer::DEXPARA:		m_vecStatString[_Para].Format(_T("민첩 : %d"), LoadStat.Dex); break;
		case CDataRenderer::INTPARA:		m_vecStatString[_Para].Format(_T("지력 : %d"), LoadStat.Int); break;
		case CDataRenderer::LUCKPARA:		m_vecStatString[_Para].Format(_T("행운 : %d"), LoadStat.Luck); break;
		case CDataRenderer::DAMAGEPARA:
		{
			float HalfRange = (float)CEditGlobal::CharStatDlg->GetDetailStat(_Para) * 0.01f;
			int Damage = LoadStat.Damage;
			float Min = Damage * (1.0f - HalfRange);
			float Max = Damage * (1.0f + HalfRange);

			m_vecStatString[_Para].Format(_T("공격력 : %d ~ %d"), (int)Min, (int)Max);
		}
		break;
		case CDataRenderer::DEFPARA:	m_vecStatString[_Para].Format(_T("방어력 : %d"), LoadStat.Defense); break;
		case CDataRenderer::CRIPARA:		m_vecStatString[_Para].Format(_T("크리티컬 확률 : %d%%"), LoadStat.Critical); break;
		case CDataRenderer::CRIDAMAGEPARA:	m_vecStatString[_Para].Format(_T("크리티컬 데미지: %d%%"), LoadStat.CriticalDamage); break;
		case CDataRenderer::AVOIDPARA:		m_vecStatString[_Para].Format(_T("회피율 : %d%%"), LoadStat.Avoidability); break;
		case CDataRenderer::LEVELPARA:		m_vecStatString[_Para].Format(_T("Level %d"), m_iCurLevel); break;
		default:
			return;
		}
	}
	else 
	{
		float Para = m_pCurParameterData[_Para];
		int K = m_iCurLevel - 1;

		switch (_Para)
		{
		case CDataRenderer::HPPARA:			m_vecStatString[_Para].Format(_T("HP : %d"), (int)(K*Para*2.0f) + m_CurRenStat.Heath); break;
		case CDataRenderer::MPPARA:			m_vecStatString[_Para].Format(_T("MP : %d"), (int)(K*Para) + m_CurRenStat.Mana); break;
		case CDataRenderer::STRPARA:		m_vecStatString[_Para].Format(_T("체력 : %d"), (int)(K*Para) + m_CurRenStat.Str); break;
		case CDataRenderer::DEXPARA:		m_vecStatString[_Para].Format(_T("민첩 : %d"), (int)(K*Para) + m_CurRenStat.Dex); break;
		case CDataRenderer::INTPARA:		m_vecStatString[_Para].Format(_T("지력 : %d"), (int)(K*Para) + m_CurRenStat.Int); break;
		case CDataRenderer::LUCKPARA:		m_vecStatString[_Para].Format(_T("행운 : %d"), (int)(K*Para) + m_CurRenStat.Luck); break;
		case CDataRenderer::DAMAGEPARA:	
		{
			float HalfRange = (float)CEditGlobal::CharStatDlg->GetDetailStat(_Para) * 0.01f;
			int Damage = (int)(K*Para*0.5f) + m_CurRenStat.Damage;
			float Min = Damage * (1.0f - HalfRange);
			float Max = Damage * (1.0f + HalfRange);

			m_vecStatString[_Para].Format(_T("공격력 : %d ~ %d"),(int)Min,(int)Max);
		}
		break; 
		case CDataRenderer::DEFPARA:		m_vecStatString[_Para].Format(_T("방어력 : %d"), (int)(K*Para*0.5f) + m_CurRenStat.Defense); break;
		case CDataRenderer::CRIPARA:		m_vecStatString[_Para].Format(_T("크리티컬 확률 : %d%%"),GetDetailStat(_Para) ); break;
		case CDataRenderer::CRIDAMAGEPARA:	m_vecStatString[_Para].Format(_T("크리티컬 데미지: %d%%"), GetDetailStat(_Para)); break;
		case CDataRenderer::AVOIDPARA:		m_vecStatString[_Para].Format(_T("회피율 : %d%%"), GetDetailStat(_Para)) ; break;
		case CDataRenderer::LEVELPARA:		m_vecStatString[_Para].Format(_T("Level %d"), m_iCurLevel); break;
		default:
			return;
		}
	}
}
int CDataRenderer::GetDetailStat(const PARA& _Para)
{
	return CEditGlobal::CharStatDlg->GetDetailStat((int)_Para);

}
void CDataRenderer::UpdateStat()
{
	for (size_t i = 0; i < MAXPARA; i++)
	{
		UpdateStat((int)i);
	}
	
}
void CDataRenderer::SetCurMainStat(const MAINSTAT& _MainStat)
{
	m_CurMainStat = _MainStat;
}
void CDataRenderer::SetCurCharacter(const CString& _Character)
{
	m_CurCharacter = _Character;
}