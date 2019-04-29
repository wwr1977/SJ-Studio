#pragma once
#include "FontRenderer.h"

class CScriptRenderer : public CFontRenderer
{
public:
	enum SCRIPTLINEMODE
	{
		AUTOLINE,	// 자동으로 개행을 발생 (한줄에 들어가는 폰트의 갯수의 기준이 필요)
		SCRIPTLINE	// 스크립트 내의 있는 특수 문자(||)를 인식하여 개행을 발생한다.
	}m_LineMode;

	enum TYPING_MODE
	{
		NOTIME_TYPING,			// 한번에 모든 글자를 띄운다.
		TIME_TYPING,			// 정해진 시간 간격을 기준으로 한글자씩 찍는다.
	}m_TypingMode;

private:
	tstring							m_ScriptString;
	vector<tstring>					m_SeparateScript;
	vector<tstring>					m_vecRenderString;
	int								m_LineInterval;
	
	//한줄에 들어가는 폰트의 갯수의 기준(이 변수를 기준으로 행을 나눈다)
	UINT							m_StdFontCount;

	float							m_fAccTime;
	float							m_fTypingTime;
	int								m_iCurTypingFont;
	int								m_iCurTypingLine;
	bool							m_bTyping;
public:
	void Update();
	void Render();

public:
	void SetAutoMode(const UINT& _FontCount);
	void SetTimeTyping(const TYPING_MODE& _Mode);
	void SetTypingTime(const float& _Time);
	void StandbyTyping();
	void SetScriptMode();
	void SetLineInterval(const int& _Interval);
	void SetScriptString(const tstring& _String);
	void CreateRenderString();
	const bool IsTyping();
	void SkipTyping();

public:
	const size_t GetCurScriptCount();
public:
	CScriptRenderer();
	~CScriptRenderer();
};

