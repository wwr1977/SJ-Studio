#include"DXVertex.h"

COLORVTX	DXVTX::ColorVtx[4] = {  { Vec4(-0.5f,  0.5f, 0.0f, 1.0f)  , FColor::White ,Vec2(0.f, 0.f) }				// 왼쪽위
								,{ Vec4( 0.5f,  0.5f, 0.0f, 1.0f)  , FColor::White ,Vec2(1.f, 0.f) }				// 오른쪽위
								,{ Vec4(-0.5f, -0.5f, 0.0f, 1.0f)  , FColor::White ,Vec2(0.f, 1.f) }				// 왼쪽 아래
								,{ Vec4(0.5f, -0.5f, 0.0f, 1.0f)   , FColor::White ,Vec2(1.f, 1.f) } };				// 오른쪽 아래

BASICVTX	DXVTX::BasicVtx[4] = { { Vec3(-0.5f,  0.5f, 0.0f) ,Vec3(0.f, 0.f, 1.0f) ,Vec2(0.f, 0.f) }
							   ,{ Vec3( 0.5f,  0.5f, 0.0f) ,Vec3(0.f, 0.f, 1.0f), Vec2(1.f, 0.f) }		
							   ,{ Vec3(-0.5f, -0.5f, 0.0f) ,Vec3(0.f, 0.f, 1.0f), Vec2(0.f, 1.f) }		
							   ,{ Vec3(0.5f, -0.5f, 0.0f)  ,Vec3(0.f, 0.f, 1.0f), Vec2(1.f, 1.f) } };

BASICVTX	DXVTX::CalBasicVtx[4] = {   { Vec3(-0.5f,  0.5f, 0.0f) , Vec3(0.f, 0.f, 1.0f), Vec2(0.f, 0.f) }
									,{ Vec3(0.5f,  0.5f, 0.0f)  , Vec3(0.f, 0.f, 1.0f), Vec2(1.f, 0.f) }
									,{ Vec3(-0.5f, -0.5f, 0.0f) , Vec3(0.f, 0.f, 1.0f), Vec2(0.f, 1.f) }
									,{ Vec3(0.5f, -0.5f, 0.0f)  , Vec3(0.f, 0.f, 1.0f), Vec2(1.f, 1.f) } };


IDX16		DXVTX::BasicIdx[2] = { IDX16(0, 3, 2) , IDX16(0, 1, 3) };
IDX32		DXVTX::BasicIdx32[2] = { IDX32(0, 3, 2) , IDX32(0, 1, 3) };