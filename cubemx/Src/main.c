
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * Copyright (c) 2020 STMicroelectronics International N.V. 
  * All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f7xx_hal.h"
#include "fatfs.h"

/* USER CODE BEGIN Includes */
#include <math.h>
#include <string.h>
#include "codec2.h"
#include "TFT_ST7735.h"

//macros
#define MOIP_UDP_SIZE			54

#define RX_MODE					0
#define TX_MODE					1

#define LNA_ON					0
#define LNA_OFF					1

#define AUDIO_MUX_NONE			0b11		//both switches off
#define AUDIO_MUX_MOD			0b10		//audio DAC -> analog FM modulator
#define AUDIO_MUX_SPK			0b01		//audio DAC -> speaker

#define FONT_MONOSPACED_16_9	1

#define P_TYPE_VOICE			(0b10<<1)
#define P_TYPE_DATA				(0b01<<1)

const uint16_t crc_poly=0x5935;
/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;
ADC_HandleTypeDef hadc2;
ADC_HandleTypeDef hadc3;
DMA_HandleTypeDef hdma_adc1;
DMA_HandleTypeDef hdma_adc2;
DMA_HandleTypeDef hdma_adc3;

CRC_HandleTypeDef hcrc;

CRYP_HandleTypeDef hcryp;
__ALIGN_BEGIN static const uint8_t pKeyCRYP[32] __ALIGN_END = {
                            0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                            0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                            0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                            0x00,0x00};
__ALIGN_BEGIN static const uint8_t pInitVectCRYP[16] __ALIGN_END = {
                            0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                            0x00,0x00,0x00,0x00,0x00,0x00};

DAC_HandleTypeDef hdac;
DMA_HandleTypeDef hdma_dac1;

HASH_HandleTypeDef hhash;

I2C_HandleTypeDef hi2c1;

RNG_HandleTypeDef hrng;

SD_HandleTypeDef hsd1;

SPI_HandleTypeDef hspi1;
SPI_HandleTypeDef hspi2;

TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim4;
TIM_HandleTypeDef htim5;
TIM_HandleTypeDef htim6;
TIM_HandleTypeDef htim7;
TIM_HandleTypeDef htim8;

UART_HandleTypeDef huart7;
UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;
UART_HandleTypeDef huart3;
DMA_HandleTypeDef hdma_usart2_tx;

PCD_HandleTypeDef hpcd_USB_OTG_FS;

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/
//ESP commands and rcv buffer
volatile uint8_t esp_cmd[100];
volatile uint8_t esp_rcv[100];
volatile uint8_t esp_cnt=0;

//FatFS file
FIL	myFile;

//Codec2
volatile struct CODEC2 *cod;
uint8_t c2_data[16];			//2*8 bytes (2*20ms - each frame is 40ms)

//ADF7021
uint16_t chip_rev=0;
float rssi=0.0;

//TFT
uint8_t addr_col=0;							//current memory position
uint8_t addr_row=0;

//FONTS
struct font_1_glyph_dsc
{
	uint8_t w_px;			//width, px
	uint16_t glyph_index;
};

#include "font_1.h"

//M17 over IP
const uint32_t M17_STREAM_PREFIX = 0x4D313720;	//this is equal to "M17 "
volatile uint8_t data_rdy=0;					//data ready to send?

struct moip_packet
{
	uint8_t dst[10];
	uint8_t src[10];
	uint16_t type;
	uint8_t nonce[14];
	uint16_t fn;
	uint8_t payload[16];
	uint16_t crc_udp;
} packet;

uint8_t udp_frame[MOIP_UDP_SIZE];

//audio
#define DC_OFFSET 2040
volatile uint8_t mic_gain=10;		//microphone gain (linear) 0 -> mute
uint16_t fm_demod_in[2*320];
uint16_t audio_samples[2*320];
int16_t pr_audio_samples[2*320];
volatile uint8_t dac_play=1;		//is the DAC playing samples?
volatile uint8_t collect_samples=0;	//collect ADC data?
volatile uint8_t buff_num=0;		//which buffer is in use?
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_ADC1_Init(void);
static void MX_DAC_Init(void);
static void MX_SPI2_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_USART3_UART_Init(void);
static void MX_I2C1_Init(void);
static void MX_SDMMC1_SD_Init(void);
static void MX_CRYP_Init(void);
static void MX_HASH_Init(void);
static void MX_RNG_Init(void);
static void MX_CRC_Init(void);
static void MX_ADC2_Init(void);
static void MX_TIM4_Init(void);
static void MX_TIM6_Init(void);
static void MX_TIM8_Init(void);
static void MX_TIM7_Init(void);
static void MX_TIM5_Init(void);
static void MX_ADC3_Init(void);
static void MX_SPI1_Init(void);
static void MX_TIM3_Init(void);
static void MX_UART7_Init(void);
static void MX_USB_OTG_FS_PCD_Init(void);

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);
                                

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */
//----------------------------------Misc Stuff---------------------------------
void Vibrator(uint8_t state)
{
	//1: on, 0:off
	HAL_GPIO_WritePin(VIBRATE_GPIO_Port, VIBRATE_Pin, state);
}

void ypcmem(uint8_t *dst, uint8_t *src, uint16_t nBytes)
{
	for(uint16_t i=0; i<nBytes; i++)
		dst[i]=src[nBytes-i-1];
}

//uncomment these 2 funcs below if you are not using hardware CRC calculation unit
uint16_t CRC_LUT[256];
void CRC_Init(uint16_t *crc_table, uint16_t poly)
{
	uint16_t remainder;

	for(uint16_t dividend=0; dividend<256; dividend++)
	{
		remainder=dividend<<8;

		for(uint8_t bit=8; bit>0; bit--)
		{
			if(remainder&(1<<15))
				remainder=(remainder<<1)^poly;
			else
				remainder=(remainder<<1);
		}

		crc_table[dividend]=remainder;
	}
}

uint16_t CRC_M17(uint16_t* crc_table, const uint8_t* message, uint16_t nBytes)
{
	uint8_t data;
	uint16_t remainder=0xFFFF;

	for(uint16_t byte=0; byte<nBytes; byte++)
	{
		data=message[byte]^(remainder>>8);
		remainder=crc_table[data]^(remainder<<8);
	}

	return(remainder);
}

//-------------------------------------M17-------------------------------------
uint64_t Encode_Callsign(const char *callsign)
{
	uint64_t encoded=0;

	for(const char *p = (callsign + strlen(callsign) - 1); p >= callsign; p--)
	{
		encoded *= 40;
		// If speed is more important than code space, you can replace this with a lookup into a 256 byte array.
		if (*p >= 'A' && *p <= 'Z')  // 1-26
			encoded += *p - 'A' + 1;
		else if (*p >= '0' && *p <= '9')  // 27-36
			encoded += *p - '0' + 27;
		else if (*p == '-')  // 37
			encoded += 37;
		else if (*p == '/')  // 38
			encoded += 38;
		else if (*p == '.')  // 39
			encoded += 39;
		else
			;
	}

	return encoded;
}

//Prepare an "M17 over IP" packet to be send
//arg1: struct with packet data
//arg2: output byte array
//arg3: transmission end flag
void M17_Framer(struct moip_packet *inp, uint8_t *out, uint8_t tr_end)
{
	//FIELD		BYTES
	//prefix	4
	//SID		2
	//dst		10
	//src		10
	//type		2
	//nonce		14
	//fn		2
	//payload	16
	//crc_udp	2

	static uint16_t sid=0xDEAD;
	uint8_t src_enc[6];
	uint8_t dst_enc[6];
	uint16_t crc=0;

	for(uint8_t i=0; i<6; i++)
	{
		src_enc[i]=Encode_Callsign(inp->src)>>(i*8);
		dst_enc[i]=Encode_Callsign(inp->dst)>>(i*8);
	}

	if(tr_end)
		inp->fn|=(1<<15);

	ypcmem(&out[0], (uint8_t*)&M17_STREAM_PREFIX, 4);
	ypcmem(&out[4], (uint8_t*)&sid, 2);
	ypcmem(&out[6], dst_enc, 6);
	ypcmem(&out[12], src_enc, 6);
	ypcmem(&out[18], &(inp->type), 2);
	ypcmem(&out[20], &(inp->nonce), 14);
	ypcmem(&out[34], &(inp->fn), 2);
	ypcmem(&out[36], &(inp->payload), 16);
	crc=HAL_CRC_Calculate(&hcrc, out, 52);//CRC_M17(CRC_LUT, out, 52); //FIXME: use HW CRC unit
	ypcmem(&out[52], (uint8_t*)&crc, 2);
	ypcmem((uint8_t*)&(inp->crc_udp), (uint8_t*)&crc, 2);

	//rotate SID
	if(sid&1)
	{
		sid>>=1;
		sid|=(1<<15);
	}
	else
		sid>>=1;
}

//--------------------------------------RF-------------------------------------
void RF_SetPWR(uint16_t val)
{
	//set DAC_OUT2
	//TODO: fix this
	HAL_DAC_Start(&hdac, DAC_CHANNEL_2);
	HAL_DAC_SetValue(&hdac, DAC_CHANNEL_2, DAC_ALIGN_12B_R, val);
}

void LNA_Ctrl(uint8_t lna_state)
{
	//0: on, 1: off
	HAL_GPIO_WritePin(LNA_EN_GPIO_Port, LNA_EN_Pin, lna_state);
}

void RF_Mode(uint8_t mode)
{
	//set TX or RX via GaAs switch
	//RX: mode=0
	//TX: mode=1
	HAL_GPIO_WritePin(TRX_SW_GPIO_Port, TRX_SW_Pin, mode);
}

//------------------------------------AUDIO------------------------------------
void AUDIO_Mux(uint8_t mux)
{
	//set SPK amp source
	HAL_GPIO_WritePin(SPK_AMP_SEL_GPIO_Port, SPK_AMP_SEL_Pin, mux>>1);
	HAL_GPIO_WritePin(FM_MOD_SEL_GPIO_Port, FM_MOD_SEL_Pin, mux&1);
}

//-------------------------------------TFT-------------------------------------
void TFT_SetBrght(uint8_t brght)
{
	HAL_TIM_Base_Start(&htim3);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);

	if(brght<255)
		htim3.Instance->CCR2=(uint16_t)(brght*999.0/255.0);
	else
		htim3.Instance->CCR2=(uint16_t)999;
}

void TFT_Reset(void)
{
	HAL_GPIO_WritePin(TFT_RST_GPIO_Port, TFT_RST_Pin, 1);
	HAL_Delay(5);
	HAL_GPIO_WritePin(TFT_RST_GPIO_Port, TFT_RST_Pin, 0);
	HAL_Delay(20);
	HAL_GPIO_WritePin(TFT_RST_GPIO_Port, TFT_RST_Pin, 1);
	HAL_Delay(150);
}

void TFT_WriteCommand(uint8_t cmd)
{
	//A0 low
	//HAL_GPIO_WritePin(TFT_A0_GPIO_Port, TFT_A0_Pin, 0);

	//CS low
	HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_CS_Pin, 0);

	//send cmd
	uint16_t spi_cmd=(uint16_t)cmd;
	HAL_SPI_Transmit(&hspi1, &spi_cmd, 1, 10);

	//CS high
	HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_CS_Pin, 1);
}

void TFT_WriteData(uint8_t cmd)
{
	//A0 high
	//HAL_GPIO_WritePin(TFT_A0_GPIO_Port, TFT_A0_Pin, 1);

	//CS low
	HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_CS_Pin, 0);

	//send data
	uint16_t spi_cmd=(uint16_t)cmd|(1<<8);
	HAL_SPI_Transmit(&hspi1, &spi_cmd, 1, 10);

	//CS high
	HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_CS_Pin, 1);
}

void TFT_CommandList(const uint8_t* list)
{
	#define INIT_DELAY	(1<<7)

	uint8_t  numCommands, numArgs;
	uint8_t  ms;
	uint16_t addr=0;

	numCommands = list[addr++];					// Number of commands to follow
	while(numCommands--)						// For each command...
	{
		TFT_WriteCommand(list[addr++]);			// Read, issue command
		numArgs = list[addr++];					// Number of args to follow
		ms = numArgs & INIT_DELAY;				// If 7th bit set, delay follows args
		numArgs &= ~INIT_DELAY;					// Mask out delay bit

		while(numArgs--)						// For each argument...
		{
			TFT_WriteData(list[addr++]);		// Read, issue argument
		}

		if(ms)
		{
			ms = list[addr++];     // Read post-command delay time (ms)
			HAL_Delay( (ms==255 ? 500 : ms) );
		}
	}
}

void TFT_Init(void)
{
	//Initialization commands for ST7735 screens
	static const uint8_t Rcmd1[] = {   // Init for 7735R, part 1 (red or green tab)
	    15,                       // 15 commands in list:
	    ST7735_SWRESET,   INIT_DELAY,  //  1: Software reset, 0 args, w/delay
	      150,                    //     150 ms delay
	    ST7735_SLPOUT ,   INIT_DELAY,  //  2: Out of sleep mode, 0 args, w/delay
	      255,                    //     500 ms delay
	    ST7735_FRMCTR1, 3      ,  //  3: Frame rate ctrl - normal mode, 3 args:
	      0x00, 0x2c, 0x2d,       //     Rate = fosc/(1x2+40) * (LINE+2C+2D)
	    ST7735_FRMCTR2, 3      ,  //  4: Frame rate control - idle mode, 3 args:
	      0x00, 0x2c, 0x2d,       //     Rate = fosc/(1x2+40) * (LINE+2C+2D)
	    ST7735_FRMCTR3, 6      ,  //  5: Frame rate ctrl - partial mode, 6 args:
	      0x00, 0x2c, 0x2d,       //     Dot inversion mode
	      0x00, 0x2c, 0x2d,       //     Line inversion mode
	    ST7735_INVCTR , 1      ,  //  6: Display inversion ctrl, 1 arg, no delay:
	      0x00,                   //     No inversion
	    ST7735_PWCTR1 , 3      ,  //  7: Power control, 3 args, no delay:
	      0xA2,
	      0x02,                   //     -4.6V
	      0x84,                   //     AUTO mode
	    ST7735_PWCTR2 , 1      ,  //  8: Power control, 1 arg, no delay:
	      0xC5,                   //     VGH25 = 2.4C VGSEL = -10 VGH = 3 * AVDD
	    ST7735_PWCTR3 , 2      ,  //  9: Power control, 2 args, no delay:
	      0x0A,                   //     Opamp current small
	      0x00,                   //     Boost frequency
	    ST7735_PWCTR4 , 2      ,  // 10: Power control, 2 args, no delay:
	      0x8A,                   //     BCLK/2, Opamp current small & Medium low
	      0x2A,
	    ST7735_PWCTR5 , 2      ,  // 11: Power control, 2 args, no delay:
	      0x8A, 0xEE,
	    ST7735_VMCTR1 , 1      ,  // 12: Power control, 1 arg, no delay:
	      0x0E,
	    ST7735_INVOFF , 0      ,  // 13: Don't invert display, no args, no delay
	    ST7735_MADCTL , 1      ,  // 14: Memory access control (directions), 1 arg:
	      0xC8,                   //     row addr/col addr, bottom to top refresh
	    ST7735_COLMOD , 1      ,  // 15: set color mode, 1 arg, no delay:
	      0x05 },                 //     16-bit color

	  Rcmd3[] = {                 // Init for 7735R, part 3 (red or green tab)
	    4,                        //  4 commands in list:
	    ST7735_GMCTRP1, 16      , //  1: Magical unicorn dust, 16 args, no delay:
	      0x02, 0x1c, 0x07, 0x12,
	      0x37, 0x32, 0x29, 0x2d,
	      0x29, 0x25, 0x2B, 0x39,
	      0x00, 0x01, 0x03, 0x10,
	    ST7735_GMCTRN1, 16      , //  2: Sparkles and rainbows, 16 args, no delay:
	      0x03, 0x1d, 0x07, 0x06,
	      0x2E, 0x2C, 0x29, 0x2D,
	      0x2E, 0x2E, 0x37, 0x3F,
	      0x00, 0x00, 0x02, 0x10,
	    ST7735_NORON  ,    INIT_DELAY, //  3: Normal display on, no args, w/delay
	      10,                     //     10 ms delay
	    ST7735_DISPON ,    INIT_DELAY, //  4: Main screen turn on, no args w/delay
		100 }; //     100 ms delay

	TFT_Reset();

	TFT_CommandList(Rcmd1);
	TFT_CommandList(Rcmd3);
}

uint16_t TFT_RGBtoCol(uint8_t r, uint8_t g, uint8_t b)
{
	r>>=3;	g>>=2;	b>>=3;

	return (uint16_t)(r<<11 | g<<6 | b);
}

void TFT_PutPixel(uint8_t x, uint8_t y, uint16_t color)
{
	if((x>=128) || (y>=128))
		return;

	//more sparkles and rainbows
	//nobody knows why this is needed
	x=127-x ;x+=2;
	y=127-y; y+=3;

	GPIOB->BSRR=(uint32_t)(1<<(4+16));		//SP1_CS LOW

	if(addr_col != x)
	{
		//GPIOC->BSRR=(uint32_t)(1<<(6+16));	//TFT_A0 LOW

		//while(!(SPI1->SR & SPI_SR_TXE));
		SPI1->DR=0x002A;
		while(SPI1->SR & SPI_SR_BSY);

		addr_col = x;
		//GPIOC->BSRR=(uint32_t)(1<<6);			//TFT_A0 HIGH

		//while(!(SPI1->SR & SPI_SR_TXE));
		SPI1->DR=0x0100;
		while(SPI1->SR & SPI_SR_BSY);
		//while(!(SPI1->SR & SPI_SR_TXE));
		SPI1->DR=(1<<8)|x;
		while(SPI1->SR & SPI_SR_BSY);
	}

	if(addr_row != y)
	{
		//GPIOC->BSRR=(uint32_t)(1<<(6+16)); //TFT_A0 LOW

		//while(!(SPI1->SR & SPI_SR_TXE));
		SPI1->DR=0x002B;
		while(SPI1->SR & SPI_SR_BSY);

		addr_row = y;
		//GPIOC->BSRR=(uint32_t)(1<<6); //TFT_A0 HIGH

		//while(!(SPI1->SR & SPI_SR_TXE));
		SPI1->DR=0x0100;
		while(SPI1->SR & SPI_SR_BSY);
		//while(!(SPI1->SR & SPI_SR_TXE));
		SPI1->DR=(1<<8)|y;
		while(SPI1->SR & SPI_SR_BSY);
	}

	//GPIOC->BSRR=(uint32_t)(1<<(6+16)); //TFT_A0 LOW

	//while(!(SPI1->SR & SPI_SR_TXE));
	SPI1->DR=0x002C;
	while(SPI1->SR & SPI_SR_BSY);

	//GPIOC->BSRR=(uint32_t)(1<<6); //TFT_A0 HIGH

	//while(!(SPI1->SR & SPI_SR_TXE));
	SPI1->DR=(1<<8)|(color>>8);
	while(SPI1->SR & SPI_SR_BSY);
	//while(!(SPI1->SR & SPI_SR_TXE));
	SPI1->DR=(1<<8)|(color);
	while(SPI1->SR & SPI_SR_BSY);

	GPIOB->BSRR=(uint32_t)(1<<4);	//SPI1_CS HIGH
}

void TFT_Clear(uint16_t color)
{
	for(uint8_t x=0; x<128; x++)
		for(uint8_t y=0; y<128;y++)
			TFT_PutPixel(x, y, color);
}

uint8_t TFT_DisplaySplash(uint8_t *img_path)
{
	uint8_t raw_image[128*128*3];
	uint16_t index=0;
	uint16_t pix;

	if(f_open(&myFile, img_path, FA_OPEN_EXISTING | FA_READ) == FR_OK)
	{
		if(f_read(&myFile, raw_image, 128*128*3, NULL) == FR_OK)
			f_close(&myFile);
		else
			return 1;
	}
	else
		return 2;

	for(uint8_t x=0; x<128; x++)
	{
		for(uint8_t y=0; y<128; y++)
		{
			pix=TFT_RGBtoCol(raw_image[((uint32_t)y*128+x)*3], raw_image[((uint32_t)y*128+x)*3+1], raw_image[((uint32_t)y*128+x)*3+2]);
			TFT_PutPixel(x, y, pix);
		}
	}

	return 0;
}

void TFT_PutStr(uint8_t x, uint8_t y, const uint8_t* str, uint8_t font, uint16_t color)
{
	if(font==FONT_MONOSPACED_16_9)//monospaced, 16px height, 9px width
	{
		for(uint8_t i=0; i<strlen(str); i++)
		{
			if(str[i]==' ')
			{
				x+=2;
			}
			else
			{
				uint8_t c=str[i];
				uint8_t width=fonts_glyph_dsc[c-'!'].w_px;
				uint16_t start=fonts_glyph_dsc[c-'!'].glyph_index;
				uint8_t b_width=(width-1)/8+1;

				for(uint8_t row=0; row<16; row++)
				{
					for(uint8_t byte=0; byte<b_width; byte++)
					{
						for(uint8_t pix=0; pix<8; pix++)
						{
							if(font_1[start+byte+row*b_width]&(1<<(7-pix)))
								TFT_PutPixel(x+pix + byte*8, y+row, color);
						}
					}
				}

				x+=width+2;
			}
		}
	}
}

void TFT_PutStrBold(uint8_t x, uint8_t y, const uint8_t* str, uint8_t font, uint16_t color)
{
	TFT_PutStr(x, y, str, font, color);
	TFT_PutStr(x+1, y, str, font, color);
}

void TFT_PutStrCentered(uint8_t y, const uint8_t* str, uint8_t font, uint16_t color)
{
	uint8_t x=0;
	uint8_t width=0;

	if(font==1)
	{
		for(uint8_t i=0; i<strlen(str); i++)
		{
			if(str[i]!=' ')
				width+=fonts_glyph_dsc[str[i]-'!'].w_px+2;
			else
				width+=2;
		}

		//x=(127-strlen(str)*9)/2;
		x=(127-width)/2;
	}

	TFT_PutStr(x, y, str, font, color);
}

//TODO: not very efficient, but works
void TFT_PutStrCenteredBold(uint8_t y, const uint8_t* str, uint8_t font, uint16_t color)
{
	uint8_t x=0;
	uint8_t width=0;

	if(font==1)
	{
		for(uint8_t i=0; i<strlen(str); i++)
		{
			if(str[i]!=' ')
				width+=fonts_glyph_dsc[str[i]-'!'].w_px+2;
			else
				width+=2;
		}

		//x=(127-strlen(str)*9)/2;
		x=(127-width)/2;
	}

	TFT_PutStr(x, y, str, font, color);
	TFT_PutStr(x+1, y, str, font, color);
}

//-------------------------------------ESP-------------------------------------
void ESP_Enable(uint8_t ena)
{
	if(ena)
		HAL_GPIO_WritePin(WIFI_EN_GPIO_Port, WIFI_EN_Pin, 1);
	else
		HAL_GPIO_WritePin(WIFI_EN_GPIO_Port, WIFI_EN_Pin, 0);
}

void ESP_GetResp(void)
{
	memset(esp_rcv, 0, sizeof(esp_rcv));
	esp_cnt=0;
	HAL_UART_Receive_IT(&huart2, esp_rcv, 1);
}

void ESP_ConnectAP(uint8_t *ap_name, uint8_t *pwd)
{
	sprintf(esp_cmd, "AT+CWMODE=1\r\n", ap_name, pwd);
	HAL_UART_Transmit(&huart2, esp_cmd, strlen(esp_cmd), 10);
	HAL_Delay(50);
	sprintf(esp_cmd, "AT+CWJAP=\"%s\",\"%s\"\r\n", ap_name, pwd);
	HAL_UART_Transmit(&huart2, esp_cmd, strlen(esp_cmd), 10);
}

//------------------------------------MoIP-------------------------------------
void MoIP_Connect(uint8_t *addr, uint16_t port)
{
	ESP_GetResp();
	sprintf(esp_cmd, "AT+CIPSTART=1,\"UDP\",\"%s\",%d\r\n", addr, port);
	HAL_UART_Transmit_IT(&huart2, esp_cmd, strlen(esp_cmd));
	while(esp_rcv[13]!='O' && esp_rcv[14]!='K');
}

//send a MoIP packet
//we expect a MOIP_UDP_SIZE byte long input at arg1
void MoIP_Send(uint8_t *inp)
{
	sprintf(esp_cmd, "AT+CIPSEND=1,%d\r\n", MOIP_UDP_SIZE);
	HAL_UART_Transmit_DMA(&huart2, esp_cmd, strlen(esp_cmd));
	HAL_Delay(10);	//FIXME: wait for '>' character
	HAL_UART_Transmit_DMA(&huart2, inp, MOIP_UDP_SIZE);
}

//-------------------------------------ADF-------------------------------------
void ADF_Init(void)
{
	HAL_GPIO_WritePin(ADF_CE_GPIO_Port, ADF_CE_Pin, 0);
	HAL_Delay(100);
	HAL_GPIO_WritePin(ADF_CE_GPIO_Port, ADF_CE_Pin, 1);
}

void ADF_WriteReg(uint32_t val)
{
	HAL_GPIO_WritePin(ADF_SLE_GPIO_Port, ADF_SLE_Pin, 0);
	HAL_Delay(0);

	HAL_GPIO_WritePin(ADF_SCLK_GPIO_Port, ADF_SCLK_Pin, 0);
	HAL_Delay(0);

	for(uint8_t i=0; i<32; i++)
	{
		HAL_GPIO_WritePin(ADF_SDATA_GPIO_Port, ADF_SDATA_Pin, (val>>(31-i))&1);
		for(uint16_t i=0; i<100; i++)
			asm("NOP");
		HAL_GPIO_WritePin(ADF_SCLK_GPIO_Port, ADF_SCLK_Pin, 1);
		for(uint16_t i=0; i<100; i++)
			asm("NOP");
		HAL_GPIO_WritePin(ADF_SCLK_GPIO_Port, ADF_SCLK_Pin, 0);
		for(uint16_t i=0; i<100; i++)
			asm("NOP");
	}

	HAL_GPIO_WritePin(ADF_SLE_GPIO_Port, ADF_SLE_Pin, 1);
	for(uint16_t i=0; i<100; i++)
		asm("NOP");
	HAL_GPIO_WritePin(ADF_SLE_GPIO_Port, ADF_SLE_Pin, 0);
	for(uint16_t i=0; i<100; i++)
		asm("NOP");
}

void ADF_SetFreq(uint32_t freq, uint8_t rx)
{
	uint8_t i=0;
	uint16_t f=0;

	if(rx)
		freq-=100000;	//IF correction

	i=freq/(12288000/(3*2));
	f=10*(freq-i*(12288000/(3*2)))/625;

	ADF_WriteReg((i<<19)|(f<<4)|(rx<<27));
}

uint16_t ADF_GetChipVersion(void)
{
	uint32_t val=0x1C7;
	uint16_t rv=0;

	HAL_GPIO_WritePin(ADF_SLE_GPIO_Port, ADF_SLE_Pin, 0);
	HAL_Delay(0);

	HAL_GPIO_WritePin(ADF_SCLK_GPIO_Port, ADF_SCLK_Pin, 0);
	HAL_Delay(0);

	for(uint8_t i=0; i<9; i++)
	{
		HAL_GPIO_WritePin(ADF_SDATA_GPIO_Port, ADF_SDATA_Pin, (val>>(8-i))&1);
		for(uint16_t i=0; i<1000; i++)
			asm("NOP");
		HAL_GPIO_WritePin(ADF_SCLK_GPIO_Port, ADF_SCLK_Pin, 1);
		for(uint16_t i=0; i<1000; i++)
			asm("NOP");
		HAL_GPIO_WritePin(ADF_SCLK_GPIO_Port, ADF_SCLK_Pin, 0);
		for(uint16_t i=0; i<1000; i++)
			asm("NOP");
	}

	HAL_GPIO_WritePin(ADF_SLE_GPIO_Port, ADF_SLE_Pin, 1);
	for(uint16_t i=0; i<1000; i++)
		asm("NOP");

	//readback
	for(uint8_t i=0; i<16+1; i++)
	{
		HAL_GPIO_WritePin(ADF_SCLK_GPIO_Port, ADF_SCLK_Pin, 1);
		for(uint16_t i=0; i<1000; i++)
			asm("NOP");
		HAL_GPIO_WritePin(ADF_SCLK_GPIO_Port, ADF_SCLK_Pin, 0);
		for(uint16_t i=0; i<1000; i++)
			asm("NOP");

		if(i)
		{
			rv|=((((ADF_SREAD_GPIO_Port->IDR)>>11)&1)<<(16-i));
		}

		for(uint16_t i=0; i<1000; i++)
			asm("NOP");
	}

	HAL_GPIO_WritePin(ADF_SCLK_GPIO_Port, ADF_SCLK_Pin, 1);
	for(uint16_t i=0; i<1000; i++)
		asm("NOP");
	HAL_GPIO_WritePin(ADF_SLE_GPIO_Port, ADF_SLE_Pin, 0);
	for(uint16_t i=0; i<1000; i++)
		asm("NOP");
	HAL_GPIO_WritePin(ADF_SCLK_GPIO_Port, ADF_SCLK_Pin, 0);

	return rv;
}

float ADF_GetRSSI(void)
{
	uint32_t val=0x147;
	uint16_t rv=0;

	HAL_GPIO_WritePin(ADF_SLE_GPIO_Port, ADF_SLE_Pin, 0);
	HAL_Delay(0);

	HAL_GPIO_WritePin(ADF_SCLK_GPIO_Port, ADF_SCLK_Pin, 0);
	HAL_Delay(0);

	for(uint8_t i=0; i<9; i++)
	{
		HAL_GPIO_WritePin(ADF_SDATA_GPIO_Port, ADF_SDATA_Pin, (val>>(8-i))&1);
		for(uint16_t i=0; i<100; i++)
			asm("NOP");
		HAL_GPIO_WritePin(ADF_SCLK_GPIO_Port, ADF_SCLK_Pin, 1);
		for(uint16_t i=0; i<100; i++)
			asm("NOP");
		HAL_GPIO_WritePin(ADF_SCLK_GPIO_Port, ADF_SCLK_Pin, 0);
		for(uint16_t i=0; i<100; i++)
			asm("NOP");
	}

	HAL_GPIO_WritePin(ADF_SLE_GPIO_Port, ADF_SLE_Pin, 1);
	for(uint16_t i=0; i<100; i++)
		asm("NOP");

	//readback
	for(uint8_t i=0; i<16+1; i++)
	{
		HAL_GPIO_WritePin(ADF_SCLK_GPIO_Port, ADF_SCLK_Pin, 1);
		for(uint16_t i=0; i<100; i++)
			asm("NOP");
		HAL_GPIO_WritePin(ADF_SCLK_GPIO_Port, ADF_SCLK_Pin, 0);
		for(uint16_t i=0; i<100; i++)
			asm("NOP");

		if(i)
		{
			rv|=((((ADF_SREAD_GPIO_Port->IDR)>>11)&1)<<(16-i));
		}

		for(uint16_t i=0; i<100; i++)
			asm("NOP");
	}

	HAL_GPIO_WritePin(ADF_SCLK_GPIO_Port, ADF_SCLK_Pin, 1);
	for(uint16_t i=0; i<100; i++)
		asm("NOP");
	HAL_GPIO_WritePin(ADF_SLE_GPIO_Port, ADF_SLE_Pin, 0);
	for(uint16_t i=0; i<100; i++)
		asm("NOP");
	HAL_GPIO_WritePin(ADF_SCLK_GPIO_Port, ADF_SCLK_Pin, 0);

	uint8_t lna_gain=(rv>>9)&3;
	uint8_t filter_gain=(rv>>7)&3;
	uint8_t rdb=rv&0x7F;
	float gain_corr=0.0;

	if(lna_gain==2 && filter_gain==2)
		gain_corr=0.0;
	else if(lna_gain==1)
	{
		if(filter_gain==2)
			gain_corr=24.0;
		else if(filter_gain==1)
			gain_corr=38.0;
		else
			gain_corr=58.0;
	}
	else if(lna_gain==0 && filter_gain==0)
		gain_corr=86.0;

	return -130.0+((float)rdb+gain_corr)/2.0;
}

//-------------------------------------IRQ-------------------------------------
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance==USART2)
	{
		esp_cnt++;
		HAL_UART_Receive_IT(&huart2, &esp_rcv[esp_cnt], 1);
	}
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if(GPIO_Pin==PTT_INT_Pin)
	{
		HAL_GPIO_TogglePin(LED_RED_GPIO_Port, LED_RED_Pin);
	}
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	//4kHz (f_s/2) test
	HAL_GPIO_TogglePin(N9_GPIO_Port, N9_Pin);
}

void HAL_DAC_ConvCpltCallbackCh1(DAC_HandleTypeDef* hdac)
{
	dac_play=0;
	HAL_GPIO_TogglePin(N7_GPIO_Port, N7_Pin);
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
	//ADC3 -> FM demod in
	if(hadc->Instance==ADC3)
	{
		buff_num++;
		buff_num%=2;

		HAL_GPIO_WritePin(N8_GPIO_Port, N8_Pin, buff_num);

		if(buff_num)
		{
			//if(rssi>-50.0){
			HAL_ADC_Start_DMA(&hadc3, &fm_demod_in[320], 320);
			dac_play=0;//}
		}
		else
		{
			//if(rssi>-50.0){
			HAL_ADC_Start_DMA(&hadc3, fm_demod_in, 320);
			dac_play=0;//}
		}
	}

	//ADC1 -> MIC in
	else if(hadc->Instance==ADC1)
	{
		for(uint16_t i=0; i<2*320; i++)
		{
			pr_audio_samples[i]=((int16_t)audio_samples[i]-DC_OFFSET)*mic_gain;
		}
		codec2_encode_3200(cod, &c2_data[0], &pr_audio_samples[0]);
		codec2_encode_3200(cod, &c2_data[8], &pr_audio_samples[160]);
		data_rdy=1;
		//start another run. FIXME: use 2 buffers and swap them
		//because this approach causes breaks in audio
		HAL_ADC_Start_DMA(&hadc1, audio_samples, 320);
	}
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  *
  * @retval None
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_ADC1_Init();
  MX_DAC_Init();
  MX_SPI2_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  MX_USART3_UART_Init();
  MX_I2C1_Init();
  MX_SDMMC1_SD_Init();
  MX_CRYP_Init();
  MX_HASH_Init();
  MX_RNG_Init();
  MX_CRC_Init();
  MX_FATFS_Init();
  MX_ADC2_Init();
  MX_TIM4_Init();
  MX_TIM6_Init();
  MX_TIM8_Init();
  MX_TIM7_Init();
  MX_TIM5_Init();
  MX_ADC3_Init();
  MX_SPI1_Init();
  MX_TIM3_Init();
  MX_UART7_Init();
  MX_USB_OTG_FS_PCD_Init();
  /* USER CODE BEGIN 2 */
  //Init stuff
  //RF_SetPWR(0);
  RF_Mode(RX_MODE);
  AUDIO_Mux(AUDIO_MUX_NONE);
  LNA_Ctrl(LNA_OFF);
  TFT_SetBrght(0);
  TFT_Reset();
  TFT_Init();
  ADF_Init();
  CRC_Init(CRC_LUT, crc_poly);	//not used if HW CRC is enabled
  hcrc.Init.GeneratingPolynomial = 0x5935;
  HAL_CRC_Init(&hcrc);

  //DAC_OUT2 test
  /*HAL_DAC_Start(&hdac, DAC_CHANNEL_2);
  while(1)
  {
	  //HAL_DAC_Start(&hdac, DAC_CHANNEL_2);
	  HAL_DAC_SetValue(&hdac, DAC_CHANNEL_2, DAC_ALIGN_12B_R, 0);
	  HAL_Delay(50);
	  //HAL_DAC_Start(&hdac, DAC_CHANNEL_2);
	  HAL_DAC_SetValue(&hdac, DAC_CHANNEL_2, DAC_ALIGN_12B_R, 0xFF);
	  HAL_Delay(50);
  }*/

  //SD card
  if(f_mount(&SDFatFS, (TCHAR const*)SDPath, 0))
  {
	  TFT_Clear(CL_BLACK);
  	  TFT_PutStrCenteredBold(128/2-8, "CARD ERROR", 1, CL_RED);
  	  TFT_SetBrght(40);

  	  while(1)
  	  {
  		  HAL_GPIO_TogglePin(LED_RED_GPIO_Port, LED_RED_Pin);
  		  HAL_Delay(500);
  	  }
  }

  chip_rev=ADF_GetChipVersion();
  if(chip_rev!=0x2104)	//not OK?
  {
	  TFT_Clear(CL_BLACK);
	  TFT_PutStrCenteredBold(128/2-8, "ADF7021 ERROR", 1, CL_RED);
	  TFT_SetBrght(40);

	  while(1)
	  {
		  HAL_GPIO_TogglePin(LED_RED_GPIO_Port, LED_RED_Pin);
		  HAL_Delay(100);
	  }
  }

  //HAL_Delay(500);
  //TFT_Clear(CL_BLACK);
  TFT_DisplaySplash("splash.raw");
  TFT_SetBrght(40);

  ESP_Enable(1);
  HAL_Delay(500);
  //TFT_Clear(CL_WHITE);

  huart2.Init.Mode = UART_MODE_TX;
  HAL_UART_Init(&huart2);

  sprintf(esp_cmd, "ATE0\r\n");
  HAL_UART_Transmit(&huart2, esp_cmd, strlen(esp_cmd), 10);
  HAL_Delay(100);

  huart2.Init.Mode = UART_MODE_TX_RX;
  HAL_UART_Init(&huart2);

  HAL_Delay(1000);
  //ESP_ConnectAP("Teletra", "");

  sprintf(esp_cmd, "AT+CIPMUX=1\r\n");
  HAL_UART_Transmit(&huart2, esp_cmd, strlen(esp_cmd), 2);

  ESP_GetResp();
  while(esp_rcv[32]!='P');	//"WIFI GOT IP"
  HAL_Delay(1);

  //MoIP test
  MoIP_Connect("192.168.1.186", 17000);
  //MoIP_Connect("m17.link", 17000);

  HAL_Delay(550);

  //Codec2
  cod = codec2_create(CODEC2_MODE_3200);
  HAL_TIM_Base_Start(&htim6);
  HAL_ADC_Start_DMA(&hadc1, audio_samples, 320);

  sprintf(packet.src, "SP5WWP");
  //sprintf(packet.dst, "KC1AWV");
  sprintf(packet.dst, "W2FBI");
  packet.type=P_TYPE_VOICE;

  /*for(uint16_t p=0; p<20; p++)
  {
	  packet.fn=p;
	  sprintf(packet.payload, "Hello M17!\r\n");
	  M17_Framer(&packet, udp_frame, p==19?1:0);
	  MoIP_Send(udp_frame);
	  HAL_Delay(10);
  }*/

  //DAC OUT2 (audio) test
  /*for(uint16_t i=0; i<320; i++)
	  audio_samples[i]=0xFFF*(sin((40.0*i)/320.0*2*3.14159265348)/2.0+0.5);
  HAL_DAC_Start_DMA(&hdac, DAC_CHANNEL_1, audio_samples, 320, DAC_ALIGN_12B_R);
  AUDIO_Mux(AUDIO_MUX_SPK);
  HAL_TIM_Base_Start(&htim6);*/

  //ADF7021 test
  /*ADF_WriteReg((uint32_t)0x0003B|((uint32_t)0x3243<<8));	//SWD (0x3B) syncword: 0x3243
  HAL_Delay(2-1);
  ADF_WriteReg((uint32_t)0x0010C|(uint32_t)1<<8);			//DPL (0x10C)
  HAL_Delay(2-1);
  ADF_WriteReg((uint32_t)0x5770B4); //0x5770B4
  HAL_Delay(2-1);
  ADF_WriteReg((uint32_t)0x01ED5);	//coarse cal ON
  HAL_Delay(2-1);
  ADF_WriteReg((uint32_t)0x505EBA6|(uint32_t)1<<4);			//0x505EBA6
  HAL_Delay(10-1);
  //((uint32_t)0x007);
  //HAL_Delay(2-1);
  //ADF_WriteReg((uint32_t)0x0008);
  //HAL_Delay(2-1);
  ADF_WriteReg((uint32_t)0x9|(uint32_t)40<<4|(uint32_t)70<<11);//|((uint32_t)1<<18)|((uint32_t)2<<20)|((uint32_t)2<<22)); //manual gain - max
  HAL_Delay(2-1);
  ADF_WriteReg((uint32_t)0x3296556B);
  HAL_Delay(2-1);
  ADF_WriteReg((uint32_t)0x003BD);

  ADF_WriteReg((uint32_t)0x475031);
  HAL_Delay(2-1);

  //ADF_WriteReg((uint32_t)0x29ECA093);	//CDR=40
  ADF_WriteReg((uint32_t)(0x29ECA093&(~(0xFF<<10)))|1<<10);	//CDR=1 for DAC test
  HAL_Delay(2-1);

  ADF_SetFreq(439425000, 1);	//SR5ND
  LNA_Ctrl(LNA_ON);

  HAL_TIM_Base_Start(&htim6);
  HAL_ADC_Start_DMA(&hadc3, fm_demod_in, 320);
  AUDIO_Mux(AUDIO_MUX_SPK);*/

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  if(data_rdy)
	  {
		  ypcmem(packet.payload, c2_data, 16);
		  M17_Framer(&packet, udp_frame, 0);
		  MoIP_Send(udp_frame);
		  packet.fn++;
		  data_rdy=0;
	  }

	  /*HAL_GPIO_WritePin(LED_GRN_GPIO_Port, LED_GRN_Pin, 0);
	  HAL_Delay(50);
	  HAL_GPIO_WritePin(LED_GRN_GPIO_Port, LED_GRN_Pin, 1);
	  HAL_Delay(950);*/
	  /*if(!dac_play)
	  {
		  if(buff_num)
			  HAL_DAC_Start_DMA(&hdac, DAC_CHANNEL_1, &fm_demod_in[0], 320, DAC_ALIGN_12B_R);
		  else
			  HAL_DAC_Start_DMA(&hdac, DAC_CHANNEL_1, &fm_demod_in[320], 320, DAC_ALIGN_12B_R);
		  //HAL_DAC_Start_DMA(&hdac, DAC_CHANNEL_1, audio_samples, 320, DAC_ALIGN_12B_R);
		  dac_play=1;
	  }*/

	  //rssi=ADF_GetRSSI();
	  /*HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, 0);
	  HAL_Delay(50);
	  HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, 1);
	  HAL_Delay(450);*/
  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */

  }
  /* USER CODE END 3 */

}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct;

    /**Configure the main internal regulator output voltage 
    */
  __HAL_RCC_PWR_CLK_ENABLE();

  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = 16;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 216;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Activate the Over-Drive mode 
    */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_7) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USART1|RCC_PERIPHCLK_USART2
                              |RCC_PERIPHCLK_USART3|RCC_PERIPHCLK_UART7
                              |RCC_PERIPHCLK_I2C1|RCC_PERIPHCLK_SDMMC1
                              |RCC_PERIPHCLK_CLK48;
  PeriphClkInitStruct.Usart1ClockSelection = RCC_USART1CLKSOURCE_HSI;
  PeriphClkInitStruct.Usart2ClockSelection = RCC_USART2CLKSOURCE_HSI;
  PeriphClkInitStruct.Usart3ClockSelection = RCC_USART3CLKSOURCE_HSI;
  PeriphClkInitStruct.Uart7ClockSelection = RCC_UART7CLKSOURCE_PCLK1;
  PeriphClkInitStruct.I2c1ClockSelection = RCC_I2C1CLKSOURCE_PCLK1;
  PeriphClkInitStruct.Clk48ClockSelection = RCC_CLK48SOURCE_PLL;
  PeriphClkInitStruct.Sdmmc1ClockSelection = RCC_SDMMC1CLKSOURCE_CLK48;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* ADC1 init function */
static void MX_ADC1_Init(void)
{

  ADC_ChannelConfTypeDef sConfig;

    /**Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion) 
    */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_RISING;
  hadc1.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T6_TRGO;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 1;
  hadc1.Init.DMAContinuousRequests = ENABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time. 
    */
  sConfig.Channel = ADC_CHANNEL_0;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* ADC2 init function */
static void MX_ADC2_Init(void)
{

  ADC_ChannelConfTypeDef sConfig;

    /**Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion) 
    */
  hadc2.Instance = ADC2;
  hadc2.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
  hadc2.Init.Resolution = ADC_RESOLUTION_12B;
  hadc2.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc2.Init.ContinuousConvMode = DISABLE;
  hadc2.Init.DiscontinuousConvMode = DISABLE;
  hadc2.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_RISING;
  hadc2.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T8_TRGO;
  hadc2.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc2.Init.NbrOfConversion = 1;
  hadc2.Init.DMAContinuousRequests = ENABLE;
  hadc2.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  if (HAL_ADC_Init(&hadc2) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time. 
    */
  sConfig.Channel = ADC_CHANNEL_3;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_480CYCLES;
  if (HAL_ADC_ConfigChannel(&hadc2, &sConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* ADC3 init function */
static void MX_ADC3_Init(void)
{

  ADC_ChannelConfTypeDef sConfig;

    /**Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion) 
    */
  hadc3.Instance = ADC3;
  hadc3.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
  hadc3.Init.Resolution = ADC_RESOLUTION_12B;
  hadc3.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc3.Init.ContinuousConvMode = DISABLE;
  hadc3.Init.DiscontinuousConvMode = DISABLE;
  hadc3.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_RISING;
  hadc3.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T6_TRGO;
  hadc3.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc3.Init.NbrOfConversion = 1;
  hadc3.Init.DMAContinuousRequests = ENABLE;
  hadc3.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  if (HAL_ADC_Init(&hadc3) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time. 
    */
  sConfig.Channel = ADC_CHANNEL_1;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
  if (HAL_ADC_ConfigChannel(&hadc3, &sConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* CRC init function */
static void MX_CRC_Init(void)
{

  hcrc.Instance = CRC;
  hcrc.Init.DefaultPolynomialUse = DEFAULT_POLYNOMIAL_DISABLE;
  hcrc.Init.DefaultInitValueUse = DEFAULT_INIT_VALUE_DISABLE;
  hcrc.Init.GeneratingPolynomial = 7;
  hcrc.Init.CRCLength = CRC_POLYLENGTH_16B;
  hcrc.Init.InitValue = 0xFFFF;
  hcrc.Init.InputDataInversionMode = CRC_INPUTDATA_INVERSION_NONE;
  hcrc.Init.OutputDataInversionMode = CRC_OUTPUTDATA_INVERSION_DISABLE;
  hcrc.InputDataFormat = CRC_INPUTDATA_FORMAT_BYTES;
  if (HAL_CRC_Init(&hcrc) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* CRYP init function */
static void MX_CRYP_Init(void)
{

  hcryp.Instance = CRYP;
  hcryp.Init.DataType = CRYP_DATATYPE_8B;
  hcryp.Init.KeySize = CRYP_KEYSIZE_256B;
  hcryp.Init.pKey = (uint8_t *)pKeyCRYP;
  hcryp.Init.pInitVect = (uint8_t *)pInitVectCRYP;
  if (HAL_CRYP_Init(&hcryp) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* DAC init function */
static void MX_DAC_Init(void)
{

  DAC_ChannelConfTypeDef sConfig;

    /**DAC Initialization 
    */
  hdac.Instance = DAC;
  if (HAL_DAC_Init(&hdac) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**DAC channel OUT1 config 
    */
  sConfig.DAC_Trigger = DAC_TRIGGER_T6_TRGO;
  sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;
  if (HAL_DAC_ConfigChannel(&hdac, &sConfig, DAC_CHANNEL_1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**DAC channel OUT2 config 
    */
  sConfig.DAC_Trigger = DAC_TRIGGER_SOFTWARE;
  if (HAL_DAC_ConfigChannel(&hdac, &sConfig, DAC_CHANNEL_2) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* HASH init function */
static void MX_HASH_Init(void)
{

  hhash.Init.DataType = HASH_DATATYPE_8B;
  if (HAL_HASH_Init(&hhash) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* I2C1 init function */
static void MX_I2C1_Init(void)
{

  hi2c1.Instance = I2C1;
  hi2c1.Init.Timing = 0x10C36994;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure Analogue filter 
    */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure Digital filter 
    */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* RNG init function */
static void MX_RNG_Init(void)
{

  hrng.Instance = RNG;
  if (HAL_RNG_Init(&hrng) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* SDMMC1 init function */
static void MX_SDMMC1_SD_Init(void)
{

  hsd1.Instance = SDMMC1;
  hsd1.Init.ClockEdge = SDMMC_CLOCK_EDGE_RISING;
  hsd1.Init.ClockBypass = SDMMC_CLOCK_BYPASS_DISABLE;
  hsd1.Init.ClockPowerSave = SDMMC_CLOCK_POWER_SAVE_ENABLE;
  hsd1.Init.BusWide = SDMMC_BUS_WIDE_1B;
  hsd1.Init.HardwareFlowControl = SDMMC_HARDWARE_FLOW_CONTROL_DISABLE;
  hsd1.Init.ClockDiv = 0;

}

/* SPI1 init function */
static void MX_SPI1_Init(void)
{

  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_1LINE;
  hspi1.Init.DataSize = SPI_DATASIZE_9BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 7;
  hspi1.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
  hspi1.Init.NSSPMode = SPI_NSS_PULSE_DISABLE;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* SPI2 init function */
static void MX_SPI2_Init(void)
{

  /* SPI2 parameter configuration*/
  hspi2.Instance = SPI2;
  hspi2.Init.Mode = SPI_MODE_MASTER;
  hspi2.Init.Direction = SPI_DIRECTION_2LINES;
  hspi2.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi2.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi2.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi2.Init.NSS = SPI_NSS_SOFT;
  hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi2.Init.CRCPolynomial = 7;
  hspi2.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
  hspi2.Init.NSSPMode = SPI_NSS_PULSE_DISABLE;
  if (HAL_SPI_Init(&hspi2) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* TIM3 init function */
static void MX_TIM3_Init(void)
{

  TIM_ClockConfigTypeDef sClockSourceConfig;
  TIM_MasterConfigTypeDef sMasterConfig;
  TIM_OC_InitTypeDef sConfigOC;

  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 215;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 999;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  if (HAL_TIM_PWM_Init(&htim3) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  HAL_TIM_MspPostInit(&htim3);

}

/* TIM4 init function */
static void MX_TIM4_Init(void)
{

  TIM_ClockConfigTypeDef sClockSourceConfig;
  TIM_MasterConfigTypeDef sMasterConfig;

  htim4.Instance = TIM4;
  htim4.Init.Prescaler = 269;
  htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim4.Init.Period = 999;
  htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim4) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim4, &sClockSourceConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMasterConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* TIM5 init function */
static void MX_TIM5_Init(void)
{

  TIM_ClockConfigTypeDef sClockSourceConfig;
  TIM_MasterConfigTypeDef sMasterConfig;

  htim5.Instance = TIM5;
  htim5.Init.Prescaler = 21599;
  htim5.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim5.Init.Period = 449;
  htim5.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim5.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim5) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim5, &sClockSourceConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim5, &sMasterConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* TIM6 init function */
static void MX_TIM6_Init(void)
{

  TIM_MasterConfigTypeDef sMasterConfig;

  htim6.Instance = TIM6;
  htim6.Init.Prescaler = 0;
  htim6.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim6.Init.Period = 13499;
  htim6.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim6) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim6, &sMasterConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* TIM7 init function */
static void MX_TIM7_Init(void)
{

  TIM_MasterConfigTypeDef sMasterConfig;

  htim7.Instance = TIM7;
  htim7.Init.Prescaler = 21599;
  htim7.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim7.Init.Period = 999;
  htim7.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim7) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim7, &sMasterConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* TIM8 init function */
static void MX_TIM8_Init(void)
{

  TIM_ClockConfigTypeDef sClockSourceConfig;
  TIM_MasterConfigTypeDef sMasterConfig;

  htim8.Instance = TIM8;
  htim8.Init.Prescaler = 43199;
  htim8.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim8.Init.Period = 49999;
  htim8.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim8.Init.RepetitionCounter = 0;
  htim8.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim8) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim8, &sClockSourceConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
  sMasterConfig.MasterOutputTrigger2 = TIM_TRGO2_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim8, &sMasterConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* UART7 init function */
static void MX_UART7_Init(void)
{

  huart7.Instance = UART7;
  huart7.Init.BaudRate = 115200;
  huart7.Init.WordLength = UART_WORDLENGTH_8B;
  huart7.Init.StopBits = UART_STOPBITS_1;
  huart7.Init.Parity = UART_PARITY_NONE;
  huart7.Init.Mode = UART_MODE_TX_RX;
  huart7.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart7.Init.OverSampling = UART_OVERSAMPLING_16;
  huart7.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart7.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart7) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* USART1 init function */
static void MX_USART1_UART_Init(void)
{

  huart1.Instance = USART1;
  huart1.Init.BaudRate = 9600;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_8;
  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* USART2 init function */
static void MX_USART2_UART_Init(void)
{

  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_8;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* USART3 init function */
static void MX_USART3_UART_Init(void)
{

  huart3.Instance = USART3;
  huart3.Init.BaudRate = 9600;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_8;
  huart3.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart3.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* USB_OTG_FS init function */
static void MX_USB_OTG_FS_PCD_Init(void)
{

  hpcd_USB_OTG_FS.Instance = USB_OTG_FS;
  hpcd_USB_OTG_FS.Init.dev_endpoints = 6;
  hpcd_USB_OTG_FS.Init.speed = PCD_SPEED_FULL;
  hpcd_USB_OTG_FS.Init.dma_enable = DISABLE;
  hpcd_USB_OTG_FS.Init.ep0_mps = DEP0CTL_MPS_64;
  hpcd_USB_OTG_FS.Init.phy_itface = PCD_PHY_EMBEDDED;
  hpcd_USB_OTG_FS.Init.Sof_enable = DISABLE;
  hpcd_USB_OTG_FS.Init.low_power_enable = DISABLE;
  hpcd_USB_OTG_FS.Init.lpm_enable = DISABLE;
  hpcd_USB_OTG_FS.Init.vbus_sensing_enable = DISABLE;
  hpcd_USB_OTG_FS.Init.use_dedicated_ep1 = DISABLE;
  if (HAL_PCD_Init(&hpcd_USB_OTG_FS) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/** 
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void) 
{
  /* DMA controller clock enable */
  __HAL_RCC_DMA2_CLK_ENABLE();
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Stream5_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream5_IRQn);
  /* DMA1_Stream6_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream6_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream6_IRQn);
  /* DMA2_Stream0_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream0_IRQn, 9, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream0_IRQn);
  /* DMA2_Stream1_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream1_IRQn);
  /* DMA2_Stream2_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream2_IRQn, 15, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream2_IRQn);

}

/** Configure pins as 
        * Analog 
        * Input 
        * Output
        * EVENT_OUT
        * EXTI
*/
static void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE, TRX_SW_Pin|N6_Pin|N5_Pin|N4_Pin 
                          |N3_Pin|N2_Pin|N1_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, XO_EN_Pin|LNA_EN_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, VIBRATE_Pin|N9_Pin|N8_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, SPK_AMP_SEL_Pin|FM_MOD_SEL_Pin|SPI1_CS_Pin|WIFI_RST_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, N7_Pin|WIFI_EN_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, ADF_CE_Pin|LED_GRN_Pin|LED_RED_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, ADF_SLE_Pin|ADF_SDATA_Pin|ADF_SCLK_Pin|TFT_RST_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPS_EN_GPIO_Port, GPS_EN_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : TRX_SW_Pin N6_Pin N5_Pin N4_Pin 
                           N3_Pin N2_Pin N1_Pin */
  GPIO_InitStruct.Pin = TRX_SW_Pin|N6_Pin|N5_Pin|N4_Pin 
                          |N3_Pin|N2_Pin|N1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pins : XO_EN_Pin VIBRATE_Pin N9_Pin N8_Pin */
  GPIO_InitStruct.Pin = XO_EN_Pin|VIBRATE_Pin|N9_Pin|N8_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : SPK_AMP_SEL_Pin FM_MOD_SEL_Pin N7_Pin WIFI_RST_Pin 
                           WIFI_EN_Pin */
  GPIO_InitStruct.Pin = SPK_AMP_SEL_Pin|FM_MOD_SEL_Pin|N7_Pin|WIFI_RST_Pin 
                          |WIFI_EN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : ADF_CE_Pin ADF_SLE_Pin ADF_SDATA_Pin ADF_SCLK_Pin */
  GPIO_InitStruct.Pin = ADF_CE_Pin|ADF_SLE_Pin|ADF_SDATA_Pin|ADF_SCLK_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pin : ADF_SREAD_Pin */
  GPIO_InitStruct.Pin = ADF_SREAD_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(ADF_SREAD_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : ADF_SWD_Pin ADF_MUX_OUT_Pin */
  GPIO_InitStruct.Pin = ADF_SWD_Pin|ADF_MUX_OUT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pin : LNA_EN_Pin */
  GPIO_InitStruct.Pin = LNA_EN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(LNA_EN_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : GPS_EN_Pin */
  GPIO_InitStruct.Pin = GPS_EN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPS_EN_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : LED_GRN_Pin LED_RED_Pin TFT_RST_Pin */
  GPIO_InitStruct.Pin = LED_GRN_Pin|LED_RED_Pin|TFT_RST_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pin : SPI1_CS_Pin */
  GPIO_InitStruct.Pin = SPI1_CS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(SPI1_CS_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : PTT_INT_Pin */
  GPIO_InitStruct.Pin = PTT_INT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(PTT_INT_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI0_IRQn, 12, 0);
  HAL_NVIC_EnableIRQ(EXTI0_IRQn);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  file: The file name as string.
  * @param  line: The line in file as a number.
  * @retval None
  */
void _Error_Handler(char *file, int line)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1)
  {
	  //HAL_GPIO_TogglePin(LED_RED_GPIO_Port, LED_RED_Pin);
	  //HAL_Delay(200);

  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
