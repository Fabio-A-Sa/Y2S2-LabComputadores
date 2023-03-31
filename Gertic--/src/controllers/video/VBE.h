#ifndef __VBE_MACROS_H
#define __VBE_MACROS_H

#define VBE_768p_INDEXED 0x105
#define VBE_480p_DC 0x110
#define VBE_600p_DC 0x115
#define VBE_1024p_DC 0x11A
#define VBE_864p_DC 0x14C

#define VBE_CTRL_INFO 0x4F00
#define VBE_MODE_INFO 0x4F01
#define VBE_MODE_SET 0x4F02

#define VBE_LINEAR_FB BIT(14)

#define DIRECT_COLOR 0x06
#define INDEXED_COLOR 0x105

#endif /* __VBE_MACROS_H */
