function [data,FormatString,error] = REMO_sync_FORM_get(port)

fprintf(port, 'sync');
res = REMO_getline(port);
[data,FormatString,error] = REMO_FORM_get(port);

end
