#include "decoder.h"
#include <vector>

struct Decoder::Impl {
    Impl();
    ~Impl();

    std::vector<char> text;
    uint8_t status;

    uint8_t step(void);
};

Decoder::Impl::Impl()
    :status(1)
{

}

Decoder::Impl::~Impl()
{
}

Decoder::Decoder():mImpl(new Impl)
{

}

Decoder::~Decoder() {

}

uint8_t Decoder::put_text(const char *s, uint16_t length) {
    uint32_t start = mImpl->text.size();
    if (start+length > 65535) {
        return 1;
    }
    for (uint32_t i = 0; i < length; i++) {
        mImpl->text.push_back(s[i]);
    }
    return 0;
}

uint32_t Decoder::get_text(char *s) {
    uint32_t length = mImpl->text.size();
    for (uint32_t i = 0; i < length; i++) {
        s[i] = mImpl->text[i];
    }
    return length;
}

uint32_t Decoder::get_length() {
    return mImpl->text.size();
}

uint8_t Decoder::Impl::step(void) {
    return 0;
}
