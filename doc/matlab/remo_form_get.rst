remo_form_get
=============

呼叫介面
^^^^^^^^
:code:`[data, formatString, error] = remo_form_get(port)`

簡介
^^^^
    由指定之串列埠port，接收結構資料。

輸入參數
^^^^^^^^
    **port**:愈進行通訊之串列埠。

回傳變數
^^^^^^^^
    data:讀回之結構資料。

    formatString:讀回之結構資料字串。

    error:錯誤代碼。

        0:成功接收資料。

        1:逾時。

        2:通訊錯誤。

範例
^^^^

matlab端程式：

.. code-block:: matlab

    port = remo_open('COM1')

    [data, FormatString, error] = remo_form_get(port)

    remo_close(port)

ASA_M128端程式：

.. code-block:: c

    #include "ASA_Lib.h"

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

        M128_HMI_Form_put("ui8x10,i32x10,f32x5", bytes, &data);

        return 0;
    }
