/**
 * @file test_form_0.c
 * @author mickey9910326
 * @date 2018.05.08
 * @brief
 */

#include "../lib/ASA_Lib.h"
#include "../src/ASA_Lib_HMI.h"
#include <string.h>

typedef struct St {
    uint8_t ui8[10];
    int32_t i32[10];
    float   f32[5];
} st_t;

int main() {
    ASA_M128_set();
    char s[20];

    st_t data = {
        {1, 2, 3,  4,  5,  6,   7,   8,   9,   10},
        {2, 4, 8, 16, 32, 64, 128, 256, 512, 1024},
        {1.0, 2.5, 4.0, 5.5, 6.0}
    };
    int bytes = sizeof(st_t);

    printf("put struct\n");
    scanf("%s", s);
    M128_HMI_Form_put("ui8x10,i32x10,f32x5", bytes, &data);

    return 0;
}
