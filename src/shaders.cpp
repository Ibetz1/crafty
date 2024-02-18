#include "shaders.hpp"

Shader shaders[MAX_POSTPRO_SHADERS] = { 0 };
Light lights[MAX_LIGHT] = { 0 };
Shader lighting;

int currentShader;

void init_shaders()
{
    // All post-processing shaders
    shaders[FX_BASE] = LoadShader(0, TextFormat("resources/shaders/glsl%i/base.fs", GLSL_VERSION));
    shaders[FX_GRAYSCALE] = LoadShader(0, TextFormat("resources/shaders/glsl%i/grayscale.fs", GLSL_VERSION));
    shaders[FX_POSTERIZATION] = LoadShader(0, TextFormat("resources/shaders/glsl%i/posterization.fs", GLSL_VERSION));
    shaders[FX_FISHEYE] = LoadShader(0, TextFormat("resources/shaders/glsl%i/fisheye.fs", GLSL_VERSION));
    shaders[FX_SOBEL] = LoadShader(0, TextFormat("resources/shaders/glsl%i/sobel.fs", GLSL_VERSION));
    shaders[FX_BLOOM] = LoadShader(0, TextFormat("resources/shaders/glsl%i/bloom.fs", GLSL_VERSION));
    shaders[FX_BLUR] = LoadShader(0, TextFormat("resources/shaders/glsl%i/blur.fs", GLSL_VERSION));

    lighting = LoadShader(TextFormat("resources/shaders/glsl%i/lighting.vs", GLSL_VERSION),
                            TextFormat("resources/shaders/glsl%i/lighting.fs", GLSL_VERSION));
    
    currentShader = FX_BASE;

    // LIGHTING
    lighting.locs[SHADER_LOC_VECTOR_VIEW] = GetShaderLocation(lighting, "viewPos");
    // Ambient light ( Can see it in pitch black )
    int ambientLoc = GetShaderLocation(lighting, "ambient");
    SetShaderValue(lighting, ambientLoc, (float[4]){ 0.1f, 0.1f, 0.1f, 1.0f }, SHADER_UNIFORM_VEC4);
}

void init_light(Vector3 position, Vector3 target, Color color)
{
    lights[0] = CreateLight(LIGHT_POINT, position, target, color, lighting);
}

void draw_lights()
{
    if (lights[0].enabled) DrawSphereEx(lights[0].position, 0.2f, 8, 8, lights[0].color);
    else DrawSphereWires(lights[0].position, 0.2f, 8, 8, ColorAlpha(lights[0].color, 0.3f));
    // for (int i = 0; i < MAX_LIGHT; i++)
    // {
    //     if (lights[i].enabled) DrawSphereEx(lights[i].position, 0.2f, 8, 8, lights[i].color);
    //     else DrawSphereWires(lights[i].position, 0.2f, 8, 8, ColorAlpha(lights[i].color, 0.3f));
    // }
}

void update_shaders()
{
    // Lights
    if (IsKeyPressed(KEY_Y)) { lights[0].enabled = !lights[0].enabled; }
    for (int i = 0; i < MAX_LIGHT; i++) UpdateLightValues(lighting, lights[i]);
    
    // Change post processing shader
    if (IsKeyPressed(KEY_J)) currentShader++;
    else if (IsKeyPressed(KEY_K)) currentShader--;

    if (currentShader >= MAX_POSTPRO_SHADERS) currentShader = 0;
    else if (currentShader < 0) currentShader = MAX_POSTPRO_SHADERS - 1;
}

// Shader Mode
void begin_shaders()
{
    BeginShaderMode(shaders[currentShader]);
}

void end_shaders()
{
    EndShaderMode();
}

void deload_shaders()
{
    for (int i = 0; i < MAX_POSTPRO_SHADERS; i++) UnloadShader(shaders[i]);
}

// Auxiliary
Shader get_lighting_shader()
{
    return lighting;
}

Shader get_shader(int i)
{
    if (i > 0 && i < MAX_POSTPRO_SHADERS)
    return shaders[i];
    return get_current_shader();
}

Shader get_current_shader()
{
    return shaders[currentShader];
}

int get_shader_index()
{
    return currentShader;
}