#include "main.h"
#include "mathext.h"

using namespace std;

#define PI 3.14f
#define IMAGE_WIDTH 640
#define IMAGE_HEIGHT 480
#define GRID_CELL_SIZE 0.1f
#define TARGET_COUNT 3

typedef chrono::duration<float> FloatSeconds;

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


	FloatSeconds currentTimeFloatSec = chrono::duration_cast<FloatSeconds>(chrono::high_resolution_clock::now().time_since_epoch());
	float startTimeMs = currentTimeFloatSec.count();

	SDL_Surface* screen = SDL_GetWindowSurface(window);
	SDL_FillRect(screen, 0, 0);
	
	const Uint32 lineColor = SDL_MapRGB(screen->format, 255, 255, 255); // white color

	while ((currentTimeFloatSec.count() - startTimeMs) < 3.0f)
	{
		cout << (currentTimeFloatSec.count() - startTimeMs) << endl;

		currentTimeFloatSec = chrono::duration_cast<FloatSeconds>(chrono::high_resolution_clock::now().time_since_epoch());

		DrawGraphics(screen, currentTimeFloatSec.count());
		DrawSimpleLine(screen, Vector2(100, 200), Vector2(300, 400), 0.01f, lineColor);

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

void DrawSimpleLine(SDL_Surface* surface, Vector2 point0, Vector2 point1, float stepSize, Uint32 color)
{

	for (float step = 0.0f; step < 1.0f; step += stepSize)
	{
		int x = point0.x + (point1.x - point0.x) * step;
		int y = point0.y + (point1.y - point0.y) * step;

		PutPixel(surface, x, y, color);
	}
}

void DrawGraphics(SDL_Surface *surface, float currentTimeSec) {

	Vector2 imageDimensions = Vector2(IMAGE_WIDTH, IMAGE_HEIGHT);
	Vector2 imageCenter = imageDimensions / 2.0f;
	Vector2 zero = Vector2(0.0, 0.0);


	for (int columnIndex = 0; columnIndex < IMAGE_WIDTH; ++columnIndex) {
		for (int rowIndex = 0; rowIndex < IMAGE_HEIGHT; ++rowIndex) {

			Vector2 position = Vector2(columnIndex, rowIndex);

			Vector2 centeredPosition = (position - (imageDimensions * 0.5f)) / imageDimensions.y;
			Vector2 gridBoundPosition = GetGridPosition(centeredPosition);

			Vector2 cellBoundPosition = gridBoundPosition - gridBoundPosition.Round();

			float redIntensity = 0.0f;
			float blueIntensity = 0.0f;

			for (int targetIndex = 0; targetIndex < TARGET_COUNT; ++targetIndex)
			{
				float f_targetIndex = float(targetIndex);

				float trigOffset = (PI / float(TARGET_COUNT)) * f_targetIndex;
				Vector2 targetPosition = Vector2(sinf(currentTimeSec + trigOffset) * 0.51f + tanf(f_targetIndex + trigOffset), cosf(currentTimeSec + trigOffset) * 0.1f + sinf(f_targetIndex + trigOffset));
				Vector2 gridBoundTargetPosition = GetGridPosition(targetPosition);
				Vector2 edgeBoundPosition = Vector2(gridBoundTargetPosition.x, gridBoundTargetPosition.y);

				float distanceToTarget = Vector2::Distance(gridBoundPosition, gridBoundPosition.Round()) + Vector2::Distance(gridBoundTargetPosition, edgeBoundPosition);

				redIntensity += (cellBoundPosition.GeometricInverse() * (GRID_CELL_SIZE / (distanceToTarget * 9.5f))).Length() * GRID_CELL_SIZE;
			}

			for (int targetIndex = 0; targetIndex < TARGET_COUNT; ++targetIndex)
			{
				float f_targetIndex = float(targetIndex);

				float trigOffset = (PI / float(TARGET_COUNT)) * f_targetIndex;

				Vector2 targetPosition = Vector2(sinf(currentTimeSec + trigOffset) * 0.51f + sinf(f_targetIndex + trigOffset), tanf(currentTimeSec + trigOffset) * 0.1 + sinf(f_targetIndex + trigOffset));
				Vector2 gridBoundTargetPosition = GetGridPosition(targetPosition);
				Vector2 edgeBoundPosition = Vector2(gridBoundTargetPosition.x, gridBoundTargetPosition.y);

				float distanceToTarget = Vector2::Distance(gridBoundPosition, gridBoundTargetPosition.Round()) + Vector2::Distance(gridBoundPosition, edgeBoundPosition);

				blueIntensity += (cellBoundPosition.GeometricInverse() * (GRID_CELL_SIZE / (distanceToTarget * 15.5f))).Length() * GRID_CELL_SIZE;

			}


			float step = smoothstep(0.2f, 1.0f, blueIntensity + redIntensity) * 255.0f;

			redIntensity *= 255.0f;
			blueIntensity *= 255.0f;
			step = redIntensity - step;
			PutPixel(surface, columnIndex, rowIndex, SDL_MapRGB(surface->format, step + redIntensity, step, step + blueIntensity));
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