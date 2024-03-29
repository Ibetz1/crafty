//!
//! base_arena.h
//!
//! CS 270.Bolden
//! Clang 16.0.6
//! Caleb Barger
//! 02/11/24 
//! Linux x86_64
//! barg8397@vandals.uidaho.edu
//!

#ifndef BASE_ARENA_H

typedef struct {
  U8* base_ptr;
  U64 offset; 
  U64 cap;
  B8 growable;
} Arena;

typedef struct {
  Arena* arena;
  U64 offset;
} Temp;

static void* arena_push(Arena* arena, U64 size);
static Arena* arena_alloc_sized(U64 size);
static Arena* arena_alloc(); 
static void arena_release(Arena *arena);
static Temp scratch_begin(Arena* arena);
static void scratch_end(Temp temp);

#define BASE_ARENA_H
#endif
