#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "i2c.h"
#include "PCF8563.h"


#define I2C_BUSS            "/dev/i2c-1"


int main(void) {
    i2c_t   i2c;
    _tm     tm;

    // print_tm(&tm);
    /* Open the i2c-0 bus */
    if (i2c_open(&i2c, I2C_BUSS) < 0) {
        fprintf(stderr, "i2c_open(): %s\n", i2c_errmsg(&i2c));
        exit(1);
    }

    get_tm(&tm);
    // print_tm(&tm);

    i2c_close(&i2c);
    return 0;
}