#pragma once
#include "includes.hpp"

//offsets values. Can change upon game's update
namespace offset {
	constexpr uintptr_t local_player = 0xDB75DC; //dwLocalPlayer 
	constexpr uintptr_t entity_list = 0x4DD344C; //dwEntityList
	constexpr uintptr_t view_matrix = 0x4DC4D64; //dwViewMatrix
	namespace player {
		constexpr uintptr_t isDormant = 0xED; //m_bDormant
		constexpr uintptr_t health = 0x100; //m_iHealth 
		constexpr uintptr_t vec_origin = 0x138; //m_vecOrigin 
		constexpr uintptr_t team = 0xF4; //m_iTeamNum
		constexpr uintptr_t crosshair_id = 0x11838; //m_iCrosshairId 
	}
}
