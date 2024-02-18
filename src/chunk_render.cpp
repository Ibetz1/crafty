#include "chunk_loader.hpp"
#include "raylib.h"
#include "raymath.h"
#include "cvec3.hpp"
#include "cube_render.hpp"
#include "stb_perlin.h"

static const vec3_s64 x_culling_offset = {1, 0, 0};
static const vec3_s64 y_culling_offset = {0, 1, 0};
static const vec3_s64 z_culling_offset = {0, 0, 1};

static World world;

/*
    takes in distance from top block
*/
static U8 block_tex_height_atlas(U64 dist) {
    if (dist == 0) { // grass layer
        return 1;
    }

    if (dist > 4) { // stone layer
        return 3;
    }

    if (dist > 0) { // dirt layer
        return 2;
    }
}

/*
    returns a block based on height
*/
vec3_u8 block_atlas(U8 tex) {
    if (tex == 1) { // grass
        return {70, 160, 20};
    }

    if (tex == 2) { // dirt
        return {130, 50, 16};
    }

    if (tex == 3) { // stone
        return {90, 80, 70};
    }

    return {255, 0, 0};
}

/*
    helps with height map generation
*/
void fill_world_col(World* world, U64 x, U64 z, U64 h) {
    for (U64 y = 0; y < h; y++) {
        Block* b = World::block_at(world, {x, y, z});
        Block::set_value(b, block_tex_height_atlas(h - y - 1));
    }
}

static const int octaves = 6;
static const float lacunarity = 2.0f;
static const float gain = 0.1f;
static const float offset = 1.0f;

void render_terrain(World* world) {
    for (U64 x = 0; x < World::block_width_x; x++) {
        for (U64 y = 0; y < World::block_width_z; y++) {
            F32 nx = (F32) x / World::block_width_x;
            F32 ny = (F32) y / World::block_width_z;

            F32 hnorm = stb_perlin_ridge_noise3(nx, ny, 0.0f, lacunarity, gain,offset, octaves);
            U64 height = (U64) (hnorm * World::block_width_y);
            fill_world_col(world, x, y, height);
        }
    }
}

static Model static_chunks[WORLD_W_C][WORLD_W_C] = { 0 };
static bool chunk_updates[WORLD_W_C][WORLD_W_C] = { 1 };

bool get_culling_data(Mesh* mesh, World* world, vec3_u64 world_cor, vec3_f32 chunk_cor) {
    vec3_s64 s_world_cor = vec3_u64::cast<S64>(world_cor);
    Block* current_block = World::block_at(world, world_cor);

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

    /*
        handles face culling
    */
    if (left_block_val == 0)  push_left_face(mesh, chunk_cor,  block_atlas(Block::get_value(current_block)));
    if (right_block_val == 0) push_right_face(mesh, chunk_cor, block_atlas(Block::get_value(current_block)));
    if (bot_block_val == 0)   push_bot_face(mesh, chunk_cor,   block_atlas(Block::get_value(current_block)));
    if (top_block_val == 0)   push_top_face(mesh, chunk_cor,   block_atlas(Block::get_value(current_block)));
    if (back_block_val == 0)  push_back_face(mesh, chunk_cor,  block_atlas(Block::get_value(current_block)));
    if (front_block_val == 0) push_front_face(mesh, chunk_cor, block_atlas(Block::get_value(current_block)));
    if (left_block_val == 0)  push_left_face(mesh, chunk_cor,  block_atlas(Block::get_value(current_block)));
    if (right_block_val == 0) push_right_face(mesh, chunk_cor, block_atlas(Block::get_value(current_block)));

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
    Model chunkModel = LoadModelFromMesh(mesh);
    
    static_chunks[chunk_cor.x][chunk_cor.y] = model;
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
    render_terrain(&world);

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