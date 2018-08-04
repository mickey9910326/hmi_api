#ifndef HMI_DECODER_H
#define HMI_DECODER_H

#include <inttypes.h>
#include <vector>
#include <memory>

class Decoder {
public:
    Decoder();
    virtual ~Decoder();
    uint8_t  put_text(const char *s, uint16_t length);
    uint32_t get_text(char* s);
    uint32_t get_length();

    uint8_t decode();
private:
    struct Impl;
    std::unique_ptr<Impl> mImpl;
};

#endif // HMI_DECODER_H
