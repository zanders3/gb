
//http://marc.rawer.de/Gameboy/Docs/GBCPUman.pdf
//http://gameboy.mongenel.com/dmg/opcodes.html
//http://imra0x%04Xazar.com/Gameboy-Z80-Opcode-Map
//http://bgb.bircd.org/pandocs.htm#videodisplay

#include "gb.h"
#include <memory>
#include "memory.h"

#ifdef _WIN32
#include <windows.h>
char buffer[256];

void logf(char const* const format, ...)
{
	va_list args;
	va_start(args, format);
	u32 num = _vsnprintf_s(buffer, 256, format, args);
	va_end(args);
	buffer[num] = '\0';
	OutputDebugStringA(buffer);
}
#else
#define logf printf
#endif

GB gb;

#define INST(disassembly, opCodeLength, function, opCodeTicks) disassembly,
const char* instructionDisassembly[256] =
{
#include "opcodes.h"
};
#undef INST

#define INST(disassembly, opCodeLength, function, opCodeTicks) opCodeLength,
const u8 instructionOpCodeLength[256] =
{
#include "opcodes.h"
};
#undef INST

#define INST(disassembly, opCodeLength, function, opCodeTicks) opCodeTicks,
const u8 instructionOpCodeTicks[256] =
{
#include "opcodes.h"
};
#undef INST

void GB_load(u8* rom, u32 romLength)
{
	memset(gb.memory, 0, sizeof(gb.memory));
	memcpy(gb.memory, rom, romLength);
	gb.af = 0x1B0;
	gb.bc = 0x13;
	gb.de = 0xD8;
	gb.hl = 0x14D;
	gb.sp = 0xFFFE;
	gb.pc = 0x100;
	gb.interruptsEnabled = false;

	memset(gb.gpu.framebuffer, 0, sizeof(gb.gpu.framebuffer));
	gb.gpu.lcdc_status = 0;
	gb.gpu.mode = 2;
	gb.gpu.modeclock = 0;
	gb.gpu.scanline = 0;
	gb.gpu.scanlinecompare = 0;

	writeMemory(0xFF05, 0x00); // TIMA
	writeMemory(0xFF06, 0x00); // TMA
	writeMemory(0xFF07, 0x00); // TAC
	writeMemory(0xFF10, 0x80); // NR10
	writeMemory(0xFF11, 0xBF); // NR11
	writeMemory(0xFF12, 0xF3); // NR12
	writeMemory(0xFF14, 0xBF); // NR14
	writeMemory(0xFF16, 0x3F); // NR21
	writeMemory(0xFF17, 0x00); // NR22
	writeMemory(0xFF19, 0xBF); // NR24
	writeMemory(0xFF1A, 0x7F); // NR30
	writeMemory(0xFF1B, 0xFF); // NR31
	writeMemory(0xFF1C, 0x9F); // NR32
	writeMemory(0xFF1E, 0xBF); // NR33
	writeMemory(0xFF20, 0xFF); // NR41
	writeMemory(0xFF21, 0x00); // NR42
	writeMemory(0xFF22, 0x00); // NR43
	writeMemory(0xFF23, 0xBF); // NR30
	writeMemory(0xFF24, 0x77); // NR50
	writeMemory(0xFF25, 0xF3); // NR51
	writeMemory(0xFF26, 0xF1); // NR52
	writeMemory(0xFF40, 0x91); // LCDC
	writeMemory(0xFF42, 0x00); // SCY
	writeMemory(0xFF43, 0x00); // SCX
	writeMemory(0xFF45, 0x00); // LYC
	writeMemory(0xFF47, 0xFC); // BGP
	writeMemory(0xFF48, 0xFF); // OBP0
	writeMemory(0xFF49, 0xFF); // OBP1
	writeMemory(0xFF4A, 0x00); // WY
	writeMemory(0xFF4B, 0x00); // WX
	writeMemory(0xFFFF, 0x00); // IE
}

void undefined() 
{
	logf("\t\taf = %04X bc = %04X de = %04X hl = %04X sp = %04X pc = %04X z = %d n = %d h = %d c = %d\n", gb.af, gb.bc, gb.de, gb.hl, gb.sp, gb.pc, gb.flags.z, gb.flags.n, gb.flags.h, gb.flags.c);
	logf("Unimplemented!\n");
}

void cpl()
{
	gb.a = ~gb.a;
	gb.flags.n = true;
	gb.flags.h = true;
}

void call(u16& nn)
{
	writeMemory16(gb.sp, gb.pc);
	gb.pc = nn;
	gb.sp -= 2;
}

void ret()
{
	gb.sp += 2;
	gb.pc = readMemory16(gb.sp);
}

void inc16(u16& reg)
{
	gb.flags.h = reg == ((u16)-1);
	reg += 1;
	gb.flags.z = reg == 0;
	gb.flags.n = false;
}

void inc8(u8& reg)
{
	gb.flags.h = reg == ((u8)-1);
	reg -= 1;
	gb.flags.z = reg == 0;
	gb.flags.n = false;
}

void dec16(u16& reg)
{
	gb.flags.h = reg == 0;
	reg -= 1;
	gb.flags.z = reg == 0;
	gb.flags.n = true;
}

void dec8(u8& reg)
{
	gb.flags.h = reg == 0;
	reg -= 1;
	gb.flags.z = reg == 0;
	gb.flags.n = true;
}

void or8(u8 reg)
{
	gb.a = gb.a | reg;
	gb.flags.flags = 0;
	gb.flags.z = gb.a == 0;
}

void cp8(u8 val)
{
	gb.flags.z = gb.a == val;
	gb.flags.n = true;
	gb.flags.c = gb.a < val;
}

void xor8(u8 reg)
{
	gb.a = gb.a ^ reg;
	gb.flags.flags = 0;
	gb.flags.z = gb.a == 0;
}

void GB_gputick();

bool GB_tick()
{
	//decode instruction
	const u8 opcode = gb.memory[gb.pc];
	const u32 opcodeLength = instructionOpCodeLength[opcode];
	if (opcodeLength == 2)
		gb.nn = gb.memory[gb.pc + 1];
	else if (opcodeLength == 3)
		gb.nn = gb.memory[gb.pc + 1] | (gb.memory[gb.pc + 2] << 8);

	{
		const char* disassembly = instructionDisassembly[opcode];
		logf("%04X ", gb.pc);
		if (opcodeLength > 1)
			logf(disassembly, gb.nn);
		else
			logf(disassembly);
		logf("\n");
	}

	//run instruction
	gb.pc += opcodeLength;
#define INST(disassembly, opCodeLength, function, opCodeTicks) case __COUNTER__: function break;
	switch (opcode)
	{
#include "opcodes.h"
	}
#undef INST

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
