#include "gb.h"

INST("NOP", 1, {}, 4)
INST("LD BC,0x%04X", 3, { gb.bc = gb.nn; }, 8)
INST("LD (BC),A", 1, { undefined(); }, 8)
INST("INC BC", 1, { inc16(gb.bc); }, 8)
INST("INC B", 1, { inc8(gb.b); }, 8)
INST("DEC B", 1, { dec8(gb.b); }, 8)
INST("LD B,%02X", 2, { gb.b = (u8)gb.nn; }, 8)
INST("RLC A", 1, { undefined(); }, 4)
INST("LD (0x%04X),SP", 3, { undefined(); }, 12)
INST("ADD HL,BC", 1, { undefined(); }, 8)
INST("LD A,(BC)", 1, { undefined(); }, 8)
INST("DEC BC", 1, { dec16(gb.bc); }, 8)
INST("INC C", 1, { inc8(gb.c); }, 4)
INST("DEC C", 1, { dec8(gb.c); }, 4)
INST("LD C,%02X", 2, { gb.c = (u8)gb.nn; }, 8)
INST("RRC A", 1, { undefined(); }, 8)
INST("STOP", 1, { undefined(); }, 0)
INST("LD DE,0x%04X", 3, { gb.de = gb.nn; }, 12)
INST("LD (DE),A", 1, { writeMemory(gb.de, gb.a); }, 8)
INST("INC DE", 1, { inc16(gb.de); }, 8)
INST("INC D", 1, { inc8(gb.d); }, 4)
INST("DEC D", 1, { dec8(gb.d); }, 4)
INST("LD D,0x%02X", 2, { gb.d = (u8)gb.nn; }, 8)
INST("RL A", 1, { undefined(); }, 8)
INST("JR 0x%02X", 2, { gb.pc += (i8)gb.nn; }, 12)
INST("ADD HL,DE", 1, { addhl16(gb.de); }, 8)
INST("LD A,(DE)", 1, { gb.a = readMemory(gb.de); }, 8)
INST("DEC DE", 1, { dec16(gb.de); }, 8)
INST("INC E", 1, { inc8(gb.e); }, 4)
INST("DEC E", 1, { dec8(gb.e); }, 4)
INST("LD E,%02X", 2, { undefined(); }, 8)
INST("RR A", 1, { undefined(); }, 8)
INST("JR NZ,%02X", 2, { if (!gb.flags.z) { gb.pc += (i8)gb.nn; } }, 12)
INST("LD HL,%04X", 3, { gb.hl = gb.nn; }, 12)
INST("LDI (HL),A", 1, { writeMemory(gb.hl, gb.a); inc16(gb.hl); }, 8)
INST("INC HL", 1, { inc16(gb.hl); }, 8)
INST("INC H", 1, { inc8(gb.h); }, 4)
INST("DEC H", 1, { dec8(gb.h); }, 4)
INST("LD H,0x%02X", 2, { undefined(); }, 8)
INST("DAA", 1, { undefined(); }, 4)
INST("JR Z,0x%02X", 2, { if (gb.flags.z) { gb.pc += (i8)gb.nn; } }, 12)
INST("ADD HL,HL", 1, { undefined(); }, 8)
INST("LDI A,(HL)", 1, { gb.a = readMemory(gb.hl); inc16(gb.hl); }, 8)
INST("DEC HL", 1, { dec16(gb.hl); }, 8)
INST("INC L", 1, { inc8(gb.l); }, 4)
INST("DEC L", 1, { dec8(gb.l); }, 4)
INST("LD L,0x%02X", 2, { undefined(); }, 8)
INST("CPL", 1, { cpl(); }, 4)
INST("JR NC,0x%02X", 2, { undefined(); }, 12)
INST("LD SP,0x%04X", 3, { gb.sp = gb.nn; }, 12)
INST("LDD (HL),A", 1, { writeMemory(gb.hl, gb.a); dec16(gb.hl); }, 8)
INST("INC SP", 1, { inc16(gb.sp); }, 8)
INST("INC (HL)", 1, { u8 val = readMemory(gb.hl); inc8(val); writeMemory(gb.hl, val); }, 12)
INST("DEC (HL)", 1, { u8 val = readMemory(gb.hl); dec8(val); writeMemory(gb.hl, val); }, 12)
INST("LD (HL),0x%02X", 2, { writeMemory(gb.hl, (u8)gb.nn); }, 16)
INST("SCF", 1, { undefined(); }, 4)
INST("JR C,0x%02X", 1, { undefined(); }, 12)
INST("ADD HL,SP", 1, { undefined(); }, 8)
INST("LDD A,(HL)", 1, { gb.a = readMemory(gb.hl); dec16(gb.hl); }, 8)
INST("DEC SP", 1, { dec16(gb.sp); }, 8)
INST("INC A", 1, { inc8(gb.a); }, 4)
INST("DEC A", 1, { dec8(gb.a); }, 4)
INST("LD A,%02X", 2, { gb.a = (u8)gb.nn; }, 8)
INST("CCF", 1, { undefined(); }, 4)
INST("LD B,B", 1, { undefined(); }, 4)
INST("LD B,C", 1, { gb.b = gb.c; }, 4)
INST("LD B,D", 1, { gb.b = gb.d; }, 4)
INST("LD B,E", 1, { gb.b = gb.e; }, 4)
INST("LD B,H", 1, { gb.b = gb.h; }, 4)
INST("LD B,L", 1, { gb.b = gb.l; }, 4)
INST("LD B,(HL)", 1, { undefined(); }, 8)
INST("LD B,A", 1, { gb.b = gb.a; }, 4)
INST("LD C,B", 1, { gb.c = gb.b; }, 4)
INST("LD C,C", 1, { undefined(); }, 4)
INST("LD C,D", 1, { gb.c = gb.d; }, 4)
INST("LD C,E", 1, { gb.c = gb.e; }, 4)
INST("LD C,H", 1, { gb.c = gb.h; }, 4)
INST("LD C,L", 1, { gb.c = gb.l; }, 4)
INST("LD C,(HL)", 1, { undefined(); }, 8)
INST("LD C,A", 1, { gb.c = gb.a;  }, 4)
INST("LD D,B", 1, { undefined(); }, 4)
INST("LD D,C", 1, { undefined(); }, 4)
INST("LD D,D", 1, { undefined(); }, 4)
INST("LD D,E", 1, { undefined(); }, 4)
INST("LD D,H", 1, { undefined(); }, 4)
INST("LD D,L", 1, { undefined(); }, 4)
INST("LD D,(HL)", 1, { gb.d = readMemory(gb.hl); }, 8)
INST("LD D,A", 1, { undefined(); }, 4)
INST("LD E,B", 1, { undefined(); }, 4)
INST("LD E,C", 1, { undefined(); }, 4)
INST("LD E,D", 1, { undefined(); }, 4)
INST("LD E,E", 1, { undefined(); }, 4)
INST("LD E,H", 1, { undefined(); }, 4)
INST("LD E,L", 1, { undefined(); }, 4)
INST("LD E,(HL)", 1, { gb.e = readMemory(gb.hl); }, 8)
INST("LD E,A", 1, { gb.e = gb.a; }, 4)
INST("LD H,B", 1, { undefined(); }, 4)
INST("LD H,C", 1, { undefined(); }, 4)
INST("LD H,D", 1, { undefined(); }, 4)
INST("LD H,E", 1, { undefined(); }, 4)
INST("LD H,H", 1, { undefined(); }, 4)
INST("LD H,L", 1, { undefined(); }, 4)
INST("LD H,(HL)", 1, { undefined(); }, 8)
INST("LD H,A", 1, { undefined(); }, 4)
INST("LD L,B", 1, { undefined(); }, 4)
INST("LD L,C", 1, { undefined(); }, 4)
INST("LD L,D", 1, { undefined(); }, 4)
INST("LD L,E", 1, { undefined(); }, 4)
INST("LD L,H", 1, { undefined(); }, 4)
INST("LD L,L", 1, { undefined(); }, 4)
INST("LD L,(HL)", 1, { undefined(); }, 8)
INST("LD L,A", 1, { undefined(); }, 4)
INST("LD (HL),B", 1, { undefined(); }, 8)
INST("LD (HL),C", 1, { undefined(); }, 8)
INST("LD (HL),D", 1, { undefined(); }, 8)
INST("LD (HL),E", 1, { undefined(); }, 8)
INST("LD (HL),H", 1, { undefined(); }, 8)
INST("LD (HL),L", 1, { undefined(); }, 8)
INST("HALT", 1, { undefined(); }, 4)
INST("LD (HL),A", 1, { undefined(); }, 8)
INST("LD A,B", 1, { gb.a = gb.b; }, 4)
INST("LD A,C", 1, { gb.a = gb.c; }, 4)
INST("LD A,D", 1, { gb.a = gb.d; }, 4)
INST("LD A,E", 1, { gb.a = gb.e; }, 4)
INST("LD A,H", 1, { gb.a = gb.h; }, 4)
INST("LD A,L", 1, { gb.a = gb.l; }, 4)
INST("LD A,(HL)", 1, { gb.a = readMemory(gb.hl); }, 8)
INST("LD A,A", 1, { undefined(); }, 4)
INST("ADD A,B", 1, { add8(gb.b); }, 4)
INST("ADD A,C", 1, { add8(gb.c); }, 4)
INST("ADD A,D", 1, { add8(gb.d); }, 4)
INST("ADD A,E", 1, { add8(gb.e); }, 4)
INST("ADD A,H", 1, { add8(gb.h); }, 4)
INST("ADD A,L", 1, { add8(gb.l); }, 4)
INST("ADD A,(HL)", 1, { undefined(); }, 8)
INST("ADD A,A", 1, { add8(gb.a); }, 4)
INST("ADC A,B", 1, { undefined(); }, 4)
INST("ADC A,C", 1, { undefined(); }, 4)
INST("ADC A,D", 1, { undefined(); }, 4)
INST("ADC A,E", 1, { undefined(); }, 4)
INST("ADC A,H", 1, { undefined(); }, 4)
INST("ADC A,L", 1, { undefined(); }, 4)
INST("ADC A,(HL)", 1, { undefined(); }, 8)
INST("ADC A,A", 1, { undefined(); }, 4)
INST("SUB A,B", 1, { undefined(); }, 4)
INST("SUB A,C", 1, { undefined(); }, 4)
INST("SUB A,D", 1, { undefined(); }, 4)
INST("SUB A,E", 1, { undefined(); }, 4)
INST("SUB A,H", 1, { undefined(); }, 4)
INST("SUB A,L", 1, { undefined(); }, 4)
INST("SUB A,(HL)", 1, { undefined(); }, 8)
INST("SUB A,A", 1, { undefined(); }, 4)
INST("SBC A,B", 1, { undefined(); }, 4)
INST("SBC A,C", 1, { undefined(); }, 4)
INST("SBC A,D", 1, { undefined(); }, 4)
INST("SBC A,E", 1, { undefined(); }, 4)
INST("SBC A,H", 1, { undefined(); }, 4)
INST("SBC A,L", 1, { undefined(); }, 4)
INST("SBC A,(HL)", 1, { undefined(); }, 8)
INST("SBC A,A", 1, { undefined(); }, 4)
INST("AND B", 1, { and8(gb.b); }, 4)
INST("AND C", 1, { and8(gb.c); }, 4)
INST("AND D", 1, { and8(gb.d); }, 4)
INST("AND E", 1, { and8(gb.e); }, 4)
INST("AND H", 1, { and8(gb.h); }, 4)
INST("AND L", 1, { and8(gb.l); }, 4)
INST("AND (HL)", 1, { undefined(); }, 8)
INST("AND A", 1, { and8(gb.a); }, 4)
INST("XOR B", 1, { xor8(gb.b); }, 4)
INST("XOR C", 1, { xor8(gb.c); }, 4)
INST("XOR D", 1, { xor8(gb.d); }, 4)
INST("XOR E", 1, { xor8(gb.e); }, 4)
INST("XOR H", 1, { xor8(gb.h); }, 4)
INST("XOR L", 1, { xor8(gb.l); }, 4)
INST("XOR (HL)", 1, { undefined(); }, 8)
INST("XOR A", 1, { xor8(gb.a); }, 4)
INST("OR B", 1, { or8(gb.b); }, 4)
INST("OR C", 1, { or8(gb.c); }, 4)
INST("OR D", 1, { or8(gb.d); }, 4)
INST("OR E", 1, { or8(gb.e); }, 4)
INST("OR H", 1, { or8(gb.h); }, 4)
INST("OR L", 1, { or8(gb.l); }, 4)
INST("OR (HL)", 1, { undefined(); }, 8)
INST("OR A", 1, { or8(gb.a); }, 4)
INST("CP B", 1, { undefined(); }, 4)
INST("CP C", 1, { undefined(); }, 4)
INST("CP D", 1, { undefined(); }, 4)
INST("CP E", 1, { undefined(); }, 4)
INST("CP H", 1, { undefined(); }, 4)
INST("CP L", 1, { undefined(); }, 4)
INST("CP (HL)", 1, { undefined(); }, 8)
INST("CP A", 1, { undefined(); }, 4)
INST("RET NZ", 1, { if (!gb.flags.z) { ret(); } }, 20)
INST("POP BC", 1, { pop16(gb.bc); }, 12)
INST("JP NZ,0x%04X", 3, { undefined(); }, 16)
INST("JP %04X", 3, { gb.pc = gb.nn; }, 16)
INST("CALL NZ,0x%04X", 3, { undefined(); }, 24)
INST("PUSH BC", 1, { push16(gb.bc); }, 16)
INST("ADD A,0x%02X", 2, { undefined(); }, 8)
INST("RST 0", 1, { undefined(); }, 16)
INST("RET Z", 1, { if (gb.flags.z) { ret(); } }, 20)
INST("RET", 1, { ret(); }, 16)
INST("JP Z,0x%04X", 3, { if (gb.flags.z) { gb.pc = gb.nn; } }, 16)
INST("EXT", 2, { extops(); }, 4)
INST("CALL Z,0x%04X", 3, { if (gb.flags.z) { call(gb.nn); } }, 24)
INST("CALL 0x%04X", 3, { call(gb.nn); }, 24)
INST("ADC A,0x%02X", 2, { undefined(); }, 8)
INST("RST 8", 1, { undefined(); }, 16)
INST("RET NC", 1, { undefined(); }, 20)
INST("POP DE", 1, { pop16(gb.de); }, 12)
INST("JP NC,0x%04X", 3, { undefined(); }, 16)
INST("XX", 1, { undefined(); }, 4)
INST("CALL NC,0x%04X", 3, { undefined(); }, 24)
INST("PUSH DE", 1, { push16(gb.de); }, 16)
INST("SUB A,0x%02X", 2, { undefined(); }, 8)
INST("RST 10", 1, { undefined(); }, 16)
INST("RET C", 1, { undefined(); }, 20)
INST("RETI", 1, { undefined(); }, 16)
INST("JP C,0x%04X", 3, { undefined(); }, 16)
INST("XX", 1, { undefined(); }, 4)
INST("CALL C,0x%04X", 3, { undefined(); }, 24)
INST("XX", 1, { undefined(); }, 4)
INST("SBC A,0x%02X", 2, { undefined(); }, 8)
INST("RST 18", 1, { undefined(); }, 16)
INST("LDH (0x%02X),A", 2, { writeMemory(0xFF00 + gb.nn, gb.a); }, 12)
INST("POP HL", 1, { pop16(gb.hl); }, 12)
INST("LDH (C),A", 1, { writeMemory(0xFF00 + gb.c, gb.a); }, 12)
INST("XX", 1, { undefined(); }, 4)
INST("XX", 1, { undefined(); }, 4)
INST("PUSH HL", 1, { push16(gb.hl); }, 16)
INST("AND 0x%02X", 2, { and8((u8)gb.nn); }, 8)
INST("RST 20", 1, { undefined(); }, 16)
INST("ADD SP,d", 1, { undefined(); }, 16)
INST("JP (HL)", 1, { gb.pc = gb.hl; }, 4)
INST("LD (0x%04X),A", 3, { writeMemory(gb.nn, gb.a); }, 12)
INST("XX", 1, { undefined(); }, 4)
INST("XX", 1, { undefined(); }, 4)
INST("XX", 1, { undefined(); }, 4)
INST("XOR 0x%02X", 2, { undefined(); }, 8)
INST("RST 28", 1, { rst(0x28); }, 16)
INST("LDH A,(0x%02X)", 2, { gb.a = readMemory(0xFF00 + gb.nn); }, 8)
INST("POP AF", 1, { pop16(gb.af); }, 12)
INST("XX", 1, { undefined(); }, 4)
INST("DI", 1, { gb.interruptsEnabled = false; }, 4)
INST("XX", 1, { undefined(); }, 4)
INST("PUSH AF", 1, { push16(gb.af); }, 16)
INST("OR 0x%02X", 2, { undefined(); }, 8)
INST("RST 30", 1, { undefined(); }, 16)
INST("LDHL SP,d", 1, { undefined(); }, 8)
INST("LD SP,HL", 1, { undefined(); }, 4)
INST("LD A,(0x%04X)", 3, { gb.a = readMemory(gb.nn); }, 12)
INST("EI", 1, { gb.interruptsEnabled = true; }, 4)
INST("XX", 1, { undefined(); }, 4)
INST("XX", 1, { undefined(); }, 4)
INST("CP 0x%02X", 2, { cp8((u8)gb.nn); }, 8)
INST("RST 38", 1, { rst(0x38); }, 16)
