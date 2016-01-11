
/*
 * In the routines that deal directly with the pcf8563 hardware, we use
 * rtc_time -- month 0-11, hour 0-23, yr = calendar year-epoch.
 */

#include <errno.h>
#include <stdint.h>		//uint_8, uint_16, uint_32, etc.
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <linux/i2c-dev.h>
#include <string.h>
#include <unistd.h>    //sleep
#include <stdint.h>   //uint_8, uint_16, uint_32, etc.

#include "PCF8563.h"
#include "i2c.h"

int c_polarity; /* 0: MO_C=1 means 19xx, otherwise MO_C=1 means 20xx */

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
// open the i2c buss connected to the PCF8563
int open_tm(char *filename, uint8_t addr){
  int     fn;

  if ((fn = open(filename, O_RDWR)) < 0) {
      /* ERROR HANDLING: you can check errno to see what went wrong */
      perror("Failed to open the i2c bus");
      exit(1);
  }
  printf("  i2c open worked\n");

  printf("  i2c device addres set to %x\n",addr);
  if (ioctl(fn, I2C_SLAVE, addr) < 0) {
      printf("Failed to acquire bus access and/or talk to slave.\n");
      /* ERROR HANDLING; you can check errno to see what went wrong */
      exit(1);
  }
  printf("  communication initiated\n  start read\n");

  return fn;
}

//---------------------------------------------- 
// This function loads the time date structure
// from the PCF8563 register buffer
int get_tm(int rtc, _tm *tm, _i2c_t *i2c){
  uint8_t   reg_buf[PCF8563_REGS];

  // select register
  reg_buf[0] = SEC_REG;
  if (write(rtc,reg_buf,1) != 1) {
    /* ERROR HANDLING: i2c transaction failed */
    printf("Failed to write to the i2c bus.\n");
    // buffer = g_strerror(errno);
    // printf(buffer);
    printf("\n\n");
  }
  // read registers
  if (read(rtc,reg_buf,7) != 7) {
    /* ERROR HANDLING: i2c transaction failed */
    printf("Failed to read from the i2c bus: %s.\n", strerror(errno));
    printf("\n\n");
    } 
  else {
    tm->tm_sec = bcd2bin(reg_buf[0] & 0x7f);
    tm->tm_min = bcd2bin(reg_buf[1] & 0x7f);
    tm->tm_hour = bcd2bin(reg_buf[2] & 0x3f);
    tm->tm_mday = bcd2bin(reg_buf[3] & 0x3f);
    tm->tm_wday = bcd2bin(reg_buf[4] & 0x3);
    tm->tm_mon = bcd2bin(reg_buf[5] & 0x0f);
    tm->tm_year = bcd2bin(reg_buf[6]);
  }
  return 0;

  return 0;
}

//---------------------------------------------- 
// This function loads the values in the date structure
// into the PCF8563.  

int set_tm(int rtc,_tm *tm, _i2c_t *i2c){
  uint8_t   reg_buf[PCF8563_REGS];

  reg_buf[0] = SEC_REG;

  reg_buf[1] = bin2bcd(tm->tm_sec);
  reg_buf[2] = bin2bcd(tm->tm_min);
  reg_buf[3] = bin2bcd(tm->tm_hour);

  reg_buf[4] = bin2bcd(tm->tm_mday);
  reg_buf[5] = bin2bcd(tm->tm_wday);

  reg_buf[6] = bin2bcd(tm->tm_mon);
  reg_buf[7] = bin2bcd(tm->tm_year);

  if(write(rtc,reg_buf,8) != 8){
      printf("Failed to write to the i2c bus.\n");
    printf("\n\n");
  }

  return 0;
}