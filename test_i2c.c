#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>     //sleep
#include "bitlit.h"

#include "i2c.h"
#include "PCF8563.h"



int main(void) {
    _i2c_t  i2c;
    _tm     tm;
    int i;

    /* Open the i2c-0 bus */
    if (i2c_open(&i2c, I2C_BUSS) < 0) {
        fprintf(stderr, "i2c_open(): %s\n", i2c_errmsg(&i2c));
        exit(1);
    }
    for(i=0; i<20; i++){
        get_tm(&tm,&i2c);
        printf("  0:%02i:%02i  0/0/0  dow 0\n",tm.tm_min,tm.tm_sec);
        sleep(2);
    }
    i2c_close(&i2c);
    return 0;
}