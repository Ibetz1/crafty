//! date: 2/17/24
//! chunks.hpp by Ian Betz
//! api layer for world chunk data management 

#ifndef _CHUNK_HPP
#define _CHUNK_HPP

#include <math.h>

extern "C" {
    #include "base_types.h"
}

#define CHUNK_W 16
#define CHUNK_H 255

 // width of world in chunks
#define WORLD_W_C 3

// world width in blocks
#define WORLD_W_B WORLD_W_C * CHUNK_W

/*
    contains block data
*/
struct block {
    U8 tex = 0;

    // set block texture
    static void set_value(block* block, U8 tex);
};

/*
    contains chunk data
*/
struct chunk {
    static const U64 width_blocks_x = CHUNK_W;
    static const U64 width_blocks_y = CHUNK_W;
    static const U64 width_blocks_z = CHUNK_H;

    U64 x_cor; // x coord in chunk space
    U64 y_cor; // y coord in chunk space
    block* base_ptr; // ptr to bottom left block

    static block* block_at(chunk* space, U64 x, U64 y, U64 z);
};

/*
    contains world data
*/
struct world {
    
    // block dimensions
    static const U64 block_width_x = WORLD_W_B;
    static const U64 block_width_y = WORLD_W_B;
    static const U64 block_width_z = CHUNK_H;

    // chunk dimensions
    static const U64 chunk_width_x = WORLD_W_C;
    static const U64 chunk_width_y = WORLD_W_C;

    // world volumes
    static const U64 block_volume = WORLD_W_B * WORLD_W_B * CHUNK_H;
    static const U64 chunk_area   = WORLD_W_C * WORLD_W_C;
    static const U64 block_area   = WORLD_W_B * WORLD_W_B;

    // allocation data
    static const U64 allocated_bytes = block_volume * sizeof(block); 

    // generate empty world chunks
    static world alloc_chunks();

    /*
        returns a pointer to a block at some block coorinates
        x, y, z are in world coordinates
    */
    static block* block_at(world* world, U64 x, U64 y, U64 z);

    /*
        returns a chunk that a block is inside of
        x, y are in world coordinates
    */
    static chunk  chunk_at_block_cor(world* world, U64 x, U64 y);
    
    /*
        returns a chunk
        x, y are in chunk coordinates
    */
    static chunk  chunk_at_chunk_cor(world* world, U64 x, U64 y);

    // pointer to beginning of world blocks
    block* base_ptr;
};


#endif