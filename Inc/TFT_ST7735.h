/***************************************************
  Arduino TFT graphics library targetted at the UNO
  and Mega boards.

  This library has been derived from the Adafruit_GFX
  library and the associated driver library. See text
  at the end of this file.

  This is a standalone library that contains the
  hardware driver, the graphics funtions and the
  proportional fonts.

  The larger fonts are Run Length Encoded to reduce
  their FLASH footprint.

 ****************************************************/

#define INITR_GREENTAB	0x0
#define INITR_REDTAB	0x1
#define INITR_BLACKTAB	0x2
#define INITR_GREENTAB2	0x3 // Use if you get random pixels on two edges of green tab display
#define INITB			0xB

// Stop fonts being loaded multiple times
#ifndef _TFT_ST7735H_
#define _TFT_ST7735H_

//These enumerate the text plotting alignment (reference datum point)
#define TL_DATUM 0 // Top left (default)
#define TC_DATUM 1 // Top centre
#define TR_DATUM 2 // Top right
#define ML_DATUM 3 // Middle left
#define CL_DATUM 3 // Centre left, same as above
#define MC_DATUM 4 // Middle centre
#define CC_DATUM 4 // Centre centre, same as above
#define MR_DATUM 5 // Middle right
#define CR_DATUM 5 // Centre right, same as above
#define BL_DATUM 6 // Bottom left
#define BC_DATUM 7 // Bottom centre
#define BR_DATUM 8 // Bottom right

// Change the width and height if required (defined in portrait mode)
// or use the constructor to over-ride defaults
#define ST7735_TFTWIDTH  128
#define ST7735_TFTHEIGHT 160

// These are the ST7735 control registers
// some flags for initR() :(

#define ST7735_TFTWIDTH  128
#define ST7735_TFTHEIGHT 160

#define ST7735_NOP     0x00
#define ST7735_SWRESET 0x01
#define ST7735_RDDID   0x04
#define ST7735_RDDST   0x09

#define ST7735_SLPIN   0x10
#define ST7735_SLPOUT  0x11
#define ST7735_PTLON   0x12
#define ST7735_NORON   0x13

#define ST7735_INVOFF  0x20
#define ST7735_INVON   0x21
#define ST7735_DISPOFF 0x28
#define ST7735_DISPON  0x29
#define ST7735_CASET   0x2A
#define ST7735_RASET   0x2B
#define ST7735_RAMWR   0x2C
#define ST7735_RAMRD   0x2E

#define ST7735_PTLAR   0x30
#define ST7735_COLMOD  0x3A
#define ST7735_MADCTL  0x36

#define ST7735_FRMCTR1 0xB1
#define ST7735_FRMCTR2 0xB2
#define ST7735_FRMCTR3 0xB3
#define ST7735_INVCTR  0xB4
#define ST7735_DISSET5 0xB6

#define ST7735_PWCTR1  0xC0
#define ST7735_PWCTR2  0xC1
#define ST7735_PWCTR3  0xC2
#define ST7735_PWCTR4  0xC3
#define ST7735_PWCTR5  0xC4
#define ST7735_VMCTR1  0xC5

#define ST7735_RDID1   0xDA
#define ST7735_RDID2   0xDB
#define ST7735_RDID3   0xDC
#define ST7735_RDID4   0xDD

#define ST7735_PWCTR6  0xFC

#define ST7735_GMCTRP1 0xE0
#define ST7735_GMCTRN1 0xE1


#define MADCTL_MY  0x80
#define MADCTL_MX  0x40
#define MADCTL_MV  0x20
#define MADCTL_ML  0x10
#define MADCTL_RGB 0x00
#define MADCTL_BGR 0x08
#define MADCTL_MH  0x04

// Color definitions for backwards compatibility
#define CL_BLACK       0x0000      /*   0,   0,   0 */
#define CL_NAVY        0x000F      /*   0,   0, 128 */
#define CL_DARKGREEN   0x03E0      /*   0, 128,   0 */
#define CL_DARKCYAN    0x03EF      /*   0, 128, 128 */
#define CL_MAROON      0x7800      /* 128,   0,   0 */
#define CL_PURPLE      0x780F      /* 128,   0, 128 */
#define CL_OLIVE       0x7BE0      /* 128, 128,   0 */
#define CL_LIGHTGREY   0xC618      /* 192, 192, 192 */
#define CL_DARKGREY    0x7BEF      /* 128, 128, 128 */
#define CL_BLUE        0x001F      /*   0,   0, 255 */
#define CL_GREEN       0x07E0      /*   0, 255,   0 */
#define CL_CYAN        0x07FF      /*   0, 255, 255 */
#define CL_RED         0xF800      /* 255,   0,   0 */
#define CL_MAGENTA     0xF81F      /* 255,   0, 255 */
#define CL_YELLOW      0xFFE0      /* 255, 255,   0 */
#define CL_WHITE       0xFFFF      /* 255, 255, 255 */
#define CL_ORANGE      0xFD20      /* 255, 165,   0 */
#define CL_GREENYELLOW 0xAFE5      /* 173, 255,  47 */

#endif

/***************************************************

  ORIGINAL LIBRARY HEADER

  This is our library for the Adafruit  ST7735 Breakout and Shield
  ----> http://www.adafruit.com/products/1651

  Check out the links above for our tutorials and wiring diagrams
  These displays use SPI to communicate, 4 or 5 pins are required to
  interface (RST is optional)
  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  MIT license, all text above must be included in any redistribution

  Updated with new functions by Bodmer 14/4/15
 ****************************************************/
