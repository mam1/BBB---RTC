#ifndef MYI2C_H_
#define MYI2C_H_
#define I2C_FILE_NAME "/dev/i2c-1"

#include <stdint.h>     //uint_8, uint_16, uint_32, etc.

// int set_i2c_register(int, uint8_t, uint8_t, uint8_t);
int get_i2c_register(int, uint8_t, uint8_t, uint8_t *);

#endif