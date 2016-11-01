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

//6. insert char //inChar�� sourceStr�� targetIdx�� �����ؼ� ���ο� ���ڿ��� �̾��ش�. 
TCHAR* insertChar(const TCHAR* sourceStr, TCHAR inChar, int targetIdx);
#endif
