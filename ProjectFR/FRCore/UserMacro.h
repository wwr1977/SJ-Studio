#pragma once


#define MAXPATH 512
#define SAFE_DELETE(p) if(nullptr != p) { delete p; p = nullptr; }
#define SAFE_DELETE_ARR(p) if(nullptr != p) { delete[] p; p = nullptr; }
#define SAFE_RELEASE_PTR(p) if(nullptr != p) { p->DestroyObject(); p = nullptr; }
#define ROOTTWO 1.414213f
#define ROOTTHREE 1.732050f
#define ONETHIRD 0.3333333f

#define ATK(NAME) _T(##NAME##"_atk")
#define DATK(NAME) _T(##NAME##"_DoubleAtk")
#define DEAD(NAME) _T(##NAME##"_dead")
#define DYING(NAME) _T(##NAME##"_dying")
#define IDLE(NAME) _T(##NAME##"_idle")
#define JUMP(NAME) _T(##NAME##"_jump")
#define LIMIT(NAME) _T(##NAME##"_limit_atk")
#define MATK(NAME)	_T(##NAME##"_magic_atk")
#define MSTAND(NAME) _T(##NAME##"_magic_standby")
#define MOVE(NAME) _T(##NAME##"_move")
#define STAND(NAME) _T(##NAME##"_standby")
#define WIN(NAME) _T(##NAME##"_win")
#define WINBE(NAME) _T(##NAME##"_win_before")
#define ICON(NAME) _T("unit_icon_"##NAME##)
#define FRAME(NAME) _T("Frame_"##NAME##)
#define ILLUST(NAME) _T("unit_ills_"##NAME##)


// 특수효과 플래그
// 지진 타입 |  세부 사항
//  00 00		  00 00 			
// 효과 타입 |	세부 사항
//    00	 |  00 00 00 

#define SCENE_EFFECT_EARTHQUAKE				0x10000000L
#define SCENE_EFFECT_BLACKOUT				0x20000000L
#define SCENE_EFFECT_FLASH					0x21000000L
#define SCENE_EFFECT_FADE_IN				0x40000000L
#define SCENE_EFFECT_FADE_OUT				0x41000000L
#define SCENE_EFFECT_ZOOM_IN				0x80000000L
#define SCENE_EFFECT_ZOOM_OUT				0x81000000L

#define EARTHQUAKE_UPDOWN					0x00000000L
#define EARTHQUAKE_LEFTRIGHT				0x00010000L
#define EARTHQUAKE_CIRCLESPACE				0x00020000L
#define EARTHQUAKE_RECTSPACE				0x00040000L

#define EARTHQUAKE_CONSISTENT				0x00000000L
#define EARTHQUAKE_INCREASE					0x00000100L
#define EARTHQUAKE_DECREASE					0x00000200L

#define FADE_COLOR_BLACK					0x00000000L	
#define FADE_COLOR_WHITE					0x00000100L
#define FADE_COLOR_RED						0x00000200L
#define FADE_COLOR_GREEN					0x00000400L
#define FADE_COLOR_BLUE						0x00000800L


#define MAXINDEX							0xFFFFFFFFL

