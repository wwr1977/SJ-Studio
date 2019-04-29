#pragma once
#include <Logic.h>


struct FontSizeData 
{
	// 글자 크기
	float					ImageMag;
	// 자간 배율
	float					NumInterval;

public:
	FontSizeData() :ImageMag(0.0f),NumInterval(0.0f){}
	FontSizeData(const float& _ImageMag,const float& _Interval):ImageMag(_ImageMag),NumInterval(_Interval) {}
	~FontSizeData() {}
};

class CNumberUI : public CLogic
{
protected:
	static map<tstring, FontSizeData>					m_mapFontSizeData;

public:
	static void InitFontNumberData();

protected:
	vector<SPTR<CFixRenderer>>					m_vecNumUI;
	int											m_iCurNumber;
	tstring										m_CurFontName;

private:
	SPTR<CGameMultiSprite>						m_NumberSprite;
	float										m_NumInterval;
	int											m_iPrevNumber;
	int											m_iNumberCount;


	//	첫번쨰 숫자가 시작하는 지점
	Vec2										m_StartNumPivot;
	// 현재 On된 NumberUI Sprite의 길이의 합을 저장하는 변수 
	Vec2										m_CurNumberFontSize;
	Vec2										m_CurFirstFontHSize;

protected:
	void SyncNumberSpacing();

private:
	int CalNumberSize(const int& _Value);

public:
	void Init();
	void Update();

	virtual void InitData(const tstring& _Data);
	BOOL SetNumber(const int& _Value);
	void OnNumberUI();
	void AllOffNumberUI();
	void SetImageScale(const float& _Scale);
	void SetCustomSize(const Vec2& _CustomSize);
	void SetCustomSize(const float& _X, const float& _Y);
	void SetRenPivot(const Vec2& _Pivot);
	void SetPos(const Vec2& _Pos);
	void SetPos(const Vec3& _Pos);
	void SetMiddlePos(const Vec2& _MiddlePos);
	void SetCurFontName(const tstring& _FontName);
	
	// 다음 숫자가 이전 숫자 크기의 n퍼센트 다음에 붙는지 설정
	// 0.9f => 이전숫자의 크기의 90퍼센트 영역 다음에 붙어라
	void SetNumInterval(const float& _Interval);
	void ChangeNumberSprite(const tstring& _SpriteKey);
	void SettingSpriteSize(const tstring& _SpriteKey);
	void SetAlpha(const float& _Alpha);
	
protected:
	void FontRenderOff();
	void FontRenderOn();

public:
	CNumberUI();
	~CNumberUI();
};

