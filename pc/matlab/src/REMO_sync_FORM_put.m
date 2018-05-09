function [error, FormatString, Bytes] = REMO_sync_FORM_put(Port,data)

fprintf(Port, 'sync');
res = REMO_getline(port);
[error, FormatString, Bytes] = REMO_FORM_put(Port,FormatString,Bytes,data);

end
