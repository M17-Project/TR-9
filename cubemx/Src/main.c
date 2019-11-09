
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
  * Copyright (c) 2019 STMicroelectronics International N.V. 
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
#include "TFT_ST7735.h"
#include "radio_config.h"	//PKT_5_0 - dwa ostatnie bajty, PKT_5_1 - 5ty bajt
#include "codec2.h"
#include "fields.h"
#include "codeplug.h"

#define FW_VER			1050					//firmware version
#define HW_VER			12						//11 - v1.1, 12 - v1.2
#define	NO_ADXL			1						//no ADXL for testing
#define	ADXL_ADDR		0x53					//ADXL address
#define SHOW_DBG_INFO	1						//show version info on spashscreen?

#define	FRAMESIZE		160*2					//20+20=40ms frame
#define	RAW_BYTES		8*2
#define	CRC_LEN			2
#define	ENC_LEN			(RAW_BYTES)*2
#define	PLOAD_LEN		93						//TODO: change to 93 (0x5D) - was 97

#define	DC_OFFSET		2040					//input signal DC offset

#define	MAX_TX_POWER	0x7F					//+20dBm

#define	TONE_SIZE		320						//tone length in samples (fs=8kHz) - must not exceed FRAMESIZE
/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;
ADC_HandleTypeDef hadc2;
DMA_HandleTypeDef hdma_adc1;
DMA_HandleTypeDef hdma_adc2;

CRC_HandleTypeDef hcrc;

CRYP_HandleTypeDef hcryp;
__ALIGN_BEGIN static const uint8_t pKeyCRYP[16] __ALIGN_END = {
                            0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                            0x00,0x00,0x00,0x00,0x00,0x00};
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

TIM_HandleTypeDef htim4;
TIM_HandleTypeDef htim5;
TIM_HandleTypeDef htim6;
TIM_HandleTypeDef htim7;
TIM_HandleTypeDef htim8;

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;
UART_HandleTypeDef huart3;
DMA_HandleTypeDef hdma_usart2_tx;

/* USER CODE BEGIN PV */

//FatFS file
FIL	myFile;

//UART & DEBUG
uint8_t test_line[50];						//test buffer

//KEYBOARD, ENCODER etc.
volatile uint8_t kbd_rcv_val=0;				//keyboard input - key ID

//TX SOUND BUFFERS
volatile uint8_t adc_busy=0;				//are we currently reading samples from the mic?
volatile uint8_t buff_num=0;				//actual sound input buffer number
volatile uint16_t buffer_1[FRAMESIZE];
volatile uint16_t buffer_2[FRAMESIZE];
volatile int16_t pr_buffer[FRAMESIZE];		//processed sound buffer

//PLAYBACK SYNTHESIS
volatile int16_t sfr1[FRAMESIZE/2];			//2 buffers for incoming vocoder stream
volatile int16_t sfr2[FRAMESIZE/2];
volatile uint16_t pb1[FRAMESIZE];			//playback buffer 1
volatile uint16_t pb2[FRAMESIZE];			//playback buffer 2
volatile uint8_t pb_sel=0;					//which one is currently in use
volatile uint8_t dac_busy=0;				//are we currently playing audio?

//VOCODER OUTPUT
volatile uint8_t bits[RAW_BYTES];			//raw bits from vocoder and later also encrypted bits

//BITSTREAM PROCESSING
volatile uint8_t c_bits[ENC_LEN];			//ECC output
//volatile uint8_t i_bits[ENC_LEN];			//interleaved bits, ready to send
volatile uint8_t f_bits[PLOAD_LEN];			//ready to send bitstream

//VOCODER STRUCT
volatile struct CODEC2 *cod;

//ENCRYPTION
uint8_t key[32];							//key
uint8_t iv[16];								//Initial Value

//CODEPLUG AUTHENTICATION TODO: fill this with real keys
#include "codeplug_auth.h"

//CRC & ECC RELATED
volatile uint16_t t_crc=0;					//tail CRC-16
uint8_t crc_tab16_init=0;
uint16_t crc_tab16[256];
#include "ecc_table.h"						//ECC precomputed table

//CONSTANTS
uint8_t EIN[12];							//Equipment Identity Number

//Si 446x RELATED
const uint8_t config[] = RADIO_CONFIGURATION_DATA_ARRAY;
volatile uint8_t ints[8];					//interrupts

//FIELDS - GENERAL STRUCT
struct FIELDS {
	volatile uint8_t	content_type;		//CONTENT_TYPE		2bit
	volatile uint8_t	enc_type;			//ENC_TYPE			2bit
	volatile uint16_t	frame;				//FRAME_NUMBER		12bit
	volatile uint8_t	key_id;				//KEY_ID			8bit
	volatile uint32_t	sender_id;			//SENDER_ID			24bit
	volatile uint32_t	recipient_id;		//RECIPIENT_ID		24bit
	volatile uint16_t	crc;				//CRC				16bit
};

//USER FIELDS - RADIO CONFIG
struct FIELDS self={CONTENT_VOICE, ENC_NONE, 0, KEY_NONE, 0, 0, 0};

//RCV FIELDS - EXTRACTED FROM RECEIVED DATA
struct FIELDS rcv={0, 0, 0, 0, 0, 0, 0};
volatile uint8_t rcv_buff[PLOAD_LEN];
volatile uint8_t rcv_voice[RAW_BYTES];

//CODEPLUG STRUCTS
struct CHANNEL_DATA {
	volatile uint8_t		name[MAX_NAME_LEN];		//channel name
	volatile uint8_t		descr[MAX_NAME_LEN];	//channel description
	volatile uint32_t		freq_rx;				//RX frequency in Hz
	volatile uint32_t		freq_tx;				//TX frequency in Hz
	volatile uint8_t		enc_type;				//encryption type
	volatile uint8_t		gps;					//gps data transmission type
	volatile uint16_t		tg_id;					//talkgroup ID
};

struct BANK_DATA {
	volatile uint8_t name[MAX_NAME_LEN];
	volatile uint8_t num_channels;
	struct CHANNEL_DATA channel[MAX_CHANNELS_PER_BANK];
};

struct CODEPLUG_DATA {
	volatile uint8_t		author[MAX_NAME_LEN];
	volatile uint8_t		version[20];
	volatile uint8_t		name[MAX_NAME_LEN];
	volatile uint16_t		num_banks;
	struct BANK_DATA		bank[MAX_BANK_NUM];
};

struct CODEPLUG_DATA codeplug;

//CODEPLUG FILE
uint8_t file_line[500][100];

//ACTIVE CHANNEL DATA
struct ACTIVE_CHANNEL_DATA {
	volatile uint8_t bank_num;
	volatile uint8_t ch_num;
	struct CHANNEL_DATA channel;
};

struct ACTIVE_CHANNEL_DATA active_channel={0, 0, "NONE", "-", 435000000, 435000000, ENC_NONE, GPS_NONE, TALKGROUP_NONE};

//RADIO STATE
volatile uint8_t r_initd=0;					//initialized?
volatile uint8_t r_tx=0;					//TX state?
volatile uint8_t rx_reload_pend=0;			//go back to RX state after PTT release?
volatile uint8_t mic_gain=10;				//microphone gain (linear) 0 -> mute
volatile float spk_volume=1.0;				//speaker volume (linear) value >=0.0
volatile float tones_volume=0.5;			//alert tones volume
volatile uint8_t tx_pwr=MAX_TX_POWER;		//TX power (arbitrary units)

//ALERT TONES PLAYBACK
volatile int16_t tone[TONE_SIZE];			//buffer for loading data from .RAW file

//TFT
uint8_t addr_col=0;							//current memory position
uint8_t addr_row=0;
uint8_t text_line[30];

//FONTS
struct font_1_glyph_dsc
{
	uint8_t w_px;			//width, px
	uint16_t glyph_index;
};

#include "font_1.h"

//BATTERY
uint16_t u_batt=0;			//raw adc value
volatile float v_batt=0.0;	//calculated value (volts)

//TEST
//

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_ADC1_Init(void);
static void MX_DAC_Init(void);
static void MX_SPI1_Init(void);
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

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);
                                

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */
//--------------------------------CRYPTO-------------------------------
void generateNonce(uint8_t* dest)
{
	uint32_t val[4]={0, 0, 0, 0};

	for(uint8_t i=0; i<4; i++)
	{
		while(HAL_RNG_GenerateRandomNumber(&hrng, &val[i]) != HAL_OK);

		dest[0+4*i]=val[i]&0xFF;
		dest[1+4*i]=(val[i]>>8)&0xFF;
		dest[2+4*i]=(val[i]>>16)&0xFF;
		dest[3+4*i]=(val[i]>>24)&0xFF;
	}
}

void retrieveEIN(uint8_t* dest)
{
	memcpy(dest, (uint8_t*)(0x1FF0F420), 12);
}

uint8_t authenticateCodeplug(uint8_t* auth_file_path)
{
	uint8_t buff_auth[16];

	if(f_open(&myFile, auth_file_path, FA_OPEN_EXISTING | FA_READ) == FR_OK)
	{
		if(f_read(&myFile, buff_auth, 16, NULL) == FR_OK)
			f_close(&myFile);
	}

	//TODO: put AES here

	//EIN match check
	for(uint8_t i=0; i<12; i++)
		if(EIN[i]!=buff_auth[i])
			return CODEPLUG_AUTH_FAIL;

	return CODEPLUG_AUTH_SUCCESS;
}

//------------------------------ECC & CRC------------------------------
static void init_crc16_tab(void)
{
	uint16_t crc;
	uint16_t c;

	for(uint16_t i=0; i<256; i++)
	{
		crc = 0;
		c   = i;

		for(uint8_t j=0; j<8; j++)
		{
			if( (crc ^ c) & 0x0001 ) crc = ( crc >> 1 ) ^ 0xA001;
			else                      crc =   crc >> 1;

			c = c >> 1;
		}

		crc_tab16[i] = crc;
	}

	crc_tab16_init = 1;
}

uint16_t crc_16(const uint8_t *input_str, uint8_t num_bytes)
{
	uint16_t crc=0x0000;
	const uint8_t *ptr;

	if (!crc_tab16_init)
		init_crc16_tab();

	ptr = input_str;

	if (ptr != NULL)
	{
		for (uint8_t a=0; a<num_bytes; a++)
		{
			crc = (crc >> 8) ^ crc_tab16[ (crc ^ (uint16_t) *ptr++) & 0x00FF ];
		}
	}

	return crc;
}

void encodeBits(const uint8_t *inp, uint8_t *outp)
{
	for(uint8_t i=0; i<RAW_BYTES*2; i+=2)
	{
		outp[i]=inp[i/2];
		outp[i+1]=ecc_16_8_table[inp[i/2]];
	}
}

void packData(uint8_t* dest, uint8_t* src, uint8_t len)
{
    memcpy(dest, src, 4);
    dest[1]|=(src[4]<<2)&0b11000000;
    dest[2]|=(src[4]<<4)&0b11000000;
    dest[3]|=(src[4]<<6)&0b11000000;

    uint8_t d=4;

    for(uint8_t i=5; i<len; i+=4, d+=3)
    {
        memcpy(&dest[d], &src[i], 3);
        dest[d+0]|=(src[i+3]<<2)&0b11000000;
        dest[d+1]|=(src[i+3]<<4)&0b11000000;
        dest[d+2]|=(src[i+3]<<6)&0b11000000;
    }
}

void formFrame(uint8_t encr_type)
{
	//if(encr_type==ENC_NONE)
	{
		memset(f_bits, 0, PLOAD_LEN);

		f_bits[0]|=self.content_type<<6;
		f_bits[0]|=self.enc_type<<4;
		f_bits[0]|=(self.frame>>8);

		f_bits[1]=self.frame&0xFF;

		//17 null bytes

		f_bits[19]=self.sender_id>>16;
		f_bits[20]=self.sender_id>>8;
		f_bits[21]=self.sender_id&0xFF;

		f_bits[22]=self.recipient_id>>16;
		f_bits[23]=self.recipient_id>>8;
		f_bits[24]=self.recipient_id&0xFF;

		//10 reserved bytes

		memcpy(&f_bits[35], bits, RAW_BYTES);

		t_crc=crc_16(f_bits, 51);
		f_bits[51]=t_crc>>8;
		f_bits[52]=t_crc&0xFF;

		//Error Correcting Coding follows
		//TODO
	}
	//else if(encr_type==ENC_STATIC)
	{
		//TODO: AES goes here
	}
}

void extractFields(uint8_t* stream)
{
	rcv.content_type=(stream[0]>>6)&3;
	rcv.enc_type=(stream[0]>>4)&3;
	rcv.frame=((stream[0]&0x0F)<<8) | stream[1];

	rcv.sender_id=(stream[19]<<16) | (stream[20]<<8) | stream[21];
	rcv.recipient_id=(stream[22]<<16) | (stream[23]<<8) | stream[24];

	memcpy(rcv_voice, &stream[35], RAW_BYTES);
}

//------------------------------SOUND PROCESSING------------------------------
void processBuff(uint8_t num)
{
	if(num)
	{
		for(uint16_t i=0; i<FRAMESIZE; i++)
			pr_buffer[i]=((int16_t)buffer_2[i]-DC_OFFSET)*mic_gain;
	}
	else
	{
		for(uint16_t i=0; i<FRAMESIZE; i++)
			pr_buffer[i]=((int16_t)buffer_1[i]-DC_OFFSET)*mic_gain;
	}
}

//------------------------------SPI - Si4463------------------------------
/*void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi)
{
	HAL_GPIO_WritePin(SPI2_CS_GPIO_Port, SPI2_CS_Pin, 1);
}*/

/*void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef *hspi)
{
	//HAL_GPIO_WritePin(SPI2_CS_GPIO_Port, SPI2_CS_Pin, 1);
}*/

void SPI_WaitForCTS(void)
{
	uint8_t cts[1]={0x00};
	const uint8_t dta[1]={0x44};

	do
	{
		HAL_GPIO_WritePin(SPI2_CS_GPIO_Port, SPI2_CS_Pin, 0);
		HAL_SPI_Transmit(&hspi2, dta, 1, 100);
		HAL_SPI_Receive(&hspi2, cts, 1, 100);
		//if(cts[0]!=0xFF)
			HAL_GPIO_WritePin(SPI2_CS_GPIO_Port, SPI2_CS_Pin, 1);
	}while(cts[0]!=0xFF);
}

void SPI_Send(uint8_t *data, uint8_t len)
{
	SPI_WaitForCTS();
	HAL_GPIO_WritePin(SPI2_CS_GPIO_Port, SPI2_CS_Pin, 0);
	HAL_SPI_Transmit(&hspi2, data, len, 100);
	HAL_GPIO_WritePin(SPI2_CS_GPIO_Port, SPI2_CS_Pin, 1);
}

void SPI_GetResponse(uint8_t len, uint8_t *data)
{
	uint8_t cts[1]={0x00};
	uint8_t dta[1]={0x44};

	while(cts[0]!=0xFF)
	{
		HAL_GPIO_WritePin(SPI2_CS_GPIO_Port, SPI2_CS_Pin, 0);
		HAL_SPI_Transmit(&hspi2, dta, 1, 100);
		HAL_SPI_Receive(&hspi2, cts, 1, 1);

		if(cts[0]!=0xFF)
			HAL_GPIO_WritePin(SPI2_CS_GPIO_Port, SPI2_CS_Pin, 1);
	}

	HAL_SPI_Receive(&hspi2, data, len, 100);

	HAL_GPIO_WritePin(SPI2_CS_GPIO_Port, SPI2_CS_Pin, 1);
}

void SPI_ReadRxDataBuff(uint8_t len, uint8_t *data)
{
	uint8_t cmd[1]={0x77};

	HAL_GPIO_WritePin(SPI2_CS_GPIO_Port, SPI2_CS_Pin, 0);
	HAL_SPI_Transmit(&hspi2, cmd, 1, 100);
	HAL_SPI_Receive(&hspi2, data, len, 100);
	HAL_GPIO_WritePin(SPI2_CS_GPIO_Port, SPI2_CS_Pin, 1);
}

//------------------------------Si446x FUNCS------------------------------
void Si_Reset(void)
{
	HAL_GPIO_WritePin(TRX_SDN_GPIO_Port, TRX_SDN_Pin, 1);
	HAL_Delay(1);
	HAL_GPIO_WritePin(TRX_SDN_GPIO_Port, TRX_SDN_Pin, 0);
	HAL_Delay(5);
}

static void Si_SetProp(uint8_t* vals, uint8_t group, uint8_t number, uint8_t len)
{
	uint8_t data[16]={0x11,	group, len, number};

	memcpy(data + 4, vals, len);

	SPI_WaitForCTS();
	SPI_Send(data, len + 4);
}

static void Si_StartupConfig(void)
{
	uint8_t buff[17];

	for(uint16_t i=0; i<sizeof(config); i++)
	{
		memcpy(buff, &config[i], sizeof(buff));
		SPI_WaitForCTS();
		SPI_Send(&buff[1], buff[0]);
		i += buff[0];
	}
}

static void Si_Interrupts(void* buff)
{
	uint8_t data = 0x20;

	SPI_Send(&data, 1);
	if(buff!=NULL)
		SPI_GetResponse(8, buff);
}

/*static void Si_InterruptsNoCTS(void* buff)
{
	uint8_t v = 0x20;

	HAL_GPIO_WritePin(SPI2_CS_GPIO_Port, SPI2_CS_Pin, 0);
	HAL_SPI_Transmit(&hspi2, &v, 1, 100);
	HAL_SPI_Receive(&hspi2, buff, 8, 100);
	HAL_GPIO_WritePin(SPI2_CS_GPIO_Port, SPI2_CS_Pin, 1);
}*/

static void Si_Interrupts2(void* buff, uint8_t clearPH, uint8_t clearMODEM, uint8_t clearCHIP)
{
	uint8_t data[] = {
		0x20,
		clearPH,
		clearMODEM,
		clearCHIP
	};

	SPI_Send(data, 4);
	if(buff!=NULL)
		SPI_GetResponse(8, buff);
}

static uint8_t Si_GetState(void)
{
	uint8_t state[2]={0, 0};
	uint8_t get_state_cmd=0x33;

	SPI_WaitForCTS();
	SPI_Send(&get_state_cmd, 1);
	SPI_GetResponse(2, state);

	return state[0]&0x0F;
}

static void Si_SetState(uint8_t newState)
{
	uint8_t data[2]={0x34, newState};

	SPI_Send(data, 2);
}

void Si_Sleep(void)
{
	uint8_t state=Si_GetState();

	if(state==7 || state==5)
		return;

	Si_SetState(1);
}

void Si_SetTxPower(uint8_t pwr)
{
	/*uint8_t msg[5]={0x11, 0x22, 0x01, 0x01, pwr};

	SPI_Send(&msg, 5);*/
	Si_SetProp(&pwr, 0x22, 0x01, 1);
}

void Si_ClearFIFO(uint8_t fifos)
{
	uint8_t msg[3]={0x15, fifos};

	SPI_Send(&msg, 2);
}

void Si_WriteTxDataBuff(uint8_t *data, uint8_t len)
{
	uint8_t cmd = 0x66;

	HAL_GPIO_WritePin(SPI2_CS_GPIO_Port, SPI2_CS_Pin, 0);
	HAL_SPI_Transmit(&hspi2, &cmd, 1, 100);
	HAL_SPI_Transmit(&hspi2, data, len, 100);
	HAL_GPIO_WritePin(SPI2_CS_GPIO_Port, SPI2_CS_Pin, 1);
}

void Si_StartTx(uint8_t ch, uint8_t end_state, uint8_t tx_len)
{
	uint8_t msg[7]={0x31, ch, (uint8_t)end_state<<4, 0, tx_len, 0, 0};

	SPI_WaitForCTS();
	SPI_Send(&msg, 7);
}

void Si_StartRx(uint8_t ch, uint8_t len)
{
	//Si_SetState(3);		//RX state
	Si_ClearFIFO(3);	//clear both FIFOs
	//Si_Interrupts2(NULL, 0, 0, 0xFF);	//clear some interrupts

	uint8_t msg[8]={0x32, ch, 0, 0, len, 0x08, 0x08, 0x08};

	SPI_Send(msg, 8);
}

void Si_TxData(uint8_t *d, uint8_t len, uint8_t ch)
{
	Si_ClearFIFO(3);
	Si_Interrupts2(NULL, 0, 0, 0xFF);

	Si_WriteTxDataBuff(d, len);
	Si_StartTx(ch, 3, len);	//3 - READY state after TX complete
}

void Si_FreqSet(uint32_t freq)	//freq in Hz
{
	uint8_t inte=0x3C;	//default values
	uint64_t frac=0x00080000;

	freq=freq*(30.0/32.0);
	inte=freq/7500000-1;
	frac=(freq-(uint32_t)(inte+1)*7500000)/75;
	frac=(uint64_t)frac*(1<<19)/100000+(uint32_t)(1<<19);

	uint8_t vals[4]={inte, (frac>>16)&0xFF, (frac>>8)&0xFF, frac&0xFF};

	Si_SetProp(&vals, 0x40, 0x00, 4);
}

uint8_t Si_GetRSSI(void)
{
	uint8_t v=0x50;
	uint8_t r[4];

	HAL_GPIO_WritePin(SPI2_CS_GPIO_Port, SPI2_CS_Pin, 0);
	HAL_SPI_Transmit(&hspi2, &v, 1, 100);
	HAL_SPI_Receive(&hspi2, r, 4, 100);
	HAL_GPIO_WritePin(SPI2_CS_GPIO_Port, SPI2_CS_Pin, 1);

	return r[0];
}

void Si_GetInfo(uint8_t *resp)
{
	uint8_t cmd=0x01;

	SPI_Send(&cmd, 1);
	SPI_WaitForCTS();
	SPI_GetResponse(8, resp);
}

uint8_t checkTRX(void)
{
	uint8_t info[8];

	Si_GetInfo(info);

	if(info[1]==0x44 && info[2]==0x63)
		return 0;

	return 1;
}

//------------------------------RF SWITCH FUNCS------------------------------
static void RF_SetTX(void)
{
	if(HW_VER<=11)		//1.1 and older HW versions had CG2179M2 RF switch
	{
		HAL_GPIO_WritePin(TRX_RX_SW_GPIO_Port, TRX_RX_SW_Pin, 1);
		HAL_GPIO_WritePin(TRX_TX_SW_GPIO_Port, TRX_TX_SW_Pin, 0);
	}
	else if(HW_VER==12)	//upgraded to HMC546MS8GETR
	{
		HAL_GPIO_WritePin(TRX_RX_SW_GPIO_Port, TRX_RX_SW_Pin, 1);
	}
	r_tx=1;
}

static void RF_SetRX(void)
{
	if(HW_VER<=11)
	{
		HAL_GPIO_WritePin(TRX_RX_SW_GPIO_Port, TRX_RX_SW_Pin, 0);
		HAL_GPIO_WritePin(TRX_TX_SW_GPIO_Port, TRX_TX_SW_Pin, 1);
	}
	else if(HW_VER==12)
	{
		HAL_GPIO_WritePin(TRX_RX_SW_GPIO_Port, TRX_RX_SW_Pin, 0);
	}
	r_tx=0;
}

//-------------------------------------MISC-------------------------------------
uint32_t getUs(void)
{
	uint32_t usTicks = HAL_RCC_GetSysClockFreq() / 1000000;

	register uint32_t ms, cycle_cnt;

	do
	{
		ms = HAL_GetTick();
		cycle_cnt = SysTick->VAL;
	} while (ms != HAL_GetTick());

	return (ms * 1000) + (usTicks * 1000 - cycle_cnt) / usTicks;
}

void _delay_us(uint16_t micros)
{
	uint32_t start = getUs();

	while (getUs()-start < (uint32_t) micros)
		__asm__("NOP");
}

//------------------------------TONES------------------------------
void playTone(const uint8_t* sound_file_path)
{
	if(f_open(&myFile, sound_file_path, FA_OPEN_EXISTING | FA_READ) == FR_OK)
	{
		if(f_read(&myFile, (int16_t*)tone, TONE_SIZE*2, NULL) == FR_OK)
			f_close(&myFile);
	}

	for(uint16_t i=0; i<TONE_SIZE; i++)
	{
		tone[i]*=tones_volume;
		pb1[i]=(tone[i]-32768)>>4;
	}

	if(!dac_busy)
	{
		dac_busy=1;
		HAL_DAC_Start_DMA(&hdac, DAC_CHANNEL_1, pb1, TONE_SIZE, DAC_ALIGN_12B_R);
	}
}

//-------------------------------TFT-------------------------------
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
	HAL_GPIO_WritePin(TFT_A0_GPIO_Port, TFT_A0_Pin, 0);

	//CS low
	HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_CS_Pin, 0);

	//send byte
	HAL_SPI_Transmit(&hspi1, &cmd, 1, 100);

	//CS high
	HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_CS_Pin, 1);
}

void TFT_WriteData(uint8_t cmd)
{
	//A0 high
	HAL_GPIO_WritePin(TFT_A0_GPIO_Port, TFT_A0_Pin, 1);

	//CS low
	HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_CS_Pin, 0);

	//send byte
	HAL_SPI_Transmit(&hspi1, &cmd, 1, 100);

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
		TFT_WriteCommand(list[addr++]);		// Read, issue command
		numArgs = list[addr++];					// Number of args to follow
		ms = numArgs & INIT_DELAY;				// If 7th bit set, delay follows args
		numArgs &= ~INIT_DELAY;					// Mask out delay bit
		while(numArgs--)						// For each argument...
		{
			TFT_WriteData(list[addr++]); // Read, issue argument
		}

		if (ms)
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
	      0xC0,                   //     row addr/col addr, bottom to top refresh
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

void TFT_SetBrght(uint8_t brght)
{
	HAL_TIM_Base_Start(&htim4);
	HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_1);

	if(brght<255)
		htim4.Instance->CCR1=(uint16_t)(brght*999.0/255.0);
	else
		htim4.Instance->CCR1=(uint16_t)999;
}

uint16_t TFT_RGBtoCol(uint8_t r, uint8_t g, uint8_t b)
{
	r>>=3;	g>>=2;	b>>=3;

	return (uint16_t)(r<<11 | g<<6 | b);
}

void TFT_PutPixel(uint8_t x, uint8_t y, uint16_t color)
{
	if((x>=128) || (y>=160))
		return;

	GPIOC->BSRR=(uint32_t)(1<<(4+16)); //TFT_CS LOW

	if(addr_col != x)
	{
		GPIOC->BSRR=(uint32_t)(1<<(6+16)); //TFT_A0 LOW

		//while(!(SPI1->SR & SPI_SR_TXE));
		*(uint8_t *)&SPI1->DR=0x2A;
		while(SPI1->SR & SPI_SR_BSY);

		addr_col = x;
		GPIOC->BSRR=(uint32_t)(1<<6); //TFT_A0 HIGH

		//while(!(SPI1->SR & SPI_SR_TXE));
		*(uint8_t *)&SPI1->DR=0x00;
		while(SPI1->SR & SPI_SR_BSY);
		//while(!(SPI1->SR & SPI_SR_TXE));
		*(uint8_t *)&SPI1->DR=x;
		while(SPI1->SR & SPI_SR_BSY);
	}

	if(addr_row != y)
	{
		GPIOC->BSRR=(uint32_t)(1<<(6+16)); //TFT_A0 LOW

		//while(!(SPI1->SR & SPI_SR_TXE));
		*(uint8_t *)&SPI1->DR=0x2B;
		while(SPI1->SR & SPI_SR_BSY);

		addr_row = y;
		GPIOC->BSRR=(uint32_t)(1<<6); //TFT_A0 HIGH

		//while(!(SPI1->SR & SPI_SR_TXE));
		*(uint8_t *)&SPI1->DR=0x00;
		while(SPI1->SR & SPI_SR_BSY);
		//while(!(SPI1->SR & SPI_SR_TXE));
		*(uint8_t *)&SPI1->DR=y;
		while(SPI1->SR & SPI_SR_BSY);
	}

	GPIOC->BSRR=(uint32_t)(1<<(6+16)); //TFT_A0 LOW

	//while(!(SPI1->SR & SPI_SR_TXE));
	*(uint8_t *)&SPI1->DR=0x2C;
	while(SPI1->SR & SPI_SR_BSY);

	GPIOC->BSRR=(uint32_t)(1<<6); //TFT_A0 HIGH

	//while(!(SPI1->SR & SPI_SR_TXE));
	*(uint8_t *)&SPI1->DR=color>>8;
	while(SPI1->SR & SPI_SR_BSY);
	//while(!(SPI1->SR & SPI_SR_TXE));
	*(uint8_t *)&SPI1->DR=color;
	while(SPI1->SR & SPI_SR_BSY);

	GPIOC->BSRR=(uint32_t)(1<<4);	//TFT_CS HIGH
}

void TFT_Clear(uint16_t color)
{
	for(uint8_t x=0; x<128; x++)
		for(uint8_t y=0; y<160;y++)
			TFT_PutPixel(x, y, color);
}

void TFT_RectangleFilled(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint16_t color)
{
	if(x0>x1)
	{
		uint8_t t=x0;
		x0=x1;
		x1=x0;
	}
	if(y0>y1)
	{
		uint8_t t=y0;
		y0=y1;
		y1=y0;
	}

	if(y1-y0>=x1-x0)
	{
		for(uint8_t x=x0; x<=x1; x++)
			for(uint8_t y=y0; y<=y1;y++)
				TFT_PutPixel(x, y, color);
	}
	else
	{
		for(uint8_t y=y0; y<=y1; y++)
			for(uint8_t x=x0; x<=x1;x++)
				TFT_PutPixel(x, y, color);
	}
}

void TFT_DisplaySplash(uint8_t* img_path)
{
	uint8_t raw_image[128*160*3];
	uint16_t index=0;
	uint16_t pix;

	if(f_open(&myFile, img_path, FA_OPEN_EXISTING | FA_READ) == FR_OK)
	{
		if(f_read(&myFile, raw_image, 128*160*3, NULL) == FR_OK)
			f_close(&myFile);
	}

	for(uint8_t x=0; x<128; x++)
	{
		for(uint8_t y=0; y<160; y++)
		{
			pix=TFT_RGBtoCol(raw_image[(y*128+x)*3], raw_image[(y*128+x)*3+1], raw_image[(y*128+x)*3+2]);
			TFT_PutPixel(x, y, pix);
		}
	}
}

void TFT_PutStr(uint8_t x, uint8_t y, const uint8_t* str, uint8_t font, uint16_t color)
{
	if(font==1)//monospaced, 16px height, 9px width
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

void displayEIN(uint8_t* src)
{
	TFT_Clear(CL_WHITE);

	TFT_PutStrCenteredBold(30, "EIN", 1, 0);
	for(uint8_t i=0; i<3; i++)
	{
		sprintf(text_line, "%02X%02X%02X%02X", EIN[i*4], EIN[i*4+1], EIN[i*4+2], EIN[i*4+3]);
		TFT_PutStrCentered(30+22+i*16, text_line, 1, 0);
	}
}

void displayVersion(void)
{
	TFT_Clear(CL_WHITE);

	TFT_PutStrCenteredBold(30, "HW_VER", 1, 0);
	sprintf(text_line, "%d", HW_VER);
	TFT_PutStrCentered(30+16, text_line, 1, 0);
	TFT_PutStrCenteredBold(30+16*2, "FW_VER", 1, 0);
	sprintf(text_line, "%d", FW_VER);
	TFT_PutStrCentered(30+16*3, text_line, 1, 0);
}

void displayTRXInfo(void)
{
	uint8_t info[8];

	Si_GetInfo(info);

	TFT_Clear(CL_WHITE);

	TFT_PutStrCenteredBold(10, "TRX", 1, 0);

	sprintf(text_line, "PART  %02X%02X", info[1], info[2]);
	TFT_PutStrCentered(10+22+16, text_line, 1, 0);
	sprintf(text_line, "REV  %02X", info[0]);
	TFT_PutStrCentered(10+22+16*2, text_line, 1, 0);
	sprintf(text_line, "PBUILD  %02X", info[3]);
	TFT_PutStrCentered(10+22+16*3, text_line, 1, 0);
	sprintf(text_line, "ID  %02X%02X", info[5], info[6]);
	TFT_PutStrCentered(10+22+16*4, text_line, 1, 0);
	sprintf(text_line, "CUST  %02X", info[6]);
	TFT_PutStrCentered(10+22+16*5, text_line, 1, 0);
	sprintf(text_line, "ROMID  %02X", info[7]);
	TFT_PutStrCentered(10+22+16*6, text_line, 1, 0);
}

void TFT_DisplayActiveChannelData(struct CODEPLUG_DATA *cd, struct ACTIVE_CHANNEL_DATA *acd)
{
	uint8_t lin[20];
	uint8_t base=47;

	TFT_PutStrCenteredBold(base, cd->bank[acd->bank_num].name, 1, 0);
	TFT_PutStrCentered(base+18, acd->channel.name, 1, 0);
	gcvt(acd->channel.freq_tx/1000000.0, 7, lin);
	TFT_PutStrCentered(base+18*2, lin, 1, 0);
	if(acd->channel.enc_type)
		TFT_PutStrCenteredBold(base+18*3, "ENCR", 1, CL_DARKGREEN);
	else
		TFT_PutStrCenteredBold(base+18*3, "NO   ENCR", 1, CL_RED);
}

//-----------------------------------CODEPLUG------------------------------------
uint32_t extractValue(uint8_t *line)
{
	for(uint16_t i=0; i<255; i++)
	{
		if(line[i]=='=')
			return atoi(&line[i+1]);
	}

	return 0;
}

void extractString(uint8_t *line, uint8_t *dest)
{
	for(uint16_t i=0; i<255; i++)
	{
		if(line[i]=='"')
		{
			uint8_t cnt=0;

			while(line[i+1+cnt]!='"')
			{
				dest[cnt]=line[i+1+cnt];
				cnt++;
			}
			break;
		}
	}
}

uint8_t extractType(uint8_t *line)
{
	//comment
	if(line[0]=='#')
		return EMPTY_OR_COMMENT;
	else if(strstr(line, K_CODEPLUG) != NULL)
		return CODEPLUG;
	else if(strstr(line, K_AUTHOR) != NULL)
		return AUTHOR;
	else if(strstr(line, K_VERSION) != NULL)
		return VERSION;
	else if(strstr(line, K_NUM_BANKS) != NULL)
		return NUM_BANKS;
	else if(strstr(line, K_NUM_CHANNELS) != NULL)
		return NUM_CHANNELS;
	else if(strstr(line, K_NUM) != NULL)
		return NUM;
	else if(strstr(line, K_FREQ_RX) != NULL)
		return FREQ_RX;
	else if(strstr(line, K_FREQ_TX) != NULL)
		return FREQ_TX;
	else if(strstr(line, K_ENCR) != NULL)
		return ENCR;
	else if(strstr(line, K_GPS) != NULL)
		return GPS;
	else if(strstr(line, K_DESCR) != NULL)
		return DESCR;
	else if(strstr(line, K_NAME) != NULL)
		return NAME;

	else if(strstr(line, K_BANK) != NULL)
		return BANK;
	else if(strstr(line, K_CHANNEL) != NULL)
		return CHANNEL;
	else if(strstr(line, K_END) != NULL)
		return END;

	return EMPTY_OR_COMMENT;
}

uint8_t LoadCodeplug(uint8_t* codeplug_file_path, struct CODEPLUG_DATA *cp)
{
	uint8_t in_codeplug=0, in_bank=0, in_channel=0;		//where are we?
	uint8_t bank_num=0, channel_num=0;					//

	uint16_t lines=0;
	uint8_t chars=0;

	if(f_open(&myFile, codeplug_file_path, FA_OPEN_EXISTING | FA_READ) == FR_OK)
	{
		while(f_read(&myFile, &file_line[lines][chars++], 1, NULL) == FR_OK)
		{
			if(file_line[lines][chars-1]==0) //EOF?
				break;
			if(chars>1 && file_line[lines][chars-2]==13 && file_line[lines][chars-1]==10)
			{
				chars=0;
				lines++;
			}
		}
	}
	else
		return 1;

	f_close(&myFile);

	for(uint16_t line=0; line<lines; line++)
	{
		if(extractType(file_line[line])==EMPTY_OR_COMMENT)
			continue;

		switch(extractType(file_line[line]))
		{
			case CODEPLUG:
				in_codeplug=1;
			break;

			case BANK:
				in_bank=1;
				bank_num++;
			break;

			case CHANNEL:
				in_channel=1;
				channel_num++;
			break;

			case END:
				if(in_channel)
					in_channel=0;
				else if(in_bank)
					{in_bank=0; channel_num=0;}
				else if(in_codeplug)
					in_codeplug=0;
			break;

			case AUTHOR:
				extractString(file_line[line], cp->author);
			break;

			case VERSION:
				extractString(file_line[line], cp->version);
			break;

			case NUM_BANKS:
				cp->num_banks=extractValue(file_line[line]);
			break;

			case NUM_CHANNELS:
				cp->bank[bank_num-1].num_channels=extractValue(file_line[line]);
			break;

			case NAME:
				if(in_bank && !in_channel)
					extractString(file_line[line], cp->bank[bank_num-1].name);
				else if(in_bank && in_channel)
					extractString(file_line[line], cp->bank[bank_num-1].channel[channel_num-1].name);
			break;

			case DESCR:
				if(in_channel)
					extractString(file_line[line], cp->bank[bank_num-1].channel[channel_num-1].descr);
			break;

			case FREQ_RX:
				if(in_channel)
					cp->bank[bank_num-1].channel[channel_num-1].freq_rx=extractValue(file_line[line]);
			break;

			case FREQ_TX:
				if(in_channel)
					cp->bank[bank_num-1].channel[channel_num-1].freq_tx=extractValue(file_line[line]);
			break;

			case ENCR:
				if(in_channel)
					cp->bank[bank_num-1].channel[channel_num-1].enc_type=extractValue(file_line[line]);
			break;

			case GPS:
				if(in_channel)
					cp->bank[bank_num-1].channel[channel_num-1].gps=extractValue(file_line[line]);
			break;

			default:
				;
			break;
		}
	}

	return 0;	//OK!
}

void ClearActiveChannel(struct ACTIVE_CHANNEL_DATA *acd)
{
	for(uint8_t i=0; i<MAX_NAME_LEN; i++)
		acd->channel.name[i]=acd->channel.descr[i]=0;
}

uint8_t SetActiveChannel(struct CODEPLUG_DATA *cp, uint8_t b, uint8_t ch, struct ACTIVE_CHANNEL_DATA *acd)
{
	if(ch<cp->bank[b].num_channels)
	{
		ClearActiveChannel(acd);
		acd->bank_num=b;
		acd->ch_num=ch;
		memcpy(acd->channel.name, cp->bank[b].channel[ch].name, strlen(cp->bank[b].channel[ch].name));
		memcpy(acd->channel.descr, cp->bank[b].channel[ch].descr, strlen(cp->bank[b].channel[ch].descr));
		acd->channel.freq_rx=cp->bank[b].channel[ch].freq_rx;
		acd->channel.freq_tx=cp->bank[b].channel[ch].freq_tx;
		acd->channel.enc_type=cp->bank[b].channel[ch].enc_type;
		acd->channel.gps=cp->bank[b].channel[ch].gps;
		acd->channel.tg_id=cp->bank[b].channel[ch].tg_id;

		return 0;
	}

	return 1;	//channel number out of range
}

uint8_t GetActiveBank(struct ACTIVE_CHANNEL_DATA *acd)
{
	return acd->bank_num;
}

uint8_t GetActiveChannel(struct ACTIVE_CHANNEL_DATA *acd)
{
	return acd->ch_num;
}

//------------------------------INTERRUPT CALLBACKS------------------------------
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
	if(hadc->Instance==ADC1)
	{
		//swapping buffers
		if(buff_num)
		{
			//PTT still pressed? load data to the other buffer
			if(!HAL_GPIO_ReadPin(PTT_INT_GPIO_Port, PTT_INT_Pin))
				HAL_ADC_Start_DMA(&hadc1, buffer_1, FRAMESIZE);

			//remember which buffer to use next
			buff_num=0;

			//process actual buffer (remove DC offset, amplify and encode speech)
			processBuff(1);
			codec2_encode(cod, bits, pr_buffer);
			codec2_encode(cod, &bits[8], &pr_buffer[FRAMESIZE/2]);

			//form frame
			formFrame(self.enc_type);

			//transmit it already, for fucks sake!!
			Si_TxData(f_bits, PLOAD_LEN, 0);
		}
		else
		{
			//PTT still pressed? load data to the other buffer
			if(!HAL_GPIO_ReadPin(PTT_INT_GPIO_Port, PTT_INT_Pin))
				HAL_ADC_Start_DMA(&hadc1, buffer_2, FRAMESIZE);

			//remember which buffer to use next
			buff_num=1;

			//process actual buffer (remove DC offset, amplify and encode speech)
			processBuff(0);
			codec2_encode(cod, bits, pr_buffer);
			codec2_encode(cod, &bits[8], &pr_buffer[FRAMESIZE/2]);

			//form frame
			formFrame(self.enc_type);

			//transmit it already, for fucks sake!!
			Si_TxData(f_bits, PLOAD_LEN, 0);
		}

		//increment FRAME_NUMBER field value
		self.frame++;

		//ADC not busy?
		if(HAL_GPIO_ReadPin(PTT_INT_GPIO_Port, PTT_INT_Pin))
			adc_busy=0;
	}
	else
	{
		v_batt=(u_batt/4096.0)*3.3*10.058;
		gcvt(v_batt, 3, test_line);
		if(strlen(test_line)==3)
			test_line[3]='0';
		else if(strlen(test_line)==1)
		{
			test_line[1]='.';
			test_line[2]='0';
			test_line[3]='0';
		}
		TFT_RectangleFilled(127-30, 3, 127, 16, CL_WHITE);
		TFT_PutStrBold(127-30, 3, test_line, 1, CL_BLACK);
		HAL_ADC_Start_DMA(&hadc2, &u_batt, 1);
	}
}

void HAL_DAC_ConvCpltCallbackCh1(DAC_HandleTypeDef* hdac)
{
	dac_busy=0;
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	;//
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance==USART3)
	{
		//keyboard - test
		if(kbd_rcv_val=='0')
		{
			SetActiveChannel(&codeplug, 0, 0, &active_channel);
			Si_FreqSet(active_channel.channel.freq_rx);

			TFT_RectangleFilled(0, 30, 127, 120, CL_WHITE);
			TFT_DisplayActiveChannelData(&codeplug, &active_channel);
		}
		else if(kbd_rcv_val=='1')
		{
			SetActiveChannel(&codeplug, 0, 1, &active_channel);
			Si_FreqSet(active_channel.channel.freq_rx);

			TFT_RectangleFilled(0, 30, 127, 120, CL_WHITE);
			TFT_DisplayActiveChannelData(&codeplug, &active_channel);
		}
		else if(kbd_rcv_val=='2')
		{
			SetActiveChannel(&codeplug, 0, 2, &active_channel);
			Si_FreqSet(active_channel.channel.freq_rx);

			TFT_RectangleFilled(0, 30, 127, 120, CL_WHITE);
			TFT_DisplayActiveChannelData(&codeplug, &active_channel);
		}

		HAL_UART_Receive_IT(&huart3, &kbd_rcv_val, 1);
	}
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	//Si4463 TRX module interrupt
	if(GPIO_Pin==GPIO_PIN_1 && r_initd)
	{
		//debug
		//HAL_GPIO_WritePin(TP1_GPIO_Port, TP1_Pin, 1);

		//Si nIRQ interrupt request
		Si_Interrupts(ints);	//check pending interrupt flags

		//PACKET_RX_PEND flag set?
		if(ints[2]&(1<<4))
		{
			//retrieve RX buff contents
			SPI_ReadRxDataBuff(PLOAD_LEN, rcv_buff);
			Si_ClearFIFO(3);

			//debug - scanner mode
			//HAL_UART_Transmit_DMA(&huart2, rcv_buff, PLOAD_LEN);

			//extract data
			extractFields(rcv_buff);

			if(rcv.content_type==CONTENT_VOICE && rcv.enc_type==ENC_NONE)
			{
				TFT_PutStrCenteredBold(22, text_line, 1, CL_LIGHTGREY);
				sprintf(text_line, "ID: %d", rcv.sender_id);
				TFT_PutStrCenteredBold(22, text_line, 1, TFT_RGBtoCol(0, 100, 0));

				codec2_decode(cod, sfr1, rcv_voice);
				codec2_decode(cod, sfr2, &rcv_voice[8]);

				rcv.frame++;

				if(pb_sel)
				{
					pb_sel=0;

					for(uint16_t j=0; j<FRAMESIZE/2; j++)
					{
						sfr1[j]*=spk_volume;
						sfr2[j]*=spk_volume;
						pb2[j]=(sfr1[j]-32768)>>4;
						pb2[j+FRAMESIZE/2]=(sfr2[j]-32768)>>4;
					}

					while(dac_busy);
					dac_busy=1;
					HAL_DAC_Start_DMA(&hdac, DAC_CHANNEL_1, pb2, FRAMESIZE, DAC_ALIGN_12B_R);
				}
				else
				{
					pb_sel=1;

					for(uint16_t j=0; j<FRAMESIZE/2; j++)
					{
						sfr1[j]*=spk_volume;
						sfr2[j]*=spk_volume;
						pb1[j]=(sfr1[j]-32768)>>4;
						pb1[j+FRAMESIZE/2]=(sfr2[j]-32768)>>4;
					}

					while(dac_busy);
					dac_busy=1;
					HAL_DAC_Start_DMA(&hdac, DAC_CHANNEL_1, pb1, FRAMESIZE, DAC_ALIGN_12B_R);
				}
			}
		}

		//debug
		//HAL_GPIO_WritePin(TP1_GPIO_Port, TP1_Pin, 0);

		//clear pending flags
		Si_Interrupts(NULL);
	}

	//PTT push
	else if(GPIO_Pin==GPIO_PIN_0 && !HAL_GPIO_ReadPin(PTT_INT_GPIO_Port, PTT_INT_Pin))
	{
		HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, 0);

		if(!dac_busy)
		{
			Si_FreqSet(active_channel.channel.freq_tx);
			playTone("tones/ready.raw");
			buff_num=0;
			adc_busy=1;
			//TODO: add a small delay here for the sound to be played BEFORE we start collecting ADC data
			HAL_ADC_Start_DMA(&hadc1, buffer_1, FRAMESIZE);
			RF_SetTX();
		}
	}

	//PTT release
	else if(GPIO_Pin==GPIO_PIN_0 && HAL_GPIO_ReadPin(PTT_INT_GPIO_Port, PTT_INT_Pin))
	{
		HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, 1);

		rx_reload_pend=1;
		HAL_NVIC_DisableIRQ(EXTI0_IRQn);	//anti-interference pick up on the PTT line
	}
}
//-----------------------------ADXL345-----------------------------
uint8_t ADXL_CheckID(uint8_t dev_addr)
{
	uint8_t resp=0;
	HAL_I2C_Mem_Read(&hi2c1, dev_addr<<1, 0x0, 1, &resp, 1, 100);

	if(resp==0xE5)
		return 0;	//OK
	return 1;	//Fuck You
}

void ADXL_WriteReg(uint8_t dev_addr, uint8_t reg, uint8_t val)
{
	HAL_I2C_Mem_Write(&hi2c1, dev_addr<<1, reg, 1, &val, 1, 100);
}

uint8_t ADXL_ReadReg(uint8_t dev_addr, uint8_t reg)
{
	uint8_t val=0;

	HAL_I2C_Mem_Read(&hi2c1, dev_addr<<1, reg, 1, &val, 1, 100);

	return val;
}

//-----------------------------SPEAKER-----------------------------
#if HW_VERSION < 12
void SPK_Enabled(uint8_t ena)
{
	if(ena)
		HAL_GPIO_WritePin(SPK_SDN_GPIO_Port, SPK_SDN_Pin, 0);
	else
		HAL_GPIO_WritePin(SPK_SDN_GPIO_Port, SPK_SDN_Pin, 1);
}
#endif
//------------------------------DEBUG------------------------------
//
//
//
//

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  *
  * @retval None
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
  retrieveEIN(EIN);
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
  MX_SPI1_Init();
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
  /* USER CODE BEGIN 2 */
  //--------------------------TR-9 INIT-----------------------------
  //turn off LEDs
  HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, 1);
  HAL_GPIO_WritePin(LED_GRN_GPIO_Port, LED_GRN_Pin, 1);

  //dim display backlight & TFT init
  TFT_SetBrght(0);
  HAL_Delay(100);
  TFT_Init();

  //TRX module check
  Si_Reset();
  if(checkTRX())	//fucked up comms with Si4463?
  {
	  TFT_Clear(CL_BLACK);
	  TFT_PutStrCenteredBold(160/2-8, "TRX ERROR", 1, CL_RED);
	  TFT_SetBrght(255);

	  while(1) //HALT! error signalling
	  {
		  HAL_GPIO_TogglePin(LED_RED_GPIO_Port, LED_RED_Pin);
		  HAL_Delay(100);
	  }
  }
  Si_StartupConfig();
  Si_Interrupts(NULL);
  Si_SetTxPower(tx_pwr);
  Si_Sleep();

  //SD card check
  if(f_mount(&SDFatFS, (TCHAR const*)SDPath, 0))
  {
	  TFT_Clear(CL_BLACK);
	  TFT_PutStrCenteredBold(160/2-8, "CARD ERROR", 1, CL_RED);
	  TFT_SetBrght(255);

	  while(1)
	  {
		  HAL_GPIO_TogglePin(LED_RED_GPIO_Port, LED_RED_Pin);
		  HAL_Delay(250);
	  }
  }

  //authentication check
  if(authenticateCodeplug("AUTH")==CODEPLUG_AUTH_FAIL);	//TODO: fix this and add an error handler
  {
	  //while(1);
  }

  //codeplug check
  if(LoadCodeplug("CODEPLUG", &codeplug))
  {
	  TFT_Clear(CL_BLACK);
	  TFT_PutStrCenteredBold(160/2-8, "NO CODEPLUG", 1, CL_RED);
	  TFT_SetBrght(255);

	  while(1)
	  {
	  	HAL_GPIO_TogglePin(LED_RED_GPIO_Port, LED_RED_Pin);
	  	HAL_Delay(500);
	  }
  }

  //ADXL345 INIT
  if(!NO_ADXL)
  {
	  if(ADXL_CheckID(ADXL_ADDR))
	  {
		  TFT_Clear(CL_BLACK);
		  TFT_PutStrCenteredBold(160/2-8, "ADXL ERROR", 1, CL_RED);
		  TFT_SetBrght(255);

		  HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, 0);

		  while(1);
	  }

	  ADXL_WriteReg(ADXL_ADDR, 0x2D, 0x00);
	  ADXL_WriteReg(ADXL_ADDR, 0x31, 0x01);
	  ADXL_WriteReg(ADXL_ADDR, 0x2D, 0x0B);
  }

  //start timers and enable interrupts
  HAL_UART_Receive_IT(&huart3, &kbd_rcv_val, 1);
  HAL_TIM_Base_Start(&htim6);	//8kHz timebase
  HAL_ADC_Start_DMA(&hadc2, &u_batt, 1);
  HAL_TIM_Base_Start(&htim8);	//0.1Hz measure u_batt

  //create codec instance and fill the CRC table TODO: use HW CRC calculation unit
  cod = codec2_create(CODEC2_MODE_3200);
  init_crc16_tab();

  //set radio related stuff
  SetActiveChannel(&codeplug, 0, 0, &active_channel);
  Si_FreqSet(active_channel.channel.freq_rx);
  RF_SetRX();
  Si_StartRx(0, PLOAD_LEN);
  r_initd=1;	//we need this to avoid getting Si IRQ request right after power-up sequence

  //TODO: change this
  if(EIN[2]==0x31)
  {
	  self.sender_id=2600653;
	  self.recipient_id=2600500;
  }
  else
  {
	  self.sender_id=2600500;
	  self.recipient_id=2600653;
  }

  //clear disp and show splash image
  TFT_Clear(CL_WHITE);
  TFT_DisplaySplash("splash.raw");
  TFT_SetBrght(255);
  HAL_Delay(1000);
  TFT_SetBrght(0);
  TFT_Clear(CL_WHITE);
  TFT_SetBrght(255);

  //displayEIN(EIN);
  //displayTRXInfo();
  //HAL_Delay(5000);
  //TFT_Clear(CL_WHITE);

  //display channel data
  TFT_DisplayActiveChannelData(&codeplug, &active_channel);
  TFT_RectangleFilled(0, 160-25, 127, 159, CL_LIGHTGREY);
  TFT_PutStrBold(10, 160-20, "Menu", 1, CL_BLACK);
  TFT_PutStrBold(128-48, 160-20, "Opcje", 1, CL_BLACK);

  //unmute the speaker and beep happily
  #if HW_VER < 12
  	  SPK_Enabled(1);
  #endif
  playTone("tones/ready.raw");

  //TEST - ADXL345
  /*uint8_t dta[6];
  while(1)
  {
	  HAL_I2C_Mem_Read(&hi2c1, ADXL_ADDR<<1, 0x32, 1, (uint8_t*)&dta, 6, 100);

	  uint8_t line[16];
	  TFT_PutStrCentered(22, line, 1, CL_WHITE);
	  sprintf(line, "Z=%d", (int16_t)((dta[1]<<8)+dta[0]));
	  TFT_PutStrCentered(22, line, 1, CL_BLACK);
	  HAL_Delay(1000);
  }*/

  //TEST - AF AMPLIFIER
  /*
  uint16_t test_sine[FRAMESIZE];

  for(uint16_t i=0; i<FRAMESIZE; i++)
	  test_sine[i]=0x0FFF*(sin((float)i/FRAMESIZE * 2*M_PI * 10.0)+1.0)/2.0;

  while(1)
  {
	  dac_busy=1;
	  HAL_DAC_Start_DMA(&hdac, DAC_CHANNEL_1, test_sine, FRAMESIZE, DAC_ALIGN_12B_R);
	  while(dac_busy);
  }
  */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while(1)
  {
	  //check if we want to go back to RX mode after the PTT button has been released
	  if(rx_reload_pend)
	  {
		  HAL_Delay(100);

		  if(r_tx)
		  {
			  //HAL_GPIO_TogglePin(TP1_GPIO_Port, TP1_Pin);
			  r_initd=0;
			  Si_Reset();
			  Si_StartupConfig();
			  Si_Interrupts(NULL);
			  Si_SetTxPower(tx_pwr);
			  Si_Sleep();
			  Si_FreqSet(active_channel.channel.freq_rx);
			  RF_SetRX();
			  Si_StartRx(0, PLOAD_LEN);
			  r_initd=1;
		  }

		  if(self.frame)
		  	self.frame=0;

		  rx_reload_pend=0;
		  HAL_NVIC_EnableIRQ(EXTI0_IRQn);
	  }

	  HAL_GPIO_TogglePin(LED_GRN_GPIO_Port, LED_GRN_Pin);
	  HAL_Delay(500);
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
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
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
                              |RCC_PERIPHCLK_USART3|RCC_PERIPHCLK_I2C1
                              |RCC_PERIPHCLK_SDMMC1|RCC_PERIPHCLK_CLK48;
  PeriphClkInitStruct.Usart1ClockSelection = RCC_USART1CLKSOURCE_HSI;
  PeriphClkInitStruct.Usart2ClockSelection = RCC_USART2CLKSOURCE_HSI;
  PeriphClkInitStruct.Usart3ClockSelection = RCC_USART3CLKSOURCE_HSI;
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

/* CRC init function */
static void MX_CRC_Init(void)
{

  hcrc.Instance = CRC;
  hcrc.Init.DefaultPolynomialUse = DEFAULT_POLYNOMIAL_ENABLE;
  hcrc.Init.DefaultInitValueUse = DEFAULT_INIT_VALUE_ENABLE;
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
  hcryp.Init.KeySize = CRYP_KEYSIZE_128B;
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
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
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

/* TIM4 init function */
static void MX_TIM4_Init(void)
{

  TIM_ClockConfigTypeDef sClockSourceConfig;
  TIM_MasterConfigTypeDef sMasterConfig;
  TIM_OC_InitTypeDef sConfigOC;

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

  if (HAL_TIM_PWM_Init(&htim4) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMasterConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 499;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim4, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  HAL_TIM_MspPostInit(&htim4);

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
  HAL_NVIC_SetPriority(DMA1_Stream5_IRQn, 10, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream5_IRQn);
  /* DMA1_Stream6_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream6_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream6_IRQn);
  /* DMA2_Stream0_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream0_IRQn, 9, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream0_IRQn);
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
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE, SPK_SDN_Pin|TRX_RX_SW_Pin|TP1_Pin|TP2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(TRX_TX_SW_GPIO_Port, TRX_TX_SW_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, SPI1_CS_Pin|TFT_RST_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(TRX_SDN_GPIO_Port, TRX_SDN_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, SPI2_CS_Pin|LED_GRN_Pin|LED_RED_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(TFT_A0_GPIO_Port, TFT_A0_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : SPK_SDN_Pin TRX_TX_SW_Pin TRX_RX_SW_Pin TP1_Pin 
                           TP2_Pin */
  GPIO_InitStruct.Pin = SPK_SDN_Pin|TRX_TX_SW_Pin|TRX_RX_SW_Pin|TP1_Pin 
                          |TP2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pin : SPI1_CS_Pin */
  GPIO_InitStruct.Pin = SPI1_CS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(SPI1_CS_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : TRX_SDN_Pin */
  GPIO_InitStruct.Pin = TRX_SDN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
  HAL_GPIO_Init(TRX_SDN_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : TRX_NIRQ_Pin */
  GPIO_InitStruct.Pin = TRX_NIRQ_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(TRX_NIRQ_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : SPI2_CS_Pin */
  GPIO_InitStruct.Pin = SPI2_CS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(SPI2_CS_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : TFT_A0_Pin */
  GPIO_InitStruct.Pin = TFT_A0_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(TFT_A0_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : TFT_RST_Pin */
  GPIO_InitStruct.Pin = TFT_RST_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
  HAL_GPIO_Init(TFT_RST_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : LED_GRN_Pin LED_RED_Pin */
  GPIO_InitStruct.Pin = LED_GRN_Pin|LED_RED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pin : PTT_INT_Pin */
  GPIO_InitStruct.Pin = PTT_INT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(PTT_INT_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI0_IRQn, 12, 0);
  HAL_NVIC_EnableIRQ(EXTI0_IRQn);

  HAL_NVIC_SetPriority(EXTI1_IRQn, 11, 0);
  HAL_NVIC_EnableIRQ(EXTI1_IRQn);

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

  HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, 0);

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
