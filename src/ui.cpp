#include "ui.hpp"
#include <vector>

std::vector <Button> buttons;

int highlighted = 0;

bool drawRect = false;
bool drawRoundedRect = true;
bool drawRoundedLines = false;

void create_button(int x, int y, int width, int height)
{
	Button a;
	a.x = x;
	a.y = y;
	a.width = width;
	a.height = height;
	a.block;
	buttons.push_back(a);
}

void escape_menu()
{

}

void create_ui(int screen_width, int screen_height)
{
	for (int i = 0; i < 9; i++)
	{
		create_button(screen_width/8+(i*((int)screen_width/12)), screen_height-(screen_width/12)-(screen_height/60), screen_width/12, screen_width/12);
	}
}

void update_hotbar(int screen_width, int screen_height)
{
	float mouseWheelMovement = GetMouseWheelMove();
	if(mouseWheelMovement != 0)
	{
		// -1 for down, 1 for up
		if (mouseWheelMovement == -1)
		{
			if (highlighted < 8) highlighted++;
			else highlighted = 0;
		}
		else
		{
			if (highlighted > 0) highlighted--;
			else highlighted = 8;

		}
 	}

	for (int i = 0; i < 9; i++)
	{
		// KEY_ONE starts at index 49
		if (IsKeyDown(i+49))
		{
			highlighted = i;
		}
	}

}

void draw_items(ItemBlock block)
{
	// ADD STUFF HERE
	DrawCube({0,0,0}, buttons[highlighted].width/1.2f, buttons[highlighted].width/1.2f, buttons[highlighted].width/1.2f, BLACK);
}

void draw_hotbar(int screen_width, int screen_height)
{	
	int i = 0;
	for (const Button& button : buttons)
	{
		DrawRectangle(button.x, button.y, button.width, button.height, Fade(LIGHTGRAY, 0.3f));

		if (i == highlighted)
		{
			// top bottom left right
			DrawLine(button.x, button.y, button.x+button.width, button.y, LIGHTGRAY);
			DrawLine(button.x, button.y+button.height, button.x+button.width, button.y+button.height, LIGHTGRAY);
			DrawLine(button.x, button.y, button.x, button.y+button.height, LIGHTGRAY);
			DrawLine(button.x+button.width, button.y, button.x+button.width, button.y+button.height, LIGHTGRAY);
		}
		else
		{
			// left
			if (i == 0) DrawLine(button.x, button.y, button.x, button.y+button.height, GRAY);
			// Draw the rest of a square (top, bottom, right)
			DrawLine(button.x+button.width, button.y, button.x+button.width, button.y+button.height, GRAY);
			DrawLine(button.x, button.y, button.x+button.width, button.y, GRAY);
			DrawLine(button.x, button.y+button.height, button.x+button.width, button.y+button.height, GRAY);
		}


		i++;
	}
}