#include "asset.h"
#include "components/components_graphics.h"
#include "components/components_transform.h"
#include "components/components_shape.h"
#include "systems_sdl2.h"
#include <SDL2/SDL_render.h>

void draw_sprite(
	const soa_position2 *e_position,
	const soa_size2 *e_size,
	const soa_clip *e_clip,
	const usize entity_count,
	SDL_Renderer *renderer,
	SDL_Texture *texture,
	const f32v2 camera)
{
	for (usize e = 0; e < entity_count; ++e) {
		const SDL_Rect srcrect = { e_clip->x[e], e_clip->y[e], e_clip->w[e], e_clip->h[e] };
		const SDL_Rect origrect = { e_position->x[e], e_position->y[e], e_size->w[e], e_size->h[e] };
		const SDL_Rect dstrect = { origrect.x - camera.x, origrect.y - camera.y, origrect.w, origrect.h };
		SDL_RenderCopy(renderer, texture, &srcrect, &dstrect);
	}
}

void draw_tilemap(
	const tilemap_t *tilemap,
	const tilemap_encoding_t *tilemap_encoding,
	const tileset_t *tileset,
	const i32v2 tile_size,
	SDL_Renderer *renderer,
	SDL_Texture *tilesheet_texture,
	const f32v2 camera)
{
	const int mapwidth = tilemap->width;
	const int mapheight = tilemap->height;
	const int tilewidth = tile_size.w;
	const int tileheight = tile_size.h;

	for (int l = 0; l < tilemap->num_layers; ++l) {
		const tilemap_layer_t *layer = &tilemap->layers[l];

		for (int y = 0; y < mapheight; ++y) {
			for (int x = 0; x < mapwidth; ++x) {
				const char tile_char = layer->offset_to_char[y * mapwidth + x];
				const tile_enum_t tile_enum = tilemap_encoding->char_to_enum[tile_char];
				if (tile_enum < ASSET_TILE_BEGIN || tile_enum > ASSET_TILE_END) continue;
				const tile_t tile = tileset->enum_to_tile[tile_enum];

				const SDL_Rect srcrect = { tile.x, tile.y, tile.w, tile.h };
				const SDL_Rect origrect = { x * tilewidth, y * tileheight, tilewidth, tileheight };
				const SDL_Rect dstrect = { origrect.x - camera.x, origrect.y - camera.y, origrect.w, origrect.h };
				SDL_RenderCopy(renderer, tilesheet_texture, &srcrect, &dstrect);
			}
		}
	}
}
