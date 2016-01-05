
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "i2c.h"

#define EEPROM_I2C_ADDR 0x50

int main(void) {
    i2c_t i2c;

    /* Open the i2c-0 bus */
    if (i2c_open(&i2c, "/dev/i2c-1") < 0) {
        fprintf(stderr, "i2c_open(): %s\n", i2c_errmsg(&i2c));
        exit(1);
    }

  
    uint8_t msg_addr[2] = { 0x51, 0x02 };
    uint8_t msg_data[1] = { 0xff, };
    struct i2c_msg msgs[2] =
        {
            /* Write 16-bit address */
            { .addr = EEPROM_I2C_ADDR, .flags = 0, .len = 2, .buf = msg_addr },
            /* Read 8-bit data */
            { .addr = EEPROM_I2C_ADDR, .flags = I2C_M_RD, .len = 1, .buf = msg_data},
        };

    /* Transfer a transaction with two I2C messages */
    if (i2c_transfer(&i2c, msgs, 2) < 0) {
        fprintf(stderr, "i2c_transfer(): %s\n", i2c_errmsg(&i2c));
        exit(1);
    }

    printf("0x%02x%02x: %02x\n", msg_addr[0], msg_addr[1], msg_data[0]);

    i2c_close(&i2c);
    return 0;
}