#pragma once

typedef unsigned int u32;
typedef unsigned short u16;
typedef unsigned char u8;

//http://bgb.bircd.org/pandocs.htm

struct GB
{
	//cpu state
	struct {
		union {
			struct {
				u8 f, a;
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
	struct Flags {
		union {
			struct {
				bool z : 1, n : 1, h : 1, c : 1;
			};
			u8 flags;
		};
	} flags;

	u8 memory[0x10000];

	//hardware i/o state
	u8 interruptReg;
	bool interruptsEnabled;

	//gpu state
	struct GPU {
		u8 lcdc_status;
		u8 mode;
		u8 scanline;
		u8 scanlinecompare;
		u16 modeclock;
		u32 framebuffer[160 * 144];
	} gpu;

} extern gb;

void GB_load(u8* rom, size_t romLength);
bool GB_tick();
