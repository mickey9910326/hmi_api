
def getTypeStr(typeNum):
    if typeNum == 0:
        return 'i8'
    elif typeNum == 1:
        return 'i16'
    elif typeNum == 2:
        return 'i32'
    elif typeNum == 3:
        return 'i64'
    elif typeNum == 4:
        return 'ui8'
    elif typeNum == 5:
        return 'ui16'
    elif typeNum == 6:
        return 'ui32'
    elif typeNum == 7:
        return 'ui64'
    elif typeNum == 8:
        return 'f32'
    elif typeNum == 9:
        return 'f64'
    elif typeNum == 15:
        return 's'
    else:
        return False

def getTypeSize(typeNum):
    if typeNum == 0: # int8_t
        return 1
    elif typeNum == 1: # int16_t
        return 2
    elif typeNum == 2: # int32_t
        return 4
    elif typeNum == 3: # int64_t
        return 8
    elif typeNum == 4: # uint8_t
        return 1
    elif typeNum == 5: # uint16_t
        return 2
    elif typeNum == 6: # uint32_t
        return 4
    elif typeNum == 7: # uint64_t
        return 8
    elif typeNum == 8: # f32
        return 4
    elif typeNum == 9: # f64
        return 8
    elif typeNum == 15: # String
        return 8
    else:
        return False

def getTypeNum(typeString):
    if typeString == 'i8': # int8_t
        return 0
    elif typeString == 'i16': # int16_t
        return 1
    elif typeString == 'i32': # int32_t
        return 2
    elif typeString == 'i64': # int64_t
        return 3
    elif typeString == 'ui8': # uint8_t
        return 4
    elif typeString == 'ui16': # uint16_t
        return 5
    elif typeString == 'ui32': # uint32_t
        return 6
    elif typeString == 'ui64': # uint64_t
        return 7
    elif typeString == 'f32': # f32
        return 8
    elif typeString == 'f64': # f64
        return 9
    elif typeString == 's': # String
        return 15
    else:
        return -1