#include "stdafx.h"
#include "UserGlobalVar.h"

const D3DXVECTOR2 GAMEVEC::LEFT2 =	Vec2{ -1.0f, 0.0f };
const D3DXVECTOR2 GAMEVEC::RIGHT2 = Vec2{ 1.0f, 0.0f };
const D3DXVECTOR2 GAMEVEC::UP2 =	Vec2{ 0.0f, 1.0f };
const D3DXVECTOR2 GAMEVEC::DOWN2 =	Vec2{ 0.0f,-1.0f };
const D3DXVECTOR2 GAMEVEC::ZERO2 =	Vec2{ 0.0f, 0.0f };
const D3DXVECTOR2 GAMEVEC::ONE2 =	Vec2{ 1.0f, 1.0f };

const D3DXVECTOR3 GAMEVEC::ONE =	Vec3(1.0f, 1.0f, 1.0f);
const D3DXVECTOR3 GAMEVEC::ZERO =	Vec3(0.0f, 0.0f, 0.0f);
const D3DXVECTOR3 GAMEVEC::LEFT =	Vec3(-1.0f, 0.0f, 0.0f);
const D3DXVECTOR3 GAMEVEC::RIGHT =	Vec3(1.0f, 0.0f, 0.0f);
const D3DXVECTOR3 GAMEVEC::UP =		Vec3(0.0f, 1.0f, 0.0f);
const D3DXVECTOR3 GAMEVEC::LEFTUP = Vec3(-0.7071f, 0.7071f, 0.0f);
const D3DXVECTOR3 GAMEVEC::LEFTDOWN = Vec3(-0.7071f, -0.7071f, 0.0f);
const D3DXVECTOR3 GAMEVEC::RIGHTUP = Vec3(0.7071f, 0.7071f, 0.0f);
const D3DXVECTOR3 GAMEVEC::RIGHTDOWN = Vec3(0.7071f, -0.7071f, 0.0f);
const D3DXVECTOR3 GAMEVEC::DOWN =	Vec3(0.0f, -1.0f, 0.0f);
const D3DXVECTOR3 GAMEVEC::FORWORD = Vec3(0.0f, 0.0f, 1.0f);
const D3DXVECTOR3 GAMEVEC::BACK =	Vec3(0.0f, 0.0f, -1.0f);

const D3DXVECTOR3 GAMEVEC::GetPosition(float _Rad)
{
	return Vec3{ cosf(_Rad),sinf(_Rad),0.0f };
}
