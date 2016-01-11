#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>     //sleep
#include "bitlit.h"

#include "i2c.h"
#include "PCF8563.h"

int main(void) {
    _i2c_t      i2c;
    _tm         tm;
    int         i;

    int         rtc;



    /* Open the i2c-0 bus */
     rtc = open_tm(I2C_BUSS, PCF8583_ADDRESS);

    uint8_t  b[4] = {1,2,3,4};

    tm.tm_hour = 23;
    tm.tm_min = 59;
    tm.tm_sec = 55;
    tm.tm_mon = 5;
    tm.tm_mday = 23;
    tm.tm_year = 16;
    tm.tm_wday = 4;
    printf("\n setting rtc to:  %02i:%02i:%02i  %02i/%02i/%02i  dow %i\n",
    tm.tm_hour,tm.tm_min,tm.tm_sec,tm.tm_mon,tm.tm_mday,tm.tm_year,tm.tm_wday);
    set_tm(rtc,&tm,&i2c);
    // sleep(10);

    for(i=0; i<120; i++){
        get_tm(rtc,&tm,&i2c);
        sleep(1);
        printf("         after set  %02i:%02i:%02i  %02i/%02i/%02i  dow %i\n",
            tm.tm_hour,tm.tm_min,tm.tm_sec,tm.tm_mon,tm.tm_mday,tm.tm_year,tm.tm_wday);
  
    }
    i2c_close(&i2c);
    return 0;
}