function [data,error] = REMO_sync_get(port)

fprintf(port, 'sync');
res = REMO_getline(port);
[data, error] = REMO_get(port);

end
