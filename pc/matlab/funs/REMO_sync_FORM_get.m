function [data,FormatString,error] = REMO_sync_FORM_get(Port,Bytes)

    fprintf(Port, 'sync');
    res = fscanf(Port,'%s');
    [data,FormatString,error] = REMO_FORM_get(Port,Bytes);
