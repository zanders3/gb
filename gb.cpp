
//http://marc.rawer.de/Gameboy/Docs/GBCPUman.pdf
//http://gameboy.mongenel.com/dmg/opcodes.html
//http://imrannazar.com/Gameboy-Z80-Opcode-Map

#include "gb.h"
#include <memory>
#include "memory.h"
#include "display.h"
#include "input.h"
#include <cassert>
#include "glwt.h"

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

//http://marc.rawer.de/Gameboy/Docs/GBCPUman.pdf
//http://www.devrs.com/gb/files/opcodes.html
//http://bgb.bircd.org/pandocs.htm
void GB_load(u8* rom, u32 romLength)
{
	memset(gb.memory, 0, sizeof(gb.memory));
	memcpy(gb.memory, rom, min(0x7FFF, romLength));
    initMemoryBanks(rom, romLength);

	gb.af = 0x1B0;
	gb.bc = 0x13;
	gb.de = 0xD8;
	gb.hl = 0x14D;
	gb.sp = 0xFFFE;
	gb.pc = 0x100;
	gb.interruptsEnabled = false;
    gb.stopped = false;

	memset(gb.gpu.framebuffer, 0, sizeof(gb.gpu.framebuffer));
	gb.gpu.lcdcStatus = 0;
	gb.gpu.mode = 2;
	gb.gpu.modeclock = 0;
	gb.gpu.scanline = 0;
	gb.gpu.scanlinecompare = 0;
	gb.joypadInput = 0xFF;

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
	logf("\t\taf = %04X bc = %04X de = %04X hl = %04X sp = %04X pc = %04X z = %d n = %d h = %d c = %d\n", gb.af, gb.bc, gb.de, gb.hl, gb.sp, gb.pc, gb.f.z, gb.f.n, gb.f.h, gb.f.c);
	logf("Unimplemented!\n");
    assert(false);
}

inline void xx()
{
	undefined();
}

inline void cpl()
{
	gb.a = ~gb.a;
	gb.f.n = true;
	gb.f.h = true;
}

inline void call(u16& nn)
{
	writeMemory16(gb.sp, gb.pc);
	gb.pc = nn;
	gb.sp -= 2;
}

inline void halt()
{
	undefined();
}

inline void stop()
{
    gb.stopped = true;
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

inline void scf()
{
	undefined();
}

inline void ccf()
{
	undefined();
}

inline void add8(u8 reg)
{
	gb.f.h = gb.a > 0 && (reg & 0xF) > (0xF - gb.a);
	gb.f.c = gb.a > 0 && reg > (0xFF - gb.a);
	gb.a += (u8)reg;
	gb.f.z = gb.a == 0;
	gb.f.n = false;
}

inline void addhl16(u16 reg)
{
	gb.f.h = gb.hl > 0 && (reg & 0xFFF) > (0xFFF - gb.hl);
	gb.f.c = gb.hl > 0 && reg > (0xFFFF - gb.hl);
	gb.hl += reg;
	gb.f.n = false;
}

inline void adc8(u8 reg)
{
	gb.f.h = gb.a > 0 && ((reg + gb.f.c) & 0xF) > (0xF - gb.a);
	gb.f.c = gb.a > 0 && (reg + gb.f.c) > (0xFF - gb.a);
	gb.a += (u8)reg;
	gb.a += (u8)gb.f.c;
	gb.f.z = gb.a == 0;
	gb.f.n = false;
}

inline void sub8(u8 reg)
{
	gb.a -= (u8)reg;
	gb.f.h = gb.a > 0 && (reg & 0xF) > (0xF - gb.a);
	gb.f.c = gb.a > 0 && reg > (0xFF - gb.a);
	gb.f.z = gb.a == 0;
	gb.f.n = true;
}

inline void sbc8(u8 reg)
{
	undefined();
}

inline void inc16(u16& reg)
{
	gb.f.h = reg == ((u16)-1);
	reg += 1;
	gb.f.z = reg == 0;
	gb.f.n = false;
}

inline void inc8(u8& reg)
{
	gb.f.h = reg == ((u8)-1);
	reg += 1;
	gb.f.z = reg == 0;
	gb.f.n = false;
}

inline void dec16(u16& reg)
{
	gb.f.h = reg == 0;
	reg -= 1;
	gb.f.z = reg == 0;
	gb.f.n = true;
}

inline void dec8(u8& reg)
{
	gb.f.h = reg == 0;
	reg -= 1;
	gb.f.z = reg == 0;
	gb.f.n = true;
}

inline void or8(u8 reg)
{
	gb.a = gb.a | reg;
	gb.f.flags = 0;
	gb.f.z = gb.a == 0;
}

inline void cp8(u8 val)
{
	gb.f.z = gb.a == val;
	gb.f.n = true;
	gb.f.c = gb.a < val;
}

inline void xor8(u8 reg)
{
	gb.a = gb.a ^ reg;
	gb.f.flags = 0;
	gb.f.z = gb.a == 0;
}

inline void and8(u8 reg)
{
	gb.a = gb.a & reg;
	gb.f.z = gb.a == 0;
	gb.f.n = false;
	gb.f.h = true;
	gb.f.c = false;
}

inline void swap8(u8& reg)
{
	u8 tmp = reg;
	reg = (tmp >> 4) | ((tmp & 0xF) << 4);
	gb.f.flags = 0;
	gb.f.z = reg == 0;
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

inline void res8(const u8 bit, u8& reg)
{
	reg = reg & ~(1 << bit);
}

inline void res8hl(const u8 bit)
{
	u8 val = readMemory(gb.hl);
	res8(bit, val);
	writeMemory(gb.hl, val);
}

inline void set8(const u8 bit, u8& reg)
{
	reg = reg | (1 << bit);
}

inline void set8hl(const u8 bit)
{
	u8 val = readMemory(gb.hl);
	set8(bit, val);
	writeMemory(gb.hl, val);
}

inline void bit8(const u8 bit, u8 val)
{
	gb.f.z = ((1 << bit) & val) == 0;
	gb.f.n = false;
	gb.f.h = true;
}

inline void sla8(u8& reg)
{
	gb.f.flags = 0;
	gb.f.c = (reg & 8) > 0;
	reg = reg << 1;
	gb.f.z = reg == 0;
}

inline void sra8(u8& reg)
{
	undefined();
}

inline void srl8(u8& reg)
{
	gb.f.n = false;
	gb.f.h = false;
	gb.f.c = reg & 0x1;
	reg = reg >> 1;
}

inline void rr8(u8& reg)//rotate r right
{
	gb.f.c = reg & 0x1;
	reg = reg >> 1;
	gb.f.z = reg == 0;
	gb.f.n = false;
	gb.f.h = false;
}

inline void rrc8(u8& reg)//rotate right carry
{
	undefined();
}

inline void rl8(u8& reg)//rotate r left
{
	undefined();
}

inline void rlc8(u8& reg)//rotate left carry
{
	gb.f.c = (reg & 0x80) > 0;
    reg = (reg << 1);
	gb.f.z = reg == 0;
	gb.f.n = false;
	gb.f.h = false;
}

inline void daa()
{
	u32 a = gb.a;
	if (!gb.f.n)
	{
		if (gb.f.h || (gb.a & 0xF) > 0x9)
			a = gb.a + 0x06;
		if (gb.f.c || gb.a > 0x9F)
			a = gb.a + 0x60;
	}
	else
	{
		if (gb.f.h)
			a = (gb.a - 6) & 0xFF;
		if (gb.f.c)
			a -= 0x60;
	}
	
	gb.f.c = (a & 0x100) == 0x100;
	gb.a = a & 0xFF;
	gb.f.z = gb.a == 0;
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

const char* GB_disasm(u16 loc, u8& opcode, u16& nn, u8& opcodeLength)
{
    opcode = gb.memory[loc];
    opcodeLength = instructionOpCodeLength[opcode];
    nn = 0;
    if (opcodeLength == 2)
        nn = gb.memory[loc + 1];
    else if (opcodeLength == 3)
        nn = gb.memory[loc + 1] | (gb.memory[loc + 2] << 8);

    if (opcode == 0xCB)//extended opcodes
    {
        opcode = gb.memory[loc + 1];
        return extendedInstructionDisassembly[opcode];
    }
    else
        return instructionDisassembly[opcode];
}

bool GB_tick(i32& ticksElapsed)
{
    //decode instruction
    const u8 opcode = gb.memory[gb.pc];

    if (gb.stopped)
    {
        gb.stopped = !(glwt_keydown['S'] || glwt_keydown['D'] || glwt_keydown['X'] || glwt_keydown['Z']);
    }
    else
    {
        u32 opcodeLength = instructionOpCodeLength[opcode];
        if (opcodeLength == 2)
            gb.nn = gb.memory[gb.pc + 1];
        else if (opcodeLength == 3)
            gb.nn = gb.memory[gb.pc + 1] | (gb.memory[gb.pc + 2] << 8);

        //run instruction
        gb.pc += opcodeLength;
#define INST(disassembly, opCodeLength, function, opCodeTicks) case __COUNTER__: function break;
        switch (opcode)
        {
#include "opcodes.h"
        }
#undef INST
        gb.f.top = 0;//the top 4 bits of the flags register is always 0
    }

    bool result = GB_gputick(opcode, ticksElapsed);

    if (gb.interruptsEnabled)
        GB_handleinterrupts();

	GB_tickinput();

	return result;
}
