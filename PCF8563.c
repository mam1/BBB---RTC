
#include <stdint.h>		//uint_8, uint_16, uint_32, etc.
#include <stdio.h>
#include <stdlib.h>

#include "PCF8563.h"
#include "i2c.h"

uint8_t   reg_buf[PCF8563_REGS];

//---------------------------------------------- 
// Convert an 8 bit binary value to an 8 bit BCD value
static uint8_t  bin2bcd(uint8_t  value) 
{ 
char retval; 

retval = 0; 

while(1) 
  { 
   // Get the tens digit by doing multiple subtraction 
   // of 10 from the binary value. 
   if(value >= 10) 
     { 
      value -= 10; 
      retval += 0x10; 
     } 
   else // Get the ones digit by adding the remainder. 
     { 
      retval += value; 
      break; 
     } 
   } 

return(retval); 
} 

//---------------------------------------------- 
// Convert an 8 bit BCD value to an 8 bit binary value
static uint8_t bcd2bin(uint8_t bcd_value) 
{ 
uint8_t temp; 

temp = bcd_value; 

// Shifting the upper digit right by 1 is 
// the same as multiplying it by 8. 
temp >>= 1; 

// Isolate the bits for the upper digit. 
temp &= 0x78; 

// Now return: (Tens * 8) + (Tens * 2) + Ones 
return(temp + (temp >> 2) + (bcd_value & 0x0f)); 
}

//---------------------------------------------- 
// This function reads a byte of data from 
// a PCF8563 register 
 

uint8_t get_byte(uint8_t reg, _tm *tm, _i2c_t *i2c){

	/* Read byte at address 0x100 of EEPROM */
    uint8_t msg_addr[2] = { PCF8583_ADDRESS, reg };
    uint8_t msg_data[1] = { 0x00, };
    struct i2c_msg msgs[2] =
        {
            /* Write 16-bit address */
            { .addr = PCF8583_ADDRESS, .flags = 0, .len = 2, .buf = msg_addr },
            /* Read 8-bit data */
            { .addr = PCF8583_ADDRESS, .flags = I2C_M_RD, .len = 1, .buf = msg_data},
        };

    /* Transfer a transaction with two I2C messages */
    if (_i2c_transfer(i2c, msgs, 2) < 0) {
        fprintf(stderr, "_i2c_transfer(): %s\n", i2c_errmsg(i2c));
        exit(1);
    }
	return msg_data[0];
}

//---------------------------------------------- 
// This function reads the PCF8563 data registers 
// and loads the data into a buffer (reg_buf)

int *get_regs(uint8_t *reg_buf, _tm *tm, _i2c_t *i2c){

    uint8_t msg_addr[2] = { PCF8583_ADDRESS, 0x02 };
    struct i2c_msg msgs[2] =
        {
            /* Write 16-bit address */
            { .addr = PCF8583_ADDRESS, .flags = 0, .len = 2, .buf = msg_addr },
            /* Read 8-bit data */
            { .addr = PCF8583_ADDRESS, .flags = I2C_M_RD, .len = 8, .buf = reg_buf},
        };

    /* Transfer a transaction with two I2C messages */
    if (_i2c_transfer(i2c, msgs, 2) < 0) {
        fprintf(stderr, "_i2c_transfer(): %s\n", i2c_errmsg(i2c));
        exit(1);
    }

  return 0;
}

//---------------------------------------------- 
// This function writes the PCF8563 data registers 
int *set_regs(uint8_t *reg_buf, _tm *tm, _i2c_t *i2c){

    uint8_t msg_addr[2] = { PCF8583_ADDRESS, 0x00 };
    struct i2c_msg msgs[2] =
        {
            /* Write 16-bit address */
            { .addr = PCF8583_ADDRESS, .flags = 0, .len = 2, .buf = msg_addr },
            /* Write 8-bit data */
            { .addr = PCF8583_ADDRESS, .flags = 0, .len = 15, .buf = reg_buf},
        };

    /* Transfer a transaction with two I2C messages */
    if (_i2c_transfer(i2c, msgs, 2) < 0) {
        fprintf(stderr, "_i2c_transfer(): %s\n", i2c_errmsg(i2c));
        exit(1);
    }

  return 0;
}

//---------------------------------------------- 
// This function loads the time date structure
// from the register buffer.  It masks unwanted
// bits and converts data from BCD to binary 

int get_tm(_tm *tm, _i2c_t *i2c){

  get_regs(reg_buf,tm,i2c);
  
  tm->tm_sec = bcd2bin(reg_buf[0] & SEC_MASK);
  tm->tm_min = bcd2bin(reg_buf[1] & MIN_MASK);
  tm->tm_hour = bcd2bin(reg_buf[2] & HOUR_MASK);
  tm->tm_mday = bcd2bin(reg_buf[3] & DAY_MASK);
  tm->tm_wday = bcd2bin(reg_buf[4] & WDAY_MASK);
  tm->tm_mon = bcd2bin(reg_buf[5] & MON_MASK);
  tm->tm_year = bcd2bin(reg_buf[6] & YEAR_MASK) + 2000;

  return 0;
}

//---------------------------------------------- 
// This function loads the values in the date structure
// into the PCF8563.  

int set_tm(_tm *tm, _i2c_t *i2c){

  reg_buf[0] = 0;
  reg_buf[1] = 0;
  reg_buf[2] = bin2bcd(tm->tm_sec & SEC_MASK);
  reg_buf[3] = bin2bcd(tm->tm_min & MIN_MASK);
  reg_buf[4] = bin2bcd(tm->tm_hour & HOUR_MASK);
  reg_buf[5] = bin2bcd(tm->tm_mday  & DAY_MASK);
  reg_buf[6] = bin2bcd(tm->tm_wday & WDAY_MASK);
  reg_buf[7] = bin2bcd(tm->tm_mon & MON_MASK);
  reg_buf[8] = bin2bcd((tm->tm_year - 2000)  & YEAR_MASK); 
  reg_buf[9] = 0x80;
  reg_buf[10] = 0x80;
  reg_buf[11] = 0x80;
  reg_buf[12] = 0x80;
  reg_buf[13] = 0;
  reg_buf[14] = 0;


  set_regs(reg_buf,tm,i2c);


  return 0;
}