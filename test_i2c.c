

#include <errno.h>
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

#include "bitlit.h"
#include "PCF8563.h"
#include "gpio.h"

void led_set(int led_num, int state){

    char                    set_state[2];
    unsigned int            gpio[4] = {66,69,68,67}; //map gpio to led
    char                    filename[100];
    int                     gpio_fd;

    set_state[1] = 0;
    if(state)
        set_state[0] = '0';
    else
        set_state[0] = '1';

    gpio_export(gpio[led_num]);
    gpio_set_dir(gpio[led_num], 1);
    snprintf(filename,sizeof(filename),"/sys/class/gpio/gpio%d/value",gpio[led_num]);
    gpio_fd = open(filename,O_WRONLY);
    write(gpio_fd,set_state,2);
    close(gpio_fd);
        
    return;
}


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

    // init_alm(rtc);
    // reset_alm(rtc);
    // i = 0;
    // while(1){

    //     get_tm(rtc,&tm);
    //     printf("  %02i:%02i:%02i  %02i/%02i/%02i  dow %i\n",
    //         tm.tm_hour,tm.tm_min,tm.tm_sec,tm.tm_mon,tm.tm_mday,tm.tm_year,tm.tm_wday);
    //     printf("  %i  ",i++);
    //     test_alm(rtc);
    //     sleep(1);
    // }

    int             h_sec, h_min;
    int             on_led;

    on_led = 0;
    led_set(0,1);
    led_set(1,0);
    led_set(2,0);
    led_set(3,0);
    sleep(1);

    while(1){
        get_tm(rtc,&tm);
        if(h_min != tm.tm_min){
            h_min = tm.tm_min;
            printf("  %02i:%02i:%02i  %02i/%02i/%02i  dow %i\n",
                tm.tm_hour,tm.tm_min,tm.tm_sec,tm.tm_mon,tm.tm_mday,tm.tm_year,tm.tm_wday);
            // process control table 

        }
        /* cycle leds */
        led_set(on_led,0);
        on_led += 1;
        if(on_led > 3) on_led = 0;
        led_set(on_led,1);
        sleep(1);

    }

    close(rtc);
    return 0;
}