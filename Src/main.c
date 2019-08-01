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
#include "usart.h"
#include "flash.h"
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
extern uint8_t row_resieve;
extern uint8_t DevState;									//0x01 UP
																					//0x02 DOWN
																					//0x04 LEFT
																					//0x08 RIGHT
	_Bool Motor1_Need_Clbr;
	_Bool Motor2_Need_Clbr;
  _Bool MOTOR_1_CALIBRATION;
  _Bool MOTOR_2_CALIBRATION;
	
	extern _Bool MOTOR_1_STEP_ERROR;
extern _Bool MOTOR_2_STEP_ERROR;
	_Bool SAVE_STEPS;
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
	  uint8_t Old_State;
	uint8_t CAN_RECIEVE_FLAG;
	 _Bool CAN_ANWS_TIME;
CanTxMsgTypeDef        TxMessage;
CanRxMsgTypeDef        RxMessage;
uint32_t cnt;
uint32_t cnt_time;
 uint8_t TxData[8];
uint8_t Hand_Controll_1;
uint8_t Hand_Controll_2;
extern  int line_mesure;
int Min_line;
int Max_line;
int Max_step;
extern _Bool Uart_flag;
extern float line_to_step_k;
int Motor_1_Steps_togo;
int Motor_2_Steps_togo;
extern _Bool first_time_step_1;
extern _Bool first_time_step_2;
int tmp_cmd_stp;
int f_tmp_cmd_stp;
int dir_1;
int dir_2;
_Bool Stop_flag_1;
_Bool Stop_flag_2;
extern int MOTOR_1_Step;

extern int MOTOR_2_Step;

uint16_t line_mes_to_pc;
uint16_t f_tmp_cmd_stp_pc;

uint16_t old_line_mes_to_pc;
uint16_t old_f_tmp_cmd_stp_pc;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void POSITION_READY (pos_rdy state, motor_num mot);
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
	MX_USART1_UART_Init();
	Min_line = *(int*)&(*(__IO uint32_t*)(Min_line_ADDRESS));
	Max_line = *(int*)&(*(__IO uint32_t*)(Max_line_ADDRESS));
	Max_step = *(int*)&(*(__IO uint32_t*)(Max_step_ADDRESS));
	line_to_step_k = *(float*)&(*(__IO uint32_t*)(Line_to_step_ADDRESS));
	MOTOR_1_Step = *(int*)&(*(__IO uint32_t*)(CURRENT_STEP_ADDRESS));
	if(MOTOR_1_Step==0xFFFFFFFF)
	{
		MOTOR_1_Step=0;
	}
		if(HAL_GPIO_ReadPin(SW_UP_GPIO_Port,SW_UP_Pin)==GPIO_PIN_SET)
	{
		DevState|=0x40;
	}
	else
	{
		DevState&=(~0x40);
	}
	
		if(HAL_GPIO_ReadPin(SW_DOWN_GPIO_Port,SW_DOWN_Pin)==GPIO_PIN_SET)
	{
		DevState|=0x80;
	}
	else
	{
		DevState&=(~0x80);
	}
	
				if(HAL_GPIO_ReadPin(SW_LEFT_GPIO_Port,SW_LEFT_Pin)==GPIO_PIN_SET)
	{
		DevState|=0x10;
	}
	else
	{
		DevState&=(~0x10);
	}
	
		if(HAL_GPIO_ReadPin(SW_RIGHT_GPIO_Port,SW_RIGHT_Pin)==GPIO_PIN_SET)
	{
		DevState|=0x20;
	}
	else
	{
		DevState&=(~0x20);
	}
	
  /* USER CODE BEGIN 2 */
	
	HAL_TIM_Base_Start_IT(&htim3);
	HAL_TIM_Base_Start_IT(&htim8);
	
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);
	MOTOR_Enable(MOTOR_1,DISABLE);
	
	HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_2);
	MOTOR_Enable(MOTOR_2,DISABLE);


	if((Min_line==-1)||(Max_line==-1)||(Max_step==-1))
	{

			Motor1_Need_Clbr=1;
			Mtr_UP=1;
			Mtr_DOWN=1;
		
			Motor2_Need_Clbr=1;
			Mtr_LEFT=1;
			Mtr_RIGHT=1;
								
	}
		

	
	
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
		
		HAL_UART_Receive_IT(&huart1,&row_resieve,1);
		
		
		
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
										POSITION_READY (STOP,MOTOR_1);
										//flash_write_koef(CURRENT_STEP_ADDRESS,MOTOR_1_Step);
										SAVE_STEPS=1;
									break;
								case KBRD_DOWN:
								
										DOWN_flag=0;
										POSITION_READY (STOP,MOTOR_1);
									//flash_write_koef(CURRENT_STEP_ADDRESS,MOTOR_1_Step);
										SAVE_STEPS=1;
									break;
								
								case KBRD_LEFT:
									
										LEFT_flag=0;
										POSITION_READY (STOP,MOTOR_2);
								
									break;
								case KBRD_RIGHT:
									
										RIGHT_flag=0;
										POSITION_READY (STOP,MOTOR_2);
									break;
							}								
						}
					}
					
					if (RxMessage.ExtId == PC_ID)
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
										POSITION_READY (STOP,MOTOR_1);
									//	flash_write_koef(CURRENT_STEP_ADDRESS,MOTOR_1_Step);
										SAVE_STEPS=1;
									break;
								case KBRD_DOWN:
								
										DOWN_flag=0;
										POSITION_READY (STOP,MOTOR_1);
										//flash_write_koef(CURRENT_STEP_ADDRESS,MOTOR_1_Step);
									SAVE_STEPS=1;
									break;
								
								case KBRD_LEFT:
									
										LEFT_flag=0;
										POSITION_READY (STOP,MOTOR_2);
								
									break;
								case KBRD_RIGHT:
									
										RIGHT_flag=0;
										POSITION_READY (STOP,MOTOR_2);
									break;
							}							
														
						}
						else if(RxMessage.Data[1] == 0x20)
						{
							if(RxMessage.Data[2] == 0x00) //Горизонтальный
							{
								if(Motor_Get_ENABLE(MOTOR_2) == DISABLE)
								{
									 tmp_cmd_stp  = 0;
									 tmp_cmd_stp  |= RxMessage.Data[6]<<24;
									 tmp_cmd_stp  |= RxMessage.Data[5]<<16;
									 tmp_cmd_stp  |= RxMessage.Data[4]<<8;
									 tmp_cmd_stp  |= RxMessage.Data[3];
									if(tmp_cmd_stp*line_to_step_k>= MOTOR_2_Step)
									{
										Motor_2_Steps_togo = tmp_cmd_stp*line_to_step_k;
									}
									else
									{
										Motor_2_Steps_togo = -(tmp_cmd_stp*line_to_step_k);
									}
									if(Motor_2_Steps_togo==0)
									{
										POSITION_READY(MOVE_COMPLEATE,MOTOR_2);
									}
									else
									{
										first_time_step_2 = 1;
										MOTOR_2_STEP_ERROR = 0;
									}
								}
								else
								{
									POSITION_READY(ERR,MOTOR_2);
								}
							}
							else if(RxMessage.Data[2] == 0x01) //Веритикальный
							{	if(Motor_Get_ENABLE(MOTOR_1) == DISABLE)
								{
									tmp_cmd_stp = 0; 
									tmp_cmd_stp  |= RxMessage.Data[6]<<24;
									tmp_cmd_stp  |= RxMessage.Data[5]<<16;
									tmp_cmd_stp  |= RxMessage.Data[4]<<8;
									tmp_cmd_stp  |= RxMessage.Data[3];
									f_tmp_cmd_stp = tmp_cmd_stp;
									
						
									Motor_1_Steps_togo = (int)((f_tmp_cmd_stp - ((float)MOTOR_1_Step/((float)Max_step/1000)))*((float)Max_step/1000));
									
									if(Motor_1_Steps_togo==0)
									{
										POSITION_READY(ERR,MOTOR_1);
									}
									else
									{
										first_time_step_1 = 1;
										MOTOR_1_STEP_ERROR = 0;
									}
								}
								else
								{
									POSITION_READY(ERR,MOTOR_1);
								}
							}
						}
						else if (RxMessage.Data[1] == 0x30) //калибровка
						{
								Motor1_Need_Clbr=1;
								Motor2_Need_Clbr=1;
								Mtr_UP=1;
								Mtr_DOWN=1;
								Mtr_LEFT=1;
								Mtr_RIGHT=1;
						}
						else if (RxMessage.Data[1] == 0x40) //калибровка
						{
								uint8_t *ptr;
								ptr = &TxData[1];
								TxData[0] = 0x41;
								*(int*)ptr = Min_line;
								CAN_TRANSMIT(MY_ID,5,TxData);
							
								TxData[0] = 0x42;
								ptr = &TxData[1];
								*(int*)ptr = Max_line;
								CAN_TRANSMIT(MY_ID,5,TxData);
							
							
							
						}
						else if (RxMessage.Data[1] == 0x50) //Стоп
						{
								
								Stop_flag_1=1;
								Stop_flag_2=1;
								Motor_2_Steps_togo = 0;
								Motor_1_Steps_togo = 0;
								first_time_step_1=1;
								first_time_step_2=1;
								Motor1_Need_Clbr=0;
								Motor2_Need_Clbr=0;
								Mtr_UP=0;
								Mtr_DOWN=0;
								Mtr_LEFT=0;
								Mtr_RIGHT=0;
								//flash_write_koef(CURRENT_STEP_ADDRESS,MOTOR_1_Step);
							SAVE_STEPS=1;
						}
					}
				}
			}
    /* USER CODE END WHILE */
				if((Old_State!=DevState)||(old_f_tmp_cmd_stp_pc!=f_tmp_cmd_stp_pc)||(old_line_mes_to_pc!=line_mes_to_pc))
		{
			
			old_f_tmp_cmd_stp_pc = f_tmp_cmd_stp_pc;
			old_line_mes_to_pc = line_mes_to_pc;
			Old_State=DevState;
			TxData[0] = STATE;
			TxData[1] = line_mes_to_pc;
			TxData[2] = line_mes_to_pc>>8;
			TxData[3] =	f_tmp_cmd_stp_pc;
			TxData[4] =	f_tmp_cmd_stp_pc>>8;
			TxData[5] = DevState;
			CAN_TRANSMIT(MY_ID,6,TxData);
		}
    /* USER CODE END WHILE */
		if(CAN_ANWS_TIME)
		{
			CAN_ANWS_TIME=0;
			TxData[0] = STATE;
			TxData[1] = line_mes_to_pc;
			TxData[2] = line_mes_to_pc>>8;
			TxData[3] =	f_tmp_cmd_stp_pc;
			TxData[4] =	f_tmp_cmd_stp_pc>>8;
			TxData[5] = DevState;
			CAN_TRANSMIT(MY_ID,6,TxData);
		}
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
	
		cnt_time++;
		cnt++;
	 if(cnt_time==1000)
	 {
		 
		 cnt_time=0;
		CAN_ANWS_TIME=1;
		 if(SAVE_STEPS)
		 {
			 SAVE_STEPS=0;
			 flash_write_koef(CURRENT_STEP_ADDRESS,MOTOR_1_Step);
		 }
		 
	 }
	 if(cnt==150)
	 {
		 cnt=0;
		 
		 int raw_line_mesure = DLS_GET_Mes()/10;
		 
		 if(raw_line_mesure)
		 {
			 line_mesure = raw_line_mesure;
			 line_mes_to_pc = line_mesure;
		 }
		 
	 }
		 
  }
	
	if(htim->Instance == TIM3)
	{
			if (HAL_CAN_Receive_IT(&hcan1, CAN_FIFO0) != HAL_OK)
  {
		Error_Handler();
  }
	f_tmp_cmd_stp_pc = (int)(((float)MOTOR_1_Step/(float)Max_step)*1000);
	if(Stop_flag_1)
		{
			
			if(Motor_Get_ENABLE(MOTOR_1)==ENABLE)
				{
					POSITION_READY (STOP,MOTOR_1);
					MOTOR_Enable(MOTOR_1,DISABLE);
					
				}
				Stop_flag_1=0;
		}
		
		if(Motor1_Need_Clbr)
		{

			if(Motor_Calibration(MOTOR_1))
			{
				Motor1_Need_Clbr=0;
				
			}
		}	
		if(!MOTOR_1_CALIBRATION)
		{
				if(Motor_Get_ENABLE(MOTOR_1)==ENABLE)
				{
					if((MOTOR_1_STEP_ERROR)&&(Hand_Controll_1==0))
					{
						MOTOR_Enable(MOTOR_1,DISABLE);
						Motor_1_Steps_togo=0;
						POSITION_READY (ERR,MOTOR_1);
					}
					if(Motor_Get_Dir(MOTOR_1)==UP)
					{
						if((DevState&0x40)==0x40)
						{
							MOTOR_Enable(MOTOR_1,DISABLE);
							Motor_1_Steps_togo=0;
							POSITION_READY (POINT_COMPLEATE,MOTOR_1);
						}
		
					}
					else if((DevState&0x80)==0x80)
					{
						MOTOR_Enable(MOTOR_1,DISABLE);
						Motor_1_Steps_togo=0;
						POSITION_READY (POINT_COMPLEATE,MOTOR_1);
					}
					
				}
				
				
				if(Hand_Controll_1)
				{
					if(UP_flag)
					{
						if(Motor_to_Switch(MOTOR_1, UP, 30))
						{
							UP_flag=0;
							POSITION_READY (POINT_COMPLEATE,MOTOR_1);
						}
					}
					else if(DOWN_flag)
					{
						if(Motor_to_Switch(MOTOR_1, DOWN, 30))
						{
							DOWN_flag=0;
							POSITION_READY (POINT_COMPLEATE,MOTOR_1);
						}
					}
					else
					{
						MOTOR_Enable(MOTOR_1, DISABLE);
						Hand_Controll_1--;
					}
				}
				if(Motor_1_Steps_togo!=0)
				{
					if(Motor_1_Steps_togo>0)
					{
							if(Motor_step(MOTOR_1,Motor_1_Steps_togo,1))
							{
								POSITION_READY (MOVE_COMPLEATE,MOTOR_1);
								Motor_1_Steps_togo=0;
							}
					}
					else
					{
							if(Motor_step(MOTOR_1,(-Motor_1_Steps_togo),-1))
							{
								POSITION_READY (MOVE_COMPLEATE,MOTOR_1);
								Motor_1_Steps_togo=0;
							}
					}
				}
		}
	}
	
	if(htim->Instance == TIM8)
	{
		if(Stop_flag_2)
		{
			if(Motor_Get_ENABLE(MOTOR_2)==ENABLE)
				{
					MOTOR_Enable(MOTOR_2,DISABLE);
					POSITION_READY (STOP,MOTOR_2);
				}
				Stop_flag_2=0;
		}
		if(Motor2_Need_Clbr)
		{
			if(Motor_Calibration(MOTOR_2))
					{
						Motor2_Need_Clbr=0;
						flash_write_koef(Min_line_ADDRESS,Min_line);
						flash_write_koef(Max_step_ADDRESS,Max_step);
						flash_write_koef_f(Line_to_step_ADDRESS,line_to_step_k);
						flash_write_koef(Max_line_ADDRESS,Max_line);
						uint8_t *ptr1;
						ptr1 = &TxData[1];
						TxData[0] = 0x41;
						*(int*)ptr1 = Min_line;
						CAN_TRANSMIT(MY_ID,5,TxData);
							
						TxData[0] = 0x42;
						ptr1 = &TxData[1];
						*(int*)ptr1 = Max_line;
						CAN_TRANSMIT(MY_ID,5,TxData);
						SAVE_STEPS=1;
					}
		}	
		if(!MOTOR_2_CALIBRATION)
		{
				if(Motor_Get_ENABLE(MOTOR_2)==ENABLE)
				{
					if((MOTOR_2_STEP_ERROR)&&(Hand_Controll_2==0))
					{
						MOTOR_Enable(MOTOR_2,DISABLE);
						Motor_2_Steps_togo=0;
						POSITION_READY (ERR,MOTOR_2);
					}
					if(Motor_Get_Dir(MOTOR_2)==LEFT)
					{
						if((DevState&0x10)==0x10)
						{
							MOTOR_Enable(MOTOR_2,DISABLE);
							Motor_2_Steps_togo=0;
							POSITION_READY (POINT_COMPLEATE,MOTOR_2);
						}
		
					}
					else if((DevState&0x20)==0x20)
					{
						MOTOR_Enable(MOTOR_2,DISABLE);
						Motor_2_Steps_togo=0;
						POSITION_READY (POINT_COMPLEATE,MOTOR_2);
					}
					
				}
				

				if(Hand_Controll_2)
				{
					if(LEFT_flag)
					{
						if(Motor_to_Switch(MOTOR_2, LEFT, 30))
						{
							LEFT_flag=0;
							POSITION_READY (POINT_COMPLEATE,MOTOR_2);
						}
						
					}
					else if(RIGHT_flag)
					{
						if(Motor_to_Switch(MOTOR_2, RIGHT, 30))
						{
							RIGHT_flag=0;
							POSITION_READY (POINT_COMPLEATE,MOTOR_2);
						}
					}
					else
					{
						MOTOR_Enable(MOTOR_2, DISABLE);
						Hand_Controll_2--;
					}
				}
				if(Motor_2_Steps_togo!=0)
				{
					if(Motor_2_Steps_togo>0)
					{
							if(Motor_step(MOTOR_2,Motor_2_Steps_togo,1))
							{
								POSITION_READY (MOVE_COMPLEATE,MOTOR_2);
								Motor_2_Steps_togo=0;
							}
					}
					else
					{
							if(Motor_step(MOTOR_2,(-Motor_2_Steps_togo),-1))
							{
								POSITION_READY (MOVE_COMPLEATE,MOTOR_2);
								Motor_2_Steps_togo=0;
							}
					}
				}
		}
	}
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

void POSITION_READY (pos_rdy state, motor_num mot)
{
					if(mot == MOTOR_1)
				{
					TxData[1] = 0x01;
				}
				else if(mot == MOTOR_2)
				{
					TxData[1] = 0x00;
				}
					
	switch (state)
	{
		
		case MOVE_COMPLEATE:
				TxData[0] = POSITION_RDY;
				TxData[2] = 0x00;
				CAN_TRANSMIT(MY_ID,3,TxData);
			break;
		case POINT_COMPLEATE:
				TxData[0] = POSITION_RDY;
				TxData[2] = 0x01;
				CAN_TRANSMIT(MY_ID,3,TxData);
			break;
		case STOP:
				TxData[0] = POSITION_RDY;
				TxData[2] = 0x02;
				CAN_TRANSMIT(MY_ID,3,TxData);
			break;
		case ERR:
				TxData[0] = POSITION_RDY;
				TxData[2] = 0x03;
				CAN_TRANSMIT(MY_ID,3,TxData);
			break;
	}
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
