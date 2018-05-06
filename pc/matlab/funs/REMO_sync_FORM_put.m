function [error] = REMO_sync_FORM_put(Port,FormatString,Bytes,data)

    fprintf(Port, 'sync');
    res = fscanf(Port,'%s');
    [error] = REMO_FORM_put(Port,FormatString,Bytes,data);
