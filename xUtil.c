#include <string.h>

#include "xUtil.h"

/** 메모리 할당
 * 메모리를 할당한다.
 * 
 * @return 할당된 메모리의 포인터
 * 
 * 메모리 할당에 실패할 경우 예외(EX_ALLOC)를 발생시킨다.
 */
void* xxalloc(int size) 
{
    void* p = malloc(size);
    
    if (p == NULL) {
        xthrow(EX_ALLOC);  
    }  
    return p;  
}

/** 할당된 메모리의 해제
 * 
 * 메모리가 NULL인 경우 예외(EX_FREE) 발생
 */
void xxfree(void* p) 
{
    if (p == NULL) {
        xthrow(EX_FREE);
    }
    
    free(p);
    // TODO: free() 가 abort 하는 것을 잡을 수 있나??
}

/** 문자열 복제
 * 메모리를 할당하여 문자열을 복제한다.
 * 
 * @return 목제된 문자열의 포인터
 * 
 * 메모리 할당에 실패할 경우 예외(EX_MALLOC)를 발생시킨다.
 */
char* xstrdup(char str[])
{
    int len = strlen(str);
    char* p = (char*)xxalloc(len + 1);
    strcpy(p, str);
    return p;
}


