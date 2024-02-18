#include <stdlib.h>
#include <cvec3.hpp>
#include "raylib.h"

struct ItemBlock
{
	vec3_u8 color; // color
	U8 block_item;  // actual block
};

void draw_items(ItemBlock block);
void create_ui(int screen_width, int screen_height);
void update_hotbar(int screen_width, int screen_height);
void draw_hotbar(int screen_width, int screen_height);



struct Button
{
	int x;
	int y;
	int width;
	int height;
	ItemBlock block;
};
