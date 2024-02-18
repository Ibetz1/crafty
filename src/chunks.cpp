//! date: 2/17/24
//! chunks.cpp by Ian Betz
//! created to manage world data

#include "base_inc.h"


#include <stdio.h>
#include <math.h>
#include "chunks.hpp"

template <typename T>
T clamp(T val, T min, T max) {
    return (T) fmin(fmax(val, min), max);
}

/*
    compute 1D coords from 3D based on chunk dimensions
*/
inline U64 compute_index_3D(U64 x, U64 y, U64 z, U64 w, U64 h, U64 d) {
    return clamp<U64>((x + y * w + z * w * d) - 1, 0, (w * h * d) - 1);
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
Block* World::block_at(World* world, U64 x, U64 y, U64 z) {
    x = clamp<F64>(x, 0, WORLD_W_B);
    y = clamp<F64>(y, 0, WORLD_W_B);
    z = clamp<F64>(z, 0, CHUNK_H);

    U64 index = compute_index_3D(x, y, z, block_width_x, block_width_y, block_width_z);

    return world->base_ptr + index;
}

// get a chunk from some world coordinates
Chunk World::chunk_at_block_cor(World* world, U64 x, U64 y) {
    U64 xnorm = floor((clamp<F64>(x, 0, block_width_x) / CHUNK_W)) * CHUNK_W;
    U64 ynorm = floor((clamp<F64>(y, 0, block_width_y) / CHUNK_W)) * CHUNK_W;

    U64 index = compute_index_3D(xnorm, ynorm, 0, block_width_x, block_width_y, block_width_z);

    return (Chunk) {
        .base_ptr = world->base_ptr + index,
    };
}

// get chunk from chunk space coords
Chunk World::chunk_at_chunk_cor(World* world, U64 x, U64 y) {
    x = clamp<U64>(x, 0, chunk_width_x) * Chunk::width_blocks_x;
    y = clamp<U64>(y, 0, chunk_width_y) * Chunk::width_blocks_y;

    U64 index = compute_index_3D(x, y, block_width_z, block_width_x, block_width_y, block_width_z);

    return (Chunk) {
        .base_ptr = world->base_ptr + index,
    };
}

/*
    chunk operations
*/

// get a pointer to a block inside of a chunk
Block* Chunk::block_at(Chunk* space, U64 x, U64 y, U64 z) {
    x = clamp<U64>(x, 0, width_blocks_x);
    y = clamp<U64>(y, 0, width_blocks_y);
    z = clamp<U64>(z, 0, width_blocks_z);

    U64 index = compute_index_3D(x, y, z, width_blocks_x, width_blocks_y, width_blocks_z);

    return space->base_ptr + index;
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