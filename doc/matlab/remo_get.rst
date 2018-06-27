remo_get
========

呼叫介面
^^^^^^^^
:code:`[data, error] = remo_get(port)`


簡介
^^^^
    由指定之串列埠port，接收矩陣資料。

輸入參數
^^^^^^^^
    **port**:愈進行通訊之串列埠。

回傳變數
^^^^^^^^
    data:讀回之矩陣資料。

    error:錯誤代碼。

        0:成功接收矩陣資料。

        1:逾時。

        2:通訊錯誤。

範例
^^^^

matlab端程式：

.. code-block:: matlab

    port = remo_open('COM1')

    data = remo_get(port)

    remo_close(port)

ASA_M128端程式：

.. code-block:: c

    #include "ASA_Lib.h"

    int main() {
        ASA_M128_set();

        float data[5] = {1.1, -1, 0,1, -2.1};
        char s[20];
        char num = 5;

        int bytes = num * sizeof(float); // float is 4 bytes => bytes = 20
        // NOTE float and double both are 4 bytes (32 bits)
        // This is the only supported floating point format in AVR-GCC

        M128_HMI_put(bytes, 8, data); // send data

        return 0;
    }
