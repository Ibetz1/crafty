//
// base_arena.c
//
// CS 270.Bolden
// Clang 16.0.6
// Caleb Barger
// 02/11/24 
// Linux x86_64
// barg8397@vandals.uidaho.edu
//

static void* arena_push(Arena* a, U64 size) {
  void* result;

  U64 aligned_size = AlignPow2(size, 8);
  if (a->offset + aligned_size > a->cap) {
    if (a->growable) {
        InvalidPath; //- TODO(cabarger): Grow arena
    } else {
      AssertMessage("out of space");
    }
  }
  result = a->base_ptr + a->offset;  
  a->offset += aligned_size; 

  return result; 
}

static Arena* arena_alloc_sized(U64 size) {
  U64 aligned_size = AlignPow2(size + sizeof(Arena), 8);
  void* memory = malloc(aligned_size);

  Arena* arena = (Arena*)memory;
  arena->base_ptr = (U8*)memory; 
  arena->offset = sizeof(Arena);
  arena->cap = aligned_size;
  arena->growable = 0;

  return arena;
}

static Arena* arena_alloc() {
  return arena_alloc_sized(KB(5));
};

static void arena_release(Arena *arena) {
  free(arena->base_ptr);
}

static Temp scratch_begin(Arena* arena) {
  Temp result;
  result.arena = arena;
  result.offset = arena->offset;
  return result;
}

static void scratch_end(Temp temp) {
  temp.arena->offset = temp.offset;
}

