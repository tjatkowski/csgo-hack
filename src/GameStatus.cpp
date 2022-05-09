#include "GameStatus.hpp"
#include "offsets.hpp"
#include "memory_access.hpp"

#include "dummyDevice.hpp"

GameStatus::GameStatus()
{
	module_base = (DWORD)GetModuleHandle("client.dll");

	entity_list = (EntityList*)(module_base + offset::entity_list);
	local_entity = entity_list->entities[0].entity;
}

void GameStatus::update()
{
	memcpy(&view_matrix, (PBYTE*)(module_base + offset::view_matrix), sizeof(view_matrix));
}

bool GameStatus::check_if_entity_valid(Entity* entity)
{
	if (!entity)
		return false;
	if (entity == local_entity)
		return false;
	if (entity->health <= 0)
		return false;
	if (entity->isDormant)
		return false;
	return true;
}

bool GameStatus::world_to_screen(Vec3 pos, Vec2& screen) {
	Vec4 clipCoords;
	clipCoords.x = pos.x * view_matrix[0 ] + pos.y * view_matrix[1 ] + pos.z * view_matrix[2 ] + view_matrix[3 ];
	clipCoords.y = pos.x * view_matrix[4 ] + pos.y * view_matrix[5 ] + pos.z * view_matrix[6 ] + view_matrix[7 ];
	clipCoords.z = pos.x * view_matrix[8 ] + pos.y * view_matrix[9 ] + pos.z * view_matrix[10] + view_matrix[11];
	clipCoords.w = pos.x * view_matrix[12] + pos.y * view_matrix[13] + pos.z * view_matrix[14] + view_matrix[15];

	if (clipCoords.w < 0.1f)
		return false;

	Vec3 NDC;
	NDC.x = clipCoords.x / clipCoords.w;
	NDC.y = clipCoords.y / clipCoords.w;
	NDC.z = clipCoords.z / clipCoords.w;

	screen.x = (windowWidth / 2 * NDC.x) + (NDC.x + windowWidth / 2);
	screen.y = -(windowHeight / 2 * NDC.y) + (NDC.y + windowHeight / 2);
	return true;
}