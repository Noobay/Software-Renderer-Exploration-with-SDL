#include "main.h"
#include "mathext.h"

using namespace std;

#define PI 3.14f
#define IMAGE_WIDTH 640
#define IMAGE_HEIGHT 480
#define GRID_CELL_SIZE 0.1f
#define TARGET_COUNT 3

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
	const Uint32 line3Color = SDL_MapRGB(screen->format, 255, 0, 0); // red color

	while ((currentTime - startTime) < runDuration)
	{
		cout << (currentTime - startTime).count() << endl;
		currentTime = chrono::high_resolution_clock::now();

		DrawLineSimple(screen, Vector2(350, 200), Vector2(400, 400), line1Color);
		DrawLineSimple(screen, Vector2(150, 200), Vector2(350, 350), line2Color);
		DrawLineSimple(screen, Vector2(100, 350), Vector2(425, 350), line3Color);

		SDL_UpdateWindowSurface(window);
	}

	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}

Vector2 GetGridPosition(const Vector2 &position)
{
	return Vector2((position.x / GRID_CELL_SIZE), (position.y / GRID_CELL_SIZE));
}

void DrawLineSimple(SDL_Surface* surface, Vector2 point0, Vector2 point1, Uint32 color)
{
	float lengthX = point1.x - point0.x;
	float lengthY = point1.y - point0.y;

	// The main issue with this method is that it is undefined where lengthX = 0 (i.e no vertical lines).
	SDL_assert(lengthX != 0);

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