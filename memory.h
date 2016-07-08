#pragma once

#include "gb.h"

u16 readMemory16(u16 loc);
u8 readMemory(u16 loc);
void writeMemory16(u16 loc, u16 val);
void writeMemory(u16 loc, u8 val);
