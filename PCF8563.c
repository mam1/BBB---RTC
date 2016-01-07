#include <stdint.h>		//uint_8, uint_16, uint_32, etc.
#include "PCF8563.h"

// //---------------------------------------------- 
// // This function converts an 8 bit binary value 
// // to an 8 bit BCD value. 
// // The input range must be from 0 to 99. 

// uint8_t  bin2bcd(uint8_t  value) 
// { 
// uint8_t retval; 

// retval = 0; 

// while(1) 
//   { 
//    // Get the tens digit by doing multiple subtraction 
//    // of 10 from the binary value. 
//    if(value >= 10) 
//      { 
//       value -= 10; 
//       retval += 0x10; 
//      } 
//    else // Get the ones digit by adding the remainder. 
//      { 
//       retval += value; 
//       break; 
//      } 
//    } 

// return(retval); 
// } 

// //---------------------------------------------- 
// // This function converts an 8 bit BCD value to 
// // an 8 bit binary value. 
// // The input range must be from 00 to 99. 

// uint8_t bcd2bin(uint8_t bcd_value) 
// { 
// uint8_t temp; 

// temp = bcd_value; 

// // Shifting the upper digit right by 1 is 
// // the same as multiplying it by 8. 
// temp >>= 1; 

// // Isolate the bits for the upper digit. 
// temp &= 0x78; 

// // Now return: (Tens * 8) + (Tens * 2) + Ones 
// return(temp + (temp >> 2) + (bcd_value & 0x0f)); 

// } 


// int set_tm(_tm *tm){


// 	return 0;
// }

int get_tm(_tm *tm){

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
    if (i2c_transfer(&i2c, msgs, 2) < 0) {
        fprintf(stderr, "i2c_transfer(): %s\n", i2c_errmsg(&i2c));
        exit(1);
    }

    printf("0x%02x%02x: %02x\n", msg_addr[0], msg_addr[1], msg_data[0]);
    tm->tm_sec = (int)bcd2bin(msg_data[0] && SEC_MASK);


	return 0;
}

// int print_tm(_tm *tm){

// 	printf("  %02i:%02i:%02i   %02i/%02i/%02i   dow  %i\n",
// 		tm->tm_sec, tm->tm_min, tm->tm_hour, 
// 		tm->tm_mday, tm->tm_mon, tm->tm_year, tm->tm_wday);

// 	return 0;
// }