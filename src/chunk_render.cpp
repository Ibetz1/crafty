#include "chunk_loader.hpp"

void render_chunk(World* world, U64 x, U64 z) {
    Chunk chunk = World::chunk_at_chunk_cor(world, x, z);

    Chunk::iterate(chunk, [](Chunk chunk, U64 x, U64 y, U64 z) {

    });
}

void test_fill_chunk(Chunk chunk) {

}


/*
    attaches to main.cpp
*/

void init_chunk_render() {

}

void update_chunk_render(F64 dt) {

}

void draw_chunk_render() {

}