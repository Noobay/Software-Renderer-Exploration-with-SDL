#include "main.h"
#include "importers/obj_importer.h"

using namespace std;

const int IMAGE_WIDTH = 640;
const int IMAGE_HEIGHT = 480;

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


	chrono::high_resolution_clock::time_point importStartTime = chrono::high_resolution_clock::now();
	chrono::high_resolution_clock::time_point importCurrentTime = chrono::high_resolution_clock::now();
	ObjImporter::OBJData* objData = ObjImporter::Import("D:\\SlowRepos\\Journey\\2D_Software_Renderer\\First SDL Window\\SDL_Software_Renderer\\assets\\Holodeck\\Holodeck.obj");
	importCurrentTime = chrono::high_resolution_clock::now();
	cout << "Import Time: " << (importCurrentTime - importStartTime).count() / 1000000000.0f << " seconds" << endl;
	fgetc(stdin);

	chrono::seconds runDuration = chrono::seconds(3);

	chrono::high_resolution_clock::time_point startTime = chrono::high_resolution_clock::now();
	chrono::high_resolution_clock::time_point currentTime = chrono::high_resolution_clock::now();

	SDL_Surface* screen = SDL_GetWindowSurface(window);
	SDL_FillRect(screen, 0, 0);

	const Uint32 line1Color = SDL_MapRGB(screen->format, 255, 255, 255); // white color
	const Uint32 line2Color = SDL_MapRGB(screen->format, 255, 0, 255); // pink color

	float objScale = 0.1;

	while (true)
	{
		currentTime = chrono::high_resolution_clock::now();
		
		int activeVertex = 0;

		for (int modelIdx = 0; modelIdx < objData->modelCount; ++modelIdx)
		{
			ObjImporter::OBJModel activeModel = objData->models[modelIdx];
			for (int bucketIdx = 0; bucketIdx < activeModel.bucketCount; ++bucketIdx)
			{
				ObjImporter::Vector3Bucket activeBucket = activeModel.vertexBuckets->at(bucketIdx);
				for (int i = 0; i < ObjImporter::BUCKET_VERTEX_COUNT - 2 && activeVertex < activeModel.vertexCount - 2; ++i)
				{
					Vector3 objectSpaceVertex1 = activeBucket.values[i];
					Vector3 objectSpaceVertex2 = activeBucket.values[i + 1];
					Vector3 objectSpaceVertex3 = activeBucket.values[i + 2];

					Vector3 screenSpaceVertex1 = Vector3(objectSpaceVertex1.x * objScale + (IMAGE_WIDTH / 2.0), 0.0f, objectSpaceVertex1.z * objScale + (IMAGE_HEIGHT / 2.0));
					Vector3 screenSpaceVertex2 = Vector3(objectSpaceVertex2.x * objScale + (IMAGE_WIDTH / 2.0), 0.0f, objectSpaceVertex2.z * objScale + (IMAGE_HEIGHT / 2.0));
					Vector3 screenSpaceVertex3 = Vector3(objectSpaceVertex3.x * objScale + (IMAGE_WIDTH / 2.0), 0.0f, objectSpaceVertex3.z * objScale + (IMAGE_HEIGHT / 2.0));

					DrawLineAdvanced(screen, Vector2(screenSpaceVertex1.x, screenSpaceVertex1.z), Vector2(screenSpaceVertex2.x, screenSpaceVertex2.z), line2Color);
					DrawLineAdvanced(screen, Vector2(screenSpaceVertex2.x, screenSpaceVertex2.z), Vector2(screenSpaceVertex3.x, screenSpaceVertex3.z), line2Color);
					DrawLineAdvanced(screen, Vector2(screenSpaceVertex3.x, screenSpaceVertex3.z), Vector2(screenSpaceVertex1.x, screenSpaceVertex1.z), line2Color);

					++activeVertex;
				}
			}
		}

		SDL_Event curEvents;
		if (SDL_PollEvent(&curEvents)) {
			if (curEvents.type == SDL_QUIT)
				break;

			//If a key was pressed
			if (curEvents.type == SDL_KEYDOWN)
			{
				//Set the proper message surface
				switch (curEvents.key.keysym.sym)
				{
					case SDLK_UP:
						objScale += 0.01f;
						break;
					case SDLK_DOWN:
						objScale -= 0.01f;
						break;
				}
			}
		}

		SDL_UpdateWindowSurface(window);
		SDL_FillRect(screen, 0, 0);

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
		float y = (from.y * (1.0f - progressX)) + (to.y * progressX);

		PutPixel(surface, (int)x, (int)y, color);
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

	if (point0.x > point1.x)
		swap(point0, point1);

	float lengthX = point1.x - point0.x;

	for (int x = (int)point0.x; x <= point1.x; ++x)
	{
		float progressX = (x - point0.x) / lengthX;
		float y_float = point0.y * (1.0f - progressX) + (point1.y * progressX);
		int y_rounded = (int)roundf(y_float);

		if (steep)
			PutPixel(surface, y_rounded, x, color);
		else
			PutPixel(surface, x, y_rounded, color);
	}

}

void PutPixel(SDL_Surface* surface, int x, int y, Uint32 pixel)
{

	int bpp = surface->format->BytesPerPixel;
	/* Here p is the address to the pixel we want to set */

	int offset = y * surface->pitch + x * bpp;

	Uint8* p = (Uint8*)surface->pixels + offset;

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