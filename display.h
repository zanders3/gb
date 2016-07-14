#pragma once

#define SCREEN_HEIGHT 100
#define SCREEN_WIDTH 100

void GB_gpuinit();
u8* GB_gpuscreen();
bool GB_gputick(u8 opcode);

