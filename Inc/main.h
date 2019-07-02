/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2019 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f2xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LIGHT_Pin GPIO_PIN_6
#define LIGHT_GPIO_Port GPIOA
#define LASER_Pin GPIO_PIN_7
#define LASER_GPIO_Port GPIOA
#define MOTOR_STEP_TIM3_CH2_Pin GPIO_PIN_0
#define MOTOR_STEP_TIM3_CH2_GPIO_Port GPIOB
#define MOTOR_ENABLE_Pin GPIO_PIN_1
#define MOTOR_ENABLE_GPIO_Port GPIOB
#define MOTOR_DIR_Pin GPIO_PIN_2
#define MOTOR_DIR_GPIO_Port GPIOB
#define MOTOR2_STEP_TIM8_CH2_Pin GPIO_PIN_7
#define MOTOR2_STEP_TIM8_CH2_GPIO_Port GPIOC
#define MOTOR2_ENABLE_Pin GPIO_PIN_8
#define MOTOR2_ENABLE_GPIO_Port GPIOA
#define MOTOR2_DIR_Pin GPIO_PIN_15
#define MOTOR2_DIR_GPIO_Port GPIOA
#define SW_LEFT_Pin GPIO_PIN_11
#define SW_LEFT_GPIO_Port GPIOC
#define SW_LEFT_EXTI_IRQn EXTI15_10_IRQn
#define SW_RIGHT_Pin GPIO_PIN_12
#define SW_RIGHT_GPIO_Port GPIOC
#define SW_RIGHT_EXTI_IRQn EXTI15_10_IRQn
#define SW_UP_Pin GPIO_PIN_2
#define SW_UP_GPIO_Port GPIOD
#define SW_UP_EXTI_IRQn EXTI2_IRQn
#define SW_DOWN_Pin GPIO_PIN_3
#define SW_DOWN_GPIO_Port GPIOB
#define SW_DOWN_EXTI_IRQn EXTI3_IRQn
/* USER CODE BEGIN Private defines */
#define LIGHT_ON HAL_GPIO_WritePin(LIGHT_GPIO_Port, LIGHT_Pin, GPIO_PIN_RESET)
#define LIGHT_OFF HAL_GPIO_WritePin(LIGHT_GPIO_Port, LIGHT_Pin, GPIO_PIN_SET)

#define LAZER_ON HAL_GPIO_WritePin(LASER_GPIO_Port, LASER_Pin, GPIO_PIN_RESET)
#define LAZER_OFF HAL_GPIO_WritePin(LASER_GPIO_Port, LASER_Pin, GPIO_PIN_SET)



#define KBRD_LEFT 			0x39
#define KBRD_RIGHT			0x38
#define KBRD_UP					0x34
#define KBRD_DOWN				0x33
#define KBRD_LIGHT			0x31
#define KBRD_LASER			0x32

#define UI_BOARD		0x00810003
#define MY_ID 			0x00810002
#define ALERT_ID 		0x0F0F0F0F
#define PC_ID				0x00810004
#define STATE 0x01
#define POSITION_RDY 0x02

#define Min_line_ADDRESS   			0x080C0000
#define Max_line_ADDRESS  			0x080C0004
#define Max_step_ADDRESS   			0x080C0008
#define Line_to_step_ADDRESS   	0x080C000C
/* USER CODE END Private defines */
typedef enum 
{
	MOVE_COMPLEATE,
	POINT_COMPLEATE,
	STOP,
	ERR
} pos_rdy;

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
