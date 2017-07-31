
#include "gb.h"

u16 g_memBreakpoint = 0xFFFF;
bool g_hitMemBreakpoint = false;

u8 readMemory(u16 loc)
{
	if (loc >= 0xE000 && loc <= 0xFDFF)
	{
		//shadow of working RAM (excluding final 512 bytes)
		return gb.memory[loc - 0x1000];
	}
	else if (loc >= 0xFF00 && loc < 0xFF80)
	{
		switch (loc)
		{
		case 0xFF00://Joypad R/W
			return gb.joypadInput;
		case 0xFF40://LCDC
			return gb.gpu.lcdcStatus;
		case 0xFF44://LY
			return gb.gpu.scanline;
		case 0xFF45://LYC
			return gb.gpu.scanlinecompare;
		case 0xFF0F://Interrupt Flag
			return gb.interruptFlag.value;
		default:
			return 0;
		}
	}
	else if (loc < 0xFFFF)
	{
		return gb.memory[loc];
	}
	else//0xFFFF Interrupt Enable
	{
		return gb.interruptReg.value;
	}
}

u16 readMemory16(u16 loc)
{
	return (readMemory(loc) << 8) | readMemory(loc + 1);
}

#include <cassert>

void writeMemory(u16 loc, u8 val)
{
    if (loc == g_memBreakpoint)
        g_hitMemBreakpoint = true;

	if (loc >= 0xE000 && loc <= 0xFDFF)
	{
		//shadow of working RAM (excluding final 512 bytes)
		gb.memory[loc - 0x1000] = val;
	}
	else if (loc >= 0xFF00 && loc < 0xFF80)
	{
		switch (loc)
		{
		case 0xFF00://Joypad R/W
			gb.joypadInput = val;
			break;
		case 0xFF40://LCDC
			gb.gpu.lcdcStatus = val;
			break;
		case 0xFF45://LYC
			gb.gpu.scanlinecompare = val;
			break;
		case 0xFF0F://Interrupt Flag
			gb.interruptFlag.value = val;
			break;
		case 0xFF46://DMA register
			gb.gpu.dmaRegister = val;
			break;
		default:
			break;
		}
	}
	else if (loc < 0xFFFF)
	{
		gb.memory[loc] = val;
	}
	else//0xFFFF Interrupt Enable
	{
		gb.interruptReg.value = val;
	}
}

void writeMemory16(u16 loc, u16 val)
{
	writeMemory(loc, val >> 8);
	writeMemory(loc + 1, val & 0xFF);
}
