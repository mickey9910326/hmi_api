/**
 * @file test_array_0.c
 * @author mickey9910326
 * @date 2018.5.7
 * @brief M128 send 5 f32 data.
 */

#include "../lib/ASA_Lib.h"
#include "../src/ASA_Lib_HMI.h"
#include <string.h>

int main() {
    ASA_M128_set();

    float data[5] = {1.1, -1, 0,1, -2.1};
    char s[20];
    char num = 5;

    char Bytes = num*sizeof(float); // float is 4 bytes => Bytes = 20
    // NOTE float and double both are 4 bytes (32 bits)
    // This is the only supported floating point format in AVR-GCC

    printf("send f32*5\n"); // send msg out
    scanf("%s", s); // wait for sync
    M128_HMI_put(Bytes, HMI_TYPE_F32, data); // send data

    return 0;
}
