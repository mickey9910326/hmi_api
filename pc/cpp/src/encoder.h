#include <stdint.h>

#define HMI_TYPE_I8   0
#define HMI_TYPE_I16  1
#define HMI_TYPE_I32  2
#define HMI_TYPE_I64  3
#define HMI_TYPE_UI8  4
#define HMI_TYPE_UI16 5
#define HMI_TYPE_UI32 6
#define HMI_TYPE_UI64 7
#define HMI_TYPE_F32  8
#define HMI_TYPE_F64  9

typedef struct hmipac_res_t {
    uint8_t error;
    uint32_t length;
    uint8_t data[65535];
}HmipacRes_t;

HmipacRes_t hmipac_struct_to_packet();
HmipacRes_t hmipac_array_to_packet(uint8_t type, uint16_t* length, void* data_p);

uint8_t get_type_size(uint8_t type);
