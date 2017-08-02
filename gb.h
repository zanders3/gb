#pragma once

#include <cstdlib>
using namespace std;

typedef unsigned int u32;
typedef unsigned short u16;
typedef unsigned char u8;
typedef short i16;
typedef int i32;
typedef char i8;
typedef unsigned long long u64;

#ifdef _WIN32
void logf(char const* const format, ...);
#else
#define logf printf
#endif

//http://bgb.bircd.org/pandocs.htm

struct GB
{
	//cpu state
	struct {
		union {
			struct {
				struct {
					union {
						struct {
							u8 top : 4;
							bool z : 1, n : 1, h : 1, c : 1;
						};
						u8 flags;
					};
				} f;
				u8 a;
			};
			u16 af;
		};
	};
	struct {
		union {
			struct {
				u8 c, b;
			};
			u16 bc;
		};
	};
	struct {
		union {
			struct {
				u8 e, d;
			};
			u16 de;
		};
	};
	struct {
		union {
			struct {
				u8 l, h;
			};
			u16 hl;
		};
	};
	u16 nn;
	u16 sp, pc;
	
	u8 memory[0x10000];

	//hardware i/o state
	struct Interrupt {
		union {
			u8 value;
			struct {
				bool vBlank : 1;
				bool lcdStat : 1;
				bool timer : 1;
				bool serial : 1;
				bool joypad : 1;
			};
		};
	};

	Interrupt interruptReg, interruptFlag;
	bool interruptsEnabled;

	//gpu state
	struct GPU {
		u8 lcdcStatus;
		u8 mode;
		u8 scanline;
		u8 scanlinecompare;
		u8 dmaRegister;
		u16 modeclock;
		u32 framebuffer[160 * 144];
	} gpu;

	//joypad input
	u8 joypadInput;

    bool stopped;
	
} extern gb;

const char* GB_disasm(u16 loc, u8& opcode, u16& nn, u8& opcodeLength);
void GB_load(u8* rom, u32 romLength);
bool GB_tick(i32& ticksElapsed);
