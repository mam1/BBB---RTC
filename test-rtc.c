

#include <stdio.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>     //sleep
#include <stdint.h>     //uint_8, uint_16, uint_32, etc.
#include <sys/ioctl.h>
#include <string.h>
#include "myi2c.h"
#include "bitlit.h"

#define _PCF8563_ADDR   0x51

#define _sec_reg        0x02
#define _min_reg        0x03
#define _hr_reg         0x04

#define _sec_mask       B8(01111111)
#define _min_mask       B8(01111111)
#define _hr_mask        B8(00111111)  

 


int main(void) {
    int         i2c_file;
    int         i;
    uint8_t     addr,reg,value;



//    printf("sec_mask %02x\n", _sec_mask);

    printf(" starting loop\n");
    addr = 0x51;
    for(i=1; i<5; i++){
        // Open a connection to the I2C userspace control file.
        if ((i2c_file = open(I2C_FILE_NAME, O_RDWR)) < 0) {
            perror("Unable to open i2c control file");
            exit(1);
        }
        printf("  %s opened, handle = %i\n",I2C_FILE_NAME, i2c_file);
        reg = 2;
        printf("  get_i2c_register returned %i\n",get_i2c_register(i2c_file, _PCF8563_ADDR, reg, &value));
        value = value && _sec_mask;
        printf("  value reg %i <%02x>\n", reg, value);

        reg = 3;
        printf("  get_i2c_register returned %i\n",get_i2c_register(i2c_file, _PCF8563_ADDR, reg, &value));
        value = value && _min_mask;
        printf("  value reg %i <%02x>\n", reg, value);

        reg = 4;
        printf("  get_i2c_register returned %i\n",get_i2c_register(i2c_file, _PCF8563_ADDR, reg, &value));
        value = value && _hr_mask;
        printf("  value reg %i <%02x>\n", reg, value);

        printf("\n");
        close(i2c_file);
        sleep(1);
    }
    printf("normal completion\n\n");

    // if(argc > 3 && !strcmp(argv[1], "r")) {
    //     int addr = strtol(argv[2], NULL, 0);
    //     int reg = strtol(argv[3], NULL, 0);
    //     unsigned char value;
    //     if(get_i2c_register(i2c_file, addr, reg, &value)) {
    //         printf("Unable to get register!\n");
    //     }
    //     else {
    //         printf("Register %d: %d (%x)\n", reg, (int)value, (int)value);
    //     }
    // }
    // else if(argc > 4 && !strcmp(argv[1], "w")) {
    //     int addr = strtol(argv[2], NULL, 0);
    //     int reg = strtol(argv[3], NULL, 0);
    //     int value = strtol(argv[4], NULL, 0);
    //     if(set_i2c_register(i2c_file, addr, reg, value)) {
    //         printf("Unable to get register!\n");
    //     }
    //     else {
    //         printf("Set register %x: %d (%x)\n", reg, value, value);
    //     }
    // }
    // else {
    //     fprintf(stderr, USAGE_MESSAGE, argv[0], argv[0]);
    // }





    return 0;
}