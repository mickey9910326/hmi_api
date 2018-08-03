#include "decoder.h"

Decoder::Decoder() {

}

Decoder::~Decoder() {

}

uint8_t Decoder::put_text(const char *s, uint16_t length) {
    uint32_t start = text.size();
    if (start+length > 65535) {
        return 1;
    }
    for (uint32_t i = 0; i < length; i++) {
        text.push_back(s[i]);
    }
    return 0;
}

uint16_t Decoder::get_text(char *s) {
    uint16_t length = text.size();
    for (uint32_t i = 0; i < length; i++) {
        s[i] = text[i];
    }
    return length;
}

uint16_t Decoder::get_length() {
    return text.size();
}
