function [error] = REMO_sync_put(Port,Type,Bytes,data)

    fprintf(Port, 'sync');
    res = fscanf(Port,'%s');
    [error] = REMO_put(Port,Type,Bytes,data);
