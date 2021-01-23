#include <iostream>
#include <chrono>
#include <random>
#include "SDL.h"
#include "vectors.h"
#include "mathext.h"

void PutPixel(SDL_Surface* surface, int x, int y, Uint32 pixel);
void DrawLineSimple(SDL_Surface* surface, Vector2 point0, Vector2 point1, Uint32 color);
