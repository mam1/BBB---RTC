
#include "i2c.h"
#include "bitlit.h"

#define PCF8583_ADDRESS     0x51
#define I2C_BUSS            "/dev/i2c-1"
#define SEC_REG             0x02
#define SEC_MASK            B8(01111111)
#define MIN_REG             0x03
#define MIN_MASK            B8(01111111)

typedef struct {
    int tm_sec;         /* seconds */
    int tm_min;         /* minutes */
    int tm_hour;        /* hours */
    int tm_mday;        /* day of the month */
    int tm_mon;         /* month */
    int tm_year;        /* year */
    int tm_wday;        /* day of the week */
    int tm_yday;        /* day in the year */
    int tm_isdst;       /* daylight saving time */
} _tm;

uint8_t bcd2bin(uint8_t);
int get_tm(_tm *, _i2c_t *);