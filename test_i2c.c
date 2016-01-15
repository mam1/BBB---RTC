#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>     //sleep
#include "bitlit.h"

// #include "i2c.h"
#include "PCF8563.h"

int main(void) {
    // _i2c_t      i2c;
    _tm         tm;
    int         i;
    int         rtc;

    /* Open the i2c-0 bus */
    rtc = open_tm(I2C_BUSS, PCF8583_ADDRESS);

    /* Set the clock */
    // tm.tm_hour = 23;
    // tm.tm_min = 59;
    // tm.tm_sec = 55;
    // tm.tm_mon = 11;
    // tm.tm_mday = 31;
    // tm.tm_year = 2010;
    // tm.tm_wday = 1;
    // printf("\n  setting rtc to:   %02i:%02i:%02i  %02i/%02i/%02i  dow %i\n",
    // tm.tm_hour,tm.tm_min,tm.tm_sec,tm.tm_mon,tm.tm_mday,tm.tm_year,tm.tm_wday);
    // set_tm(rtc,&tm);




    // for(i=0; i<12; i++){
    //     get_tm(rtc,&tm);
    //     sleep(1);
    //     printf("  %02i:%02i:%02i  %02i/%02i/%02i  dow %i\n",
    //         tm.tm_hour,tm.tm_min,tm.tm_sec,tm.tm_mon,tm.tm_mday,tm.tm_year,tm.tm_wday);
    // }

    // init_alm(rtc);
    // reset_alm(rtc);
    // while(1){
    //     if(test_alm(rtc)){
    //         reset_alm(rtc);
    //         get_tm(rtc,&tm);
    //         printf("\n  %02i:%02i:%02i  %02i/%02i/%02i  dow %i\n",
    //             tm.tm_hour,tm.tm_min,tm.tm_sec,tm.tm_mon,tm.tm_mday,tm.tm_year,tm.tm_wday);
    //    }
    //    printf("  .");
    //    sleep(1);
    // }

    init_alm(rtc);
    reset_alm(rtc);
    while(1){
        test_alm(rtc);
        sleep(1);
    }



    close(rtc);
    return 0;
}