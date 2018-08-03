#include "../src/decoder.h"
#include <gtest/gtest.h>

TEST(Decoder_get_text, basic) {
    char s[100] = {0};
    Decoder d;

    d.put_text("123456", 6);
    uint16_t res = d.get_text(s);

    ASSERT_EQ(res, 6);
    ASSERT_STREQ(s, "123456");
}

TEST(Decoder_get_text, edge_length) {
    char s[65535] = {0};
    Decoder d;
    s[0] = 's';
    s[65534] = 'e';
    char predict_s[65535];
    strncpy(predict_s, s, 65535);

    d.put_text(s, 65535);
    uint16_t res = d.get_text(s);

    ASSERT_EQ(res, 65535);
    ASSERT_STREQ(s, predict_s);
}
