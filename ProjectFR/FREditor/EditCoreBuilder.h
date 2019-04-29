#pragma once
#include<CoreBuilder.h>

class CEditCoreBuilder :public CCoreBuilder
{
private:
	HWND			m_hBaseWnd;

public:
	bool CoreBuild();
	void PathSetting();


public:
	CEditCoreBuilder(HWND _BaseWnd);
	~CEditCoreBuilder();
};

