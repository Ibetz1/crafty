#include "shaders.hpp"

Shader shaders[MAX_POSTPRO_SHADERS] = { 0 };
Light lights[MAX_LIGHTS] = { 0 };
Shader lighting;
//Light lightSource;


int currentShader;

// switch statement, depending on model type we change the shader type.

void init_shaders()
{
    // All post-processing shaders
    shaders[FX_BASE] = LoadShader(0, TextFormat("resources/shaders/glsl%i/base.fs", GLSL_VERSION));
    shaders[FX_GRAYSCALE] = LoadShader(0, TextFormat("resources/shaders/glsl%i/grayscale.fs", GLSL_VERSION));
    shaders[FX_POSTERIZATION] = LoadShader(0, TextFormat("resources/shaders/glsl%i/posterization.fs", GLSL_VERSION));
    shaders[FX_DREAM_VISION] = LoadShader(0, TextFormat("resources/shaders/glsl%i/dream_vision.fs", GLSL_VERSION));
    shaders[FX_PIXELIZER] = LoadShader(0, TextFormat("resources/shaders/glsl%i/pixelizer.fs", GLSL_VERSION));
    shaders[FX_CROSS_HATCHING] = LoadShader(0, TextFormat("resources/shaders/glsl%i/cross_hatching.fs", GLSL_VERSION));
    shaders[FX_CROSS_STITCHING] = LoadShader(0, TextFormat("resources/shaders/glsl%i/cross_stitching.fs", GLSL_VERSION));
    shaders[FX_PREDATOR_VIEW] = LoadShader(0, TextFormat("resources/shaders/glsl%i/predator.fs", GLSL_VERSION));
    shaders[FX_SCANLINES] = LoadShader(0, TextFormat("resources/shaders/glsl%i/scanlines.fs", GLSL_VERSION));
    shaders[FX_FISHEYE] = LoadShader(0, TextFormat("resources/shaders/glsl%i/fisheye.fs", GLSL_VERSION));
    shaders[FX_SOBEL] = LoadShader(0, TextFormat("resources/shaders/glsl%i/sobel.fs", GLSL_VERSION));
    shaders[FX_BLOOM] = LoadShader(0, TextFormat("resources/shaders/glsl%i/bloom.fs", GLSL_VERSION));
    shaders[FX_BLUR] = LoadShader(0, TextFormat("resources/shaders/glsl%i/blur.fs", GLSL_VERSION));
    shaders[FX_LIGHTING] = LoadShader(TextFormat("resources/shaders/glsl%i/lighting.vs", GLSL_VERSION),
                               TextFormat("resources/shaders/glsl%i/lighting.fs", GLSL_VERSION));

    lighting = LoadShader(TextFormat("resources/shaders/glsl%i/lighting.vs", GLSL_VERSION),
                            TextFormat("resources/shaders/glsl%i/lighting.fs", GLSL_VERSION));
    
    currentShader = FX_LIGHTING;



    // Important loc
    lighting.locs[SHADER_LOC_VECTOR_VIEW] = GetShaderLocation(lighting, "viewPos");
    // Ambient light ( Can see it in pitch black )
    int ambientLoc = GetShaderLocation(lighting, "ambient");
    SetShaderValue(lighting, ambientLoc, (float[4]){ 0.1f, 0.1f, 0.1f, 1.0f }, SHADER_UNIFORM_VEC4);
    
    //CreateLight(LIGHT_DIRECTIONAL, (Vector3){ 50.0f, 50.0f, 0.0f }, Vector3Zero(), WHITE, shader);


}

void init_light(Vector3 position, Vector3 target, Color color)
{
    lights[0] = CreateLight(LIGHT_POINT, position, target, color, lighting);
}

Shader get_lighting_shader()
{
    return lighting;
}

void draw_lights()
{
    for (int i = 0; i < MAX_LIGHTS; i++)
    {
        if (lights[i].enabled) DrawSphereEx(lights[i].position, 0.2f, 8, 8, lights[i].color);
        else DrawSphereWires(lights[i].position, 0.2f, 8, 8, ColorAlpha(lights[i].color, 0.3f));
    }
}

void update_shaders()
{
    if (IsKeyPressed(KEY_Y)) { lights[0].enabled = !lights[0].enabled; }
    // if (IsKeyPressed(KEY_R)) { lights[1].enabled = !lights[1].enabled; }
    // if (IsKeyPressed(KEY_G)) { lights[2].enabled = !lights[2].enabled; }
    // if (IsKeyPressed(KEY_B)) { lights[3].enabled = !lights[3].enabled; }
    for (int i = 0; i < MAX_LIGHTS; i++) UpdateLightValues(lighting, lights[i]);
    
    if (IsKeyPressed(KEY_J)) currentShader++;
    else if (IsKeyPressed(KEY_K)) currentShader--;

    if (currentShader >= MAX_POSTPRO_SHADERS) currentShader = 0;
    else if (currentShader < 0) currentShader = MAX_POSTPRO_SHADERS - 1;
}

Shader get_shader(int i)
{
    if (i > 0 && i < 15)
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

void begin_shaders()
{
    // shaders[currentShader].locs[SHADER_LOC_MATRIX_MVP] = GetShaderLocation(shaders[currentShader], "mvp");
    // shaders[currentShader].locs[SHADER_LOC_VECTOR_VIEW] = GetShaderLocation(shaders[currentShader], "viewPos");
    // shaders[currentShader].locs[SHADER_LOC_MATRIX_MODEL] = GetShaderLocationAttrib(shaders[currentShader], "instanceTransform");
    // int ambientLoc = GetShaderLocation(shaders[currentShader], "ambient");
    // SetShaderValue(shaders[currentShader], ambientLoc, (float[4]){ 0.2f, 0.2f, 0.2f, 1.0f }, SHADER_UNIFORM_VEC4);
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
