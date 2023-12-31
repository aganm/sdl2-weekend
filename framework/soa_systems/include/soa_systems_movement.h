#pragma once

/**
 * @file
 * @brief Movement systems.
 */

#include <types/primitive.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct soa_slot_t soa_slot_t;
typedef struct soa_position soa_position2;
typedef struct soa_rotation soa_rotation1;
typedef struct soa_movement soa_movement2;
typedef struct soa_speed soa_speed;
typedef struct soa_velocity soa_velocity2;

void soa_movement_to_velocity(
	const soa_movement2 *e_movement,
	const soa_speed *e_speed,
	soa_velocity2 *e_velocity,
	const usize entity_count);

void soa_follow_one_target(
	soa_movement2 *f_movement,
	const soa_position2 *f_position,
	const soa_speed *f_speed,
	const usize follower_count,
	const soa_position2 *t_position,
	const soa_slot_t target_slot);

void soa_forward_movement_from_rotation(
	soa_movement2 *e_movement,
	const soa_rotation1 *e_rotation,
	const usize entity_count);

#ifdef __cplusplus
}
#endif
