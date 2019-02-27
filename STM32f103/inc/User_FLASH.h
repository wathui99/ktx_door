/*
-	in this file I only use 1 page of flash start from 0x8019000
*/
#ifndef __USER_FLASH_H
#define __USER_FLASH_H

#include "stm32f10x.h"
#include "stm32f10x_flash.h"

static uint32_t startAddress = 0x800C800; //PAGE_0 start from 0x0800_0000 => start from 50KB
/*PAGE_SIZE is 1KB*/

/*
-	use to erase the flash page
-	need to call everytime want to write.
*/
void User_FLASH_Erase(uint8_t page);

/*
- write data at address (startAddress + 1024*page + 2*pos)
- maximum pos is 511. 1KB=1024. 1024/2 = 512
*/
void User_FLASH_Write(uint16_t data, uint16_t pos, uint8_t page);

/*
- read data at address (startAddress + 1024*page + 2*pos)
- maximum pos is 511. 1KB=1024. 1024/2 = 512
*/
uint16_t User_FLASH_Read(uint16_t pos, uint8_t page);

#endif /*__USER_FLASH_H */
/*******END OF FILE****/
