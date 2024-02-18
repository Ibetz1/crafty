#include "stb_perlin.h"
#include "perlin.hpp"
#include <iostream>

#define STB_PERLIN_IMPLEMENTATION

const int octaves = 6;
const float lacunarity = 2.0f;
const float gain = 0.1f;
const float offset = 1.0f;

float array[16][16];
int seed;

// Typical values to start playing with:
//     octaves    =   6     -- number of "octaves" of noise3() to sum
//     lacunarity = ~ 2.0   -- spacing between successive octaves (use exactly 2.0 for wrapping output)
//     gain       =   0.5   -- relative weighting applied to each successive octave
//     offset     =   1.0?  -- used to invert the ridges, may need to be larger, not sure

void handle_noise()
{
    // Generate perlin noise into 16 x 16
    for (float i = 0; i < 16; i++)
    {
        for (float j = 0; j < 16; j++)
        {
            array[int(i)][int(j)] = stb_perlin_fbm_noise3(i/16, j/16, 0, lacunarity, gain, octaves);
        }
    }
   
   // Visualize
   for (int i = 0; i < 16; i++)
    {
        for (int j = 0; j < 16; j++)
        {
            printf("%.4f ", array[i][j]);
        }
        printf("\n");
    }
}
