#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "i2c.h"
#include "PCF8563.h"

#define PCF8583_ADDRESS     0x51
#define I2C_BUSS            "/dev/i2c-1"
#define SEC_REG             0x02

int main(void) {
    _i2c_t  i2c;
    _tm     tm;

    /* Open the i2c-0 bus */
    if (i2c_open(&i2c, I2C_BUSS) < 0) {
        fprintf(stderr, "i2c_open(): %s\n", i2c_errmsg(&i2c));
        exit(1);
    }

    get_tm(&tm);

    /* Read byte at address 0x100 of EEPROM */
    uint8_t msg_addr[2] = { PCF8583_ADDRESS, SEC_REG };
    uint8_t msg_data[1] = { 0x00, };
    struct i2c_msg msgs[2] =
        {
            /* Write 16-bit address */
            { .addr = PCF8583_ADDRESS, .flags = 0, .len = 2, .buf = msg_addr },
            /* Read 8-bit data */
            { .addr = PCF8583_ADDRESS, .flags = I2C_M_RD, .len = 1, .buf = msg_data},
        };

    /* Transfer a transaction with two I2C messages */
    if (_i2c_transfer(&i2c, msgs, 2) < 0) {
        fprintf(stderr, "_i2c_transfer(): %s\n", i2c_errmsg(&i2c));
        exit(1);
    }

    printf("0x%02x%02x: %02x\n", msg_addr[0], msg_addr[1], msg_data[0]);

    i2c_close(&i2c);
    return 0;
}