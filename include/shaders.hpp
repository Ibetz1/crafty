#ifndef _SHADERS_HPP
#define _SHADERS_HPP

//#include "rlights.h"
#include "raylib.h"
#include "raymath.h"
#include <stdlib.h>

//define RLIGHTS_IMPLEMENTATION
//#define RLIGHTS_H
#include "rlights.h"

#define MAX_INSTANCES  10000

#if defined(PLATFORM_DESKTOP)
    #define GLSL_VERSION            330
#else   // PLATFORM_ANDROID, PLATFORM_WEB
    #define GLSL_VERSION            100
#endif

#define MAX_POSTPRO_SHADERS         14
#define MAX_LIGHTS  1

void init_light(Vector3 position, Vector3 target, Color color);
void init_shaders();
void update_shaders();

void draw_lights();
void begin_shaders();
void end_shaders();
void deload_shaders();

Shader get_current_shader();
Shader get_shader(int i);
Shader get_lighting_shader();
int get_shader_index();



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