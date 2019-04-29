#pragma once
#include <Logic.h>


struct FontSizeData 
{
	// ���� ũ��
	float					ImageMag;
	// �ڰ� ����
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


	//	ù���� ���ڰ� �����ϴ� ����
	Vec2										m_StartNumPivot;
	// ���� On�� NumberUI Sprite�� ������ ���� �����ϴ� ���� 
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
	
	// ���� ���ڰ� ���� ���� ũ���� n�ۼ�Ʈ ������ �ٴ��� ����
	// 0.9f => ���������� ũ���� 90�ۼ�Ʈ ���� ������ �پ��
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

