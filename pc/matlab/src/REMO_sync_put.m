function [error] = REMO_sync_put(port,data)

fprintf(port, 'sync');
res = REMO_getline(port);
[error] = REMO_put(port,data);

end
