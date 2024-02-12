/*
	HAPI Start
	----------
	This solution contains an already set up HAPI project and this main file
	
	The directory structure and main files are:

	HAPI_Start - contains the Visual Studio solution file (.sln)
		HAPI_Start - contains the Visual Studio HAPI_APP project file (.vcxproj) and source code
			HAPI - the directory with all the HAPI library files
			Data - a place to put your data files with a few sample ones provided

	Additionally in the top directory there is a batch file for creating a redistributable Demo folder

	For help using HAPI please see the Reference folder in the HAPI sub-directory
*/

// Include the HAPI header to get access to all of HAPIs interfaces
#include <HAPI_lib.h>

// HAPI itself is wrapped in the HAPISPACE namespace
using namespace HAPISPACE;

// Every HAPI program has a HAPI_Main as an entry point
// When this function exits the program will close down

struct star
{
	int x;
	int y;
	int z;
	int sx;
	int sy;
	HAPI_TColour colo;
};

void setScreen(BYTE* screen, int width, int height, HAPI_TColour col)
{
	for (int offset = 0; offset < width * height * 4; offset += 4)
	{
		memcpy(screen + offset, &col, 4);
	}
}

void pixel(BYTE* screen, int width, HAPI_TColour col, int x, int y)
{
	memcpy(screen + ((x + y * width) * 4), &col, 4);
}

void clearScreen(BYTE* screen, int width, int height)
{
	memset(screen, 0, width * height * 4);
}

void HAPI_Main()
{
	HAPI.SetShowFPS(true);
	int width{ 1024 };
	int height{ 768 };
	const int NOS{ 4000 };
	bool zoom{ false };
	HAPI_TColour c{ 0, 0, 0 };

	if (!HAPI.Initialise(width, height, "Cheese"))
		return;

	BYTE* screen = HAPI.GetScreenPointer();

	star stars[NOS];
	int eyeDist{ 500 };
	int cx = width / 2;
	int cy = height / 2;

	for (int s = 0; s < NOS; s++)
	{
		star newStar;
		newStar.x = rand() % width;
		newStar.y = rand() % height;
		newStar.z = rand() % 500;
		newStar.colo = HAPI_TColour(70, 130, 180);
		stars[s] = newStar;
	}

	while (HAPI.Update())
	{

		if (HAPI.GetKeyboardData().scanCode['W'])
		{
			zoom = true;
		}

		if (HAPI.GetKeyboardData().scanCode['S'])
		{
			zoom = false;
		}

		setScreen(screen, width, height, c);

		for (int s = 0; s < NOS; s++)
		{
			if (zoom == false)
			{
				stars[s].z--;

				if (stars[s].z <= 0)
				{
					stars[s].z = 500;
				}
			}

			if (zoom == true)
			{
				stars[s].z++;

				if (stars[s].z >= 500)
				{
					stars[s].z = 0;
				}
			}

			stars[s].sx = ((eyeDist * (stars[s].x - cx)) / (stars[s].z + eyeDist)) + cx;
			stars[s].sy = ((eyeDist * (stars[s].y - cy)) / (stars[s].z + eyeDist)) + cy;
			pixel(screen, width, stars[s].colo, stars[s].sx, stars[s].sy);
		}
	}
}