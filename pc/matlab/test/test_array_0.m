clear
clc

addpath('../src')

port = REMO_open(4);
msg = REMO_get_msg(port);
data = REMO_sync_get(port);

REMO_close(port);
