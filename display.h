#pragma once

#define SCREEN_HEIGHT 144
#define SCREEN_WIDTH 160
#define TILES_WIDTH (16 * 8)
#define TILES_HEIGHT (24 * 8)

struct Sprite
{
    u8 YPos, XPos, TileNum, Attributes;
};

void GB_gpuinit();
const u8* GB_gpuscreen();
const u8* GB_tiledata();
const u8* GB_bgscreen();
bool GB_gputick(u8 opcode, i32& ticksElapsed);

