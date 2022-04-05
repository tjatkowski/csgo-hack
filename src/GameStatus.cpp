#include "GameStatus.hpp"
#include "offsets.hpp"
#include "memory_access.hpp"

GameStatus::GameStatus()
{
	module_base = (DWORD)GetModuleHandle("client.dll");
}

uintptr_t GameStatus::get_local_player()
{
	return read_memory<uintptr_t>(module_base + offset::local_player);
}

uintptr_t GameStatus::get_player(int index)
{
	return read_memory<uintptr_t>(module_base + offset::entity_list + index * 0x10);
}

int GameStatus::get_team(uintptr_t player)
{
	return read_memory<int>(player + offset::player::team);
}

int GameStatus::get_crosshair_id(uintptr_t player)
{
	return read_memory<int>(player + offset::player::crosshair_id);
}
