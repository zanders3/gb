#pragma once

#define SCREEN_HEIGHT 144
#define SCREEN_WIDTH 160

void GB_gpuinit();
const u8* GB_gpuscreen();
bool GB_gputick(u8 opcode, i32& ticksElapsed);

