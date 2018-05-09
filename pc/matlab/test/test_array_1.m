clear
clc

addpath('../src')

port = remo_open(4);

msg = remo_get_msg(port);
data = remo_sync_get(port);

data(1) = 99;
msg = remo_get_msg(port);
remo_sync_put(port, data);

msg = remo_get_msg(port);
data = remo_sync_get(port);

remo_close(port);
