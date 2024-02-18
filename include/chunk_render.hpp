//! date: 2/17/24
//! chunk_chunk_render.hpp by Ian Betz
//! header defined for chunk re

#ifndef _CHUNK_RENDER_HPP
#define _CHUNK_RENDER_HPP

#include "raylib.h"
#include "chunk_loader.hpp"

void init_chunk_render();

void draw_chunk_render();

Model* static_model_from_chunk(Chunk* chunk); 

#endif
