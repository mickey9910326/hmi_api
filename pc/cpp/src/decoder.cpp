#include "decoder.h"
#include <vector>

#define PAC_HEADER 0xFD

enum Pactype {
    pact_err,
    pact_string,
    pact_array,
    pact_struct,
    pact_sync,
    pact_ack
};

enum Stat {
    stat_header,
    stat_seq,
    stat_info,
    stat_extdata,
    stat_byte,
    stat_data,
    stat_chksum,
};

struct Decoder::Impl {
    Impl();
    ~Impl();

    std::vector<char> text;
    std::vector<char> dataBuffer;
    uint32_t used_length;
    enum Pactype pactype;   ///< which the packet is
    enum Stat status;

    uint8_t get_ch(void);
    uint8_t step(void);
};

Decoder::Impl::Impl()
    :status(stat_header)
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

uint8_t Decoder::Impl::get_ch(void) {
    return text[used_length];
}

uint8_t Decoder::Impl::step(void) {
    uint8_t ch = this->get_ch();
    switch (status) {
        case stat_header:
            if (ch==PAC_HEADER) {
                status = stat_seq;
            }
        case stat_seq:
        case stat_info:
        case stat_extdata:
        case stat_byte:
        case stat_data:
        case stat_chksum:
            break;
    }
    return 0;
}

// NOTE Packet defination

/*
Packet layer
Header    | 0xFD           | 1 bytes
Seq       | 0x??           | 1 bytes
Info      | 0x??           | 1 bytes
ExtraData | ??             | M bytes (Max is 8 bytes)
Bytes     | 0x?? 0x??      | 2 bytes -> N
HmiData   | ??             | N bytes
Chksum    | 0x??           | 1 bytes
*/

/*
Byte Seq
num         | bit 0~4 | 代表當前續傳編號
isContinous | bit 5   | 代表是否傳輸完畢
isNeedRes   | bit 6   | 是否需要對方回應
isRes       | bit 7   | 這封包是否為回應封包
*/

/*
Byte Info
ExtraInfobyte | bit 0~2 | 額外訊息長度 (max is 8)
hasAddres     | bit 3   | 額外訊息是否包含地址
isHmiData     | bit 7   | 是否為HMI資料封包
*/

/*
HmiData Layer
Type   | 0x??      | 1 bytes
Bytes  | 0x?? 0x?? | 2 bytes
Data   | ??        | N bytes
Chksum | 0x??      | 1 bytes
*/
