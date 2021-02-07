#include "main.h"

using namespace std;

#define IMAGE_WIDTH 640
#define IMAGE_HEIGHT 480

int main(int argc, char* argv[])
{

	SDL_Init(SDL_INIT_VIDEO);

	SDL_Window* window = SDL_CreateWindow(
		"SDL2Test",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		IMAGE_WIDTH,
		IMAGE_HEIGHT,
		0
	);


	chrono::seconds runDuration = chrono::seconds(3);

	chrono::high_resolution_clock::time_point startTime = chrono::high_resolution_clock::now();
	chrono::high_resolution_clock::time_point currentTime = chrono::high_resolution_clock::now();

	SDL_Surface* screen = SDL_GetWindowSurface(window);
	SDL_FillRect(screen, 0, 0);
	
	const Uint32 line1Color = SDL_MapRGB(screen->format, 255, 255, 255); // white color
	const Uint32 line2Color = SDL_MapRGB(screen->format, 255, 0, 255); // pink color

	while ((currentTime - startTime) < runDuration)
	{
		cout << (currentTime - startTime).count() << endl;
		currentTime = chrono::high_resolution_clock::now();

		DrawLineSimple(screen, Vector2(50, 0), Vector2(50, 400), line1Color);
		DrawLineAdvanced(screen, Vector2(100, 0), Vector2(100, 400), line2Color);

		SDL_UpdateWindowSurface(window);
	}

	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}

void DrawLineSimple(SDL_Surface* surface, Vector2 point0, Vector2 point1, Uint32 color)
{
	// The main issue with this method is that it is undefined where lengthX = 0 (i.e no vertical lines).

	float lengthX = point1.x - point0.x;
	float lengthY = point1.y - point0.y;

	Vector2 from = Vector2(0.0, 0.0);
	Vector2 to = Vector2(0.0, 0.0);

	if (lengthX > 0)
	{
		from.x = point0.x;
		to.x = point1.x;
	}
	else
	{
		from.x = point1.x;
		to.x = point0.x;
	}

	if (lengthY > 0)
	{
		from.y = point0.y;
		to.y = point1.y;
	}
	else
	{
		from.y = point1.y;
		to.y = point0.y;
	}

	for (float x = from.x; x <= to.x; x++)
	{
		float deltaCurrentToOriginX = x - from.x;

		float progressX = deltaCurrentToOriginX / lengthX;
		int y = (float)(from.y * (1. - progressX)) + (to.y * progressX);

		PutPixel(surface, x, y, color);
	}
}

void DrawLineAdvanced(SDL_Surface* surface, Vector2 point0, Vector2 point1, Uint32 color)
{
	bool steep = false;

	float diffX = point0.x - point1.x;
	float diffY = point0.y - point1.y;

	if (abs(diffX) < abs(diffY))
	{
		// This line prevents the difference over X to be equal to 0, unless both differences over X and Y are 0 and no line should be drawn anyway-
		// resolving the case where it is undefined.
		swap(point0.x, point0.y);
		swap(point1.x, point1.y);

		steep = true;
	}

	if (diffX > 0)
	{
		swap(point0, point1);
	}

	for (int x = point0.x; x <= point1.x; ++x)
	{
		float progressX = (x - point0.x) / (point1.x - point0.x);
		float y = point0.y * (1.0f - progressX) + (point1.y * progressX);

		if (steep)
		{
			PutPixel(surface, y, x, color);
		}
		else
		{
			PutPixel(surface, x, y, color);
		}
	}

}

void PutPixel(SDL_Surface* surface, int x, int y, Uint32 pixel)
{
	int bpp = surface->format->BytesPerPixel;
	/* Here p is the address to the pixel we want to set */
	Uint8* p = (Uint8*)surface->pixels + y * surface->pitch + x * bpp;

	switch (bpp) {
		case 1:
			*p = pixel;
			break;

		case 2:
			*(Uint16*)p = pixel;
			break;

		case 3:
			if (SDL_BYTEORDER == SDL_BIG_ENDIAN) {
				p[0] = (pixel >> 16) & 0xff;
				p[1] = (pixel >> 8) & 0xff;
				p[2] = pixel & 0xff;
			}
			else {
				p[0] = pixel & 0xff;
				p[1] = (pixel >> 8) & 0xff;
				p[2] = (pixel >> 16) & 0xff;
			}
			break;

		case 4:
			*(Uint32*)p = pixel;
			break;
	}
}