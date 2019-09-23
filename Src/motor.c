#include "motor.h"
#include "tim.h"
#include "flash.h"
 uint8_t DevState;												//0x01 UP
																					//0x02 DOWN
																					//0x04 LEFT
																					//0x08 RIGHT
int MOTOR_1_Step_Transmition	= 10000;
int MOTOR_2_Step_Transmition	= 10000;
 int MOTOR_1_Step;

 int MOTOR_2_Step;
 
 _Bool MOTOR_1_STEP_ERROR;
 _Bool MOTOR_2_STEP_ERROR;
 
 	extern _Bool Mtr_UP;
	extern _Bool Mtr_DOWN;
	extern _Bool Mtr_LEFT;
	extern _Bool Mtr_RIGHT;
 
 extern _Bool MOTOR_1_CALIBRATION;
 extern _Bool MOTOR_2_CALIBRATION;
 
 extern int Min_line;
 extern int Max_line;
 extern int Max_step;
 int line_mesure;
 float line_to_step_k;
 
 int temp_steps;
 int temp_steps_togo;
 int temp_steps_2;
 int temp_steps_togo_2;
  _Bool first_time_step_1;
  _Bool first_time_step_2;
	
	int i=0;
	int old_stp=0;
	
	int i_2=0;
	int old_stp_2=0;

void MOTOR_ToggleDir(motor_num motor)
{
	switch (motor)
	{
		case MOTOR_1:
			HAL_GPIO_TogglePin(MOTOR_DIR_GPIO_Port, MOTOR_DIR_Pin);
			break;
		case MOTOR_2:
			HAL_GPIO_TogglePin(MOTOR2_DIR_GPIO_Port, MOTOR2_DIR_Pin);
			break;
	}
		
	
}

_Bool MOTOR_Direction (motor_num motor, motor_direction dir)
{
		switch (motor)
	{
		case MOTOR_2:
			switch (dir)
			{
				case LEFT:
						HAL_GPIO_WritePin(MOTOR2_DIR_GPIO_Port, MOTOR2_DIR_Pin, GPIO_PIN_SET);
						return 1;
					
				case RIGHT:
						HAL_GPIO_WritePin(MOTOR2_DIR_GPIO_Port, MOTOR2_DIR_Pin, GPIO_PIN_RESET);
						return 1;
					
				case UP:
					return 0;
				
				case DOWN:
					return 0;
					
			}
			break;
		case MOTOR_1:
		switch (dir)
			{
				case UP:
						HAL_GPIO_WritePin(MOTOR_DIR_GPIO_Port, MOTOR_DIR_Pin, GPIO_PIN_SET);
						return 1;
					
				case DOWN:
						HAL_GPIO_WritePin(MOTOR_DIR_GPIO_Port, MOTOR_DIR_Pin, GPIO_PIN_RESET);
				return 1;
				
				case LEFT:
					return 0;
				
				case RIGHT:
					return 0;
				
			}
			break;
	}
	return 0;
}

void MOTOR_Enable (motor_num motor,FunctionalState state)
{
	switch(motor)
	{
		case MOTOR_1:
					switch (state)
				{
					case ENABLE:
						HAL_GPIO_WritePin(MOTOR_ENABLE_GPIO_Port, MOTOR_ENABLE_Pin, GPIO_PIN_SET);
						break;
					case DISABLE:
						HAL_GPIO_WritePin(MOTOR_ENABLE_GPIO_Port, MOTOR_ENABLE_Pin, GPIO_PIN_RESET);
						break;
				}
			break;
		case MOTOR_2:
			
				switch (state)
				{
					case ENABLE:
						HAL_GPIO_WritePin(MOTOR2_ENABLE_GPIO_Port, MOTOR2_ENABLE_Pin, GPIO_PIN_SET);
						break;
					case DISABLE:
						HAL_GPIO_WritePin(MOTOR2_ENABLE_GPIO_Port, MOTOR2_ENABLE_Pin, GPIO_PIN_RESET);
						break;
				}
			break;
		
	}
}

void Motor_Speed (motor_num motor,uint8_t percent)
{
	switch (motor)
	{
		case MOTOR_1:
			//max 305 arr 288 ccr
			htim3.Instance->ARR = 1130 - percent*10;
			htim3.Instance->CCR3 = (htim3.Instance->ARR)*0.5;
			break;
		case MOTOR_2:
			
			htim8.Instance->ARR = 1130 - percent*10;
			htim8.Instance->CCR2 = (htim8.Instance->ARR)*0.5;
			break;
	}
}

_Bool Motor_Calibration (motor_num motor)
{

	switch (motor)
	{
		case MOTOR_1:
				
				MOTOR_1_CALIBRATION=1;
				
		
				if(Mtr_UP)
				{
					if(Motor_to_Switch(motor,UP,100))
					{
						
						Mtr_UP=0;
						MOTOR_1_Step = 0;
					}
				}

				else if(Mtr_DOWN)
				{
					if(Motor_to_Switch(motor,DOWN,100))
					{
						Max_step = MOTOR_1_Step;
						
						Mtr_DOWN=0;
						MOTOR_1_CALIBRATION=0;
						MOTOR_1_STEP_ERROR=0;	
						return 1;
					}
					else {return 0;}
				}
				else {return 0;}

		case MOTOR_2:
			
			MOTOR_2_CALIBRATION=1;
		
		
				if(Mtr_LEFT)
				{
					if(Motor_to_Switch(motor,LEFT,100))
					{
						Min_line = line_mesure;
						
						Mtr_LEFT=0;
						MOTOR_2_Step = 0;
					}
				}

				else if(Mtr_RIGHT)
				{
					if(Motor_to_Switch(motor,RIGHT,100))
					{
						Max_line = line_mesure;
						line_to_step_k = MOTOR_2_Step/(Max_line - Min_line);

						Mtr_RIGHT=0;
						MOTOR_2_CALIBRATION=0;
						MOTOR_2_STEP_ERROR=0;	
						return 1;
					}
					else {return 0;}
				}
				else {return 0;}

	}
	return 0;
}

_Bool Motor_to_Switch (motor_num motor, motor_direction dir, int speed)
{
	
	Motor_Speed(motor,speed);
	if(MOTOR_Direction(motor,dir))
	{
		switch (motor)
		{
			case MOTOR_1:
				switch (dir)
				{
					case UP:
						MOTOR_Enable(motor,ENABLE);
						if((DevState&0x40)==0x40) 
						{
							MOTOR_Enable(motor,DISABLE);
							return 1;
						}
					break;
					case DOWN:
						
						MOTOR_Enable(motor,ENABLE);
						if((DevState&0x80)==0x80)
							{
										MOTOR_Enable(motor,DISABLE);
										return 1;
							}
			
					
					break;	
					case LEFT:
						return 0;
				
					case RIGHT:
						return 0;
			
				}
				break;
			case MOTOR_2:
				
			switch (dir)
				{
					case LEFT:
						MOTOR_Enable(motor,ENABLE);
						if((DevState&0x10)==0x10)
						{
								MOTOR_Enable(motor,DISABLE);
								return 1;
						}
						break;
					
					case RIGHT:
						MOTOR_Enable(motor,ENABLE);
						if((DevState&0x20)==0x20)
						{
								MOTOR_Enable(motor,DISABLE);
								return 1;
						}
					
					break;
						
					case UP:
						return 0;
				
					case DOWN:
						return 0;
			
				}
				
				break;
		}
		
		return 0;
	}
	else
	{
		return 0;
	}
	
}

FunctionalState Motor_Get_ENABLE (motor_num motor)
{
	switch (motor)
	{
		case MOTOR_1:
			if(HAL_GPIO_ReadPin(MOTOR_ENABLE_GPIO_Port, MOTOR_ENABLE_Pin) == GPIO_PIN_SET)
			{
				return ENABLE;
			}
			else
			{
				return DISABLE;
			}
			
		case MOTOR_2:
			
			if(HAL_GPIO_ReadPin(MOTOR2_ENABLE_GPIO_Port, MOTOR2_ENABLE_Pin) == GPIO_PIN_SET)
			{
				return ENABLE;
			}
			else
			{
				return DISABLE;
			}
	}
	
	return DISABLE;
}


motor_direction Motor_Get_Dir (motor_num motor)
{
		switch (motor)
	{
		case MOTOR_1:
			if(HAL_GPIO_ReadPin(MOTOR_DIR_GPIO_Port, MOTOR_DIR_Pin) == GPIO_PIN_SET)
			{
				return UP;
			}
			else
			{
				return DOWN;
			}
			
		case MOTOR_2:
			
			if(HAL_GPIO_ReadPin(MOTOR2_DIR_GPIO_Port, MOTOR2_DIR_Pin) == GPIO_PIN_SET)
			{
				return LEFT;
			}
			else
			{
				return RIGHT;
			}
	}
	return RIGHT;
}


_Bool Motor_step (motor_num motor,uint32_t step,int sign)
{
	
			switch (motor)
	{
		case MOTOR_1:
			if(first_time_step_1)
			{
				temp_steps = MOTOR_1_Step;
				temp_steps_togo = step;
				first_time_step_1=0;
				Motor_Speed(MOTOR_1,0);
				i=0;
			}
			if(temp_steps_togo < 2 * MOTOR_1_Step_Transmition)
			{
				Motor_Speed(MOTOR_1,30);
				if(sign>0)
				{
					MOTOR_Direction(MOTOR_1,DOWN);
					
					if(MOTOR_1_Step != temp_steps + temp_steps_togo)
					{
						MOTOR_Enable(MOTOR_1,ENABLE);
						if(Motor_Get_ENABLE(MOTOR_1)==DISABLE)
						{
							temp_steps_togo = 0;
							temp_steps = MOTOR_1_Step;
							return 0;
						}
					}
					else
					{
						MOTOR_Enable(MOTOR_1,DISABLE);
						return 1;
					}
				}
				else
				{
					MOTOR_Direction(MOTOR_1,UP);
					
					if(MOTOR_1_Step != temp_steps - temp_steps_togo)
					{
						MOTOR_Enable(MOTOR_1,ENABLE);
						if(Motor_Get_ENABLE(MOTOR_1)==DISABLE)
						{
							temp_steps_togo = 0;
							temp_steps = MOTOR_1_Step;
							return 0;
						}
					}
					else
					{
						MOTOR_Enable(MOTOR_1,DISABLE);
						return 1;
					}
				}
			}
			else
			{
				
				
				
				if(sign>0)
				{
					MOTOR_Direction(MOTOR_1,DOWN);
					
					if(MOTOR_1_Step != temp_steps + temp_steps_togo)
					{
						MOTOR_Enable(MOTOR_1,ENABLE);
						
							if((((MOTOR_1_Step - temp_steps)%(MOTOR_1_Step_Transmition/100))==0)&&((MOTOR_1_Step - temp_steps)<=MOTOR_1_Step_Transmition)&&(i<=100))
							{
								i++;
								Motor_Speed(MOTOR_1,i);
							}
							if(((((temp_steps+temp_steps_togo) - MOTOR_1_Step)%(MOTOR_1_Step_Transmition/100))==0)&&(((temp_steps+temp_steps_togo) - MOTOR_1_Step)<=MOTOR_1_Step_Transmition)&&(i>=2))
							{
								i--;
								Motor_Speed(MOTOR_1,i);
							}
						
						if(Motor_Get_ENABLE(MOTOR_1)==DISABLE)
						{
							temp_steps_togo = 0;
							temp_steps = MOTOR_1_Step;
							return 0;
						}
					}
					else
					{
						
					MOTOR_Enable(MOTOR_1,DISABLE);
					return 1;
					}
				}
				
				else
				{
					MOTOR_Direction(MOTOR_1,UP);
					
					
					if(MOTOR_1_Step != temp_steps - temp_steps_togo)
					{
						MOTOR_Enable(MOTOR_1,ENABLE);
						
							if((((temp_steps - MOTOR_1_Step )%(MOTOR_1_Step_Transmition/100))==0)&&((temp_steps - MOTOR_1_Step )<=MOTOR_1_Step_Transmition)&&(i<=100))
							{
								i++;
								Motor_Speed(MOTOR_1,i);
							}
							if(((( MOTOR_1_Step - (temp_steps-temp_steps_togo))%(MOTOR_1_Step_Transmition/100))==0)&&(( MOTOR_1_Step - (temp_steps-temp_steps_togo))<=MOTOR_1_Step_Transmition)&&(i>=2))
							{
								i--;
								Motor_Speed(MOTOR_1,i);
							}
						
						
						if(Motor_Get_ENABLE(MOTOR_1)==DISABLE)
						{
							temp_steps_togo = 0;
							temp_steps = MOTOR_1_Step;
							return 0;
						}
					}
					else
					{
					MOTOR_Enable(MOTOR_1,DISABLE);
					return 1;
					}
				}
			}
			
		break;
			
		case MOTOR_2:
			
						if(first_time_step_2)
			{
				temp_steps_2 = MOTOR_2_Step;
				temp_steps_togo_2 = step;
				first_time_step_2=0;
				Motor_Speed(MOTOR_2,0);
				i_2=0;
			}
		
		
		
			if(temp_steps_togo_2 < 2 * MOTOR_2_Step_Transmition)
			{
				Motor_Speed(MOTOR_2,30);
				if(sign>0)
				{
					MOTOR_Direction(MOTOR_2,RIGHT);
					
					if(MOTOR_2_Step != temp_steps_2 + temp_steps_togo_2)
					{
						MOTOR_Enable(MOTOR_2,ENABLE);
						
						if(Motor_Get_ENABLE(MOTOR_2)==DISABLE)
						{
							temp_steps_togo_2 = 0;
							temp_steps_2 = MOTOR_2_Step;
							return 0;
						}
						
					}
					else
					{
					MOTOR_Enable(MOTOR_2,DISABLE);
					return 1;
					}
				}
				else
				{
					MOTOR_Direction(MOTOR_2,LEFT);
					
					if(MOTOR_2_Step != temp_steps_2 - temp_steps_togo_2)
					{
						MOTOR_Enable(MOTOR_2,ENABLE);
						if(Motor_Get_ENABLE(MOTOR_2)==DISABLE)
						{
							temp_steps_togo_2 = 0;
							temp_steps_2 = MOTOR_2_Step;
							return 0;
						}
					}
					else
					{
					MOTOR_Enable(MOTOR_2,DISABLE);
					return 1;
					}
				}
			}
			else
			{
				
				
				
				if(sign>0)
				{
					MOTOR_Direction(MOTOR_2,RIGHT);
					
					if(MOTOR_2_Step != temp_steps_2 + temp_steps_togo_2)
					{
						MOTOR_Enable(MOTOR_2,ENABLE);
						
					
							if((((MOTOR_2_Step - temp_steps_2)%(MOTOR_2_Step_Transmition/100))==0)&&((MOTOR_2_Step - temp_steps_2)<=MOTOR_2_Step_Transmition)&&(i_2<=100))
							{
								i_2++;
								Motor_Speed(MOTOR_2,i_2);
							}
							if(((((temp_steps_2+temp_steps_togo_2) - MOTOR_2_Step)%(MOTOR_2_Step_Transmition/100))==0)&&(((temp_steps_2+temp_steps_togo_2) - MOTOR_2_Step)<=MOTOR_2_Step_Transmition)&&(i_2>=2))
							{
								i_2--;
								Motor_Speed(MOTOR_2,i_2);
							}
						
						if(Motor_Get_ENABLE(MOTOR_2)==DISABLE)
						{
							temp_steps_togo_2 = 0;
							temp_steps_2 = MOTOR_2_Step;
							return 0;
						}
					}
					else
					{
					MOTOR_Enable(MOTOR_2,DISABLE);
					return 1;
					}
				}
				
				else
				{
					MOTOR_Direction(MOTOR_2,LEFT);
					
					
					if(MOTOR_2_Step != temp_steps_2 - temp_steps_togo_2) 
					{
						MOTOR_Enable(MOTOR_2,ENABLE);
					
							if((((temp_steps_2 - MOTOR_2_Step )%(MOTOR_2_Step_Transmition/100))==0)&&((temp_steps_2 - MOTOR_2_Step )<=MOTOR_2_Step_Transmition)&&(i_2<=100))
							{
								i_2++;
								Motor_Speed(MOTOR_2,i_2);
							}
							if(((( MOTOR_2_Step - (temp_steps_2-temp_steps_togo_2))%(MOTOR_2_Step_Transmition/100))==0)&&(( MOTOR_2_Step - (temp_steps_2-temp_steps_togo_2))<=MOTOR_2_Step_Transmition)&&(i>=2))
							{
								i_2--;
								Motor_Speed(MOTOR_2,i_2);
							}
						
						
						if(Motor_Get_ENABLE(MOTOR_2)==DISABLE)
						{
							temp_steps_togo_2 = 0;
							temp_steps_2 = MOTOR_2_Step;
							return 0;
						}
					}
					else
					{
					MOTOR_Enable(MOTOR_2,DISABLE);
					return 1;
					}
				}
			}
		
		
			
	}
	return 0;
}
