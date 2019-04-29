#include "stdafx.h"
#include "ScriptRenderer.h"


CScriptRenderer::CScriptRenderer()
	:m_ScriptString(_T("")), m_LineInterval(0)
	, m_LineMode(SCRIPTLINE), m_TypingMode(NOTIME_TYPING)
	, m_fAccTime(0.f), m_fTypingTime(0.1f)
	, m_iCurTypingFont(0), m_iCurTypingLine(0)
	, m_bTyping(false)
{
	m_SeparateScript.clear();
	m_vecRenderString.clear();
}


CScriptRenderer::~CScriptRenderer()
{
	m_SeparateScript.clear();
	m_vecRenderString.clear();
}
void CScriptRenderer::Update()
{
	if (TIME_TYPING != m_TypingMode || false == m_bTyping )
		return;

	if (m_iCurTypingLine >= m_SeparateScript.size() &&
		m_iCurTypingFont >= m_SeparateScript[m_iCurTypingLine].size())
		return;

	if (m_fAccTime >= m_fTypingTime)
	{
		m_fAccTime = 0.f;
		
		m_vecRenderString[m_iCurTypingLine].push_back(m_SeparateScript[m_iCurTypingLine][m_iCurTypingFont]);
		++m_iCurTypingFont;

		if (m_iCurTypingFont >= m_SeparateScript[m_iCurTypingLine].size()) 
		{
			m_iCurTypingFont = 0;
			++m_iCurTypingLine;

			// 모든 스크립트를 전부다 랜더링 스트링에 집어넣음
			if (m_iCurTypingLine >= m_SeparateScript.size())
			{
				m_bTyping = false;
			}
		}

		return;
	}

	m_fAccTime += DELTATIME;
}
void CScriptRenderer::Render()
{
	if (true == m_vecRenderString.empty())
		return;

	size_t RenderCount = m_vecRenderString.size();
	Vec3 ActorPos = TRANS->GetPos() + Vec3(m_RenPivot.x, m_RenPivot.y, 0.0f);

	for (size_t i = 0; i < RenderCount; i++)
	{
		switch (m_CurMode)
		{
		case CFontRenderer::START_LEFT:
		{
			POINT Pos = POINT({ (long)ActorPos.x + m_LeftStartPivot.x , (long)ActorPos.y + m_LeftStartPivot.y  - (long)(m_LineInterval*i) });
			COMRESMGR->DrawFont(m_FontName, m_vecRenderString[i], Pos, m_fFontMag, m_FontColor);
		}
		break;
		case CFontRenderer::START_MIDDLE:
			COMRESMGR->DrawFont(m_FontName, m_vecRenderString[i], ActorPos + m_MiddleStartPivot + Vec3{0.f,(float)(-m_LineInterval*i) ,0.f}, m_fFontMag, m_FontColor);
			break;
		case CFontRenderer::START_RIGHT:
		{
			POINT Pos = POINT({ (long)ActorPos.x + m_LeftStartPivot.x , (long)ActorPos.y + m_LeftStartPivot.y - (long)(m_LineInterval*i) });
			COMRESMGR->DrawFont_RightStart(m_FontName, m_vecRenderString[i], Pos, m_fFontMag, m_FontColor);
		}
		break;
		}


	}


}
void CScriptRenderer::SetAutoMode(const UINT& _FontCount)
{
	m_LineMode = AUTOLINE;
	m_StdFontCount = _FontCount;

}
void CScriptRenderer::SetTimeTyping(const TYPING_MODE& _Mode)
{
	m_TypingMode = _Mode;
}
void CScriptRenderer::SetTypingTime(const float& _Time)
{
	m_TypingMode = TIME_TYPING;
	m_fTypingTime = _Time;
}
void CScriptRenderer::StandbyTyping() 
{
	m_bTyping = true;
	m_fAccTime = 0;
	m_iCurTypingFont = 0;
	m_iCurTypingLine = 0;
}
void CScriptRenderer::SetScriptMode()
{
	m_LineMode = SCRIPTLINE;
}
void CScriptRenderer::SetLineInterval(const int& _Interval)
{
	m_LineInterval = _Interval;
}
void CScriptRenderer::SetScriptString(const tstring& _String)
{
	m_ScriptString = _String;

	CreateRenderString();
}
void CScriptRenderer::CreateRenderString()
{
	m_SeparateScript.clear();
	m_vecRenderString.clear();

	if (_T("") == m_ScriptString)
		return;

	switch (m_LineMode)
	{
	case CScriptRenderer::AUTOLINE: 
	{
		if (0 >= m_StdFontCount)
			return;

	}
		return;
	case CScriptRenderer::SCRIPTLINE: 
	{
		tstring Temp = m_ScriptString;
		size_t Index = Temp.find_first_of(_T("||"));
		size_t Count = 0;

		while(-1 != Index )
		{
			m_SeparateScript.push_back(_T(""));
			m_vecRenderString.push_back(_T(""));
		
			m_SeparateScript[Count++].assign(Temp, 0, Index );
			Temp.replace(0, Index + 2, _T(""));
			Index = Temp.find_first_of(_T("||"));
		}

		if (Temp != _T("")) 
		{
			m_SeparateScript.push_back(Temp);
			m_vecRenderString.push_back(_T(""));
		}

		if (m_TypingMode == NOTIME_TYPING)
			m_vecRenderString.assign(m_SeparateScript.begin(), m_SeparateScript.end());
	
	}
		return;
	}
}
const bool CScriptRenderer::IsTyping()
{
	return m_bTyping;
}
void CScriptRenderer::SkipTyping()
{
	m_bTyping = false;

	m_vecRenderString.clear();
	m_vecRenderString.assign(m_SeparateScript.begin(), m_SeparateScript.end());

}
const size_t CScriptRenderer::GetCurScriptCount()
{
	return m_vecRenderString.size();
}