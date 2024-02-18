//! date: 2/17/24
//! chunks.cpp by Ian Betz
//! created to manage world data

#include "base_inc.h"

#include <stdio.h>
#include <math.h>
#include "chunk_loader.hpp"
#include "cvec3.hpp"

/*
    compute 1D coords from 3D based on chunk dimensions
*/
inline U64 compute_index_3D(vec3_u64 pos, U64 w, U64 h, U64 d) {
    return clamp<U64>((pos.x + pos.z * w + pos.y * w * d) - 1, 0, (w * h * d) - 1);
}

/*
    world operations
*/

// allocate a block of memory for every single block in the world
World World::alloc_chunks() {
    return (World) {
        .base_ptr  = (Block*) calloc(chunk_area, allocated_bytes),
    };
}

// get block in world coords
Block* World::block_at(World* world, vec3_u64 pos) {
    // pos = vec3_u64::vclamp(pos, {0, 0, 0}, {block_width_x, block_width_y, block_width_z});

    U64 index = compute_index_3D(pos, block_width_x, block_width_y, block_width_z);

    return world->base_ptr + index;
}

// get a chunk from some world coordinates
Chunk World::chunk_at_block_cor(World* world, vec2_u64 pos) {
    U64 xnorm = floor((clamp<F64>(pos.x, 0, block_width_x - 1) / Chunk::width_blocks_x)) * Chunk::width_blocks_x;
    U64 znorm = floor((clamp<F64>(pos.y, 0, block_width_z - 1) / Chunk::width_blocks_z)) * Chunk::width_blocks_z;

    U64 index = compute_index_3D({xnorm, 0, znorm}, block_width_x, block_width_y, block_width_z);

    return (Chunk) {
        .pos = {xnorm, znorm},
        .world = world,
    };
}

// get chunk from chunk space coords
Chunk World::chunk_at_chunk_cor(World* world, vec2_u64 pos) {
    vec3_u64 world_cor = {
        pos.x * Chunk::width_blocks_x,
        block_width_z,
        pos.y * Chunk::width_blocks_y,
    };

    U64 index = compute_index_3D(world_cor, block_width_x, block_width_y, block_width_z);

    return (Chunk) {
        .pos = pos,
        .world = world,
    };
}

/*
    chunk operations
*/

// apply an iterator to a chunk
void Chunk::iterate(Chunk chunk, void (*iter)(Chunk chunk, vec3_u64 wpos)) {
    for (U64 x = 0; x < Chunk::width_blocks_x; x++) {
        for (U64 y = 0; y < Chunk::width_blocks_y; y++) {
            for (U64 z = 0; z < Chunk::width_blocks_z; z++) {
                vec3_u64 cor = {
                    x + (chunk.pos.x * Chunk::width_blocks_x), 
                    y,
                    z + (chunk.pos.y * Chunk::width_blocks_z) 
                };
                iter(chunk, cor);
            }
        }
    }
}

/*
    block operations
*/

// set texture
void Block::set_value(Block* blk, U8 val) {
    blk->tex = val;
}

// get texture
U8 Block::get_value(Block* blk) {
    return blk->tex;
}
