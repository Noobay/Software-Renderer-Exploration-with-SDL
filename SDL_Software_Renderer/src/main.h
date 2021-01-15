#include <iostream>
#include <chrono>
#include <random>
#include "SDL.h"
#include "vectors.h"
#include "mathext.h"

void PutPixel(SDL_Surface* surface, int x, int y, Uint32 pixel);
void DrawGraphics(SDL_Surface* surface, float currentTimeSec);