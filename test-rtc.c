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


#define I2C_BUSS 				"/dev/i2c-1"
// #define PCF8583_WRITE_ADDRESS 	0xA0 
// #define PCF8583_READ_ADDRESS  	0xA1

#define PCF8583_WRITE_ADDRESS 	0x50 
#define PCF8583_READ_ADDRESS  	0x51

int file;
char *filename = I2C_BUSS;
uint8_t buf[16];

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

void i2c_open(void){
	char *bussname = I2C_BUSS;
	if((file = open(bussname, O_RDWR)) < 0) {
	    /* ERROR HANDLING: you can check errno to see what went wrong */
	    perror("Failed to open the i2c bus");
	    exit(1);
	}

	return;
}


int main(void) {



	printf("\n\n*** i2c test program ***\n\n");
	int file;
	char *filename = "/dev/i2c-1";
	if ((file = open(filename, O_RDWR)) < 0) {
	    /* ERROR HANDLING: you can check errno to see what went wrong */
	    perror("Failed to open the i2c bus");
	    exit(1);
	}
	printf("  i2c open worked\n");
	int addr = PCF8583_READ_ADDRESS;     // The I2C address of the device
	printf("  i2c device addres set to %x\n",addr);
	if (ioctl(file, I2C_SLAVE, addr) < 0) {
	    printf("Failed to acquire bus access and/or talk to slave.\n");
	    /* ERROR HANDLING; you can check errno to see what went wrong */
	    exit(1);
	}
	printf("  communication initiated\n  start read\n");

	unsigned char buf[9] = {0};
	unsigned char rbuf[1] = {0x02};

	printf("  register addres set to %i\n  read time\n",rbuf[0]);
	int i;
	// char	buffer[100];
	 
  	for(i = 0; i<10; i++){
  		// select register
	  	if (write(file,rbuf,1) != 1) {
		    /* ERROR HANDLING: i2c transaction failed */
		    printf("Failed to write to the i2c bus.\n");
		    // buffer = g_strerror(errno);
		    // printf(buffer);
		    printf("\n\n");
		}
		// read registers
		if (read(file,buf,7) != 7) {
		 	/* ERROR HANDLING: i2c transaction failed */
		    printf("Failed to read from the i2c bus: %s.\n", strerror(errno));
		    printf("\n\n");
		} 
		else {
			rtc_time.tm_sec = bcd2bin(buf[0] & 0x7f);
		  	rtc_time.tm_min = bcd2bin(buf[1] & 0x7f);
		  	rtc_time.tm_hour = bcd2bin(buf[2] & 0x3f);
		  	rtc_time.tm_mday = bcd2bin(buf[3] & 0x3f);
		  	rtc_time.tm_mon = bcd2bin(buf[4] & 0x1f) - 1;
		  	rtc_time.tm_year = bcd2bin(buf[5]) + 100;
		  	rtc_time.tm_wday = bcd2bin(buf[6] & 0x3);

		  	printf("    time %02i:%02i:%02i \t date %02i/%02i/%02i  \t dow %i\n",
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

	rtc_time.tm_sec = 57;
  	rtc_time.tm_min = 59;
  	rtc_time.tm_hour = 23;
  	rtc_time.tm_mday = 30;
  	rtc_time.tm_mon = 3;
  	rtc_time.tm_year = 16;
  	rtc_time.tm_wday = 4;

	printf("  set time to: ");
  	printf("time %02i:%02i:%02i \t date %02i/%02i/%02i  \t dow %i\n",
	 	rtc_time.tm_hour, 
	 	rtc_time.tm_min, 
	 	rtc_time.tm_sec, 
	 	rtc_time.tm_mday, 
	 	rtc_time.tm_mon, 
	 	rtc_time.tm_year, 
	 	rtc_time.tm_wday);

  //unsigned char reg = 0x10; // Device register to access
//buf[0] = reg;

buf[0] = 0x02;
// if (write(file,buf,1) != 1) {
//     /* ERROR HANDLING: i2c transaction failed */
//     printf("Failed to write to the i2c bus.\n");
//     // buff = g_strerror(errno);
//     // printf(buffer);
//     printf("\n\n");
// }

buf[1] = bin2bcd(rtc_time.tm_sec);
buf[2] = bin2bcd(rtc_time.tm_min);
buf[3] = bin2bcd(rtc_time.tm_hour);

buf[4] = bin2bcd(rtc_time.tm_mday);
buf[6] = bin2bcd(rtc_time.tm_mon);
buf[7] = bin2bcd(rtc_time.tm_year);
buf[5] = bin2bcd(rtc_time.tm_wday);

if(write(file,buf,8) != 8){
    printf("Failed to write to the i2c bus.\n");
	printf("\n\n");
}

printf("  set worked\n");

  	for(i = 0; i<10; i++){
  		// select register
	  	if (write(file,rbuf,1) != 1) {
		    /* ERROR HANDLING: i2c transaction failed */
		    printf("Failed to write to the i2c bus.\n");
		    // buffer = g_strerror(errno);
		    // printf(buffer);
		    printf("\n\n");
		}
		// read registers
		if (read(file,buf,7) != 7) {
		 	/* ERROR HANDLING: i2c transaction failed */
		    printf("Failed to read from the i2c bus: %s.\n", strerror(errno));
		    printf("\n\n");
		} 
		else {
			rtc_time.tm_sec = bcd2bin(buf[0] & 0x7f);
		  	rtc_time.tm_min = bcd2bin(buf[1] & 0x7f);
		  	rtc_time.tm_hour = bcd2bin(buf[2] & 0x3f);
		  	rtc_time.tm_mday = bcd2bin(buf[3] & 0x3f);
		  	rtc_time.tm_mon = bcd2bin(buf[4] & 0x1f) - 1;
		  	rtc_time.tm_year = bcd2bin(buf[5]) + 100;
		  	rtc_time.tm_wday = bcd2bin(buf[6] & 0x3);

		  	printf("    time %02i:%02i:%02i \t date %02i/%02i/%02i  \t dow %i\n",
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





  	printf("\nnormal termination\n\n");

	return 0;
}
