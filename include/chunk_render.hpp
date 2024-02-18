#ifndef _CHUNK_RENDER_HPP
#define _CHUNK_RENDER_HPP

#include "chunk_loader.hpp"

void render_chunk(World* world, U64 x, U64 z);

void test_fill_chunk(Chunk chunk);

void init_chunk_render();

void update_chunk_render(F64 dt);

void draw_chunk_render();

#endif