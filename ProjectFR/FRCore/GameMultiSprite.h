#pragma once
#include "ObjBase.h"
#include"GameSprite.h"

class CGameTexture;
//class CGameSprite;
class CGameMultiSprite : public CObjBase
{
private:
	tstring									m_TexKey;
	Vec2									m_StartPos;
	Vec2									m_MultiSpriteSize;
	// ��Ƽ ��������Ʈ�� �ڸ��� ���ϴ� �������� 0��° ��ġ�� ���°��
	// �� ������ ���� ���Ѵ�(�ִϸ��̼� ���������Ӱ� ������)
	int										m_StartFrame;
	int										m_ImageCount;
	int										m_XCount;
	int										m_YCount;
	Vec2									m_SpritePivot;
	LPDIRECT3DDEVICE9						m_pDevice;
	vector<SPTR<CGameSprite>>				m_VecSprite;
	ANISPRITETYPE							m_AniSpriteType;


	

public:
	SPTR<CGameSprite> operator[](const size_t& _Index);
	bool isInit()
	{
		if (nullptr == m_pDevice)
		{
			return FALSE;
		}
		return TRUE;
	}
	void PushSprite(CGameSprite* _Sprite);
	SPTR<CGameSprite> GetSprite(size_t _Index);
	void VecReserve(int _VecSize)
	{
		if (0 <= _VecSize)
		{
			return;
		}
		m_VecSprite.reserve(_VecSize);
	}
	size_t GetSpriteCount()
	{
		return m_VecSprite.size();
	}
	int GetSpriteLastIndex() 
	{
		return (int)m_VecSprite.size() - 1;
	}
	void SetMultiSpriteSize(const Vec2& _Size)
	{
		m_MultiSpriteSize = _Size;
	}
	const Vec2 GetMultiSpriteSize() const
	{
		return m_MultiSpriteSize;
	}
	const float GetMultiSpriteAspect() 
	{
		return m_MultiSpriteSize.x / m_MultiSpriteSize.y;
	}
	void SetSpritePivot(const Vec2& _Pivot);

	void SetStartPos(const Vec2& _StartPos);
	Vec2 GetStartPos();

	const Vec2 GetSpriteSize(const int& _Index = 0);

	const Vec2 GetSpritePivot();

	void SetCountData(int _XCount, int _YCount, int _ImageCount);
	int GetXCount();
	int GetYCount();
	int GetImageCount();

	void SetSpriteType(ANISPRITETYPE _Type);
	ANISPRITETYPE GetSpriteType();

	void SetTexKey(const tstring& _TexKey);
	tstring GetTexKey();
	const TCHAR* GetTexKey_str();

	// ���� ��Ƽ��������Ʈ�� �ڸ��� ������������ �ٸ���� �� �Լ� ȣ��
	void SetStartFrame(const int& _StartFrame);
	int GetStartFrame();
	
	void CopySpriteVec(vector<SPTR<CGameSprite>>* _vecSprite);
	void AddVecSprite(const vector<SPTR<CGameSprite>>& _vecSprite);
	void AddSprite(const SPTR<CGameSprite> _Sprite);
	void MergeMultiSprtie(SPTR<CGameMultiSprite> _MultiSprite);
	
public:
	static CGameMultiSprite* CreateMultiSprite(LPDIRECT3DDEVICE9 _pDevice);

public:
	CGameMultiSprite(LPDIRECT3DDEVICE9 _Device);
	~CGameMultiSprite();
};

