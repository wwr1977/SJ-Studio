#pragma once


class CGameString
{
public:
	static const int StringToInteger(const tstring& _string);
	static const float StringToFloat(const tstring& _string);
	static void ChangeFloatString(tstring& _String);
	
private:
	CGameString() {}
	~CGameString() {}
};

