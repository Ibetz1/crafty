#include "chunk_loader.hpp"
#include "raylib.h"
#include "raymath.h"
#include "cvec3.hpp"
#include "cube_render.hpp"

static const vec3_s64 x_culling_offset = {1, 0, 0};
static const vec3_s64 y_culling_offset = {0, 1, 0};
static const vec3_s64 z_culling_offset = {0, 0, 1};

static World world;


static Model static_chunks[WORLD_W_C][WORLD_W_C] = { 0 };
static bool chunk_updates[WORLD_W_C][WORLD_W_C] = { 1 };

// Generate a simple triangle mesh from code
static Mesh generate_cubeMesh(void)
{
    Mesh mesh = { 0 };
    mesh.triangleCount = 0;

    vec3_f32 offset = {3, 0, 0};

    push_bot_face(&mesh, offset);
    push_top_face(&mesh, offset);
    push_back_face(&mesh, offset);
    push_front_face(&mesh, offset);
    push_left_face(&mesh, offset);
    push_right_face(&mesh, offset);

    // Upload mesh data from CPU (RAM) to GPU (VRAM) memory
    UploadMesh(&mesh, false);

    return mesh;
}

bool get_culling_data(Mesh* mesh, World* world, vec3_u64 world_cor, vec3_f32 chunk_cor) {
    vec3_s64 s_world_cor = vec3_u64::cast<S64>(world_cor);

    /*
        x axis face culling
    */

    vec3_s64 left_block_cor  = vec3_s64::sub(s_world_cor, x_culling_offset);
    vec3_s64 right_block_cor = vec3_s64::add(s_world_cor, x_culling_offset);
    U8 left_block_val = 0;
    U8 right_block_val = 0;

    if (!(left_block_cor.x < 0)) {
        Block* block = World::block_at(world, vec3_s64::cast<U64>(left_block_cor));
        left_block_val = Block::get_value(block);
    }

    if (!(right_block_cor.x >= World::block_width_x)) {
        Block* block = World::block_at(world, vec3_s64::cast<U64>(right_block_cor));
        right_block_val = Block::get_value(block);
    }

    /*
        y axis face culling
    */

    vec3_s64 bot_block_cor = vec3_s64::sub(s_world_cor, y_culling_offset); 
    vec3_s64 top_block_cor = vec3_s64::add(s_world_cor, y_culling_offset);
    U8 bot_block_val = 0;
    U8 top_block_val = 0;

    if (!(bot_block_cor.y < 0)) {
        Block* block = World::block_at(world, vec3_s64::cast<U64>(bot_block_cor));
        bot_block_val = Block::get_value(block);
    }

    if (!(top_block_cor.y >= World::block_width_y)) {
        Block* block = World::block_at(world, vec3_s64::cast<U64>(top_block_cor));
        top_block_val = Block::get_value(block);
    }

    /*
        z axis face culling
    */

    vec3_s64 back_block_cor   = vec3_s64::sub(s_world_cor, z_culling_offset); 
    vec3_s64 front_block_cor  = vec3_s64::add(s_world_cor, z_culling_offset); 
    U8 back_block_val = 0;
    U8 front_block_val = 0;

    if (!(back_block_cor.z < 0)) {
        Block* block = World::block_at(world, vec3_s64::cast<U64>(back_block_cor));
        back_block_val = Block::get_value(block);
    }

    if (!(front_block_cor.z >= World::block_width_z)) {
        Block* block = World::block_at(world, vec3_s64::cast<U64>(front_block_cor));
        front_block_val = Block::get_value(block);
    }

    if (left_block_val == 0)  push_left_face(mesh, chunk_cor);
    if (right_block_val == 0) push_right_face(mesh, chunk_cor);
    if (bot_block_val == 0)   push_bot_face(mesh, chunk_cor);
    if (top_block_val == 0)   push_top_face(mesh, chunk_cor);
    if (back_block_val == 0)  push_back_face(mesh, chunk_cor);
    if (front_block_val == 0) push_front_face(mesh, chunk_cor);
    if (left_block_val == 0)  push_left_face(mesh, chunk_cor);
    if (right_block_val == 0) push_right_face(mesh, chunk_cor);

    return false;
}

void render_chunk(World* world, vec2_u64 chunk_cor) {
    Chunk chunk = World::chunk_at_chunk_cor(world, chunk_cor);

    UnloadModel(static_chunks[chunk_cor.x][chunk_cor.y]);
    Mesh mesh = { 0 };
    mesh.triangleCount = 0;

    for (U64 x = 0; x < Chunk::width_blocks_x; x++) {
        for (U64 y = 0; y < Chunk::width_blocks_y; y++) {
            for (U64 z = 0; z < Chunk::width_blocks_z; z++) {
                
                vec3_u64 world_cor = {
                    x + (chunk.pos.x * Chunk::width_blocks_x), 
                    y,
                    z + (chunk.pos.y * Chunk::width_blocks_z) 
                };

                Block* b = World::block_at(chunk.world, world_cor);

                if (Block::get_value(b) != 0) {
                    get_culling_data(&mesh, world, world_cor, {(F32) x, (F32) y, (F32) z});
                } 
            }
        }
    }

    UploadMesh(&mesh, false);
    static_chunks[chunk_cor.x][chunk_cor.y] = LoadModelFromMesh(mesh);
    chunk_updates[chunk_cor.x][chunk_cor.y] = false;
}

void test_fill_chunk(World* world, vec2_u64 chunk_cor) {
    Chunk chunk = World::chunk_at_chunk_cor(world, chunk_cor);
    
    Chunk::iterate(chunk, [](Chunk chunk, vec3_u64 wpos) {
        Block* b = World::block_at(chunk.world, wpos);
        Block::set_value(b, 1);
    });
}

/*
    attaches to main.cpp
*/

void init_chunk_render() {
    world = World::alloc_chunks();
    test_fill_chunk(&world, {0, 0});
    test_fill_chunk(&world, {1, 0});
    test_fill_chunk(&world, {1, 1});
    test_fill_chunk(&world, {0, 1});

    for (U64 x = 0; x < World::chunk_width_x; x++) {
        for (U64 y = 0; y < World::chunk_width_y; y++) {
            render_chunk(&world, {x, y});
        }
    }

}

void update_chunk_render(F64 dt) {
}

void draw_chunk_render() {
    for (U64 x = 0; x < World::chunk_width_x; x++) {
        for (U64 y = 0; y < World::chunk_width_y; y++) {
            DrawModel(static_chunks[x][y], {(F32) x * Chunk::width_blocks_x, 
                                            0, 
                                            (F32) y * Chunk::width_blocks_z}, 
                                            1.0f, WHITE);
        }
    }
}