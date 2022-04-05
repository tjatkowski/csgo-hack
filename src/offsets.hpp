#pragma once
#include "includes.hpp"

//offsets values. Can change upon game's update
namespace offset {
	constexpr uintptr_t local_player = 0xDB65DC;
	constexpr uintptr_t entity_list = 0x4DD245C;
	namespace player {
		constexpr uintptr_t team = 0xF4;
		constexpr uintptr_t crosshair_id = 0x11838;
	}
}
