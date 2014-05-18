#include <string.h>

#include "xUtil.h"

/** 
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

/** 
 * 할당된 메모리의 해제한다.
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

/** 
 * 메모리를 할당하여 문자열을 복제한다.
 * 
 * @return 목제된 문자열의 포인터
 * 
 * 메모리 할당에 실패할 경우 예외(EX_MALLOC)를 발생시킨다.
 */
char* xxstrdup(char str[])
{
    int len = strlen(str);
    char* p = (char*)xxalloc(len + 1);
    return strcpy(p, str);
}

void* xxmemdup(void* src, int size)
{
    void* dst = xxalloc(size);
    return memcpy(dst, src, size);
}

