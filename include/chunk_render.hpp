//! date: 2/17/24
//! chunk_chunk_render.hpp by Ian Betz
//! header defined for chunk re

#ifndef _CHUNK_RENDER_HPP
#define _CHUNK_RENDER_HPP

#include "chunk_loader.hpp"

void render_chunk(World* world, vec2_u64 chunk_cor);

void init_chunk_render();

void update_chunk_render(F64 dt);

void draw_chunk_render();

#endif