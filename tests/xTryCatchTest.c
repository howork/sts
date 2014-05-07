#include <stdio.h>
#include <stdlib.h>

#include "xTryCatch.h"


#define pp(fmt, ...)  printf("%s: " fmt "\n", __FUNCTION__, ##__VA_ARGS__)

DECL_XTRY_CATCH(EX_ONE, EX_TWO, EX_THREE);
//DEF_XTRY_CATCH(100);

int gun()
{
    pp("== BEGIN ==");

    xthrow(EX_TWO);

    pp("== END ==");

    return 0;
}

int fun()
{
    pp("== BEGIN ==");

    xtry {
        pp("hello");
        gun();
        pp("world");
    }
    xcatch(EX_ONE) {
        pp("catch EX_ONE");
    }
    xfinally {
        pp("final");
    }
    xendtry;

    pp("== END ==");

    return 0;
}


int crossTryBasic()
{
    pp("== BEGIN ==");

    xtry {
        pp("hello");
        xthrow(EX_ONE);
        pp("world");
    }
    xcatch(EX_ONE) {
        pp("catch EX_ONE");
    }
    xfinally {
        pp("final");
    }
    xendtry;

    pp("== MIDDLE ==");

    xtry {
        pp("hello");
        fun();
        pp("world");
    }
    xcatch(EX_ONE) {
        pp("catch EX_ONE");
    }
    xfinally {
        pp("final");
    }
    xendtry;

    pp("== END ==");

    return 0;
}

void crossTryLoop()
{
    pp("== BEGIN ==");

    xtry {
        int i;
        for (i = 0; i < 10; i++) {
            xtry {
                if (i == 3) {
                    xthrow(EX_ONE);
                }
                if (i == 5) {
                    xthrow(EX_THREE);
                }
            }
            xcatch(EX_ONE) {
                pp("catch EX_ONE, i = %d", i);
            }
            xfinally {
                pp("for final  i = %d", i);
            }
            xendtry;
        }

    }
    xcatch(EX_THREE) {
        pp("catch EX_THREE");
    }
    xfinally {
        pp("final");
    }
    xendtry;

    pp("== END ==");
}

void crossTryRethrow()
{
    pp("== BEGIN ==");

    xtry {
        xtry {
            xthrow(EX_ONE);
        }
        xcatch(EX_ONE) {
            pp("catch EX_ONE");
            xthrow(EX_TWO);
        }
        xcatch(EX_TWO) {
            pp("catch EX_TWO");
        }
        xcatch(EX_THREE) {
            pp("catch EX_THREE");
        }
        xfinally {
            pp("inner final");
            xrethrow(EX_THREE);
        }
        xendtry;

    }
    xcatch(EX_THREE) {
        pp("catch EX_THREE");
    }
    xfinally {
        pp("final");
    }
    xendtry;

    pp("== END ==");
}

////////////////////////////////////////////////////////

int localTryBasic()
{
    pp("== BEGIN ==");

    localtry(EX_FUN, EX_GUN) {
        pp("hello");
        localthrow(EX_FUN);
        pp("world");
    }
    xcatch(EX_FUN) {
        pp("catch EX_FUN");
    }
    xcatch(EX_GUN) {
        pp("catch EX_GUN");
    }
    xfinally {
        pp("final");
    }
    xendtry;

    pp("== END ==");

    return 0;
}

int localTryNested()
{
    pp("== BEGIN ==");

    localtry(EX_FUN, EX_GUN) {
        pp("hello");
        localtry(EX_INNER_FUN) {
            pp("inner hello");
            //localthrow(EX_GUN);   // occur uncaught exception
                                    // outer localtry에서 선언된 것을 사용할 수 있으나
                                    // (그냥 정수이므로)
                                    // 본 localtry의 catch절에서 catch하여야 한다.
            localthrow(EX_INNER_FUN);
            pp("inner world");
        }
        xcatch(EX_INNER_FUN) {
            pp("catch EX_INNER_FUN");
        }
        xfinally {
            pp("inner final");
        }
        xendtry;
        pp("wonderful");
        localthrow(EX_FUN);
        pp("world");
    }
    xcatch(EX_FUN) {
        pp("catch EX_FUN");
    }
    xcatch(EX_GUN) {
        pp("catch EX_GUN");
    }
    xfinally {
        pp("final");
    }
    xendtry;

    pp("== END ==");

    return 0;
}

int localTryOmitted()
{
    // xcatch 절은 생략할 수 있으나 xfinally 절은 생략할 수 없다.
    // 단, 간단히 finally만을 써도 된다.

    pp("== BEGIN ==");

    localtry(EX_FUN, EX_GUN) {
        pp("hello");
        localthrow(EX_FUN);
        pp("world");
    }
    xcatch(EX_FUN) {
        pp("catch EX_FUN");
    }
    xcatch(EX_GUN) {
        pp("catch EX_GUN");
    }
    xfinally
    xendtry;

    localtry(EX_FUN, EX_GUN) {
        pp("hello");
        localthrow(EX_GUN);
        pp("world");
    }
    xfinally {
        pp("final");
    }
    xendtry;

    pp("== END ==");

    return 0;
}

///////////////////////////////////////////////////////////////////
void mixedTryBasic()
{
    pp("== BEGIN ==");

    xtry {
        pp("hello");
        localtry(E1) {
            pp("local hello");
            localthrow(E1);
            pp("local world");
        }
        xcatch(E1) {
            pp("local xcatch E1");
        }
        xfinally {
            pp("local final");
            xthrow(EX_TWO);
        }
        xendtry;
    }
    xcatch(EX_ONE) {
        pp("catch EX_ONE");
    }
    xcatch(EX_TWO) {
        pp("catch EX_TWO");
    }
    xfinally {
        pp("final");
    }
    xendtry;

    pp("== END ==");
}
///////////////////////////////////////////////////////////////////
int main()
{
    pp("=============  Cross  ================");

    xtry {
        crossTryBasic();
    }
    xcatch(EX_TWO) {
        pp("catch EX_TWO");
    }
    xfinally {
        pp("final");
    }
    xendtry;

    pp("=============  Loop  ================");

    crossTryLoop();

    pp("============= Rethrow =====================");

    crossTryRethrow();

    pp("============= Mixed =====================");

    mixedTryBasic();

    pp("============= Local ======================");

    localTryBasic();
    localTryNested();
    localTryOmitted();

    return 0;
}
