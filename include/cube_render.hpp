#ifndef _MESH_TRIANGLES_HPP
#define _MESH_TRIANGLES_HPP

#include "raylib.h"
#include "cvec3.hpp"

/*
    bottom face template matrix
*/

static float BotFaceSeg1[3][3] {
    {0, 0, 0},
    {1, 0, 0},
    {1, 0, 1},
};

static float BotFaceSeg2[3][3] {
    {0, 0, 0},
    {1, 0, 1},
    {0, 0, 1},
};

/*
    top face template matrix
*/

static float TopFaceSeg1[3][3] = {
    {0, 1, 0},
    {1, 1, 1},
    {1, 1, 0},
};

static float TopFaceSeg2[3][3] = {
    {0, 1, 0},
    {0, 1, 1},
    {1, 1, 1},
};

/*
    back face template matrix
*/

static float BackFaceSeg1[3][3] = {
    {0, 1, 0},
    {1, 1, 0},
    {1, 0, 0},
};

static float BackFaceSeg2[3][3] = {
    {0, 1, 0},
    {1, 0, 0},
    {0, 0, 0},
};

/*
    front face template matrix
*/

static float FontFaceSeg1[3][3] {
    {0, 1, 1},
    {1, 0, 1},
    {1, 1, 1},
};

static float FontFaceSeg2[3][3] {
    {0, 1, 1},
    {0, 0, 1},
    {1, 0, 1},
};


/*
    left face template matrix
*/
static float LeftFaceSeg1[3][3] = {
    {0, 1, 0},
    {0, 0, 0},
    {0, 1, 1},
};

static float LeftFaceSeg2[3][3] = {
    {0, 1, 1},
    {0, 0, 0},
    {0, 0, 1},
};

/*
    right face template matrix
*/

static float RightFaceSeg1[3][3] = {
    {1, 1, 0},
    {1, 1, 1},
    {1, 0, 0},
};

static float RightFaceSeg2[3][3] = {
    {1, 1, 1},
    {1, 0, 1},
    {1, 0, 0},
};

static float ZeroNormalSeg[3][3] = {
    {0, 0, 0},
    {0, 0, 0},
    {0, 0, 0},
};

// cube building functions
void push_bot_face  (Mesh* mesh, vec3_f32 offset, vec3_u8 color, U8 alpha);
void push_top_face  (Mesh* mesh, vec3_f32 offset, vec3_u8 color, U8 alpha);
void push_back_face (Mesh* mesh, vec3_f32 offset, vec3_u8 color, U8 alpha);
void push_front_face(Mesh* mesh, vec3_f32 offset, vec3_u8 color, U8 alpha);
void push_left_face (Mesh* mesh, vec3_f32 offset, vec3_u8 color, U8 alpha);
void push_right_face(Mesh* mesh, vec3_f32 offset, vec3_u8 color, U8 alpha);

#endif
