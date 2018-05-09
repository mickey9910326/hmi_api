function [error, FormatString, Bytes] = remo_sync_form_put(Port,data)

fprintf(Port, 'sync');
res = remo_getline(port);
[error, FormatString, Bytes] = remo_form_put(Port,FormatString,Bytes,data);

end
