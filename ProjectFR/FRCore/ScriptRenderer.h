#pragma once
#include "FontRenderer.h"

class CScriptRenderer : public CFontRenderer
{
public:
	enum SCRIPTLINEMODE
	{
		AUTOLINE,	// �ڵ����� ������ �߻� (���ٿ� ���� ��Ʈ�� ������ ������ �ʿ�)
		SCRIPTLINE	// ��ũ��Ʈ ���� �ִ� Ư�� ����(||)�� �ν��Ͽ� ������ �߻��Ѵ�.
	}m_LineMode;

	enum TYPING_MODE
	{
		NOTIME_TYPING,			// �ѹ��� ��� ���ڸ� ����.
		TIME_TYPING,			// ������ �ð� ������ �������� �ѱ��ھ� ��´�.
	}m_TypingMode;

private:
	tstring							m_ScriptString;
	vector<tstring>					m_SeparateScript;
	vector<tstring>					m_vecRenderString;
	int								m_LineInterval;
	
	//���ٿ� ���� ��Ʈ�� ������ ����(�� ������ �������� ���� ������)
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

