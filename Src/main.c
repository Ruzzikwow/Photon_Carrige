/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "can.h"
#include "tim.h"
#include "gpio.h"
#include "motor.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
extern uint8_t DevState;									//0x01 UP
																					//0x02 DOWN
																					//0x04 LEFT
																					//0x08 RIGHT
	_Bool Motor1_Need_Clbr;
	_Bool Motor2_Need_Clbr;
  _Bool MOTOR_1_CALIBRATION;
  _Bool MOTOR_2_CALIBRATION;
	
	_Bool Light_cnt;
	_Bool Lazer_cnt;
	
	 _Bool Mtr_UP;
	 _Bool Mtr_DOWN;
	 _Bool Mtr_LEFT;
	 _Bool Mtr_RIGHT;
	 
	 _Bool UP_flag;
	 _Bool DOWN_flag;
	 _Bool LEFT_flag;
	 _Bool RIGHT_flag;
	uint8_t CAN_RECIEVE_FLAG;
CanTxMsgTypeDef        TxMessage;
CanRxMsgTypeDef        RxMessage;

uint8_t Hand_Controll_1;
uint8_t Hand_Controll_2;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

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
  MX_TIM8_Init();
  MX_TIM3_Init();
  MX_CAN1_Init();
	
		if(HAL_GPIO_ReadPin(SW_UP_GPIO_Port,SW_UP_Pin)==GPIO_PIN_SET)
	{
		DevState|=0x01;
	}
	else
	{
		DevState&=(~0x01);
	}
	
		if(HAL_GPIO_ReadPin(SW_DOWN_GPIO_Port,SW_DOWN_Pin)==GPIO_PIN_SET)
	{
		DevState|=0x02;
	}
	else
	{
		DevState&=(~0x02);
	}
	
				if(HAL_GPIO_ReadPin(SW_LEFT_GPIO_Port,SW_LEFT_Pin)==GPIO_PIN_SET)
	{
		DevState|=0x04;
	}
	else
	{
		DevState&=(~0x04);
	}
	
		if(HAL_GPIO_ReadPin(SW_RIGHT_GPIO_Port,SW_RIGHT_Pin)==GPIO_PIN_SET)
	{
		DevState|=0x08;
	}
	else
	{
		DevState&=(~0x08);
	}
	
  /* USER CODE BEGIN 2 */
	
	HAL_TIM_Base_Start_IT(&htim3);
	HAL_TIM_Base_Start_IT(&htim8);
	
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);
	MOTOR_Enable(MOTOR_1,DISABLE);
	
	HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_2);
	MOTOR_Enable(MOTOR_2,DISABLE);


	Motor1_Need_Clbr=0;
	Motor2_Need_Clbr=0;
	Mtr_UP=1;
	Mtr_DOWN=1;
	Mtr_LEFT=1;
	Mtr_RIGHT=1;

	



  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
		
		
			if(Motor_Get_ENABLE(MOTOR_1))
	{
		
		switch(Motor_Get_Dir(MOTOR_1))
		{
			case UP:
				DevState|=0x04;
				DevState&=(~0x08);
				break;
			case DOWN:
				DevState|=0x08;
				DevState&=(~0x04);
				break;
			case LEFT:
				break;
			case RIGHT:
				break;
		}
	}
		else
	{
		DevState&=(~0x04);
		DevState&=(~0x08);
	}
	
		if(Motor_Get_ENABLE(MOTOR_2))
	{
		
		switch(Motor_Get_Dir(MOTOR_2))
		{
			case UP:
				break;
			case DOWN:
				break;
			case LEFT:
				DevState|=0x01;
				DevState&=(~0x02);
				break;
			case RIGHT:
				DevState|=0x02;
				DevState&=(~0x01);
				break;
		}
	}
	else
	{
		DevState&=(~0x01);
		DevState&=(~0x02);
	}
	
	
			if(CAN_RECIEVE_FLAG)
			{
				CAN_RECIEVE_FLAG=0;
				
				if(RxMessage.DLC>0)
				{
					if (RxMessage.ExtId == UI_BOARD)
					{
						if(RxMessage.Data[1]==0x00)
						{
							switch (RxMessage.Data[2])
							{
								case KBRD_LASER:
										Lazer_cnt^=1;
										if(Lazer_cnt)
										{
											LAZER_ON;
										}
										else
										{
											LAZER_OFF;
										}
									break;
								case KBRD_LIGHT:
									
										Light_cnt^=1;
										if(Light_cnt)
										{
											LIGHT_ON;
										}
										else
										{
											LIGHT_OFF;
										}
									break;
										
								case KBRD_UP:
									
										UP_flag=1;
										DOWN_flag=0;
										Hand_Controll_1++;
									break;
								case KBRD_DOWN:
										UP_flag=0;
										DOWN_flag=1;
										Hand_Controll_1++;
									break;
								
								case KBRD_LEFT:
									
										LEFT_flag=1;
										RIGHT_flag=0;
										Hand_Controll_2++;
								
									break;
								case KBRD_RIGHT:
									
										RIGHT_flag=1;
										LEFT_flag=0;
										Hand_Controll_2++;
									break;
							}							
						}
						else if(RxMessage.Data[1]==0x10)
						{
							switch (RxMessage.Data[2])
							{
								case KBRD_UP:
									
										UP_flag=0;
										
								
									break;
								case KBRD_DOWN:
								
										DOWN_flag=0;
										
									break;
								
								case KBRD_LEFT:
									
										LEFT_flag=0;
										
								
									break;
								case KBRD_RIGHT:
									
										RIGHT_flag=0;
									
									break;
							}								
						}
					}
				}
			}
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
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /**Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 240;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 5;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /**Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM2 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM2) {
    HAL_IncTick();
  }
	
	if(htim->Instance == TIM3)
	{
		if(Motor1_Need_Clbr)
		{

			if(Motor_Calibration(MOTOR_1))
			{
				Motor1_Need_Clbr=0;
				LIGHT_ON;
			}
		}	
		if(!MOTOR_1_CALIBRATION)
		{
				if(Motor_Get_ENABLE(MOTOR_1)==ENABLE)
				{
					if(Motor_Get_Dir(MOTOR_1)==UP)
					{
						if((DevState&0x40)==0x40)
						{
							MOTOR_Enable(MOTOR_1,DISABLE);
							
						}
		
					}
					else if((DevState&0x80)==0x80)
					{
						MOTOR_Enable(MOTOR_1,DISABLE);
					}
					
				}
				
				
				if(Hand_Controll_1)
				{
					if(UP_flag)
					{
						if(Motor_to_Switch(MOTOR_1, UP, 30))
						{
							UP_flag=0;
						}
					}
					else if(DOWN_flag)
					{
						if(Motor_to_Switch(MOTOR_1, DOWN, 30))
						{
							DOWN_flag=0;
						}
					}
					else
					{
						MOTOR_Enable(MOTOR_1, DISABLE);
						Hand_Controll_1--;
					}
				}
		}
	}
	
	if(htim->Instance == TIM8)
	{
		if(Motor2_Need_Clbr)
		{
			if(Motor_Calibration(MOTOR_2))
					{
						LAZER_ON;
					}
		}	
		if(!MOTOR_2_CALIBRATION)
		{
				if(Motor_Get_ENABLE(MOTOR_2)==ENABLE)
				{
					if(Motor_Get_Dir(MOTOR_2)==LEFT)
					{
						if((DevState&0x10)==0x10)
						{
							MOTOR_Enable(MOTOR_2,DISABLE);
							
						}
		
					}
					else if((DevState&0x20)==0x20)
					{
						MOTOR_Enable(MOTOR_2,DISABLE);
					}
					
				}
				

				if(Hand_Controll_2)
				{
					if(LEFT_flag)
					{
						if(Motor_to_Switch(MOTOR_2, LEFT, 30))
						{
							LEFT_flag=0;
						}
						
					}
					else if(RIGHT_flag)
					{
						if(Motor_to_Switch(MOTOR_2, RIGHT, 30))
						{
							RIGHT_flag=0;
						}
					}
					else
					{
						MOTOR_Enable(MOTOR_2, DISABLE);
						Hand_Controll_2--;
					}
				}
		}
	}
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

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
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
