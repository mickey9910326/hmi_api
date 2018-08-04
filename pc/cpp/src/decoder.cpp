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
    std::vector<char> extraDataBuffer;
    uint8_t  chksum;
    uint16_t bytes;
    uint8_t  seq;
    uint8_t  info;
    uint8_t  is_continous;
    uint32_t used_length;
    enum Pactype pactype;   ///< which the packet is
    enum Stat status;
    uint8_t  count;

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
    // TODO ERROR CODE
    uint8_t ch = this->get_ch();
    switch (status) {
        case stat_header:
            if (ch==PAC_HEADER) {
                status = stat_seq;
                chksum = 0;
                // NOTE trash is needed?
            }
            break;
        case stat_seq:
            chksum += ch;
            if (seq&0x80) {
                if ( (ch&0x1F) != (seq&0x1F)+1 ) {
                    return 1;
                } else {
                    status = stat_info;
                    seq    = ch;
                }
            } else {
                status = stat_info;
                seq    = ch;
            }
            break;
        case stat_info:
            chksum += ch;
            if ((ch&0x07) == 0) {
                // Extra data bytes is 0
                status = stat_byte;
                info   = ch;
                count  = 0;
            } else {
                status = stat_extdata;
                info   = ch;
                count  = 0;
            }
            break;
        case stat_extdata:
            chksum += ch;
            extraDataBuffer.push_back(ch);
            count++;
            if (count == (ch&0x07)) {
                status = stat_byte;
                count  = 0;
            }
            break;
        case stat_byte:
            chksum += ch;
            if (count == 0) {
                bytes = ch<<8;
                count++;
            } else {
                // count = 1
                bytes += ch;
                status = stat_data;
                count  = 0;
            }
            break;
        case stat_data:
            chksum += ch;
            extraDataBuffer.push_back(ch);
            count++;
            if (count == bytes) {
                status = stat_chksum;
                count  = 0;
            }
            break;
        case stat_chksum:
            if (ch!=chksum) {
                return 1;
            }
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
