#include "User_FLASH.h"

void User_FLASH_Erase(uint8_t page) {
	FLASH_Unlock();
	FLASH_ClearFlag(FLASH_FLAG_EOP|FLASH_FLAG_PGERR|FLASH_FLAG_WRPRTERR);
  FLASH_ErasePage(startAddress+page*1024);
	FLASH_Lock();
}

void User_FLASH_Write(uint16_t data, uint16_t pos, uint8_t page) {
	
	/*-----------unlock flash before write-----------*/
	FLASH_Unlock();
	/*-----------erase flash before write-----------*/
	//User_FLASH_Erase();
	/*-----------starting write-----------*/
	/*
	*1024/page, moi lan ghi la 2bytes
	*/
	FLASH_ProgramHalfWord(startAddress + page*1024 + pos*2,data);
	/*-----------lock flash-----------*/
	FLASH_Lock();
}

uint16_t User_FLASH_Read(uint16_t pos, uint8_t page) {
	return *(uint16_t*)(startAddress + page*1024 + pos*2);
}
