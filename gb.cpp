
//http://marc.rawer.de/Gameboy/Docs/GBCPUman.pdf
//http://gameboy.mongenel.com/dmg/opcodes.html
//http://imrannazar.com/Gameboy-Z80-Opcode-Map

#include "gb.h"
#include <memory>
#include "memory.h"
#include "display.h"

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
#endif

GB gb;

#define INST(disassembly, opCodeLength, function, opCodeTicks) disassembly,
const char* instructionDisassembly[256] =
{
#include "opcodes.h"
};
#undef INST

#define EXT_INST(code, disassembly, function) disassembly,
const char* extendedInstructionDisassembly[256] =
{
#include "extendedopcodes.h"
};
#undef EXT_INST

#define INST(disassembly, opCodeLength, function, opCodeTicks) opCodeLength,
const u8 instructionOpCodeLength[256] =
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
	gb.gpu.lcdcStatus = 0;
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

inline void cpl()
{
	gb.a = ~gb.a;
	gb.flags.n = true;
	gb.flags.h = true;
}

inline void call(u16& nn)
{
	writeMemory16(gb.sp, gb.pc);
	gb.pc = nn;
	gb.sp -= 2;
}

inline void ret()
{
	gb.sp += 2;
	gb.pc = readMemory16(gb.sp);
}

inline void rst(u8 n)
{
	writeMemory16(gb.sp, gb.pc);
	gb.pc = n;
	gb.sp -= 2;
}

inline void add8(u8 reg)
{
	gb.flags.h = gb.a > 0 && (reg & 0xF) > (0xF - gb.a);
	gb.flags.c = gb.a > 0 && reg > (0xFF - gb.a);
	gb.a += (u8)reg;
	gb.flags.z = gb.a == 0;
	gb.flags.n = false;
}

inline void addhl16(u16 reg)
{
	gb.flags.h = gb.hl > 0 && (reg & 0xFFF) > (0xFFF - gb.hl);
	gb.flags.c = gb.hl > 0 && reg > (0xFFFF - gb.hl);
	gb.hl += reg;
	gb.flags.n = false;
}

inline void inc16(u16& reg)
{
	gb.flags.h = reg == ((u16)-1);
	reg += 1;
	gb.flags.z = reg == 0;
	gb.flags.n = false;
}

inline void inc8(u8& reg)
{
	gb.flags.h = reg == ((u8)-1);
	reg += 1;
	gb.flags.z = reg == 0;
	gb.flags.n = false;
}

inline void dec16(u16& reg)
{
	gb.flags.h = reg == 0;
	reg -= 1;
	gb.flags.z = reg == 0;
	gb.flags.n = true;
}

inline void dec8(u8& reg)
{
	gb.flags.h = reg == 0;
	reg -= 1;
	gb.flags.z = reg == 0;
	gb.flags.n = true;
}

inline void or8(u8 reg)
{
	gb.a = gb.a | reg;
	gb.flags.flags = 0;
	gb.flags.z = gb.a == 0;
}

inline void cp8(u8 val)
{
	gb.flags.z = gb.a == val;
	gb.flags.n = true;
	gb.flags.c = gb.a < val;
}

inline void xor8(u8 reg)
{
	gb.a = gb.a ^ reg;
	gb.flags.flags = 0;
	gb.flags.z = gb.a == 0;
}

inline void and8(u8 reg)
{
	gb.a = gb.a & reg;
	gb.flags.z = gb.a == 0;
	gb.flags.n = false;
	gb.flags.h = true;
	gb.flags.c = false;
}

inline void swap8(u8& reg)
{
	u8 tmp = reg;
	reg = (tmp >> 4) | ((tmp & 0xF) << 4);
	gb.flags.flags = 0;
	gb.flags.z = reg == 0;
}

inline void pop16(u16& reg)
{
	gb.sp += 2;
	reg = readMemory16(gb.sp);
}

inline void push16(u16& reg)
{
	writeMemory16(gb.sp, reg);
	gb.sp -= 2;
}

inline void res8(u32 index, u8& reg)
{
	reg = reg & ~(1 << index);
}

inline void sla8(u8& reg)
{
	gb.flags.flags = 0;
	gb.flags.c = (reg & 8) > 0;
	reg = reg << 1;
	gb.flags.z = reg == 0;
}

inline void bit8(u8 bit, u8 val)
{
	gb.flags.z = ((1 << bit) & val) > 0;
	gb.flags.n = false;
	gb.flags.h = true;
}

inline void extops()
{
	u8 extopcode = gb.memory[gb.pc-1];
#define EXT_INST(code, disassembly, function) case code: function break;
	switch (extopcode)
	{
#include "extendedopcodes.h"
	}
#undef INST
}

bool g_disassemble = false;
u16 g_breakpoint = 0xFFFF;//0x29C;
//0x29C is hit, 
//2. 0x31A is where code is written from - this is currently not hit :(
//3. jumped from first vblank interrupt

void GB_handleinterrupts()
{
	if (gb.interruptFlag.vBlank && gb.interruptReg.vBlank)
	{
		gb.interruptFlag.vBlank = false;
		gb.interruptsEnabled = false;
		u16 loc = 0x40;
		call(loc);
	}
	//else if remaining interrupts!!! (v. important)
}

bool GB_tick()
{
	if (gb.pc == g_breakpoint)
	{
		g_disassemble = true;
	}

	//decode instruction
	const u8 opcode = gb.memory[gb.pc];
	u32 opcodeLength = instructionOpCodeLength[opcode];
	if (opcodeLength == 2)
		gb.nn = gb.memory[gb.pc + 1];
	else if (opcodeLength == 3)
		gb.nn = gb.memory[gb.pc + 1] | (gb.memory[gb.pc + 2] << 8);

	if (g_disassemble)
	{
		const char* disassembly = instructionDisassembly[opcode];
		if (opcode == 0xCB)//extended opcodes
		{
			u8 extOpcode = gb.memory[gb.pc + 1];
			disassembly = extendedInstructionDisassembly[extOpcode];
		}
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

	bool result = GB_gputick(opcode);

	if (gb.interruptsEnabled)
		GB_handleinterrupts();

	return result;
}
