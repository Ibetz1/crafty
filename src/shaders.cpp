#include "shaders.h"

Shader shaders[MAX_POSTPRO_SHADERS] = { 0 };
//Shader lighting;

int currentShader;

void init_shaders(Material matInstances, Material matDefault)
{
    // Load all postpro shaders
    // NOTE 1: All postpro shader use the base vertex shader (DEFAULT_VERTEX_SHADER)
    // NOTE 2: We load the correct shader depending on GLSL version

    // NOTE: Defining 0 (NULL) for vertex shader forces usage of internal default vertex shader
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
    shaders[FX_LIGHTING] = LoadShader(TextFormat("resources/shaders/glsl%i/lighting_instancing.vs", GLSL_VERSION),
                              TextFormat("resources/shaders/glsl%i/lighting.fs", GLSL_VERSION));
    
    currentShader = FX_LIGHTING;
    int ambientLoc = GetShaderLocation(shaders[currentShader], "ambient");
    SetShaderValue(shaders[currentShader], ambientLoc, (float[4]){ 0.2f, 0.2f, 0.2f, 1.0f }, SHADER_UNIFORM_VEC4);
    
    matInstances = LoadMaterialDefault();
    matInstances.shader = shaders[currentShader];
    matInstances.maps[MATERIAL_MAP_DIFFUSE].color = RED;

    matDefault = LoadMaterialDefault();
    matDefault.maps[MATERIAL_MAP_DIFFUSE].color = BLUE;


}
void update_shaders(float cameraPos[])
{
    SetShaderValue(shaders[currentShader], shaders[currentShader].locs[SHADER_LOC_VECTOR_VIEW], cameraPos, SHADER_UNIFORM_VEC3);
    if (IsKeyPressed(KEY_J)) currentShader++;
    else if (IsKeyPressed(KEY_K)) currentShader--;

    if (currentShader >= MAX_POSTPRO_SHADERS) currentShader = 0;
    else if (currentShader < 0) currentShader = MAX_POSTPRO_SHADERS - 1;
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
    shaders[currentShader].locs[SHADER_LOC_MATRIX_MVP] = GetShaderLocation(shaders[currentShader], "mvp");
    shaders[currentShader].locs[SHADER_LOC_VECTOR_VIEW] = GetShaderLocation(shaders[currentShader], "viewPos");
    shaders[currentShader].locs[SHADER_LOC_MATRIX_MODEL] = GetShaderLocationAttrib(shaders[currentShader], "instanceTransform");
    int ambientLoc = GetShaderLocation(shaders[currentShader], "ambient");
    SetShaderValue(shaders[currentShader], ambientLoc, (float[4]){ 0.2f, 0.2f, 0.2f, 1.0f }, SHADER_UNIFORM_VEC4);
    BeginShaderMode(shaders[currentShader]);
}

void end_shaders()
{
    EndShaderMode();
}

void deload_shaders()
{
    for (int i = 0; i < MAX_POSTPRO_SHADERS; i++) UnloadShader(shaders[i]);
    //UnloadShader(shade);
}
