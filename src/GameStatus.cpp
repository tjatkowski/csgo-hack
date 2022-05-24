#include "GameStatus.hpp"
#include "offsets.hpp"
#include "memory_access.hpp"

#include "dummyDevice.hpp"

#define PI 3.141

Vec3 Entity::get_head_position() {
	Vec3 head_position;
	head_position.x = *(float*)(bone_matrix + 0x30 * 8 + 0x0C);
	head_position.y = *(float*)(bone_matrix + 0x30 * 8 + 0x1C);
	head_position.z = *(float*)(bone_matrix + 0x30 * 8 + 0x2C);
	return head_position;
}

GameStatus::GameStatus()
{
	module_base = (DWORD)GetModuleHandle("client.dll");
	engine_base = (DWORD)GetModuleHandle("engine.dll");

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

void GameStatus::aimAt(Vec3 target) {
	Vec3* viewAngles = (Vec3*)(*(uint32_t*)(engine_base + offset::client_state) + offset::client_state_view_angles);

	Vec3 origin = local_entity->vec_origin;
	Vec3 view_offset = local_entity->view_offset;
	Vec3 my_pos;
	my_pos.x = origin.x + view_offset.x;
	my_pos.y = origin.y + view_offset.y;
	my_pos.z = origin.z + view_offset.z;

	Vec3 delta;
	delta.x = target.x - my_pos.x;
	delta.y = target.y - my_pos.y; 
	delta.z = target.z - my_pos.z;

	float delta_length = sqrt(delta.x * delta.x + delta.y * delta.y + delta.z * delta.z);

	float pitch = -asin(delta.z / delta_length) * (180 / PI);
	float yaw = atan2(delta.y, delta.x) * (180 / PI);

	if (pitch >= -89 && pitch <= 89 && yaw >= -180 && yaw <= 180) {
		viewAngles->x = pitch;
		viewAngles->y = yaw;
	}
	
}
