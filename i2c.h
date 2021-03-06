/*
 * c-periphery
 * https://github.com/vsergeev/c-periphery
 * License: MIT
 */

#ifndef _PERIPHERY_I2C_H
#define _PERIPHERY_I2C_H

#include <stddef.h>
#include <stdint.h>
#include <limits.h>

#include <linux/i2c.h>
#include <linux/i2c-dev.h>

enum i2c_error_code {
    I2C_ERROR_ARG               = -1, /* Invalid arguments */
    I2C_ERROR_OPEN              = -2, /* Opening I2C device */
    I2C_ERROR_QUERY_SUPPORT     = -3, /* Querying I2C support on I2C device */
    I2C_ERROR_NOT_SUPPORTED     = -4, /* I2C not suppoted on this device */
    I2C_ERROR_TRANSFER          = -5, /* I2C transfer */
    I2C_ERROR_CLOSE             = -6, /* Closing I2C device */
};

typedef struct i2c_handle {
    int fd;
    struct {
        int c_errno;
        char errmsg[96];
    } error;
} _i2c_t;

/* Primary Functions */
int i2c_open(_i2c_t *i2c, const char *path);
int _i2c_transfer(_i2c_t *i2c, struct i2c_msg *msgs, size_t count);
int _i2c_master_send(_i2c_t *i2c, uint8_t data[], int size);
int i2c_close(_i2c_t *i2c);

/* Miscellaneous */
int i2c_fd(_i2c_t *i2c);
int _i2c_tostring(_i2c_t *i2c, char *str, size_t len);

/* Error Handling */
int i2c_errno(_i2c_t *i2c);
const char *i2c_errmsg(_i2c_t *i2c);

/* struct i2c_msg from <linux/i2c.h>:

    struct i2c_msg {
    	__u16 addr;
    	__u16 flags;
    #define I2C_M_TEN		0x0010
    #define I2C_M_RD		0x0001
    #define I2C_M_STOP		0x8000
    #define I2C_M_NOSTART		0x4000
    #define I2C_M_REV_DIR_ADDR	0x2000
    #define I2C_M_IGNORE_NAK	0x1000
    #define I2C_M_NO_RD_ACK		0x0800
    #define I2C_M_RECV_LEN		0x0400
    	__u16 len;
    	__u8 *buf;
    };
*/

#endif

