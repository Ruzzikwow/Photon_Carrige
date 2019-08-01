#include "flash.h"
#include "stdio.h"
#include "ftostr.h"
#include "string.h"



#define SECTOR_9_START_ADDR  0x080C0000
#define SECTOR_9_END_ADDR    0x080DFFFF

#define MAX_CNT_STRING			1001


typedef enum 
{
	bit_8,
	bit_16,
	bit_32

} type_prog;

void flash_write (uint32_t flash_addres, type_prog type_program, uint32_t data );
	


extern uint8_t flag_jornal_full;
uint32_t PAGEError = 0;
static FLASH_EraseInitTypeDef EraseInitStruct;
uint32_t SECTORError = 0;
uint32_t current_flash_addr = SECTOR_9_START_ADDR;

uint16_t nmbr_of_str = 1;
uint16_t tmp_nmbr_of_str = 0;

void flash_write (uint32_t flash_addres, type_prog type_program, uint32_t data )
{
	  EraseInitStruct.TypeErase     = FLASH_TYPEERASE_SECTORS;
		EraseInitStruct.VoltageRange  = FLASH_VOLTAGE_RANGE_3;
		EraseInitStruct.NbSectors     = 1;
		HAL_FLASH_Unlock();
		
		if(flash_addres==SECTOR_9_START_ADDR)
			{
				EraseInitStruct.Sector = FLASH_SECTOR_9;
				HAL_FLASHEx_Erase(&EraseInitStruct, &SECTORError);
			}
		
			switch(type_program)
			{
				case bit_8:
				
					HAL_FLASH_Program(FLASH_TYPEPROGRAM_BYTE, flash_addres, data );
					break;
				
				case bit_16:
					HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, flash_addres, data );
					break;
				
				case bit_32:
					HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, flash_addres, data );
					break;

			}
		
	
	HAL_FLASH_Lock();
}

void flash_write_koef_f (uint32_t  flash_addres, float data_f )
{
	uint8_t  data_tmp[4];
	uint32_t data=0;
	uint8_t *ptr;
	ptr = &data_tmp[0];
	
	*(float*)ptr = data_f;
			
	for(int i=0; i<4; i++)
	{
		data |= data_tmp[i]<<(i*8);
	}
	  EraseInitStruct.TypeErase     = FLASH_TYPEERASE_SECTORS;
		EraseInitStruct.VoltageRange  = FLASH_VOLTAGE_RANGE_3;
		EraseInitStruct.NbSectors     = 1;
		HAL_FLASH_Unlock();
	
	if(flash_addres==Min_line_ADDRESS)
	{
		EraseInitStruct.Sector = FLASH_SECTOR_11;
		HAL_FLASHEx_Erase(&EraseInitStruct, &SECTORError);
	}
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, flash_addres, data );

	
	HAL_FLASH_Lock();
}

void flash_write_koef (uint32_t  flash_addres, int data_f )
{
	uint8_t  data_tmp[4];
	uint32_t data=0;
	uint8_t *ptr;
	ptr = &data_tmp[0];
	
	*(uint32_t*)ptr = data_f;
			
	for(int i=0; i<4; i++)
	{
		data |= data_tmp[i]<<(i*8);
	}
	  EraseInitStruct.TypeErase     = FLASH_TYPEERASE_SECTORS;
		EraseInitStruct.VoltageRange  = FLASH_VOLTAGE_RANGE_3;
		EraseInitStruct.NbSectors     = 1;
		HAL_FLASH_Unlock();
	
	if(flash_addres==Min_line_ADDRESS)
	{
		EraseInitStruct.Sector = FLASH_SECTOR_11;
		HAL_FLASHEx_Erase(&EraseInitStruct, &SECTORError);
	}
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, flash_addres, data );

	
	HAL_FLASH_Lock();
}



