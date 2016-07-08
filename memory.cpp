
#include "gb.h"

u8 readMemory(u16 loc)
{
	if (loc >= 0xE000 && loc <= 0xFDFF)
	{
		//shadow of working RAM (excluding final 512 bytes)
		return gb.memory[loc - 0x1000];
	}
	else if (loc >= 0xF000)
	{
		switch (loc)
		{
		case 0xFF40://LCDC
			return gb.gpu.lcdc_status;
		case 0xFF44://LY
			return gb.gpu.scanline;
		case 0xFF45://LYC
			return gb.gpu.scanlinecompare;
		case 0xFFFF:
			return gb.interruptReg;
		default:
			return 0;
		}
	}
	else
	{
		return gb.memory[loc];
	}
}

u16 readMemory16(u16 loc)
{
	return (readMemory(loc) << 8) | readMemory(loc + 1);
}

void writeMemory(u16 loc, u8 val)
{
	if (loc >= 0xE000 && loc <= 0xFDFF)
	{
		//shadow of working RAM (excluding final 512 bytes)
		gb.memory[loc - 0x1000] = val;
	}
	else if (loc >= 0xF000)
	{
		switch (loc)
		{
		case 0xFF40://LCDC
			gb.gpu.lcdc_status = val;
			break;
		case 0xFF45://LYC
			gb.gpu.scanlinecompare = val;
			break;
		case 0xFFFF:
			gb.interruptReg = val;
			break;
		default:
			break;
		}
	}
	else
	{
		gb.memory[loc] = val;
	}
}

void writeMemory16(u16 loc, u16 val)
{
	writeMemory(loc, val >> 8);
	writeMemory(loc + 1, val & 0xFF);
}
