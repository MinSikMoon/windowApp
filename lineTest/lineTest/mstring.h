#ifndef _mstring_
#define _mstring_

#include <tchar.h>


//1. getLen
int getLen(const TCHAR* inStr);

//2. subString
TCHAR* subTchar(const TCHAR* inStr, int fromIdx, int toIdx);

//3. cloneTchar
TCHAR* cloneTchar(const TCHAR* source);

//4. addString
TCHAR* addTchar(const TCHAR* source, const TCHAR* added);

//5. charToStr 
TCHAR* charToStr(TCHAR inChar);

//6. insert char //inChar를 sourceStr의 targetIdx에 삽입해서 새로운 문자열을 뽑아준다. 
TCHAR* insertChar(const TCHAR* sourceStr, TCHAR inChar, int targetIdx);
#endif