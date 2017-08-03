
#include "gb.h"
#include <memory>
#include <cassert>

//http://imrannazar.com/GameBoy-Emulation-in-JavaScript:-Memory-Banking

struct MBC {
    u8* rom;
    u32 romLength;
    u32 romOffset;
    u32 romBank;
    u32 ramOffset;
    u8 cartType;
    bool mode;
    u8 ram[0x2000];
} g_mbc;

u16 g_memBreakpoint = 0xFFFF;
bool g_hitMemBreakpoint = false;

void initMemoryBanks(u8* rom, u32 romLength)
{
    g_mbc.rom = rom;
    g_mbc.romLength = romLength;
    g_mbc.romOffset = 0x4000;
    g_mbc.romBank = 0;
    g_mbc.ramOffset = 0x0000;
    g_mbc.cartType = rom[0x147];
    g_mbc.mode = false;
    memset(g_mbc.ram, 0, sizeof(g_mbc.ram));
}

u8 readMemory(u16 loc)
{
    if (loc >= 0x4000 && loc <= 0x7FFF)
    {
        //ROM (switched bank)
        return g_mbc.rom[g_mbc.romOffset + (loc & 0x3FFF)];
    }
    else if (loc >= 0xA000 && loc <= 0xBFFF)
    {
        //RAM (switched bank)
        return g_mbc.ram[g_mbc.ramOffset + (loc & 0x1FFF)];
    }
	else if (loc >= 0xE000 && loc <= 0xFDFF)
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

    if (loc <= 0x1FFF)
    {
        //External RAM switch
    }
    else if (loc >= 0x2000 && loc <= 0x3FFF)
    {
        // ROM Bank
        switch (g_mbc.cartType)
        {
        case 1:
        case 2:
        case 3:
            val &= 0x0F;
            if (!val) val = 1;
            g_mbc.romBank = (g_mbc.romBank & 0x60) + val;
            g_mbc.romOffset = g_mbc.romBank * 0x4000;
            break;
        default:
            assert(false);
            break;
        }
    }
    else if (loc >= 0x4000 && loc <= 0x5FFF)
    {
        // RAM Bank
        switch (g_mbc.cartType)
        {
        case 1:
        case 2:
        case 3:
            if (g_mbc.mode)
            {
                //RAM mode: set RAM bank
                g_mbc.ramOffset = (val & 3) * 0x2000;
            }
            else
            {
                //ROM mode: set high bits of ROM bank
                g_mbc.romBank = (g_mbc.romBank & 0x1F) + ((val & 3) << 5);
                g_mbc.romOffset = g_mbc.romBank * 0x4000;
            }
            break;
        default:
            assert(false);
            break;
        }
    }
    else if (loc >= 0x6000 && loc <= 0x7FFF)
    {
        switch (g_mbc.cartType)
        {
        case 2:
        case 3:
            g_mbc.mode = val & 1;
            break;
        default:
            assert(false);
            break;
        }
    }
    else if (loc >= 0xA000 && loc <= 0xBFFF)
    {
        // External RAM
        g_mbc.ram[g_mbc.ramOffset + (loc & 0x1FFF)] = val;
    }
    else if (loc >= 0xE000 && loc <= 0xFDFF)
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
