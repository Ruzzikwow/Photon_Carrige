#include "motor.h"
#include "tim.h"

 uint8_t DevState;												//0x01 UP
																					//0x02 DOWN
																					//0x04 LEFT
																					//0x08 RIGHT
int MOTOR_1_Step_Transmition	= 0x00002FFF;
int MOTOR_2_Step_Transmition	= 0x00002FFF;
 int MOTOR_1_Step;

 int MOTOR_2_Step;
 
 _Bool MOTOR_1_STEP_ERROR;
 _Bool MOTOR_2_STEP_ERROR;
 
 extern _Bool MOTOR_1_CALIBRATION;
 extern _Bool MOTOR_2_CALIBRATION;
 
 uint32_t temp_steps;

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
			htim3.Instance->CCR3 = (htim3.Instance->ARR)*0.99;
			break;
		case MOTOR_2:
			
			htim8.Instance->ARR = 1130 - percent*10;
			htim8.Instance->CCR2 = (htim8.Instance->ARR)*0.99;
			break;
	}
}

_Bool Motor_Calibration (motor_num motor)
{
	
	switch (motor)
	{
		case MOTOR_1:
				
				MOTOR_1_CALIBRATION=1;
				MOTOR_1_Step = 0;
				if(Motor_to_Switch(motor,UP))
				{
						if(Motor_to_Switch(motor,DOWN))
						{
							MOTOR_1_CALIBRATION=0;
							MOTOR_1_STEP_ERROR=0;	
							return 1;
						}
						else {return 0;}

				}
				else {return 0;}

		case MOTOR_2:
			MOTOR_2_Step = 0;
			MOTOR_2_CALIBRATION=1;
			if(Motor_to_Switch(motor,LEFT))
				{
						if(Motor_to_Switch(motor,RIGHT))
						{
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

_Bool Motor_to_Switch (motor_num motor, motor_direction dir)
{
	
	Motor_Speed(motor,100);
	if(MOTOR_Direction(motor,dir))
	{
		switch (motor)
		{
			case MOTOR_1:
				switch (dir)
				{
					case UP:
						MOTOR_Enable(motor,ENABLE);
						while((DevState&0x01)!=0x01) 
						{
								if(Motor_Get_ENABLE(MOTOR_1)==DISABLE)
							{
							
							}
						}
						MOTOR_Enable(motor,DISABLE);
						return 1;
					
					case DOWN:
						
						MOTOR_Enable(motor,ENABLE);
						while((DevState&0x02)!=0x02)
							{
								if(Motor_Get_ENABLE(MOTOR_1)==DISABLE)
								{
									
								}
							}
						MOTOR_Enable(motor,DISABLE);
						return 1;
					
						
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
						while((DevState&0x04)!=0x04)
						{
							if(Motor_Get_ENABLE(MOTOR_2)==DISABLE)
							{
							
							}
						}
						MOTOR_Enable(motor,DISABLE);
						return 1;
					
					case RIGHT:
						MOTOR_Enable(motor,ENABLE);
						while((DevState&0x08)!=0x08)
						{
							if(Motor_Get_ENABLE(MOTOR_2)==DISABLE)
							{
							
							}
						}
						MOTOR_Enable(motor,DISABLE);
						return 1;
					
						
					case UP:
						return 0;
				
					case DOWN:
						return 0;
			
				}
				
				break;
		}
		
		return 1;
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
			
			temp_steps = MOTOR_1_Step;
		
			if(step < 2 * MOTOR_1_Step_Transmition)
			{
				Motor_Speed(MOTOR_1,10);
				if(sign>0)
				{
					MOTOR_Direction(MOTOR_1,DOWN);
					MOTOR_Enable(MOTOR_1,ENABLE);
					while(MOTOR_1_Step != temp_steps + step)
					{
						if(Motor_Get_ENABLE(MOTOR_1)==DISABLE)
						{
							step = 0;
							temp_steps = MOTOR_1_Step;
							return 0;
						}
					}
					MOTOR_Enable(MOTOR_1,DISABLE);
					return 1;
				}
				else
				{
					MOTOR_Direction(MOTOR_1,UP);
					MOTOR_Enable(MOTOR_1,ENABLE);
					while(MOTOR_1_Step != temp_steps - step)
					{
						if(Motor_Get_ENABLE(MOTOR_1)==DISABLE)
						{
							step = 0;
							temp_steps = MOTOR_1_Step;
							return 0;
						}
					}
					MOTOR_Enable(MOTOR_1,DISABLE);
					return 1;
				}
			}
			else
			{
				Motor_Speed(MOTOR_1,0);
				
				int i=0;
				uint32_t old_stp=0;
				if(sign>0)
				{
					MOTOR_Direction(MOTOR_1,DOWN);
					MOTOR_Enable(MOTOR_1,ENABLE);
					while(MOTOR_1_Step != temp_steps + step)
					{
						
						if(old_stp!=MOTOR_1_Step)
						{
							old_stp = MOTOR_1_Step;
							if((((MOTOR_1_Step - temp_steps)%(MOTOR_1_Step_Transmition/100))==0)&&((MOTOR_1_Step - temp_steps)<=MOTOR_1_Step_Transmition)&&(i<=100))
							{
								i++;
								Motor_Speed(MOTOR_1,i);
							}
							if(((((temp_steps+step) - MOTOR_1_Step)%(MOTOR_1_Step_Transmition/100))==0)&&(((temp_steps+step) - MOTOR_1_Step)<=MOTOR_1_Step_Transmition)&&(i>=2))
							{
								i--;
								Motor_Speed(MOTOR_1,i);
							}
						}
						if(Motor_Get_ENABLE(MOTOR_1)==DISABLE)
						{
							step = 0;
							temp_steps = MOTOR_1_Step;
							return 0;
						}
					}
					MOTOR_Enable(MOTOR_1,DISABLE);
					return 1;
				}
				
				else
				{
					MOTOR_Direction(MOTOR_1,UP);
					
					MOTOR_Enable(MOTOR_1,ENABLE);
					while(MOTOR_1_Step != temp_steps - step)
					{
							if(old_stp!=MOTOR_1_Step)
						{
							old_stp = MOTOR_1_Step;
							if((((temp_steps - MOTOR_1_Step )%(MOTOR_1_Step_Transmition/100))==0)&&((temp_steps - MOTOR_1_Step )<=MOTOR_1_Step_Transmition)&&(i<=100))
							{
								i++;
								Motor_Speed(MOTOR_1,i);
							}
							if(((( MOTOR_1_Step - (temp_steps-step))%(MOTOR_1_Step_Transmition/100))==0)&&(( MOTOR_1_Step - (temp_steps-step))<=MOTOR_1_Step_Transmition)&&(i>=2))
							{
								i--;
								Motor_Speed(MOTOR_1,i);
							}
						}
						
						if(Motor_Get_ENABLE(MOTOR_1)==DISABLE)
						{
							step = 0;
							temp_steps = MOTOR_1_Step;
							return 0;
						}
					}
					MOTOR_Enable(MOTOR_1,DISABLE);
					return 1;
				}
			}
			
			break;
			
		case MOTOR_2:
			
			temp_steps = MOTOR_2_Step;
		
		
		
			if(step < 2 * MOTOR_2_Step_Transmition)
			{
				Motor_Speed(MOTOR_2,10);
				if(sign>0)
				{
					MOTOR_Direction(MOTOR_2,RIGHT);
					MOTOR_Enable(MOTOR_2,ENABLE);
					while(MOTOR_2_Step != temp_steps + step)
					{
						
						if(Motor_Get_ENABLE(MOTOR_2)==DISABLE)
						{
							step = 0;
							temp_steps = MOTOR_2_Step;
							return 0;
						}
						
					}
					MOTOR_Enable(MOTOR_2,DISABLE);
					return 1;
				}
				else
				{
					MOTOR_Direction(MOTOR_2,LEFT);
					MOTOR_Enable(MOTOR_2,ENABLE);
					while(MOTOR_2_Step != temp_steps - step)
					{
						if(Motor_Get_ENABLE(MOTOR_2)==DISABLE)
						{
							step = 0;
							temp_steps = MOTOR_2_Step;
							return 0;
						}
					}
					MOTOR_Enable(MOTOR_2,DISABLE);
					return 1;
				}
			}
			else
			{
				Motor_Speed(MOTOR_2,0);
				
				int i=0;
				uint32_t old_stp=0;
				if(sign>0)
				{
					MOTOR_Direction(MOTOR_2,RIGHT);
					MOTOR_Enable(MOTOR_2,ENABLE);
					while(MOTOR_2_Step != temp_steps + step)
					{
						
						if(old_stp!=MOTOR_2_Step)
						{
							old_stp = MOTOR_2_Step;
							if((((MOTOR_2_Step - temp_steps)%(MOTOR_2_Step_Transmition/100))==0)&&((MOTOR_2_Step - temp_steps)<=MOTOR_2_Step_Transmition)&&(i<=100))
							{
								i++;
								Motor_Speed(MOTOR_2,i);
							}
							if(((((temp_steps+step) - MOTOR_2_Step)%(MOTOR_2_Step_Transmition/100))==0)&&(((temp_steps+step) - MOTOR_2_Step)<=MOTOR_2_Step_Transmition)&&(i>=2))
							{
								i--;
								Motor_Speed(MOTOR_2,i);
							}
						}
						if(Motor_Get_ENABLE(MOTOR_2)==DISABLE)
						{
							step = 0;
							temp_steps = MOTOR_2_Step;
							return 0;
						}
					}
					MOTOR_Enable(MOTOR_2,DISABLE);
					return 1;
				}
				
				else
				{
					MOTOR_Direction(MOTOR_2,LEFT);
					
					MOTOR_Enable(MOTOR_2,ENABLE);
					while(MOTOR_2_Step != temp_steps - step)
					{
							if(old_stp!=MOTOR_2_Step)
						{
							old_stp = MOTOR_2_Step;
							if((((temp_steps - MOTOR_2_Step )%(MOTOR_2_Step_Transmition/100))==0)&&((temp_steps - MOTOR_2_Step )<=MOTOR_2_Step_Transmition)&&(i<=100))
							{
								i++;
								Motor_Speed(MOTOR_2,i);
							}
							if(((( MOTOR_2_Step - (temp_steps-step))%(MOTOR_2_Step_Transmition/100))==0)&&(( MOTOR_2_Step - (temp_steps-step))<=MOTOR_2_Step_Transmition)&&(i>=2))
							{
								i--;
								Motor_Speed(MOTOR_2,i);
							}
						}
						
						if(Motor_Get_ENABLE(MOTOR_2)==DISABLE)
						{
							step = 0;
							temp_steps = MOTOR_2_Step;
							return 0;
						}
					}
					MOTOR_Enable(MOTOR_2,DISABLE);
					return 1;
				}
			}
		
			break;
			
	}
}
