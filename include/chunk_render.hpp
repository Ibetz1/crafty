#ifndef _CHUNK_RENDER_HPP
#define _CHUNK_RENDER_HPP

extern "C" {
    #include "base_inc.h"
}

void init_world();

void render_chunk(U64 x, U64 y);

void test_fill_chunk(U64 x, U64 y);

#endif