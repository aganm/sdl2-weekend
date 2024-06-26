#include <math.h>
#include <soa.h>
#include <soa_components_movement.h>
#include <soa_components_physics.h>
#include <soa_components_transform.h>
#include <soa_systems_movement.h>

void soa_movement_to_velocity(
	const soa_movement2 *e_movement,
	const soa_speed *e_speed,
	soa_velocity2 *e_velocity,
	const usize entity_count)
{
	for (usize e = 0; e < entity_count; e++) {
		const f32 x = e_movement->x[e];
		const f32 y = e_movement->y[e];
		const f32 length = sqrtf(x * x + y * y);
		const f32 dir_x = length != 0.f ? x / length : 0.f;
		const f32 dir_y = length != 0.f ? y / length : 0.f;
		const f32 speed = e_speed->val[e];
		e_velocity->x[e] += dir_x * speed;
		e_velocity->y[e] += dir_y * speed;
	}
}

void soa_follow_one_target(
	soa_movement2 *f_movement,
	const soa_position2 *f_position,
	const soa_speed *f_speed,
	const usize follower_count,
	const soa_position2 *t_position,
	const soa_slot_t target_slot)
{
	const usize t = target_slot.idx;
	const f32 target_x = t_position->x[t];
	const f32 target_y = t_position->y[t];

	for (usize f = 0; f < follower_count; f++) {
		const f32 follower_x = f_position->x[f];
		const f32 follower_y = f_position->y[f];
		const f32 speed = f_speed->val[f];
		f_movement->x[f] = follower_x > target_x ? -speed : follower_x < target_x ? speed : f_movement->x[f];
		f_movement->y[f] = follower_y > target_y ? -speed : follower_y < target_y ? speed : f_movement->y[f];
	}
}

void soa_forward_movement_from_rotation(
	soa_movement2 *e_movement,
	const soa_rotation1 *e_rotation,
	const usize entity_count)
{
	for (usize e = 0; e < entity_count; e++) {
		const f32 rad = e_rotation->x[e];
		e_movement->x[e] = -cosf(rad);
		e_movement->y[e] = -sinf(rad);
	}
}
