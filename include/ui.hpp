#include <stdlib.h>



void create_ui(int screen_width, int screen_height);
void update_hotbar(int screen_width, int screen_height);
void draw_hotbar(int screen_width, int screen_height);


// Draw a rectangle background,
// White borders drawRecOutline
// Selected hotbar highlighted with a second outline
struct Button
{
	int x;
	int y;
	int width;
	int height;
};

// struct Line
// {
// 	int start_x;
// 	int start_y;
// 	int end_x;
// 	int end_y;
// };