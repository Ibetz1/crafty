#include "chunk_loader.hpp"
#include "raylib.h"
#include "raymath.h"

void render_chunk(World* world, U64 x, U64 z) {
    Chunk chunk = World::chunk_at_chunk_cor(world, x, z);

    Chunk::iterate(chunk, [](Chunk chunk, U64 x, U64 y, U64 z) {
        Block* block = Chunk::block_at(&chunk, x, y, z);

        if (Block::get_value(block) == 1) {
            DrawCubeWires({(F32) x, (F32) y, (F32) z}, 1.0f, 1.0f, 1.0f, BLUE);
        } else {
            DrawCubeWires({(F32) x, (F32) y, (F32) z}, 1.0f, 1.0f, 1.0f, RED);
        }
    });
}

void test_fill_chunk(World* world, U64 x, U64 z) {
    Chunk chunk = World::chunk_at_chunk_cor(world, x, z);

    Chunk::iterate(chunk, [](Chunk chunk, U64 x, U64 y, U64 z) {
        if (y > 10 && y < 15) {
            Block* b = Chunk::block_at(&chunk, x, y, z);
            Block::set_value(b, 1);
        }
    });
}


/*
    attaches to main.cpp
*/

static World world;

void init_chunk_render() {
    world = World::alloc_chunks();
    test_fill_chunk(&world, 0, 0);
}

void update_chunk_render(F64 dt) {

}

void draw_chunk_render() {
    render_chunk(&world, 0, 0);
}