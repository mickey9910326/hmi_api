from struct import *
from hmi_pack.type import *

def decode_array(typeNum,data):
    if typeNum == 0:
        # int8_t
        typeS = 'b'
        typeByte = 1
    elif typeNum == 1:
        # int16_t
        typeS = 'h'
        typeByte = 2
    elif typeNum == 2:
        # int32_t
        typeS = 'i'
        typeByte = 4
    elif typeNum == 3:
        # int64_t
        typeS = 'q'
        typeByte = 8
    elif typeNum == 4:
        # uint8_t
        typeS = 'B'
        typeByte = 1
    elif typeNum == 5:
        # uint16_t
        typeS = 'H'
        typeByte = 2
    elif typeNum == 6:
        # uint32_t
        typeS = 'I'
        typeByte = 4
    elif typeNum == 7:
        # uint64_t
        typeS = 'Q'
        typeByte = 8
    elif typeNum == 8:
        # f32
        typeS = 'f'
        typeByte = 4
    elif typeNum == 9:
        # f64
        typeS = 'd'
        typeByte = 8
    else:
        return False
    return unpack('<'+str(int(round(len(data)/typeByte)))+typeS, data)

def decodePackStr(typeNum):
    if typeNum == 0: # int8_t
        return 'b'
    elif typeNum == 1: # int16_t
        return 'h'
    elif typeNum == 2: # int32_t
        return 'i'
    elif typeNum == 3: # int64_t
        return 'q'
    elif typeNum == 4: # uint8_t
        return 'B'
    elif typeNum == 5: # uint16_t
        return 'H'
    elif typeNum == 6: # uint32_t
        return 'I'
    elif typeNum == 7: # uint64_t
        return 'Q'
    elif typeNum == 8: # f32
        return 'f'
    elif typeNum == 9: # f64
        return 'd'
    else:
        return False

def decodeTextToArrey(text):
    # decode Data Type
    lines =  text.split('\n')
    res = 0
    typeNum = 0
    dataList = list()
    resText = str();

    status = 1
    for s in lines:
        if status == 1:
            if decodeIsSpaceLine(s):
                pass
            else:
                typeNum = decodeTypeLine(s)
                status = 2;
                isContinued = 1;
        elif status == 2:
            if decodeIsSpaceLine(s):
                pass
            elif (isContinued==1):
                newList, isContinued = decodeDataLine(s,typeNum)
                dataList += newList
                status = 2;
            if (isContinued==0):
                status = 3;
            if (isContinued==-1):
                res = -1
                return lineIdx, typeNum, dataList, res, resText
        elif status == 3:
            resText += s
            resText += '\n'


    return typeNum, dataList, res, resText

def decodeTextToStruct(text):
    # decode Data Type
    lines =  text.split('\n')
    res = 0
    lineIdx = 0
    dataList = list()

    resTypeNumList = list()
    resDataListList = list()
    resArrayNums = 0
    isContinued = 0

    status = 1
    for s in lines:
        if status == 1:
            if decodeIsSpaceLine(s):
                pass
            else:
                typeNum = decodeTypeLine(s)
                if typeNum is -1:
                    res = -1
                    return 0, [], [], res
                else:
                    status = 2
                    isContinued = 1
                    dataList = list()
        elif status == 2:
            if decodeIsSpaceLine(s):
                pass
            elif isContinued is 1:
                newList, isContinued = decodeDataLine(s, typeNum)
                dataList += newList
                status = 2

            if   isContinued is 1:
                print('isContinued ' + s)
                pass
            elif isContinued is 0:
                resTypeNumList.append(typeNum)
                resDataListList.append(dataList)
                resArrayNums +=1
                status = 1
            elif isContinued is -1:
                print('-1 ' + s)
                res = -1
                return 0, [], [], res
    if isContinued is 1:
        res = -1
        return 0, [], [], res
    return resArrayNums, resTypeNumList, resDataListList, res

def isNum(ch):
    if ch > '0' and ch < '9': # space
        return True
    return False

def isLowerAlphabet(ch):
    if ch > 'a' and ch < 'z': # space
        return True
    return False

def decodeIsSpaceLine(s):
    for ch in s:
        if ch == '\x20': # space
            pass
        else:
            return False
    return True

def decodeTypeLine(s):
    resTypeString = str();
    typeNum = -1
    status = 0
    for ch in s:
        if status == 0: # remove space
            if ch == '\x20': # space
                pass
            elif ch.isdigit() or ch.islower():
                status = 1
            else:
                status = 99
        if status == 1:
            if ch.isdigit() or ch.islower():
                resTypeString += str(ch);
            else:
                status = 2
        if status == 2:
            if ch == '\x20': # space
                pass
            elif ch == ':':
                status = 3
            else:
                print('sys : type line error : ' + s)
                return -1
        if status == 3:
            resTypeNum = getTypeNum(resTypeString)
            print('sys : get type num of \'' + resTypeString + '\' is ' + str(resTypeNum))
            return resTypeNum
            break
    return -1

def decodeDataLine(s,typeNum):
    def isLastDot(s):
        for ch in s[::-1]:
            if ch == '\x20': # space
                pass
            elif ch == ',':
                return 1
            else:
                return 0
    print('sys : decode Data Line : ' + s)
    if typeNum>=0 and typeNum<=7:
        isInt = 1
    elif typeNum>=8 and typeNum<=9:
        isInt = 0
    else:
        raise Exception('typeNum error')

    isContinued = isLastDot(s)
    datas = s.split(',')
    resDataList = list()
    if isContinued:
        del datas[-1]
    for data in datas:
        try:
            if isInt:
                num = int(data)
            else:
                num = float(data)
            resDataList.append(num)
        except (ValueError,SyntaxError):
            return [], -1
    return resDataList, isContinued

def decodeFormatString(s):
    typeStrs = s.split(',')
    typeNumList = list();
    typeDataNumList = list();
    for typeStr in typeStrs:
        deTypeStr = typeStr.split('x')
        print(deTypeStr)
        typeNumList.append(getTypeNum(deTypeStr[0]))
        typeDataNumList.append(int(deTypeStr[1]))
    return typeNumList,typeDataNumList

def decode_struct(totalBytes,formatString,data):
    dataBytes = totalBytes-len(formatString)-1;
    typeNumList , typeDataNumList = decodeFormatString(formatString)
    dataIdx = 0;
    dataLastIdx = 0;
    dataListList = list();
    for idx in range(len(typeNumList)):
        dataLastIdx = dataIdx + getTypeSize(typeNumList[idx])*typeDataNumList[idx]
        dataList = unpack('<'+str(typeDataNumList[idx])+decodePackStr(typeNumList[idx]), data[dataIdx:dataLastIdx])
        dataIdx = dataLastIdx
        dataListList.append(dataList)
    return typeNumList, dataListList

def transUi8ToString(text):
    # decode Data Type
    lines =  text.split('\n')
    res = 0
    lineIdx = 0
    dataList = list()
    resText = str()

    resTypeNumList = list()
    resDataListList = list()
    resArrayNums = 0

    status = 1
    for idx,s in enumerate(lines):
        if status == 1:
            if decodeIsSpaceLine(s):
                resText += s
                if idx+1 == len(lines):
                    pass
                else:
                    resText += '\n'
                pass
            else:
                typeNum = decodeTypeLine(s)
                if typeNum == 4:
                    resText += 's:'
                    resText += '\n'
                    status = 2;
                    isContinued = 1;
                else:
                    resText += s
                    resText += '\n'
        elif status == 2:
            if decodeIsSpaceLine(s):
                resText += s
                resText += '\n'
                pass
            elif (isContinued==1):
                newList, isContinued = decodeDataLine(s,typeNum)
                dataList += newList
                status = 2;
            if (isContinued==0):
                # status = 3;
                status = 1;
                resText += '  \''
                resText += bytes(dataList).decode("utf-8")
                resText += '\'\n'
                dataList = list()
            if (isContinued==-1):
                res = -1
                return res, resText
    return res, resText

def transStringToUi8(text):
    # decode Data Type
    lines =  text.split('\n')
    res = 0
    lineIdx = 0
    dataList = list()
    resText = str()

    resTypeNumList = list()
    resDataListList = list()
    resArrayNums = 0

    status = 1
    for idx,s in enumerate(lines):
        if status == 1:
            if decodeIsSpaceLine(s):
                resText += s
                if idx+1 == len(lines):
                    pass
                else:
                    resText += '\n'
                pass
            else:
                typeNum = decodeTypeLine(s)
                if typeNum == 15:
                    resText += 'ui8:'
                    resText += '\n'
                    status = 2;
                    isContinued = 1;
                else:
                    resText += s
                    resText += '\n'
        elif status == 2:
            start = s.find('\'')
            end   = s.rfind('\'')
            status = 3
            print(start)
            print(end)
            datas = s[start+1:end]
            print(datas)
            print(datas.encode())
            datas = s[start+1:end].encode()

            s = '  '
            for idx, data in enumerate(datas):
                s += str((data))
                if idx+1 != len(datas):
                    s += ',  '
                else:
                    resText += s
                    resText += '\n'
                    s = '  '
                if len(s) > 100: #換行
                    resText += s
                    resText += '\n'
                    s = '  '
            status = 1
    return res, resText
