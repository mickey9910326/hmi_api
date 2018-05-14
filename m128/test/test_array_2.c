/**
 * @file test_array_2.c
 * @author mickey9910326
 * @date 2018.05.14
 * @brief M128 send 100 f32 data.
 */

#include "../lib/ASA_Lib.h"
#include "../src/ASA_Lib_HMI.h"
#include <string.h>

int main() {
    ASA_M128_set();

    float data[100];
    char s[20];
    char num = 100;

    for (uint8_t i = 0; i < num; i++) {
        data[i] = i;
    }

    int bytes = num*sizeof(float); // float is 4 bytes => bytes = 400
    // NOTE float and double both are 4 bytes (32 bits)
    // This is the only supported floating point format in AVR-GCC

    printf("send f32*100 \n"); // send msg out
    scanf("%s", s); // wait for sync
    M128_HMI_put(bytes, HMI_TYPE_F32, data); // send data

    return 0;
}
