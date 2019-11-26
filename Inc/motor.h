#include "main.h"

typedef enum 
{
	MOTOR_1,
	MOTOR_2
} motor_num;

typedef enum 
{
	LEFT,
	RIGHT,
	UP,
	DOWN
} motor_direction;

void MOTOR_ToggleDir(motor_num motor);
_Bool MOTOR_Direction (motor_num motor, motor_direction dir);
void MOTOR_Enable (motor_num motor,FunctionalState state);
void Motor_Speed (motor_num motor,uint8_t percent, uint8_t mult);
_Bool Motor_to_Switch (motor_num motor, motor_direction dir, int speed);
_Bool Motor_Calibration (motor_num motor);
FunctionalState Motor_Get_ENABLE (motor_num motor);
motor_direction Motor_Get_Dir (motor_num motor);
_Bool Motor_step (motor_num motor,uint32_t step,int sign, uint8_t mult);

