#include "SDL.h" 
#include <random>

void PutPixel(SDL_Surface* surface, int x, int y, Uint32 pixel);

const Uint16 IMAGE_WIDTH = 640;
const Uint16 IMAGE_HEIGHT = 480;

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

	SDL_Surface *screen = SDL_GetWindowSurface(window);
	SDL_FillRect(screen, 0, 0);

	DrawGraphics(screen);

	SDL_UpdateWindowSurface(window);

	SDL_Delay(3000);

	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}

void DrawGraphics(SDL_Surface *surface) {

	for (int columnIndex = 0; columnIndex < IMAGE_WIDTH; ++columnIndex) {
		for (int rowIndex = 0; rowIndex < IMAGE_HEIGHT; ++rowIndex) {

			int r = rand() % 255;
			int g = rand() % 255;
			int b = rand() % 255;

			PutPixel(surface, columnIndex, rowIndex, SDL_MapRGB(surface->format, r, g, b));
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