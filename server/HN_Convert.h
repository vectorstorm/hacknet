#ifndef HN_CONVERT_H
#define HN_CONVERT_H

/* 
 * This header file contains Nethack -> HackNet conversions that
 * are not specific to a particular database type.  Currently,
 * this only means colors.
 */

/*  Colors */

#define HI_OBJ			CLR_MAGENTA
#define HI_METAL		CLR_CYAN
#define HI_COPPER		CLR_YELLOW
#define HI_SILVER		CLR_SILVER
#define HI_GOLD			CLR_YELLOW
#define HI_LEATHER		CLR_BROWN
#define HI_CLOTH		CLR_BROWN
#define HI_ORGANIC		CLR_BROWN
#define HI_WOOD			CLR_BROWN
#define HI_PAPER		CLR_WHITE
#define HI_GLASS		CLR_BRIGHT_CYAN
#define HI_MINERAL		CLR_GRAY
#define HI_ZAP			CLR_BRIGHT_BLUE

#define CLR_BLACK       	COLOR_Black
#define CLR_WHITE       	COLOR_White
#define CLR_BROWN       	COLOR_Brown
#define CLR_GREEN		COLOR_Green
#define CLR_RED			COLOR_Red
#define CLR_BLUE		COLOR_Blue
#define CLR_MAGENTA		COLOR_Magenta
#define CLR_BRIGHT_MAGENTA	COLOR_Magenta
#define CLR_GRAY		COLOR_Gray
#define CLR_YELLOW		COLOR_Yellow
#define CLR_ORANGE		COLOR_Orange
#define CLR_CYAN		COLOR_Cyan
#define CLR_BRIGHT_GREEN	COLOR_BrightGreen
#define CLR_BRIGHT_CYAN		COLOR_BrightCyan
#define CLR_BRIGHT_BLUE		COLOR_BrightBlue

#endif // HN_CONVERT_H
