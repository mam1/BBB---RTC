#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>		//sleep
#include <stdint.h>		//uint_8, uint_16, uint_32, etc.
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
//#include <time.h>

// I2C buss
#define I2C_BUSS 				"/dev/i2c-1"

// Device address
#define PCF8583_WRITE_ADDRESS 	0xa2 
#define PCF8583_READ_ADDRESS  	0xa3

// Register addresses 
#define PCF8583_CTRL_STATUS_REG    0x00 
#define PCF8583_100S_REG           0x01 
#define PCF8583_SECONDS_REG        0x02 
#define PCF8583_MINUTES_REG        0x03 
#define PCF8583_HOURS_REG          0x04 
#define PCF8583_DATE_REG           0x05 
#define PCF8583_MONTHS_REG         0x06 
#define PCF8583_TIMER_REG          0x07 

#define PCF8583_ALARM_CONTROL_REG  0x08 
#define PCF8583_ALARM_100S_REG     0x09 
#define PCF8583_ALARM_SECS_REG     0x0A 
#define PCF8583_ALARM_MINS_REG     0x0B 
#define PCF8583_ALARM_HOURS_REG    0x0C 
#define PCF8583_ALARM_DATE_REG     0x0D 
#define PCF8583_ALARM_MONTHS_REG   0x0E 
#define PCF8583_ALARM_TIMER_REG    0x0F 

int file;
char *filename = I2C_BUSS;

struct tm
{
  int	tm_sec;
  int	tm_min;
  int	tm_hour;
  int	tm_mday;
  int	tm_mon;
  int	tm_year;
  int	tm_wday;
  int	tm_yday;
  int	tm_isdst;
} rtc_time;

//---------------------------------------------- 
// This function converts an 8 bit binary value 
// to an 8 bit BCD value. 
// The input range must be from 0 to 99. 

uint8_t  bin2bcd(uint8_t  value) 
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
// This function converts an 8 bit BCD value to 
// an 8 bit binary value. 
// The input range must be from 00 to 99. 

char bcd2bin(char bcd_value) 
{ 
char temp; 

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
// This function opens an i2c buss.  
//  

int my_i2c_open(char *bussname){
	int 	file;
	printf("opening %s\n",bussname);
	if((file = open(bussname, O_RDWR)) < 0) {
	    /* ERROR HANDLING: you can check errno to see what went wrong */
	    perror("Failed to open the i2c bus\n");
//	    perror(strerror(errno));
	    exit(1);
	}
	if (ioctl(file, I2C_SLAVE, (PCF8583_READ_ADDRESS>>1)) < 0) {
	    printf("Failed to acquire bus access and/or talk to slave\n    erro = %i\n",errno);
	    /* ERROR HANDLING; you can check errno to see what went wrong */
	    exit(1);
	}
	return file;
}

//---------------------------------------------- 
// This function writes x bytes to i2c buss  
// from buf

int i2c_write(int file,uint8_t *buf,int cnt){
	if (write(file,buf,cnt) != cnt) {
	 	/* ERROR HANDLING: i2c transaction failed */
	    printf("Failed to write to the i2c bus: %s.\n", strerror(errno));
	    printf("\n\n");
	    return 1;
	    } 
	 else 
	return 0;
}


//---------------------------------------------- 
// This function reads x bytes from i2c buss  
// 

int i2c_read(int file,char *buf,int cnt){
	if (read(file,buf,cnt) != cnt) {
	 	/* ERROR HANDLING: i2c transaction failed */
	    printf("Failed to write to the i2c bus: %s.\n", strerror(errno));
	    printf("\n\n");
	    return 1;
	    } 
	 else 
	return 0;
}


int xxmain(void) {

	unsigned char buf[9] = {0};
	int i;
	uint8_t 	r_addr, w_addr, bbb;
	int 		file;

	r_addr = PCF8583_READ_ADDRESS>>1;
	w_addr = PCF8583_WRITE_ADDRESS>>1;


	printf("\n\ni2c test program\n\n");
	file = my_i2c_open(I2C_BUSS);
	printf("  i2c open worked\n");
	printf("  communication initiated\nstart read loop\n"); 


  	for(i = 0; i<70; i++){
	// Using I2C Read
	i2c_write(file,&w_addr,1);
	bbb = 0;
	i2c_write(file,&bbb,1);   // Start at seconds reg.
	i2c_write(file,&r_addr,1);
	if (read(file,buf,9) != 9) {
	 	/* ERROR HANDLING: i2c transaction failed */
	    printf("Failed to read from the i2c bus: %s.\n", strerror(errno));
	    printf("\n\n");
	    } 
	 else {
		rtc_time.tm_sec = bcd2bin(buf[2] & 0x3f);
		rtc_time.tm_min = bcd2bin(buf[3] & 0x3f);
		rtc_time.tm_hour = bcd2bin(buf[4] & 0x1f);
		rtc_time.tm_mday = bcd2bin(buf[5] & 0x1f);
		rtc_time.tm_mon = bcd2bin(buf[7] & 0xf) - 1;
		rtc_time.tm_year = bcd2bin(buf[8]) + 2000;
		rtc_time.tm_wday = bcd2bin(buf[6] & 0x3);
		printf("time %02i:%02i:%02i \t date %02i/%02i/%02i  \t dow %i\n",
			  	 	rtc_time.tm_hour, 
			  	 	rtc_time.tm_min, 
			  	 	rtc_time.tm_sec, 
			  	 	rtc_time.tm_mday, 
			  	 	rtc_time.tm_mon, 
			  	 	rtc_time.tm_year, 
			  	 	rtc_time.tm_wday);
		sleep(1);
	}
}
	return 0;
}
