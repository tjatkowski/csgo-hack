#pragma once

#include "includes.hpp"

//holds information about current process
class GameStatus {
public:
	GameStatus();

	uintptr_t get_local_player();
	uintptr_t get_player(int index);
	int get_team(uintptr_t player);
	int get_crosshair_id(uintptr_t player);
private:
	uintptr_t module_base;
};