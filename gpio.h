#define SYSFS_GPIO_DIR "/sys/class/gpio"
// #define POLL_TIMEOUT (3 * 1000) /* 3 seconds */
 #define MAX_BUF 100

int gpio_export(unsigned int gpio);
int gpio_set_dir(unsigned int gpio, unsigned int out_flag);
