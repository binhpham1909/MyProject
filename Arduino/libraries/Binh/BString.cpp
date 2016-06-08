#include "BString.h"

uint8_t BString::numberOfCharInString(String *StringFrom, String findChar){
    uint16_t lastPositionOfChar = 0, nextPositionOfChar = 0;
    uint8_t numberOfChar = 0;
    do{
        nextPositionOfChar = StringFrom->indexOf(findChar,lastPositionOfChar);
        if(nextPositionOfChar>lastPositionOfChar){
            numberOfChar++;
            lastPositionOfChar = nextPositionOfChar;
        }        
    }while(nextPositionOfChar>0);
    return numberOfChar;
}
// function to convert IP string to IPAdress 32bit
uint32_t BString::StringToIPAdress(String IPvalue){
    if(numberOfCharInString(&IPvalue, ".")!=4){
        return 0;
    }
	union bytes32{
        uint32_t b32;
        uint8_t b8[4];
    } mybytes32;
    mybytes32.b32 = 0;
	int _start=0;	// vị trí xuất phát String IP
    int _dot=IPvalue.indexOf('.');	// vị trí dấu . đầu tiên
    int _ends=IPvalue.length();	// tổng chiều dài String IP
    for(char k=0;k<4;k++){
        String ipk=IPvalue.substring(_start,_dot);	// octet 1: từ vị trí xuất phát đến dấu . đầu tiên
        mybytes32.b8[k]=(uint8_t)ipk.toInt();	// convert String to Int
        _start=_dot+1;	// dấu . đầu thành điểm xuất phát
        if(k<3){	
			_dot=IPvalue.indexOf('.',_start);}
            else{	// tại octet 4(octet cuối) điểm kết thúc là chiều dài String IP(ends)
              _dot=_ends;};
    }
    return mybytes32.b32;
};


//use: ex: StringToArray(&StringFrom, STORAGE.DEVICE_SERIAL, MAX_SERIAL_LEN);
boolean BString::StringToArray(String *StringFrom, char* arrayTo, int maxlen){
    int _len=StringFrom->length();
    if(maxlen>_len){
        StringFrom->toCharArray(arrayTo,_len+1);
        return true;
    }else{
        return false;
    }
};


// Analysis command : ?key:value&&key2=value2 HTTP/1
// ?	start
// :	separate
// $$	end
// " "	enall (ex: space char in and of request in html)
String BString::StringToKeyValue(String *_request, String _separate, String _end,String _enall, String *_key){
    int g_start,g_compare,g_end,i;
    g_start = _request->indexOf(*_key); // position start of key
    g_compare = _request->indexOf(_separate,g_start);   // position of charactor separate
    g_end = _request->indexOf(_end,g_compare);  // position end of value
    if(g_end<0){    
      g_end = _request->indexOf(_enall,g_compare);
    }
    if(g_start<0){
      return "";
    }else{
      return _request->substring(g_compare+_separate.length(), g_end);
    }
}

// Get value at key in GET Request
String BString::htmlGETValue(String *_request,String _key){
	return StringToKeyValue(_request,"=","&&"," ",&_key);
}