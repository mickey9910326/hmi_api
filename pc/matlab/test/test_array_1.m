clear
clc

addpath('../src')

port = REMO_open(4);

msg = REMO_get_msg(port);
data = REMO_sync_get(port);

data(1) = 99;
msg = REMO_get_msg(port);
REMO_sync_put(port, data);

msg = REMO_get_msg(port);
data = REMO_sync_get(port);

REMO_close(port);
