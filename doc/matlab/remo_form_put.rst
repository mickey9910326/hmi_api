remo_form_put
=============

呼叫介面
^^^^^^^^
:code:`[error, formatString, bytes] = remo_form_put(port, data)`

簡介
^^^^
    由指定之串列埠port，接收結構資料。

輸入參數
^^^^^^^^
    **port**:愈進行通訊之串列埠。

    **data**:愈發送之結構資料。

回傳變數
^^^^^^^^
    error:錯誤代碼。

        0:成功發送資料。

        1:逾時。

        2:通訊錯誤。

    formatString:發送資料之結構字串。

    bytes:發送資料之資料大小。
