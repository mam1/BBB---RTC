
#include <stdint.h>		//uint_8, uint_16, uint_32, etc.
#include <stdio.h>
#include <stdlib.h>

#include "PCF8563.h"
#include "i2c.h"



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
// This function reads the PCF8563 time/date registers 
// and loads the data into a buffer (reg_buf)

int *get_regs(uint8_t *reg_buf, _tm *tm, _i2c_t *i2c){

    uint8_t msg_addr[2] = { PCF8583_ADDRESS, CNT_REG_1 };
    struct i2c_msg msgs[2] =
        {
            /* Write 16-bit address */
            { .addr = PCF8583_ADDRESS, .flags = 0, .len = 2, .buf = msg_addr },
            /* Read 8-bit data */
            { .addr = PCF8583_ADDRESS, .flags = I2C_M_RD, .len = PCF8563_REGS - 2, .buf = reg_buf + 2},
        };

    /* Transfer a transaction with two I2C messages */
    if (_i2c_transfer(i2c, msgs, 2) < 0) {
        fprintf(stderr, "_i2c_transfer(): %s\n", i2c_errmsg(i2c));
        exit(1);
    }

  return 0;
}

//---------------------------------------------- 
// This function writes all the PCF8563 registers 
int *set_regs(uint8_t *reg_buf, _tm *tm, _i2c_t *i2c){

    uint8_t msg_addr[2] = { PCF8583_ADDRESS, CNT_REG_1 };
    struct i2c_msg msgs[2] =
        {
            /* Write 16-bit address */
            { .addr = PCF8583_ADDRESS, .flags = 0, .len = 2, .buf = msg_addr },
            /* Write 8-bit data */
            { .addr = PCF8583_ADDRESS, .flags = 0, .len = PCF8563_REGS, .buf = reg_buf},
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
  uint8_t   reg_buf[PCF8563_REGS];

  get_regs(reg_buf,tm,i2c);
  
  tm->tm_sec = bcd2bin(reg_buf[SEC_REG] & SEC_MASK);
  tm->tm_min = bcd2bin(reg_buf[MIN_REG] & MIN_MASK);
  tm->tm_hour = bcd2bin(reg_buf[HOUR_REG] & HOUR_MASK);
  tm->tm_mday = bcd2bin(reg_buf[DAY_REG] & DAY_MASK);
  tm->tm_wday = bcd2bin(reg_buf[WDAY_REG] & WDAY_MASK);
  tm->tm_mon = bcd2bin(reg_buf[MON_REG] & MON_MASK);
  tm->tm_year = bcd2bin(reg_buf[YEAR_REG] & YEAR_MASK);

  return 0;
}

//---------------------------------------------- 
// This function loads the values in the date structure
// into the PCF8563.  

int set_tm(_tm *tm, _i2c_t *i2c){
  uint8_t   reg_buf[PCF8563_REGS];

  reg_buf[0] = CNT_REG_1;
  reg_buf[1] = CNT_REG_2;
  reg_buf[SEC_REG] = bin2bcd(tm->tm_sec);     //  SEC_MASK);
  reg_buf[MIN_REG] = bin2bcd(tm->tm_min);     //  MIN_MASK);
  reg_buf[HOUR_REG] = bin2bcd(tm->tm_hour);     //  HOUR_MASK);
  reg_buf[DAY_REG] = bin2bcd(tm->tm_mday );     //  DAY_MASK);
  reg_buf[WDAY_REG] = bin2bcd(tm->tm_wday);     //  WDAY_MASK);
  reg_buf[MON_REG] = bin2bcd(tm->tm_mon);     //  MON_MASK);
  reg_buf[YEAR_REG] = bin2bcd((tm->tm_year) );     //  YEAR_MASK); 
  reg_buf[9] = ALM_REG_MIN;
  reg_buf[10] = ALM_REG_HOUR;
  reg_buf[11] = ALM_REG_DAY;
  reg_buf[12] = ALM_REG_WDAY;
  reg_buf[13] = CLKOUT_REG;
  reg_buf[14] = TIMER_REG_1;
  reg_buf[15] = TIMER_REG_2;

  set_regs(reg_buf,tm,i2c);

  return 0;
}