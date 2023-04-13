#ifndef __VBE_MACROS_H
#define __VBE_MACROS_H

#define VBE_768p_INDEXED 0x105
#define VBE_480p_DC 0x110
#define VBE_600p_DC 0x115
#define VBE_1024p_DC 0x11A
#define VBE_864p_DC 0x14C

#define VBE_MODE_SET 0x4F02
#define VBE_LINEAR_FB BIT(14)

#define DIRECT_COLOR 0x06
#define INDEXED_COLOR 0x105
#define BIOS_SET_VIDEOMODE 0x00
#define BIOS_VIDEOCARD_SERV 0x10

// Colors

#define RED 0xff0000
#define ORANGE 0xd17b4f
#define BLUE 0x19DBF0
#define GREEN 0x2FF019
#define YELLOW 0xF5DA2A
#define DARKBLUE 0x0000ff
#define PRESSED 0x333FFF
#define TRANSPARENT 0xFFFFFE

#endif
