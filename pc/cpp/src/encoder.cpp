
// #define HMI_ARRAY_HEADER 0xA

#include <stdint.h>
#include "encoder.h"

HmipacRes_t hmipac_struct_to_packet(char* FormatString, uint16_t bytes, void* data_p);
HmipacRes_t hmipac_array_to_packet(uint8_t type, uint16_t length, void* data_p) {
    HmipacRes_t res;
    res.error = 0;

    uint8_t usize = get_type_size(type); ///< data unit size
    if (usize == 0) {
        res.error = 1;
        return res;
    }

    uint32_t dsize = usize * length; /// total data size
    if (dsize>65535) {
        res.error = 2;
        return res;
    }

    res.length = 3+1+2+dsize+1;
    res.data[0] = 0xAB;
    res.data[1] = 0xAB;
    res.data[2] = 0xAB;
    res.data[3] = type;
    res.data[4] = dsize>>8;
    res.data[5] = dsize&0xFF;

    uint8_t chksum = res.data[4] + res.data[5];

    for (uint32_t i = 0; i < dsize; i++) {
        res.data[i+6] = ((uint8_t*)data_p)[i];
        chksum += res.data[i+6];
    }

    res.data[6+dsize] = chksum;
    return res;
}

uint8_t get_type_size(uint8_t type) {
    switch (type) {
        case HMI_TYPE_I8:
        case HMI_TYPE_UI8:
            return 1;
        case HMI_TYPE_I16:
        case HMI_TYPE_UI16:
            return 2;
        case HMI_TYPE_I32:
        case HMI_TYPE_UI32:
        case HMI_TYPE_F32:
            return 4;
        case HMI_TYPE_I64:
        case HMI_TYPE_UI64:
        case HMI_TYPE_F64:
            return 8;
        default:
            return 0;
    }
}
