#include <iostream>
#include <chrono>
#include <random>
#include "SDL.h"
#include "vectors.h"
#include "mathext.h"

void PutPixel(SDL_Surface* surface, int x, int y, Uint32 pixel);
void DrawGraphics(SDL_Surface* surface, float currentTimeSec);
void DrawSimpleLine(SDL_Surface* surface, Vector2 point0, Vector2 point1, float stepSize, Uint32 color);
