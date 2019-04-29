#pragma once


class GAMEVEC 
{
public:
	static const D3DXVECTOR2 LEFT2;
	static const D3DXVECTOR2 RIGHT2;
	static const D3DXVECTOR2 UP2;
	static const D3DXVECTOR2 DOWN2;
	static const D3DXVECTOR2 ZERO2;
	static const D3DXVECTOR2 ONE2;

	static const D3DXVECTOR3 LEFT;
	static const D3DXVECTOR3 RIGHT;
	static const D3DXVECTOR3 UP;
	static const D3DXVECTOR3 DOWN;
	static const D3DXVECTOR3 LEFTUP;
	static const D3DXVECTOR3 LEFTDOWN;
	static const D3DXVECTOR3 RIGHTUP;
	static const D3DXVECTOR3 RIGHTDOWN;
	static const D3DXVECTOR3 FORWORD;
	static const D3DXVECTOR3 BACK;
	static const D3DXVECTOR3 ZERO;
	static const D3DXVECTOR3 ONE;

	static const D3DXVECTOR3 GetPosition(float _Rad);

private:
	GAMEVEC() {}
	~GAMEVEC() {}
};

