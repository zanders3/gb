
#include "gb.h"
#include "display.h"
#include <memory>
#include <assert.h>

//http://bgb.bircd.org/pandocs.htm

u8 screenData[SCREEN_HEIGHT][SCREEN_WIDTH][4];
u8 bgScreenData[SCREEN_HEIGHT][SCREEN_WIDTH][4];
u8 tileData[TILES_HEIGHT][TILES_WIDTH][4];

#define INST(disassembly, opCodeLength, function, opCodeTicks) opCodeTicks,
const u8 instructionOpCodeTicks[256] =
{
#include "opcodes.h"
};
#undef INST

struct Tile
{
	struct TileRow {
		u8 lsbcolor;
		u8 msbcolor;
	} rows[8];
};

void GB_gpuinit()
{
	memset(screenData, 0, sizeof(screenData));
    memset(tileData, 0, sizeof(tileData));
}

template <typename T> void GB_gpudrawtile(T& screen, i32 idx, i32 ix, i32 iy)
{
	assert(ix >= 0 && ix < 20*8);
	assert(iy >= 0 && iy < 18*8);
	assert(idx >= 0 && idx <= 255);
    static_assert(sizeof(Tile) == 0x10, "invalid size");
	const Tile* tiles = reinterpret_cast<Tile*>(&gb.memory[0x8000]);
	for (i32 row = 0; row < 8; ++row)
	{
		const i32 py = iy + row;
		const Tile::TileRow& line = tiles[idx].rows[row];
		for (i32 x = 7, bit = 1; x >= 0; x--, bit += bit)
		{
			u8 col = 4 - (((line.lsbcolor & bit) ? 1 : 0) | ((line.msbcolor & bit) ? 2 : 0));
			col *= 63;
			const i32 px = ix + x;
            screen[py][px][0] = col;
            screen[py][px][1] = col;
            screen[py][px][2] = col;
            screen[py][px][3] = 255;
		}
	}
}

const u8* GB_tiledata()
{
    for (int i = 0; i <= 255; i++)
        GB_gpudrawtile(tileData, i, (i % 16) * 8, (i / 16) * 8);
    return &tileData[0][0][0];
}

const u8* GB_bgscreen()
{
    const u8* backgroundMap = &gb.memory[0x9800];

    for (i32 y = 0; y < 18; ++y)
    {
        for (i32 x = 0; x < 20; ++x)
        {
            const u8 tileIdx = backgroundMap[(y * 32) + x];
            GB_gpudrawtile(bgScreenData, tileIdx, x * 8, y * 8);
        }
    }

    return &bgScreenData[0][0][0];
}

const u8* GB_gpuscreen()
{
	const u8* backgroundMap = &gb.memory[0x9800];

	for (i32 y = 0; y < 18; ++y)
	{
		for (i32 x = 0; x < 20; ++x)
		{
			const u8 tileIdx = backgroundMap[(y * 32) + x];
			GB_gpudrawtile(screenData, tileIdx, x * 8, y * 8);
		}
	}

	const Sprite* sprites = reinterpret_cast<Sprite*>(&gb.memory[0xFE00]);
	for (i32 i = 0; i < 40; ++i)
	{
		const Sprite& sprite = sprites[i];
		if (sprite.XPos > 0 && sprite.XPos < 168 && sprite.YPos > 0 && sprite.YPos < 160)
		{
			GB_gpudrawtile(screenData, sprite.TileNum, sprite.XPos - 8, sprite.YPos - 16);
		}
	}

    return &screenData[0][0][0];
}

bool GB_gputick(u8 opcode, i32& ticksElapsed)
{
	//tick gpu
	bool scanlineComplete = false;
	{
		u8 elapsedTicks = instructionOpCodeTicks[opcode];
        ticksElapsed = elapsedTicks;
		gb.gpu.modeclock += elapsedTicks;
		switch (gb.gpu.mode)
		{
		case 2://LCD reading from OAM memory
			if (gb.gpu.modeclock >= 80)
			{
				gb.gpu.modeclock = 0;
				gb.gpu.mode = 3;
			}
			break;
		case 3://LCD reading from OAM and VRAM
			if (gb.gpu.modeclock >= 172)
			{
				gb.gpu.modeclock = 0;
				gb.gpu.mode = 0;
			}
			break;
		case 0://H-Blank period
			if (gb.gpu.modeclock >= 204)
			{
				gb.gpu.modeclock = 0;
				gb.gpu.scanline++;
				scanlineComplete = true;

				if (gb.gpu.scanline == 143)
				{
					gb.interruptFlag.vBlank = true;
					gb.gpu.mode = 1;
				}
				else
				{
					gb.gpu.mode = 2;
				}
			}
			break;
		case 1://V-Blank period
			if (gb.gpu.modeclock >= 456)
			{
				gb.gpu.modeclock = 0;
				gb.gpu.scanline++;

				scanlineComplete = true;

				if (gb.gpu.scanline > 153)
				{
					gb.gpu.mode = 2;
					gb.gpu.scanline = 0;
				}
			}
			break;
		}
		//update lcdc status register mode bits
		gb.gpu.lcdcStatus = (gb.gpu.lcdcStatus & 0xF8) | gb.gpu.mode;
		if (gb.gpu.scanlinecompare == gb.gpu.scanline)
			gb.gpu.lcdcStatus |= 4;//calculate coincidence flag
	}

	//run dma transfer
	if (gb.gpu.dmaRegister != 0 && gb.gpu.mode == 3)//LCD controller copying to OAM
	{
		u16 sourceLocation = (gb.gpu.dmaRegister << 8);
		memcpy(&gb.memory[0xFE00], &gb.memory[sourceLocation], 0x9F);
		gb.gpu.dmaRegister = 0;
	}

	return scanlineComplete;
}
