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


void update_shaders();
void init_shaders();
void deload_shaders();
void begin_shaders();
void end_shaders();
Shader get_current_shader();
Shader get_shader(int i);
int get_shader_index();
void draw_lights();
Shader get_lighting_shader();
void init_light(Vector3 position, Vector3 target, Color color);


typedef enum {
    FX_BASE = 0,
    FX_GRAYSCALE,
    FX_POSTERIZATION,
    FX_DREAM_VISION,
    FX_PIXELIZER,
    FX_CROSS_HATCHING,
    FX_CROSS_STITCHING,
    FX_PREDATOR_VIEW,
    FX_SCANLINES,
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
    "DREAM_VISION",
    "PIXELIZER",
    "CROSS_HATCHING",
    "CROSS_STITCHING",
    "PREDATOR_VIEW",
    "SCANLINES",
    "FISHEYE",
    "SOBEL",
    "BLOOM",
    "BLUR",
    "LIGHTING"
    //"FXAA"
};

#endif