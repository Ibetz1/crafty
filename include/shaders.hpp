#ifndef _SHADERS_HPP
#define _SHADERS_HPP

#include "raylib.h"
#include "raymath.h"
#include <stdlib.h>

#include "rlights.h"

#define MAX_INSTANCES  10000

#if defined(PLATFORM_DESKTOP)
    #define GLSL_VERSION            330
#else   // PLATFORM_ANDROID, PLATFORM_WEB
    #define GLSL_VERSION            100
#endif

#define MAX_POSTPRO_SHADERS         7
#define MAX_LIGHT  10

void create_light(int type, Vector3 position, Vector3 target, Color color);
void init_shaders();

void update_shaders(Camera*camera);

void draw_lights();
void draw_shaders();

void begin_shaders();
void end_shaders();
void deload_shaders();

void draw_current_shader(int screen_width, int screen_height);

Shader get_shader(int i);
Shader get_current_shader();
Shader get_lighting_shader();
int get_shader_index();

void test_mesh_shaders();


typedef enum {
    FX_BASE = 0,
    FX_GRAYSCALE,
    FX_POSTERIZATION,
    FX_FISHEYE,
    FX_SOBEL,
    FX_BLOOM,
    FX_BLUR,
    FX_LIGHTING,
    //FX_FXAA
} PostproShader;

static const char *postproShaderText[] = {
    "BASE",
    "GRAYSCALE",
    "POSTERIZATION",
    "FISHEYE",
    "SOBEL",
    "BLOOM",
    "BLUR",
    "LIGHTING"
    //"FXAA"
};

#endif