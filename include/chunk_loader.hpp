//! date: 2/17/24
//! chunk_loader.hpp by Ian Betz
//! api layer for world chunk data management 

#ifndef _CHUNK_HPP
#define _CHUNK_HPP

#include <math.h>
#include "cvec3.hpp"

extern "C" {
    #include "base_types.h"
}

#define CHUNK_W 16
#define CHUNK_H 48

 // width of world in chunks
#define WORLD_W_C 8

// world width in blocks
#define WORLD_W_B WORLD_W_C * CHUNK_W

// predefs
struct Block;
struct Chunk;
struct World;

/*
    contains block data
*/
struct Block {
    U8 tex = 0;

    // set block texture
    static void set_value(Block* block, U8 tex);

    // get block texture
    static U8 get_value(Block* block);
};

/*
    contains chunk data
*/
struct Chunk {
    static const U64 width_blocks_x = CHUNK_W;
    static const U64 width_blocks_y = CHUNK_H;
    static const U64 width_blocks_z = CHUNK_W;

    vec2_u64 pos;
    World* world;

    static void iterate(Chunk chunk, void (*iter)(Chunk chunk, vec3_u64 wpos));
};

/*
    contains world data
*/
struct World {
    
    // block dimensions
    static const U64 block_width_x = WORLD_W_B;
    static const U64 block_width_y = CHUNK_H;
    static const U64 block_width_z = WORLD_W_B;

    // chunk dimensions
    static const U64 chunk_width_x = WORLD_W_C;
    static const U64 chunk_width_y = WORLD_W_C;

    // world volumes
    static const U64 block_volume = WORLD_W_B * WORLD_W_B * CHUNK_H;
    static const U64 chunk_area   = WORLD_W_C * WORLD_W_C;
    static const U64 block_area   = WORLD_W_B * WORLD_W_B;

    // allocation data
    static const U64 allocated_bytes = block_volume * sizeof(Block); 

    // generate empty world chunks
    static World alloc_chunks();

    /*
        returns a pointer to a block at some block coorinates
        x, y, z are in world coordinates
    */
    static Block* block_at(World* world, vec3_u64 pos);

    /*
        returns a chunk that a block is inside of
        x, y are in world coordinates
    */
    static Chunk  chunk_at_block_cor(World* world, vec2_u64 pos);
    
    /*
        returns a chunk
        x, y are in chunk coordinates
    */
    static Chunk chunk_at_chunk_cor(World* world, vec2_u64 pos);

    // pointer to beginning of world blocks
    Block* base_ptr;
};


#endif