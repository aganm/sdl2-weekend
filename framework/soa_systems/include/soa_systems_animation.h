#pragma once

/**
 * @file
 * @brief Animation systems.
 */

#include <types/primitive.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct soa_animation soa_animation;
typedef struct soa_clip soa_clip;
typedef struct soa_velocity soa_velocity2;
typedef struct tileset_t tileset_t;

void soa_progress_animation_if_moving(
	soa_animation *e_animation,
	const soa_velocity2 *e_velocity,
	const usize entity_count,
	const f32seconds dt);

void soa_fetch_tileset_animation(
	const soa_animation *e_animation,
	soa_clip *e_clip,
	const usize entity_count,
	const tileset_t *tileset);

#ifdef __cplusplus
}
#endif
