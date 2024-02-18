#include "cube_render.hpp"
#include "memory.h"
#include "base_inc.h"
#include "raylib.h"
#include "cvec3.hpp"

static void push_mat_3_buffer(float** buf, U64 buf_len, float matrix[3][3]) {
    float* old_buf = *buf;
    float* new_buf = (float*) malloc((buf_len + 9) * sizeof(float));

    //- cabarger: Yes, memcpy... But this gracefully handles the 
    // case where buf_len is 0.
    for (U64 float_index=0; 
         float_index < buf_len;
         ++float_index)
    {
        new_buf[float_index] = old_buf[float_index];
    }

    free(old_buf);

    memcpy(new_buf + buf_len, matrix, 9 * sizeof(float));
    *buf = new_buf;
}

static void push_mesh_triangle(Mesh* mesh, float triangle[3][3], float normal[3][3], U8 colors[3][4]) {
    push_mat_3_buffer(&mesh->vertices, mesh->triangleCount * 9, triangle);
    push_mat_3_buffer(&mesh->normals,  mesh->triangleCount * 9, normal);

    mesh->triangleCount++;
    mesh->vertexCount = mesh->triangleCount * 3;

    U8* old_colors = mesh->colors;
    mesh->colors = (U8*)malloc(mesh->vertexCount * 4);

    //- cabarger: Ditto, see above memcpy comment.
    for (U64 byte_index=0; 
         byte_index < (mesh->vertexCount - 3) * 4;
         ++byte_index)
    {
        mesh->colors[byte_index]  = old_colors[byte_index];
    }
    memcpy(&mesh->colors[(mesh->vertexCount - 3) * 4], colors, 12);
}

struct mat3 {
    float components[3][3] = { 0 };
};

static mat3 offset_matrix_vector(float matrix[3][3], vec3_f32 offset) {
    mat3 ret;

    for (int x = 0; x < 3; x++) {
        for (int y = 0; y < 3; y++) {
            ret.components[x][y] = matrix[x][y] + offset.data[y];
        }
    }

    return ret;
}

/*
    face generation
*/
void push_bot_face(Mesh* mesh, vec3_f32 offset, vec3_u8 color) {
    U8 colors[3][4] = {
        {color.x, color.y, color.z, 255},
        {color.x, color.y, color.z, 255},
        {color.x, color.y, color.z, 255},
    };

    mat3 seg1 = offset_matrix_vector(BotFaceSeg1, offset);
    mat3 seg2 = offset_matrix_vector(BotFaceSeg2, offset);

    push_mesh_triangle(mesh, seg1.components, ZeroNormalSeg, colors);
    push_mesh_triangle(mesh, seg2.components, ZeroNormalSeg, colors);
}

void push_top_face(Mesh* mesh, vec3_f32 offset, vec3_u8 color) {
    U8 colors[3][4] = {
        {color.x, color.y, color.z, 255},
        {color.x, color.y, color.z, 255},
        {color.x, color.y, color.z, 255},
    };

    mat3 seg1 = offset_matrix_vector(TopFaceSeg1, offset);
    mat3 seg2 = offset_matrix_vector(TopFaceSeg2, offset);

    push_mesh_triangle(mesh, seg1.components, ZeroNormalSeg, colors);
    push_mesh_triangle(mesh, seg2.components, ZeroNormalSeg, colors);
}

void push_back_face(Mesh* mesh, vec3_f32 offset, vec3_u8 color) {
    F32 scalar = 0.75;

    U8 cr = (U8) (scalar * (F32) color.x);
    U8 cg = (U8) (scalar * (F32) color.y);
    U8 cb = (U8) (scalar * (F32) color.z);

    U8 colors[3][4] = {
        {cr, cg, cb, 255},
        {cr, cg, cb, 255},
        {cr, cg, cb, 255},
    };

    mat3 seg1 = offset_matrix_vector(BackFaceSeg1, offset);
    mat3 seg2 = offset_matrix_vector(BackFaceSeg2, offset);

    push_mesh_triangle(mesh, seg1.components, ZeroNormalSeg, colors);
    push_mesh_triangle(mesh, seg2.components, ZeroNormalSeg, colors);
}

void push_front_face(Mesh* mesh, vec3_f32 offset, vec3_u8 color) {
    F32 scalar = 0.5;

    U8 cr = (U8) (scalar * (F32) color.x);
    U8 cg = (U8) (scalar * (F32) color.y);
    U8 cb = (U8) (scalar * (F32) color.z);

    U8 colors[3][4] = {
        {cr, cg, cb, 255},
        {cr, cg, cb, 255},
        {cr, cg, cb, 255},
    };

    mat3 seg1 = offset_matrix_vector(FontFaceSeg1, offset);
    mat3 seg2 = offset_matrix_vector(FontFaceSeg2, offset);

    push_mesh_triangle(mesh, seg1.components, ZeroNormalSeg, colors);
    push_mesh_triangle(mesh, seg2.components, ZeroNormalSeg, colors);
}

void push_left_face(Mesh* mesh, vec3_f32 offset, vec3_u8 color) {
    F32 scalar = 0.75;

    U8 cr = (U8) (scalar * (F32) color.x);
    U8 cg = (U8) (scalar * (F32) color.y);
    U8 cb = (U8) (scalar * (F32) color.z);

    U8 colors[3][4] = {
        {cr, cg, cb, 255},
        {cr, cg, cb, 255},
        {cr, cg, cb, 255},
    };

    mat3 seg1 = offset_matrix_vector(LeftFaceSeg1, offset);
    mat3 seg2 = offset_matrix_vector(LeftFaceSeg2, offset);

    push_mesh_triangle(mesh, seg1.components, ZeroNormalSeg, colors);
    push_mesh_triangle(mesh, seg2.components, ZeroNormalSeg, colors);
}

void push_right_face(Mesh* mesh, vec3_f32 offset, vec3_u8 color) {
    F32 scalar = 0.5;

    U8 cr = (U8) (scalar * (F32) color.x);
    U8 cg = (U8) (scalar * (F32) color.y);
    U8 cb = (U8) (scalar * (F32) color.z);

    U8 colors[3][4] = {
        {cr, cg, cb, 255},
        {cr, cg, cb, 255},
        {cr, cg, cb, 255},
    };
    mat3 seg1 = offset_matrix_vector(RightFaceSeg1, offset);
    mat3 seg2 = offset_matrix_vector(RightFaceSeg2, offset);
    
    push_mesh_triangle(mesh, seg1.components, ZeroNormalSeg, colors);
    push_mesh_triangle(mesh, seg2.components, ZeroNormalSeg, colors);
}
