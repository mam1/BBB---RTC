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

int file;
char *filename = "/dev/i2c-1";
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



uint8_t bcd_to_decimal(uint8_t d)
{
    return ((d & 0x0F) + (((d & 0xF0) >> 4) * 10)); 
}

int main(void) {

	printf("\n\ni2c test program\n\n");

	if((file = open(filename, O_RDWR)) < 0) {
	    /* ERROR HANDLING: you can check errno to see what went wrong */
	    perror("Failed to open the i2c bus");
	    exit(1);
	}

	printf("  i2c open worked\n");
	int addr = 0x51;     // The I2C address of the device
	printf("i2c device addres set to %x\n",addr);
	if (ioctl(file, I2C_SLAVE, addr) < 0) {
	    printf("Failed to acquire bus access and/or talk to slave.\n");
	    /* ERROR HANDLING; you can check errno to see what went wrong */
	    exit(1);
	}
	printf("  communication initiated\nstart read\n");

	unsigned char buf[9] = {0};
	int i;
	 
  for(i = 0; i<20; i++){
	// Using I2C Read
	if (read(file,buf,9) != 9) {
	 	/* ERROR HANDLING: i2c transaction failed */
	    printf("Failed to read from the i2c bus: %s.\n", strerror(errno));
	    printf("\n\n");
	    } 
	 else 
	  	printf("  read worked\n");
	 for(i=0;i<10;i++)
	 	printf("    byte %i <%2x>\n",i,buf[i]);
	
	 rtc_time.tm_sec = bcd_to_decimal(buf[2] & 0x7f);
  	 rtc_time.tm_min = bcd_to_decimal(buf[3] & 0x7f);
  	 rtc_time.tm_hour = bcd_to_decimal(buf[4] & 0x3f);
  	 rtc_time.tm_mday = bcd_to_decimal(buf[5] & 0x3f);
  	 rtc_time.tm_mon = bcd_to_decimal(buf[7] & 0x1f) - 1;
  	 rtc_time.tm_year = bcd_to_decimal(buf[8]) + 100;
  	 rtc_time.tm_wday = bcd_to_decimal(buf[6] & 0x7f);


	  	 printf("time %i:%i:%i   date %i/%i/%i   dow %i\n",
	  	 	rtc_time.tm_hour, 
	  	 	rtc_time.tm_min, 
	  	 	rtc_time.tm_sec, 
	  	 	rtc_time.tm_mday, 
	  	 	rtc_time.tm_mon, 
	  	 	rtc_time.tm_year, 
	  	 	rtc_time.tm_wday);
	  	 sleep(2);
	}
}
