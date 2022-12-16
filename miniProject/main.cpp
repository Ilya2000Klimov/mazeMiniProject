// O------------------------------------------------------------------------------O
// | Example "Hello World" Program (main.cpp)                                     |
// O------------------------------------------------------------------------------O

#include <iostream>
#include <stack>
#include <queue>
#include <vector>


#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

using olc::vf2d;
using olc::Pixel;
using std::vector;




class Window : public olc::PixelGameEngine
{
public:
	Window()
	{
		// Name your application
		sAppName = "Maze Game";
	}
private:
	int maseW = 15;
	int maseH = 15;

	int mazeRealW;
	int mazeRealH;

	int centerScreenW = ScreenWidth() / 2;
	int centerScreenH = ScreenHeight() / 2;

	uint8_t* maze;
	uint8_t* mazeDirections;

	vf2d playerPosition;
	vf2d goalPosition;

	enum scene
	{
		menue = 1;
		
	};

public:
	bool OnUserCreate() override
	{
		// Called once at the start, so create things here
		Clear(olc::DARK_BLUE);

		// Draw Boundary
		DrawLine(10, 10, ScreenWidth() - 10, 10, olc::YELLOW);
		DrawLine(10, 10, 10, ScreenHeight() - 10, olc::YELLOW);
		DrawLine(ScreenWidth() - 10, 10, ScreenWidth() - 10, ScreenHeight() - 10, olc::YELLOW);

		DrawLine(10, ScreenHeight() - 10, ScreenWidth() - 10, ScreenHeight() - 10, olc::YELLOW);

		//draw text
		DrawString(ScreenWidth()*7/10, 2, "Back to Menue", olc::YELLOW);


		return true;
	}
	bool OnUserUpdate(float fElapsedTime) override
	{
		/*// Called once per frame, draws random coloured pixels
		for (int x = 0; x < ScreenWidth(); x++)
			for (int y = 0; y < ScreenHeight(); y++)
				Draw(x, y, olc::Pixel(rand() % 256, rand() % 256, rand() % 256));
		return true;*/

		return true;
	}
};

#include "wtypes.h"

void GetDesktopResolution(int& horizontal, int& vertical)
{
	RECT desktop;
	// Get a handle to the desktop window
	const HWND hDesktop = GetDesktopWindow();
	// Get the size of screen to the variable desktop
	GetWindowRect(hDesktop, &desktop);
	// The top left corner will have coordinates (0,0)
	// and the bottom right corner will have coordinates
	// (horizontal, vertical)
	horizontal = desktop.right;
	vertical = desktop.bottom;
}


int main()
{

	int horizontal = 0;
	int vertical = 0;
	GetDesktopResolution(horizontal, vertical);

	Window demo;
	if (demo.Construct(horizontal / 5, vertical / 5, 4, 4))
		demo.Start();
	return 0;
}