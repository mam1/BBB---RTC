#ifndef MYI2C_H_
#define MYI2C_H_



int set_i2c_register(int, unsigned char, unsigned char, unsigned char);
int get_i2c_register(int, unsigned char, unsigned char, unsigned char);

#endif