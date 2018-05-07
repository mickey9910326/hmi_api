function [data,error] = REMO_sync_get(Port,Type,Bytes)

    fprintf(Port, 'sync');
    res = fscanf(Port,'%s');
    [data, error] = REMO_get(Port,Type,Bytes);
