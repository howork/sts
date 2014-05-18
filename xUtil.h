/*
  Copyright (c) 2014 Munho Choi

  See the file LICENSE.txt or LICENSE-KOR.txt for copying permission.
*/

#ifndef __XUTIL_H__
#define __XUTIL_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define     FALSE   (0 != 0)
#define     TRUE    (!FALSE)

//////////////////////////////// TYPE DEF ////////////////////////////////

typedef int                 int32;
typedef short               int16;
typedef char                int8;
typedef long long           int64;

typedef unsigned int        uint32;
typedef unsigned short      uint16;
typedef unsigned char       uint8;
typedef unsigned long long  uint64;

typedef uint32              uint;
typedef uint8               byte;

//typedef int                 bool; 

typedef uint                xint;
typedef double              xfloat;


//////////////////////////////// Exception  ////////////////////////////////

#include "xTryCatch.h"

DECL_XTRY_CATCH(EX_ALLOC, EX_FREE);



//////////////////////////////// DEBUG MACRO ////////////////////////////////

#ifdef DEBUG
    #define DD      do { \
                        if (1) fprintf(stderr, "DEBUG[ %s:%d:%s() ]\n", \
                                        __FILE__, __LINE__, __func__);  \
                    } while (0);
#else
    #define DD      do { \
                        if (0) fprintf(stderr, "DEBUG[ %s:%d:%s() ]\n", \
                                        __FILE__, __LINE__, __func__);  \
                    } while (0);
#endif

#ifdef DEBUG
    #define DDS(str)    do { \
                            if (1) fprintf(stderr, "DEBUG[ %s:%d:%s() ] %s \n", \
                                        __FILE__, __LINE__, __func__, str); \
                        } while (0)
#else
    #define DDS(str)    do { \
                            if (0) fprintf(stderr, "DEBUG[ %s:%d:%s() ] %s \n", \
                                        __FILE__, __LINE__, __func__, str); \
                        } while (0)
#endif

#ifdef DEBUG
    #define DD2(fmt, args...)   do { \
                                if (1) fprintf(stderr, " DEBUG[ %s:%d:%s() ] " fmt, \
                                        __FILE__, __LINE__, __func__, ##args); \
                            } while (0)
#else
    #define DD2(fmt, args...)   do { \
                                if (0) fprintf(stderr, " DEBUG[ %s:%d:%s() ] " fmt, \
                                        __FILE__, __LINE__, __func__, ##args); \
                            } while (0)
#endif


#ifdef DEBUG_FUNCALL
    #define FFI     do { \
                        if (1) fprintf(stderr, "DEBUG[ %s():%d ] {{{ \n", __func__, __LINE__); \
                    } while (0);
#else
    #define FFI     do { \
                        if (0) fprintf(stderr, "DEBUG[ %s():%d ] {{{ \n", __func__, __LINE__); \
                    } while (0);
#endif

#ifdef DEBUG_FUNCALL
    #define FFO     do { \
                        if (1) fprintf(stderr, "DEBUG[ %s(): %d ] }}} \n", __func__, __LINE__); \
                    } while (0);
#else
    #define FFO     do { \
                        if (0) fprintf(stderr, "DEBUG[ %s(): %d ] }}} \n", __func__, __LINE__); \
                    } while (0);
#endif


//////////////////////////////// MEMORY ALLOC MACRO ////////////////////////////////

// inline functions require C99 standard

#define xalloc(type)            (type*)xxalloc(sizeof(type))
#define xnalloc(type, count)    (type*)xxalloc(sizeof(type)*(count))
#define xtalloc(type, size)     (type)xxalloc(size)

#define xfree(p)                xxfree(p)

#define xstrdup(p)              xxstrdup(p)
#define xmemdup(type, p)        (type*)xxmemdup(p, sizeof(type))


/// 메모리 할당
void* xxalloc(int size);

/// 할당된 메모리의 해제
void xxfree(void* p);

/// 문자열 복제
char* xxstrdup(char str[]);

/// 메모리 복제
void* xxmemdup(void* src, int size);

#ifdef __cplusplus
}
#endif

#endif // __XUTIL_H__
