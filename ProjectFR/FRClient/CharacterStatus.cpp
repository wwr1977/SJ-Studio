#include "Precom.h"
#include "CharacterStatus.h"
#include "CharacterParameter.h"
#include "StdGauge.h"

CCharacterStatus::CCharacterStatus()
	:m_CurCharacterRen(nullptr), m_Parameter(nullptr)
	, m_CurCharacterNameRen(nullptr)
	, m_CurCharNo(MAXBATTLECHARACTER)
	, m_ModelPos(Vec3{-50.f,-55.f,7.f})
{
	m_vecStatusString.clear();
}


CCharacterStatus::~CCharacterStatus()
{
	m_Parameter = nullptr;
	m_vecStatusString.clear();
}

void CCharacterStatus::Init()
{
	m_CurCharacterRen =   CActObject::CreateActObject(_T("StatusModel"),ACTOR->GetScene())->CreateCom<CAniRenderer>(RENDATA(LAYER_UI, 0.9f, true));
	m_CurCharacterNameRen = ACTOR->CreateCom<CFontRenderer>(RENDATA(LAYER_UI, 1.0f, true));
	m_CurCharacterNameRen->SetFontStyle(_T("캐릭터닉네임폰트"));
	m_CurCharacterNameRen->SetLeftStartPivot(POINT{ -230,170 });
	m_CurCharacterNameRen->SetFontMag(2.3f);
	m_CurCharacterNameRen->SetFontColor(D3DXCOLOR(1.f, 1.f, 0.1f, 1.0f));

	for (size_t i = 0; i < CClientGlobal::CharacterName.size(); i++)
	{
		m_CharacterName.push_back(CClientGlobal::CharacterName[i]);
		m_CurCharacterRen->CreateAnimation(Ani_Idle(m_CharacterName[i].UnitName));
	}

	//m_CurCharacterRen->SetRenPivot(Vec2{ -200.0f,-100.0f });
	m_CurCharacterRen->SetImageMag(2.5f);

	m_CurCharacterRen->SetActorPos(m_ModelPos);
	m_CurCharacterRen->ActorOff();

	m_Parameter = CActObject::CreateActObject(_T("Parameter"), ACTOR->GetScene())->CreateCom<CCharacterParameter>();
	m_Parameter->ParameterOff();
	m_Parameter->SetPos(Vec3{ 370.f,70.f,7.f });


	for (size_t i = 0; i < MAXGAUGE; i++) 
	{
		m_vecGauge.push_back(CActObject::CreateActObject(_T("Status Gauge"),ACTOR->GetScene())->CreateCom<CStdGauge>());
		
		if (i == 0)
			continue;

		m_vecGauge[i]->CreateGaugeFrontFrame(_T("StatGaugeFrame"));
		//m_vecGauge[i]->SetGaugeFrame(_T("StatGaugeFrame"));
		m_vecGauge[i]->SetFrameSize(Vec2{ 250.f,30.f });
		m_vecGauge[i]->SetGaugeSize(Vec2{ 240.f,21.f });
		m_vecGauge[i]->SetGaugePivot(Vec2{ 0.f,-2.7f });
		m_vecGauge[i]->SetMaxGauge(1000);
		//m_vecGauge[i]->SetCurGauge(200);
		m_vecGauge[i]->SetPos(Vec3{ 30.f,-150.f -40.f*i,7.f});
		m_vecGauge[i]->SetGaugeColor(D3DXCOLOR(0.6f, 0.6f, 0.6f, 1.0f));
	}

	m_vecGauge[GAUGE_XP]->SetGaugeFrame(_T("XPFrameXL"));
	m_vecGauge[GAUGE_XP]->CreateGaugeFont(_T("볼드굴림"));
	m_vecGauge[GAUGE_XP]->SetGaugeFont(0);
	m_vecGauge[GAUGE_XP]->SetFrameSize(Vec2{ 400.f,25.f });
	m_vecGauge[GAUGE_XP]->SetGaugeSize(Vec2{ 392.f,21.f });
	m_vecGauge[GAUGE_XP]->SetMaxGauge(1000);
	m_vecGauge[GAUGE_XP]->SetCurGauge(200);
	m_vecGauge[GAUGE_XP]->SetPos(Vec3{ -50.f,-150.f,7.f });
	m_vecGauge[GAUGE_XP]->SetGaugeColor(D3DXCOLOR(0.35f, 0.2f, 0.56f, 1.0f));

	m_vecStatusString.assign(MAXSTATUSSTRING, _T(""));

	for (size_t i = 0; i < MAXSTATUSSTRING; i++)
		m_vecStatusString[i].reserve(128);
	

	StatusOff();
}
void CCharacterStatus::Update() 
{

}
void CCharacterStatus::ChangeCharacterStatus(const CharacterIndex& _Index)
{
	if (_Index >= MAXBATTLECHARACTER)
		return;

	m_CurCharacterRen->ChangeAni(Idle(m_CharacterName[_Index].UnitName));
	
	tstring Temp = m_CharacterName[_Index].UnitNickName +_T(" ") +m_CharacterName[_Index].UnitKName;
	m_CurCharacterNameRen->SetString(Temp);

	m_Parameter->SetChangeParameter(_Index);

	TCHAR Buf[128] = _T("");
	size_t BufSize = sizeof(Buf) / sizeof(TCHAR);
	size_t CurLv = CClientGlobal::GetCurCharacterLevel(_Index);

	tstring CharName = CClientGlobal::CharacterName[_Index].UnitName;
	STATPARAMETERDATA Para = CClientGlobal::CharacterParameter[CharName];

	//UINT MaxExp = CClientGlobal::ExpTable[CurLv];
	STAT CurStat = CClientGlobal::GetCurCharacterStat(_Index, CurLv);
	CHARACTERDATA CurData = CClientGlobal::GetCurCharData(_Index);

	STAT ItemStat;

	for (size_t i = 0; i < MAXITEMEQUIP; i++)
	{
		size_t ItemNo = CurData.EquipItem[i];
		if (-1 == ItemNo)
			continue;

		ItemStat += CClientGlobal::vecItem[ItemNo].ItemStat;
	}
	
	m_vecGauge[GAUGE_XP]->SetCurGauge((int)CurData.CurExp);
	m_vecGauge[GAUGE_XP]->SetMaxGauge((int)CurData.MaxExp);

	for (size_t i = GAUGE_STR; i <= GAUGE_LUK; i++)
		m_vecGauge[i]->SetGaugeColor(D3DXCOLOR(0.6f, 0.6f, 0.6f, 1.0f));

	
	switch (Para.MainStat)
	{
	case STRENGTH:
		m_vecGauge[GAUGE_STR]->SetGaugeColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.9f));
		break;
	case DEXERITY:
		m_vecGauge[GAUGE_DEX]->SetGaugeColor(D3DXCOLOR(0.0f, 1.0f, 0.0f, 0.9f));
		break;
	case INTELIGENCE:
		m_vecGauge[GAUGE_INT]->SetGaugeColor(D3DXCOLOR(0.0f, 0.0f, 1.0f, 0.9f));
		break;
	case LUCK:
		m_vecGauge[GAUGE_LUK]->SetGaugeColor(D3DXCOLOR(1.0f, 1.0f, 0.0f, 0.9f));
		break;
	}

	m_vecGauge[GAUGE_STR]->SetCurGauge(CurStat.Str + ItemStat.Str);
	
	m_vecGauge[GAUGE_DEX]->SetCurGauge(CurStat.Dex + ItemStat.Dex);
	
	m_vecGauge[GAUGE_INT]->SetCurGauge(CurStat.Int + ItemStat.Int);
	
	m_vecGauge[GAUGE_LUK]->SetCurGauge(CurStat.Luck + ItemStat.Luck);
	
	_stprintf_s(Buf, BufSize,_T("Lv %3zd"), CurLv);
	m_vecStatusString[STRING_LV] = Buf;

	if (0 == ItemStat.Str) 
	{
		_stprintf_s(Buf, BufSize, _T("체력 : %3d(-)"), CurStat.Str);
		m_vecStatusString[STRING_STR] = Buf;
	}
	else 
	{
		_stprintf_s(Buf, BufSize, _T("체력 : %3d(%+d)"), CurStat.Str + ItemStat.Str,ItemStat.Str);
		m_vecStatusString[STRING_STR] = Buf;
	}

	if (0 == ItemStat.Dex)
	{
		_stprintf_s(Buf, BufSize, _T("솜씨 : %3d(-)"), CurStat.Dex);
		m_vecStatusString[STRING_DEX] = Buf;
	}
	else
	{
		_stprintf_s(Buf, BufSize, _T("솜씨 : %3d(%+d)"), CurStat.Dex+ ItemStat.Dex,ItemStat.Dex);
		m_vecStatusString[STRING_DEX] = Buf;
	}


	if (0 == ItemStat.Int)
	{
		_stprintf_s(Buf, BufSize, _T("지력 : %3d(-)"), CurStat.Int);
		m_vecStatusString[STRING_INT] = Buf;
	}
	else
	{
		_stprintf_s(Buf, BufSize, _T("지력 : %3d(%+d)"), CurStat.Int+ ItemStat.Int,ItemStat.Int);
		m_vecStatusString[STRING_INT] = Buf;
	}

	if (0 == ItemStat.Luck)
	{
		_stprintf_s(Buf, BufSize, _T("행운 : %3d(-)"), CurStat.Luck);
		m_vecStatusString[STRING_LUK] = Buf;
	}
	else
	{
		_stprintf_s(Buf, BufSize, _T("행운 : %3d(%+d)"), CurStat.Luck + ItemStat.Luck,ItemStat.Luck);
		m_vecStatusString[STRING_LUK] = Buf;
	}


	if (0 == ItemStat.Heath)
	{
		_stprintf_s(Buf, BufSize, _T("HP : %5zd/%5zd(-)"), CurData.CurHp, CurData.MaxHp);
		m_vecStatusString[STRING_HP] = Buf;
	}
	else
	{
		_stprintf_s(Buf, BufSize, _T("HP : %5zd/%5zd(%+d)"), CurData.CurHp, CurData.MaxHp,ItemStat.Heath);
		m_vecStatusString[STRING_HP] = Buf;
	}

	if (0 == ItemStat.Mana)
	{
		_stprintf_s(Buf, BufSize, _T("MP : %3zd/%3zd(-)"), CurData.CurMp, CurData.MaxMp);
		m_vecStatusString[STRING_MP] = Buf;
	}
	else
	{
		_stprintf_s(Buf, BufSize, _T("MP : %3zd/%3zd(%+d)"), CurData.CurMp, CurData.MaxMp + ItemStat.Mana,ItemStat.Mana);
		m_vecStatusString[STRING_MP] = Buf;
	}


	if (0 == ItemStat.Damage)
	{
		_stprintf_s(Buf, BufSize, _T("데미지 : %5d ~ %5d(-)"), CurStat.MinDamage, CurStat.MaxDamage);
		m_vecStatusString[STRING_DEM] = Buf;
	}
	else
	{
		int MinDem, MaxDem;
		float Range = (float)Para.DamageRange * 0.01f;
		MinDem = (int)ceilf((float)ItemStat.Damage *( 1.0f - Range));
		MaxDem = (int)ceilf((float)ItemStat.Damage *( 1.0f + Range));

		_stprintf_s(Buf, BufSize, _T("데미지 : %5d ~ %5d(%+d)"), CurStat.MinDamage + MinDem, CurStat.MaxDamage+ MaxDem,ItemStat.Damage);
		m_vecStatusString[STRING_DEM] = Buf;
	}

	if (0 == ItemStat.Defense)
	{
		_stprintf_s(Buf, BufSize, _T("방어력 : %4d(-)"), CurStat.Defense);
		m_vecStatusString[STRING_DEF] = Buf;
	}
	else
	{
		_stprintf_s(Buf, BufSize, _T("방어력 : %4d(%+d)"), CurStat.Defense + ItemStat.Defense,ItemStat.Defense);
		m_vecStatusString[STRING_DEF] = Buf;
	}

	if (0 == ItemStat.Critical)
	{
		_stprintf_s(Buf, BufSize, _T("크리티컬 : %3d%%(-)"), CurStat.Critical);
		m_vecStatusString[STRING_CRI] = Buf;
	}
	else
	{
		_stprintf_s(Buf, BufSize, _T("크리티컬 : %3d%%(%+d%%)"), CurStat.Critical + ItemStat.Critical, ItemStat.Critical);
		m_vecStatusString[STRING_CRI] = Buf;
	}

	if (0 == ItemStat.CriticalDamage)
	{
		_stprintf_s(Buf, BufSize, _T("크리티컬 데미지 : %3d%%(-)"), CurStat.CriticalDamage);
		m_vecStatusString[STRING_CRIDEM] = Buf;
	}
	else
	{
		_stprintf_s(Buf, BufSize, _T("크리티컬 데미지 : %3d%%(%+d%%)"), CurStat.CriticalDamage + ItemStat.CriticalDamage, ItemStat.CriticalDamage);
		m_vecStatusString[STRING_CRIDEM] = Buf;
	}

	if (0 == ItemStat.Avoidability)
	{
		_stprintf_s(Buf, BufSize, _T("회피 : %3d%%(-)"), CurStat.Avoidability);
		m_vecStatusString[STRING_AVD] = Buf;
	}
	else
	{
		_stprintf_s(Buf, BufSize, _T("회피 : %3d%%(%+d%%)"), CurStat.Avoidability + ItemStat.Avoidability, ItemStat.Avoidability);
		m_vecStatusString[STRING_AVD] = Buf;
	}

}
void CCharacterStatus::UIFontRender()
{
	tstring Head  = _T("체력");

	COMRESMGR->DrawFont(_T("캐릭라인폰트"), m_vecStatusString[STRING_LV], POINT{ -240 ,-130 }, 2.3f, D3DCOLOR_ARGB(255, 255, 220, 0));
	
	COMRESMGR->DrawFont(_T("볼드굴림"), m_vecStatusString[STRING_STR], POINT{ -250 ,-200 }, 2.3f, D3DCOLOR_ARGB(255, 255, 255, 255));
	COMRESMGR->DrawFont(_T("볼드굴림"), m_vecStatusString[STRING_DEX], POINT{ -250 ,-240 }, 2.3f, D3DCOLOR_ARGB(255, 255, 255, 255));
	COMRESMGR->DrawFont(_T("볼드굴림"), m_vecStatusString[STRING_INT], POINT{ -250 ,-280 }, 2.3f, D3DCOLOR_ARGB(255, 255, 255, 255));
	COMRESMGR->DrawFont(_T("볼드굴림"), m_vecStatusString[STRING_LUK], POINT{ -250 ,-320 }, 2.3f, D3DCOLOR_ARGB(255, 255, 255, 255));

	COMRESMGR->DrawFont(_T("볼드굴림"), m_vecStatusString[STRING_HP], POINT{ 210 ,-160 }, 2.3f, D3DCOLOR_ARGB(255, 255, 255, 255));
	COMRESMGR->DrawFont(_T("볼드굴림"), m_vecStatusString[STRING_MP], POINT{ 440 ,-160 }, 2.3f, D3DCOLOR_ARGB(255, 255, 255, 255));
	COMRESMGR->DrawFont(_T("볼드굴림"), m_vecStatusString[STRING_DEM], POINT{ 210 ,-192 }, 2.3f, D3DCOLOR_ARGB(255, 255, 255, 255));
	COMRESMGR->DrawFont(_T("볼드굴림"), m_vecStatusString[STRING_DEF], POINT{ 210 ,-224 }, 2.3f, D3DCOLOR_ARGB(255, 255, 255, 255));
	COMRESMGR->DrawFont(_T("볼드굴림"), m_vecStatusString[STRING_CRI], POINT{ 210 ,-256 }, 2.3f, D3DCOLOR_ARGB(255, 255, 255, 255));
	COMRESMGR->DrawFont(_T("볼드굴림"), m_vecStatusString[STRING_CRIDEM], POINT{ 210 ,-288 }, 2.3f, D3DCOLOR_ARGB(255, 255, 255, 255));
	COMRESMGR->DrawFont(_T("볼드굴림"), m_vecStatusString[STRING_AVD], POINT{ 210 ,-320 }, 2.3f, D3DCOLOR_ARGB(255, 255, 255, 255));


	//COMRESMGR->DrawFont(_T("볼드굴림"), _T("체력 : 999(-)"), POINT{ -250 ,-200 }, 2.3f, D3DCOLOR_ARGB(255, 255, 255, 255));
	//COMRESMGR->DrawFont(_T("볼드굴림"), _T("민첩 : 863(+200)"), POINT{ -250 ,-240 }, 2.3f, D3DCOLOR_ARGB(255, 255, 255, 255));
	//COMRESMGR->DrawFont(_T("볼드굴림"), _T("지력 : 12(-)"), POINT{ -250 ,-280 }, 2.3f, D3DCOLOR_ARGB(255, 255, 255, 255));
	//COMRESMGR->DrawFont(_T("볼드굴림"), _T("행운 : 763(-)"), POINT{ -250 ,-320 }, 2.3f, D3DCOLOR_ARGB(255, 255, 255, 255));

	//COMRESMGR->DrawFont(_T("볼드굴림"), _T("HP : 10000/10000(-)"), POINT{ 210 ,-160 }, 2.3f, D3DCOLOR_ARGB(255, 255, 255, 255));
	//COMRESMGR->DrawFont(_T("볼드굴림"), _T("MP : 200/200(-)"), POINT{ 410 ,-160 }, 2.3f, D3DCOLOR_ARGB(255, 255, 255, 255));
	//COMRESMGR->DrawFont(_T("볼드굴림"), _T("데미지 : 1000 ~ 2000 (-)"), POINT{ 210 ,-192 }, 2.3f, D3DCOLOR_ARGB(255, 255, 255, 255));
	//COMRESMGR->DrawFont(_T("볼드굴림"), _T("방어력 : 200 (+55)"), POINT{ 210 ,-224 }, 2.3f, D3DCOLOR_ARGB(255, 255, 255, 255));
	//COMRESMGR->DrawFont(_T("볼드굴림"), _T("크리티컬 : 20 % (+5%)"), POINT{ 210 ,-256 }, 2.3f, D3DCOLOR_ARGB(255, 255, 255, 255));
	//COMRESMGR->DrawFont(_T("볼드굴림"), _T("크리티컬 데미지 : 200% (+30%)"), POINT{ 210 ,-288 }, 2.3f, D3DCOLOR_ARGB(255, 255, 255, 255));
	//COMRESMGR->DrawFont(_T("볼드굴림"), _T("회피 : 8% (-)"), POINT{ 210 ,-320 }, 2.3f, D3DCOLOR_ARGB(255, 255, 255, 255));


}
void CCharacterStatus::StatusOn()
{
	ACTOR->On();
	m_CurCharacterRen->ActorOn();
	m_Parameter->ParameterOn();
}
void CCharacterStatus::StatusOn(const CharacterIndex& _Index) 
{
	ACTOR->On();
	m_CurCharacterRen->ActorOn();
	m_Parameter->ParameterOn();
	ChangeCharacterStatus(_Index);

	for (size_t i = 0; i < m_vecGauge.size(); i++)
	{
		if (nullptr != m_vecGauge[i])
			m_vecGauge[i]->GaugeOn();
	}
}
void CCharacterStatus::StatusOff() 
{
	ACTOR->Off();
	m_CurCharacterRen->ActorOff();
	m_Parameter->ParameterOff();

	for (size_t i = 0; i < m_vecGauge.size(); i++)
	{
		if (nullptr != m_vecGauge[i])
			m_vecGauge[i]->GaugeOff();
	}
}