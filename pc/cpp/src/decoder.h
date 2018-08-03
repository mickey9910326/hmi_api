#ifndef HMI_DECODER_H
#define HMI_DECODER_H

#include <inttypes.h>
#include <vector>

class Decoder {
private:
    std::vector<char> text;

public:
    Decoder();
    virtual ~Decoder();
    uint8_t  put_text(const char *s, uint16_t length);
    uint16_t get_text(char* s);
    uint16_t get_length();

};

#endif // HMI_DECODER_H
