function msg = REMO_get_msg(Port)

    msg = '';
    isPrevSpace = 0;
    while 1
        ch = fread(Port, 1, 'char');
        if (ch==10) | (ch==0)
            break
        end
        if ch == 32
            isPrevSpace = 1;
        elseif isPrevSpace == 1
            isPrevSpace = 0;
            msg = [msg, ' ', char(ch)];
        else
            msg = [msg, char(ch)];
        end
    end
