function [data,FormatString,error] = REMO_sync_FORM_get(Port,Bytes)

fprintf(Port, 'sync');
res = REMO_getline(port);
[data,FormatString,error] = REMO_FORM_get(Port,Bytes);

end
