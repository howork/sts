#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>

#include "xToken.h"
#include "xSymbolTable.h"
#include "xLexer.h"

struct _xLexer {
            unsigned int line;
            FILE *source;
            xSymbolTable* table;
        };


static char* basicTypes[] = { "char", "int", "bool", "float" };


/* The make_lexer function constructs and returns a lexer (i.e., a pointer
   to the hidden structure in terms of which the functions are
   implemented).  The lexer requires an open input file to provide the
   text in the source language to be scanned.  The caller can specify
   stdin if the text is expected to come in from the keyboard or from a pipe.
 */

xLexer* xLexer_create(xSymbolTable* table, FILE *in) 
{
    xLexer* lex = (xLexer*) malloc(sizeof (struct _xLexer));

    lex->source = in;
    lex->line = 1;
    lex->table = table;
    
/* The kw_put function adds an entry for a specified keyword into a
   specified lexeme-token hash table, dynamically allocating storage for
   its key.  It can also be used to introduce the names of basic types.
 */
    char* strdup(char*);    // TODO: include header file
                            // I don't know why strdup() isn't exist in string.h of MinGW
/*    
    for (int i = 0; i < sizeof(basicTypes) / sizeof(basicTypes[0]); i++) {
        xSymbolTable_put(lex->words, strdup(basicTypes[i]), xToken_createBasic(basicTypes[i]));
    }
*/
    return lex;
}

struct {
        char* symbol;
        xToken token;
    } opSymbols[] = {
        { "(",      { XLPAREN } },
        { ")",      { XRPAREN } },
        { "[",      { XLBRACKET} },
        { "]",      { XRBRACKET} },
        { "{",      { XLBRACE } },
        { "}",      { XRBRACE } },
        { "*",      { XSTAR   } },
        { "+",      { XPLUS   } },
        { "-",      { XMINUS  } },
        { "/",      { XSLASH  } },
        { ";",      { XSCOLON } },
        { "<",      { XLT     } },
        { "<=",     { XLE     } },
        { ">",      { XGT     } },
        { ">=",     { XGE     } },
        { "=",      { XASSIGN } },
        { "==",     { XEQ     } },
        { "!",      { XEXCL   } },
        { "!=",     { XNEQ    } },
        { "&&",     { XBAND   } },
        { "||",     { XBOR    } },
    };
    
#define arrayLength(a)  (sizeof(a)/sizeof((a)[0]))
    
static xToken* searchOpSymbol(char* str)    
{
    for (int i = 0; i < arrayLength(opSymbols); i++) {
        if (strcmp(str, opSymbols[i].symbol) == 0) {
            return &opSymbols[i].token;
        }
    }
    return NULL;
}

/* The scan function collects characters from the source text until it has
   recognized a token, which it then constructs and returns.  If there are
   no more tokens in the source text, scan returns a null pointer.
 */

xToken* xLexer_scan(xLexer* lex) 
{
    int peek = fgetc(lex->source);

    // skip whitespace characters, tallying newlines.
    while (isspace(peek)) {
        if (peek == '\n') {
            lex->line++;
        }
        peek = fgetc(lex->source);
    }

    // if we have reached the end of the input, return a null token.
    if (peek == EOF) {
        return NULL;
    }

    // otherwise, scan for a token. 

    if (isalpha(peek)) {
        // scan an identifier, keyword, or basic type....
        char buf[1024];
        int len = 0;
        
        do {
            buf[len++] = peek;
            peek = fgetc(lex->source);
        } while (isalnum(peek));
        buf[len] = '\0';
        
        ungetc(peek, lex->source);
        
        char *lexeme = buf;

        // Check whether it has been encountered before (or is a keyword or a basic type).  
        // If so, return the stored token.
        xToken* tok = xSymbolTable_get(lex->table, lexeme);
        if (tok == NULL) { // Otherwise, build a token and return it. 
            tok = xToken_createId(lexeme);
            xSymbolTable_put(lex->table, lexeme, tok);
        }
        return tok;
    } 
    

    if (isdigit(peek)) {    // Scan a numeral.
        int value = 0;
        do {
            value = 10 * value + (peek - '0');
            peek = fgetc(lex->source);
        } while (isdigit(peek));
        
        if (peek == '.') {  // The numeral contains a decimal point and therefore 
                            // denotes a real number.  Scan its fractional part.
            double realValue = (double) value;
            unsigned long denominator = 10;
            while (isdigit(peek = fgetc(lex->source))) {
                realValue += (peek - '0') / (double) denominator;
                denominator *= 10;
            }
            ungetc((char) peek, lex->source);
            return xToken_createFloat(realValue);
        } 
        
        // The numeral doesn't contain a decimal point and so denotes an integer.
        ungetc((char) peek, lex->source);
        return xToken_createInt(value);
    }
    
    char str[10] = { 0 };
    
    str[0] = peek;
    xToken* tok0 = searchOpSymbol(str);

    str[1] = fgetc(lex->source);
    xToken* tok1 = searchOpSymbol(str);

    str[2] = fgetc(lex->source);
    xToken* tok2 = searchOpSymbol(str);
    
    if (tok2 != NULL) { // three charactors operator
        return tok2;
    }

    if (tok1 != NULL) { // two charactors operator
        ungetc(str[2], lex->source);
        return tok1;
    }

    if (tok0 != NULL) { // one charactor operator/delimiter
        ungetc(str[2], lex->source);
        ungetc(str[1], lex->source);
        return tok0;
    }
    
    // TODO: throw "unexpected charactor"
    assert(false);
    
    return NULL;
}

/* Returns the (one-based) line number of the line in the source text file 
 * on which the next token will be sought.
 */

unsigned int xLexer_line(xLexer* lex) 
{
    return lex->line;
}

/* Frees the storage associated with the lexer pointer to which its argument points.
 */

/*
 심볼테이블은 해제하지 않으므로 밖(lex를 만든 곳)에서 해제하여야 한다.
 */
void xLexer_free(xLexer* lex) 
{
    free(lex);
}
