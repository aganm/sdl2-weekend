#pragma once

#include <primitive_types.h>

typedef struct soa_slot_t soa_slot_t;
typedef struct soa_position2 soa_position2;

f32v2 get_one_position2(
	const soa_position2 *e_position,
	const soa_slot_t entity_slot);

void change_one_position2(
	soa_position2 *e_position,
	const soa_slot_t entity_slot,
	const f32v2 new_position);
