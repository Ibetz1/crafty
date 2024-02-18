#include <stdlib.h>
#include <cvec3.hpp>
#include "raylib.h"


void create_ui();
void update_hotbar(int screen_width, int screen_height);
void draw_hotbar(int screen_width, int screen_height);



struct Button
{
	int x;
	int y;
	int width;
	int height;
};
