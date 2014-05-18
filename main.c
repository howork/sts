/*
 * Copyright (c) 2014 Munho Choi
 * 
 * See the file LICENSE.txt or LICENSE-KOR.txt for copying permission.
*/

#include <stdio.h>
#include <stdlib.h>

#include "xUtil.h"
#include "xToken.h"
#include "xSymbolTable.h"
#include "xLexer.h"

int main(int argc, char* argv[]) {

    xtry {
        {
            xToken* tk = xToken_createInt(123);
            printToken(tk);
            xToken_free(tk);
        }
        
        {
            xSymbolTable* table = xSymbolTable_create(0);
    
//             xToken* tkFor = xToken_create(XFOR, "for");
//             xSymbolTable_put(table, "for", tkFor);
//             xToken* x = xSymbolTable_get(table, "for");
//             printToken(tkFor);
//             printToken(x);
//            xToken_free(tkFor);

            struct {
                        char* name;
                        xTokenTag tag;
                    } keywords[] = {
                        { "if",     XIF     },
                        { "else",   XELSE   },
                        { "while",  XWHILE  },
                        { "do",     XDO     },
                        { "break",  XBREAK  },
                        { "true",   XTRUE   },
                        { "false",  XFALSE  },
                    };
            
            for (int i = 0; i < sizeof(keywords) / sizeof(keywords[0]); i++) {
                xSymbolTable_put(table, keywords[i].name, xToken_create(keywords[i].tag, keywords[i].name));
            }

            xLexer* lex = xLexer_create(table, stdin);
            xToken* tk = xLexer_scan(lex);
            printToken(tk);

            xSymbolTable_free(table);
            xLexer_free(lex);
        }
        
    }
    xcatch(EX_ALLOC) {
        DDS("memory alloc error");
    }
    xcatch(EX_FREE) {
        DDS("memory free error");
    }
    xfinally {
        printf("Good bye!\n");
    }
    xendtry;
    
    return (EXIT_SUCCESS);
}

