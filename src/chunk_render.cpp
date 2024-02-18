//! date: 2/17/24
//! chunk_render.cpp by Ian Betz
//! handles rendering of chunk voxels

#include "chunk_render.hpp"
#include "chunk_loader.hpp"

static World world;

static Mesh static_DEBUG_block_mesh;                         
static Material static_DEBUG_block_material;
static Model static_DEBUG_block_model;

void init_world() {
    world = World::alloc_chunks();
    static_DEBUG_block_mesh = GenMeshCube(1.0f, 1.0f, 1.0f);                            
    static_DEBUG_block_material = LoadMaterialDefault();
    static_DEBUG_block_model = LoadModelFromMesh(static_DEBUG_block_mesh);
}


static bool test_neigbor_blocks(Block* block, U64 x, U64 y, U64 z) {
    if 


}

// DrawModelWires(static_DEBUG_block_model, {0, 0, 0}, 1.0f, BLUE);

void render_chunk(U64 x, U64 y) {
    Chunk chunk = World::chunk_at_chunk_cor(&world, x, y);

    iterate_chunk(chunk, [](Chunk chunk, U64 x, U64 y, U64 z) {
        Block* b = Chunk::block_at(&chunk, x, y, z);

        if (Block::get_value(b) == 1) {
            DrawModelWires(static_DEBUG_block_model, {(F32) x, (F32) y, (F32) z}, 1.0f, BLUE);
        } else {
            DrawModelWires(static_DEBUG_block_model, {(F32) x, (F32) y, (F32) z}, 1.0f, RED);
        }
    });
}

void test_fill_chunk(U64 x, U64 y) {
    Chunk chunk = World::chunk_at_chunk_cor(&world, x, y);

    iterate_chunk(chunk, [](Chunk chunk, U64 x, U64 y, U64 z) {
        Block* b = Chunk::block_at(&chunk, x, y, z);

        if (x == 0) {
            Block* b = Chunk::block_at(&chunk, x, y, z);
            Block::set_value(b, 1);
        }
    });
}