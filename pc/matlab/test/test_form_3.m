clear
clc

addpath('../src')

port = remo_open(4);

msg = remo_get_msg(port);
data = remo_sync_form_get(port);

data1 = data;
data1.type1(1) = 123;

msg = remo_get_msg(port);
remo_sync_form_put(port,data1);

msg = remo_get_msg(port);
data2 = remo_sync_form_get(port);

remo_close(port);
