
#include "gb.h"
#include "input.h"
#include "glwt.h"

//bgb.bircd.org/pandocs.htm#joypadinput

void GB_tickinput()
{
	switch (gb.joypadInput & 0x30)//Select Button Keys
	{
	case 0x10://Select Direction Keys
		gb.joypadInput = 0xEF;
		if (glwt_keydown[Key_DownArrow])
			gb.joypadInput &= ~(1 << 3);
		if (glwt_keydown[Key_UpArrow])
			gb.joypadInput &= ~(1 << 2);
		if (glwt_keydown[Key_LeftArrow])
			gb.joypadInput &= ~(1 << 1);
		if (glwt_keydown[Key_RightArrow])
			gb.joypadInput &= ~(1 << 0);
		break;
	case 0x20://Select Button Keys
		gb.joypadInput = 0xDF;
		if (glwt_keydown['s'])//Start Button
			gb.joypadInput &= ~(1 << 3);
		if (glwt_keydown['d'])//Select Button
			gb.joypadInput &= ~(1 << 2);
		if (glwt_keydown[Key_Backspace])//B Button
			gb.joypadInput &= ~(1 << 1);
		if (glwt_keydown[Key_Enter])//A Button
			gb.joypadInput &= ~(1 << 0);
		break;
	default:
		gb.joypadInput = 0xFF;
		break;
	}
}
