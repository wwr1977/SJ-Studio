#pragma once
#include"DXHeader.h"

struct COLORVTX 
{
	Vec4		Pos;
	FColor		Color;
	Vec2		TexCoord;

	static UINT TypeSize() 
	{
		return (UINT)sizeof(COLORVTX);
	}
};

struct BASICVTX 
{
	Vec3			Pos;
	Vec3			Normal;
	Vec2			TexCoord;

	static UINT TypeSize()
	{
		return (UINT)sizeof(BASICVTX);
	}
};
/*
	WORD _1, _2, _3;
*/
struct IDX16 
{
	WORD _1, _2, _3;

public:
	static UINT MemSize()
	{
		return (UINT)sizeof(WORD);
	}
	static DXGI_FORMAT Format() 
	{
		return DXGI_FORMAT_R16_UINT;
	}

public:
	IDX16() :_1(0), _2(0), _3(0) {}
	IDX16(const WORD& __1, const WORD& __2, const WORD& __3)
		:_1(__1), _2(__2) , _3(__3) {}
};

struct IDX32
{
	DWORD _1, _2, _3;

public:
	static UINT MemSize()
	{
		return (UINT)sizeof(DWORD);
	}
	static DXGI_FORMAT Format()
	{
		return DXGI_FORMAT_R32_UINT;
	}
public:
	IDX32() :_1(0), _2(0), _3(0) {}
	IDX32(const DWORD& __1, const DWORD& __2, const DWORD& __3)
		:_1(__1), _2(__2), _3(__3) {}
};



typedef class DXVTX 
{
public:
	static COLORVTX				ColorVtx[4];
	static BASICVTX				BasicVtx[4];
	static BASICVTX				CalBasicVtx[4];

	static IDX16				BasicIdx[2];
	static IDX32				BasicIdx32[2];


}DXVtx;
