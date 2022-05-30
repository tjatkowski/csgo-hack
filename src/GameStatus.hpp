#pragma once

#include "includes.hpp"
#include "offsets.hpp"

#define STR_MERGE_IMPL(a, b) a##b
#define STR_MERGE(a, b) STR_MERGE_IMPL(a, b)
#define MAKE_PAD(size) STR_MERGE(_pad, __COUNTER__)[size]
#define DEFINE_MEMBER_N(type, name, offset) struct {unsigned char MAKE_PAD(offset); type name;}


class Entity {
public:
	Vec3 get_head_position();
	union {
		DEFINE_MEMBER_N(bool, isDormant, offset::player::isDormant);
		DEFINE_MEMBER_N(int, health, offset::player::health);
		DEFINE_MEMBER_N(Vec3, vec_origin, offset::player::vec_origin);
		DEFINE_MEMBER_N(int, team, offset::player::team);
		DEFINE_MEMBER_N(int, crosshair_id, offset::player::crosshair_id);
		DEFINE_MEMBER_N(Vec3, view_offset, offset::player::view_offset);
		DEFINE_MEMBER_N(uintptr_t, bone_matrix, offset::player::bone_matrix);
	};
};

class EntityListElement {
public:
	struct Entity* entity;
	char padding[12];
};

class EntityList {
public:
	EntityListElement entities[32];
};

class GameStatus {
public:
	GameStatus();

	void update();

	bool check_if_entity_valid(Entity* entity);

	bool world_to_screen(Vec3 pos, Vec2& screen);

	void aimAt(Vec3 target);

	float getAimOffset(Vec3 target);
//private:
	uintptr_t module_base;
	uintptr_t engine_base;
	Entity* local_entity;
	EntityList* entity_list;
	float view_matrix[16];

	
};