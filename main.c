/*
 * Copyright (c) 2014 Munho Choi
 * 
 * See the file LICENSE.txt or LICENSE-KOR.txt for copying permission.
*/

#include <stdio.h>
#include <stdlib.h>

#include "xUtil.h"
#include "xToken.h"

int main(int argc, char* argv[]) {

    xtry {
        xToken* tok = xToken_createInt(10);
        char tmp[100];
        printf("%s\n", xToken_toString(tok, tmp));
        xToken_free(tok);
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

