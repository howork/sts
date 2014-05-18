#ifndef _XLEXER_H_
#define _XLEXER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "xToken.h"
#include "xSymbolTable.h"

/*
 *  Lexers are implemented as opaque structures accessed through pointers. 
 */

typedef struct _xLexer xLexer;

/* 
 * Constructs and returns a lexer (i.e., a pointer to the xLexer structure).
 * The lexer requires an open input file to provide the text in the source language to be scanned.
 * The caller can specify stdin if the text is expected to come in from the keyboard or from a pipe.
 * 
 * TODO: lexer from string
 */

xLexer* xLexer_create(xSymbolTable* table, FILE *in);


/* 
 * Collects characters from the source text until it has recognized a token.
 * If there are no more tokens in the source text, scan returns a null pointer.
 */

xToken* xLexer_scan(xLexer* lex);


/* 
 * Returns the (one-based) line number of the line in the source text file on which 
 * the next token will be sought.
 */

unsigned int xLexer_line(xLexer* lex);


/* 
 * Frees the storage associated with the lexer pointer to which its argument points.
 */

void xLexer_free(xLexer* lex);

#ifdef __cplusplus
}
#endif

#endif  // _XLEXER_H_
