#ifndef __mString__
#define __mString__

//1. ���� ���ϱ�
int getLen(const TCHAR* inStr);

//2. subString
TCHAR* subTchar(const TCHAR* inStr, int fromIdx, int toIdx);

//3. cloneTchar
TCHAR* cloneTchar(const TCHAR* source);

//4. addString
TCHAR* addTchar(const TCHAR* source, const TCHAR* added);



#endif 
