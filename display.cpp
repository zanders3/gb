
#include "gb.h"
#include "display.h"
#include <memory>
#include <assert.h>

//http://bgb.bircd.org/pandocs.htm

u8 screenData[SCREEN_HEIGHT][SCREEN_WIDTH][4];

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
}

void GB_gpudrawtile(i32 idx, i32 ix, i32 iy)
{
	assert(ix >= 0 && ix < 20);
	assert(iy >= 0 && iy < 18);
	assert(idx >= 0 && idx < 255);
	const Tile* tiles = reinterpret_cast<Tile*>(&gb.memory[0x8000]);
	for (i32 row = 0; row < 8; ++row)
	{
		const i32 py = (iy * 8) + row;
		const Tile::TileRow& line = tiles[idx].rows[row];
		for (i32 x = 7, bit = 1; x >= 0; x--, bit += bit)
		{
			u8 col = (line.lsbcolor & bit) ? 127 : 0;
			col += (line.msbcolor & bit) ? 127 : 0;
			const i32 px = (ix * 8) + x;
			screenData[py][px][0] = col;
			screenData[py][px][1] = col;
			screenData[py][px][2] = col;
		}
	}
}

u8* GB_gpuscreen()
{
	const u8* backgroundMap = &gb.memory[0x9800];

	for (i32 y = 0; y < 18; ++y)
	{
		for (i32 x = 0; x < 20; ++x)
		{
			const u8 tileIdx = backgroundMap[(y * 32) + x];
			GB_gpudrawtile(tileIdx, x, y);
		}
	}

	return &screenData[0][0][0];
}

bool GB_gputick(u8 opcode)
{
	//tick gpu
	bool scanlineComplete = false;
	{
		u8 elapsedTicks = instructionOpCodeTicks[opcode];
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
		gb.gpu.lcdc_status = (gb.gpu.lcdc_status & 0xF8) | gb.gpu.mode;
		if (gb.gpu.scanlinecompare == gb.gpu.scanline)
			gb.gpu.lcdc_status |= 4;//calculate coincidence flag
	}

	return scanlineComplete;
}
