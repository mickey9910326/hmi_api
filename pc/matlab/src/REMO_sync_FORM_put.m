function [error] = REMO_sync_FORM_put(Port,FormatString,Bytes,data)

fprintf(Port, 'sync');
res = REMO_getline(port);
[error] = REMO_FORM_put(Port,FormatString,Bytes,data);

end
