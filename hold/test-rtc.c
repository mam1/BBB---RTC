
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "lsquaredc.h"

int main2(void) {
//  uint16_t init_sequence1[] = {I2C_RESTART,0x51,0,I2C_RESTART,0xA3,I2C_READ};
  uint16_t init_sequence1[] = {0xa2, 0x02, I2C_RESTART, 0xa3, I2C_READ};
  // uint16_t init_sequence2[] = {0x51, 0x81, 3};
  // uint16_t pn_query[] = {0x51, 0x8a, I2C_RESTART, 0x71, I2C_READ};
  // uint8_t status;
  int i2c_handle;
  int result;
  uint8_t buf[100];

  buf[0] = 0;

  i2c_handle = i2c_open(1);
  printf("Opened bus, result=%d\n", i2c_handle);
  printf("buffer <%x>\nread rtc\n",buf[0]);
  result = i2c_send_sequence(i2c_handle, init_sequence1, 3, buf);
  printf("Sequence processed, result=%d\n", result);
  printf("buffer <%x>\n\n",buf[0]);

  // result = i2c_send_sequence(i2c_handle, init_sequence2, 3, 0);
  // printf("Sequence processed, result=%d\n", result);

  // result = i2c_send_sequence(i2c_handle, pn_query, 5, &status);
  // printf("Sequence processed, result=%d\n", result);
  // printf("Status=%d\n", (int)(status));

  i2c_close(i2c_handle);

  return 0;
}
