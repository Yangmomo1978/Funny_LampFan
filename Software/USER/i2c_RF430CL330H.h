#ifndef _I2C_RF430CL330H_H
#define _I2C_RF430CL330H_H

#include "stm32f10x.h"

#define RF430CL330H_ADDR 0x50 

//Standard Bits
#define 	BIT0					(0x0001)
#define		BIT1					(0x0002)
#define		BIT2					(0x0004)
#define		BIT3					(0x0008)
#define		BIT4					(0x0010)
#define		BIT5					(0x0020)
#define		BIT6					(0x0040)
#define		BIT7					(0x0080)
#define		BIT8					(0x0100)
#define		BIT9					(0x0200)
#define		BITA					(0x0400)
#define		BITB					(0x0800)
#define		BITC					(0x1000)
#define		BITD					(0x2000)
#define		BITE					(0x4000)
#define		BITF					(0x8000)

//define the values for Granite's registers we want to access
#define CONTROL_REG 		0xFFFE
#define STATUS_REG			0xFFFC
#define INT_ENABLE_REG		0xFFFA
#define INT_FLAG_REG		0xFFF8
#define CRC_RESULT_REG		0xFFF6
#define CRC_LENGTH_REG		0xFFF4
#define CRC_START_ADDR_REG	0xFFF2
#define COMM_WD_CTRL_REG	0xFFF0
#define VERSION_REG			0xFFEE //contains the software version of the ROM
#define TEST_FUNCTION_REG   0xFFE2
#define TEST_MODE_REG		0xFFE0

//define the different virtual register bits
//CONTROL_REG bits
#define SW_RESET		BIT0
#define RF_ENABLE		BIT1
#define INT_ENABLE		BIT2
#define INTO_HIGH		BIT3
#define INTO_DRIVE		BIT4
#define BIP8_ENABLE		BIT5
#define STANDBY_ENABLE	BIT6
#define TEST430_ENABLE	BIT7
//STATUS_REG bits
#define READY			BIT0
#define CRC_ACTIVE		BIT1
#define RF_BUSY			BIT2
//INT_ENABLE_REG bits
#define EOR_INT_ENABLE		BIT1
#define EOW_INT_ENABLE		BIT2
#define CRC_INT_ENABLE		BIT3
#define BIP8_ERROR_INT_ENABLE	BIT4
#define NDEF_ERROR_INT_ENABLE	BIT5
#define GENERIC_ERROR_INT_ENABLE	BIT7
//INT_FLAG_REG bits
#define EOR_INT_FLAG	BIT1
#define EOW_INT_FLAG	BIT2
#define CRC_INT_FLAG	BIT3
#define BIP8_ERROR_INT_FLAG	BIT4
#define NDEF_ERROR_INT_FLAG	BIT5
#define GENERIC_ERROR_INT_FLAG	BIT7
//COMM_WD_CTRL_REG bits
#define WD_ENABLE	BIT0
#define TIMEOUT_PERIOD_2_SEC	0
#define TIMEOUT_PERIOD_32_SEC	BIT1
#define TIMEOUT_PERIOD_8_5_MIN	BIT2
#define TIMEOUT_PERIOD_MASK		BIT1 + BIT2 + BIT3
#define TEST_MODE_KEY           0x004E



/*Functions*/
unsigned int Read_Register(unsigned int reg_addr);
void Read_Continuous(unsigned int reg_addr, unsigned char* read_data, unsigned int data_length);
void Write_Register(unsigned int reg_addr, unsigned int value);
void Write_Continuous(unsigned int reg_addr, unsigned char* write_data, unsigned int data_length);

#endif