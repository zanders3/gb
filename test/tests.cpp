
#include "gb.h"
#include <cassert>
#include <initializer_list>

void runtest(const std::initializer_list<u8>& opcodes, i32 desiredTicks)
{
    gb.pc = 0x0;
    gb.f.flags = 0x0;
    for (i32 i = 0; i < opcodes.size(); ++i)
        gb.memory[i] = opcodes.begin()[i];
    i32 ticks;
    GB_tick(ticks);
    assert(ticks == desiredTicks);
}

void assertEq(u8 value, u8 expected)
{
    if (value != expected)
    {
        __debugbreak();
    }
}

const u8 ZERO = 0x10;
const u8 ADD_SUBTRACT = 0x20;
const u8 HALF_CARRY = 0x40;
const u8 CARRY = 0x80;

void runtests0x()
{
    //00 NOP
    runtest({ 0x0 }, 4);

    //02 LD (BC), A
    gb.a = 0x42;
    gb.b = 0xC0;
    gb.c = 0x02;
    runtest({ 0x02, 0xED, 0x0 }, 8);
    assertEq(gb.memory[0xC002], 0x42);

    //04 INC B
    gb.b = 0x42;
    runtest({ 0x04 }, 4);
    assertEq(gb.b, 0x43);
    assertEq(gb.f.flags, 0x0);

    //04 INC B ZERO
    gb.b = 0xFF;
    runtest({ 0x04 }, 4);
    assertEq(gb.b, 0x0);
    assertEq(gb.f.flags, ZERO | HALF_CARRY);

    //04 INC B HALF CARRY
    gb.b = 0x0F;
    runtest({ 0x04 }, 4);
    assertEq(gb.b, 0x10);
    assertEq(gb.f.flags, HALF_CARRY);

    //05 DEC B
    gb.b = 0x42;
    runtest({ 0x05 }, 4);
    assertEq(gb.b, 0x41);
    assertEq(gb.f.flags, ADD_SUBTRACT);

    //05 DEC B ZERO
    gb.b = 0x01;
    runtest({ 0x05 }, 4);
    assertEq(gb.b, 0x0);
    assertEq(gb.f.flags, ZERO | ADD_SUBTRACT);

    //05 DEC B HALF CARRY
    gb.b = 0x0;
    runtest({ 0x05 }, 4);
    assertEq(gb.b, 0xFF);
    assertEq(gb.f.flags, ADD_SUBTRACT | HALF_CARRY);

    //06 LD B,n
    runtest({ 0x06, 0x42 }, 8);
    assertEq(gb.b, 0x42);

    //07 RLCA
    gb.a = 0x77;
    runtest({ 0x07 }, 4);
    assertEq(gb.a, 0xEE);
    assertEq(gb.f.flags, 0x0);

    //07 RLCA CARRY
    gb.a = 0xF7;
    runtest({ 0x07 }, 4);
    assertEq(gb.a, 0xEF);
    assertEq(gb.f.flags, CARRY);

    //0A LD A,(BC)
    gb.b = 0x0;
    gb.c = 0x2;
    runtest({ 0x0A, 0xED, 0x42 }, 8);
    assertEq(gb.a, 0x42);

    //0F RRCA
    gb.a = 0xEE;
    runtest({ 0x0F }, 4);
    assertEq(gb.a, 0x77);
    assertEq(gb.f.flags, 0x0);

    //0F RRCA CARRY
    gb.a = 0xEF;
    runtest({ 0x0F }, 4);
    assertEq(gb.a, 0xF7);
    assertEq(gb.f.flags, CARRY);
}

void runtests1x()
{
    //12 LD (DE),A
    gb.a = 0x42;
    gb.d = 0xC0;
    gb.e = 0x02;
    runtest({ 0x12, 0xED, 0x0 }, 8);
    assertEq(gb.memory[0xC002], 0x42);

    //16 LD D,n
    runtest({ 0x16, 0x42 }, 8);
    assertEq(gb.d, 0x42);

    //17 RLA
    gb.a = 0x55;
    runtest({ 0x17 }, 4);
    assertEq(gb.a, 0xAA);
    assertEq(gb.f.flags, 0x0);

    //17 RLA CARRY
    gb.a = 0xAA;
    gb.f.flags = CARRY;
    runtest({ 0x17 }, 4);
    assertEq(gb.a, 0x55);
    assertEq(gb.f.flags, CARRY);

    //1A LD A,(DE)
    gb.d = 0xC0;
    gb.e = 0x02;
    runtest({ 0x1A, 0xED, 0x42 }, 8);
    assertEq(gb.a, 0x42);

    //1F RRA
    gb.a = 0xAA;
    runtest({ 0x1F }, 4);
    assertEq(gb.a, 0x55);
    assertEq(gb.f.flags, 0x0);

    //1F RRA CARRY
    gb.a = 0x55;
    gb.f.flags = CARRY;
    runtest({ 0x1F }, 4);
    assertEq(gb.a, 0xAA);
    assertEq(gb.f.flags, CARRY);
}

void runtests()
{
    u8 rom = 0;
    GB_load(&rom, 0);

    runtests0x();
    //runtests1x();
}
