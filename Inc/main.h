/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H__
#define __MAIN_H__

/* Includes ------------------------------------------------------------------*/

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private define ------------------------------------------------------------*/

#define TRX_SW_Pin GPIO_PIN_6
#define TRX_SW_GPIO_Port GPIOE
#define XO_EN_Pin GPIO_PIN_1
#define XO_EN_GPIO_Port GPIOC
#define VIBRATE_Pin GPIO_PIN_2
#define VIBRATE_GPIO_Port GPIOC
#define N9_Pin GPIO_PIN_4
#define N9_GPIO_Port GPIOC
#define N8_Pin GPIO_PIN_5
#define N8_GPIO_Port GPIOC
#define SPK_AMP_SEL_Pin GPIO_PIN_0
#define SPK_AMP_SEL_GPIO_Port GPIOB
#define FM_MOD_SEL_Pin GPIO_PIN_1
#define FM_MOD_SEL_GPIO_Port GPIOB
#define N7_Pin GPIO_PIN_2
#define N7_GPIO_Port GPIOB
#define N6_Pin GPIO_PIN_9
#define N6_GPIO_Port GPIOE
#define N5_Pin GPIO_PIN_10
#define N5_GPIO_Port GPIOE
#define N4_Pin GPIO_PIN_11
#define N4_GPIO_Port GPIOE
#define N3_Pin GPIO_PIN_12
#define N3_GPIO_Port GPIOE
#define N2_Pin GPIO_PIN_13
#define N2_GPIO_Port GPIOE
#define N1_Pin GPIO_PIN_14
#define N1_GPIO_Port GPIOE
#define ADF_CE_Pin GPIO_PIN_8
#define ADF_CE_GPIO_Port GPIOD
#define ADF_SLE_Pin GPIO_PIN_9
#define ADF_SLE_GPIO_Port GPIOD
#define ADF_SDATA_Pin GPIO_PIN_10
#define ADF_SDATA_GPIO_Port GPIOD
#define ADF_SREAD_Pin GPIO_PIN_11
#define ADF_SREAD_GPIO_Port GPIOD
#define ADF_SCLK_Pin GPIO_PIN_12
#define ADF_SCLK_GPIO_Port GPIOD
#define ADF_SWD_Pin GPIO_PIN_13
#define ADF_SWD_GPIO_Port GPIOD
#define ADF_MUX_OUT_Pin GPIO_PIN_14
#define ADF_MUX_OUT_GPIO_Port GPIOD
#define LNA_EN_Pin GPIO_PIN_6
#define LNA_EN_GPIO_Port GPIOC
#define GPS_EN_Pin GPIO_PIN_8
#define GPS_EN_GPIO_Port GPIOA
#define LED_GRN_Pin GPIO_PIN_0
#define LED_GRN_GPIO_Port GPIOD
#define LED_RED_Pin GPIO_PIN_1
#define LED_RED_GPIO_Port GPIOD
#define TFT_RST_Pin GPIO_PIN_4
#define TFT_RST_GPIO_Port GPIOD
#define SPI1_CS_Pin GPIO_PIN_4
#define SPI1_CS_GPIO_Port GPIOB
#define BLIGHT_CTRL_Pin GPIO_PIN_5
#define BLIGHT_CTRL_GPIO_Port GPIOB
#define WIFI_RST_Pin GPIO_PIN_8
#define WIFI_RST_GPIO_Port GPIOB
#define WIFI_EN_Pin GPIO_PIN_9
#define WIFI_EN_GPIO_Port GPIOB
#define PTT_INT_Pin GPIO_PIN_0
#define PTT_INT_GPIO_Port GPIOE
#define PTT_INT_EXTI_IRQn EXTI0_IRQn

/* ########################## Assert Selection ############################## */
/**
  * @brief Uncomment the line below to expanse the "assert_param" macro in the 
  *        HAL drivers code
  */
/* #define USE_FULL_ASSERT    1U */

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
 extern "C" {
#endif
void _Error_Handler(char *, int);

#define Error_Handler() _Error_Handler(__FILE__, __LINE__)
#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
