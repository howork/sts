#ifndef _XSYMBOLTABLE_H_
#define _XSYMBOLTABLE_H_

#ifdef __cplusplus
extern "C" {
#endif

/* 
 * The lexical analyzers maintain internal tables of previously constructed tokens, 
 * keyed by the lexemes that prompted their construction.  
 * When the lexer reads in a new lexeme, it examines the table to determine whether 
 * a token for that lexeme has already been constructed, 
 * and if so it returns that token instead of building a new copy.
 * 
 * This header file defines the interface to a data type (xSymbolTable) for such internal tables 
 * -- lookup tables in which the keys are strings and the values are tokens.
 */

#include "xToken.h"

typedef struct _xSymbolTable xSymbolTable;

/*
 * 해쉬테이블의 디폴드 용량(DEFAULT_CAPACITY)은  비교적 작은 값인데, 
 * 이는 본 스크립트가 심플한 환경에서 사용된다고 가정했기 때문이다.
 */
#define xSymbolTable_DEFAULT_CAPACITY   17
#define xSymbolTable_MAX_LOAD_FACTOR    0.8



/**
 * 해쉬 테이블을 구축한다.
 * 빈 해쉬테이블에 대한 메모리를 할당하고, 최기화한다.
 * 
 * @param capacity 초기 해쉬테이블의 용량,  값이 0일 경우 디폴드값 사용
 * @return 구축된 해쉬 테이블에 대한 포인터
 */

xSymbolTable* xSymbolTable_create(int capacity);


/**
 * 해쉬테이블에 (lexeme, token) 쌍을 추가한다.
 * 해쉬체이블에 해당 lexeme이 등록되어 있다면 token 값을 대체한다.
 * lexeme과 token은 복제한 후 저장한다.
 * 
 * @param table 홰쉬 테이블에 대한 포인터
 * @param lexeme 렉심에 대한 문자열
 * @param tok 렉심에 해당하는 토큰
 */

void xSymbolTable_put(xSymbolTable* table, char *lexeme, xToken* tok);

/**
 * 해쉬테이블에 저장된 lexeme을 찾에 이와 연관된 토큰을 찾는다.
 * 
 * @param table  해쉬테이블
 * @param lexeme 찾고자 하는 렉심
 * @return 찾고자 하는 렉심이 등록되어 있는 경우에는 해당 토근, 그렇지 않은 경우는 null 포인터를 리턴
 */

xToken* xSymbolTable_get(xSymbolTable* table, char *lexeme);

/**
 * 현재의 적재 비율을 구한다.
 * @param table 해쉬 테이블
 * @return  적재 비율
 */
int xSymbolTable_load(xSymbolTable* table);

/**
 * 주어진 해쉬테이블을 해제한다.
 * 저장된 모든 렉심 및 토근들도 해제한다.
 * @param table 해쉬 테이블
 */
void xSymbolTable_free(xSymbolTable* table);

#ifdef __cplusplus
extern "C" {
#endif

#endif  // _XSYMBOLTABLE_H_
