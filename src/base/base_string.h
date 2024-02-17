//
// base_string.h
//
// CS 270.Bolden
// Clang 16.0.6
// Caleb Barger
// 02/11/24 
// Linux x86_64
// barg8397@vandals.uidaho.edu
//

#ifndef BASE_STRING_H

typedef struct String8 String8;
struct String8 {
    U8* str;
    U64 len; 
};

typedef struct String8Node String8Node;
struct String8Node {
    String8Node* next;
    String8 val;
};

#define Str8Lit(a) (String8){.str = a, .len = sizeof(a)}
internal String8 str8_cat(Arena* arena, String8 a, String8 b);
internal String8 str8_from_memz(U8* str); 
internal String8 str8_sub(String8 str, U64 start, U64 end); 
internal String8 str8_copy(Arena* arena, String8 source); 
internal String8 str8_copyz(Arena* arena, String8 source);

#define BASE_STRING_H
#endif
