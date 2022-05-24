#pragma once
#include "includes.hpp"

//offsets values. Can change upon game's update
namespace offset {
	constexpr uintptr_t local_player = 14394812; //dwLocalPlayer 
	constexpr uintptr_t entity_list = 81619420; //dwEntityList
	constexpr uintptr_t view_matrix = 81560308; //dwViewMatrix
	constexpr uintptr_t client_state = 0x58BFDC; //dwClientState 
	constexpr uintptr_t client_state_view_angles = 0x4D90; //dwClientState_ViewAngles 
	namespace player {
		constexpr uintptr_t isDormant = 0xED; //m_bDormant
		constexpr uintptr_t health = 0x100; //m_iHealth 
		constexpr uintptr_t view_offset = 0x108; //m_vecViewOffset 
		constexpr uintptr_t vec_origin = 0x138; //m_vecOrigin 
		constexpr uintptr_t team = 0xF4; //m_iTeamNum
		constexpr uintptr_t crosshair_id = 0x11838; //m_iCrosshairId 
		constexpr uintptr_t bone_matrix = 0x26A8; //m_dwBoneMatrix 
	}
}
