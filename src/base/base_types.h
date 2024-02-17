//
// base_types.h
//
// CS 270.Bolden
// Clang 16.0.6
// Caleb Barger
// 02/09/24 
// Linux x86_64
// barg8397@vandals.uidaho.edu
//

#ifndef BASE_TYPES_H

////////////////////////////////
//~ cabarger: Foreign includes

#include <stdint.h>
#include "stdlib.h"
#include <string.h> 
#include <stdio.h> 

////////////////////////////////
//~ cabarger: Codebase keywords

#define internal static
#define global static
#define local_persist static

////////////////////////////////
//~ cabarger: Base types

typedef uint8_t U8;
typedef uint16_t U16;
typedef uint32_t U32;
typedef uint64_t U64;
typedef int8_t S8;
typedef int16_t S16;
typedef int32_t S32;
typedef int64_t S64;
typedef S8 B8;
typedef S16 B16;
typedef S32 B32;
typedef S64 B64;
typedef float F32;
typedef double F64;

#define AlignPow2(a, b) ((a + b - 1) & ~(b - 1))

#define AssertBreak(m) (*((volatile U32*)0) = 0xCA1EB)  
#define AssertMessage(m) AssertBreak(m)
#define InvalidPath AssertMessage("Invalid path")

#define KB(n) (n * 1024)
#define MB(n) (KB(n) * 1024)
#define GB(n) (MB(n) * 1024)

#define Max(a, b) (a > b ? a : b)
#define Min(a, b) (a < b ? a : b)

#define BASE_TYPES_H
#endif
