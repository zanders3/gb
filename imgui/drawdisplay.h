#pragma once

#include "gb.h"

void* getscreentex();

void display_init(int width, int height);
void display_draw(const u8* gpuScreen);
