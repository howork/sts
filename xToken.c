#include "xUtil.h"
#include "xToken.h"

/* There are four constructors for tokens, depending on whether the token 
   is an integer numeral, a floating point numeral, an identifier, or something else.
 */


/* constructs and returns a token for something other than a numeral, an identifier, or an identifier.  
   It presupposes that the given tag value is an element of the enumeration (other than XINT, XFLOAT, XID).
 */

xToken* xToken_createSimple(xTokenTag kind) 
{
    xToken* tok = xalloc(xToken);
    tok->tag = kind;
    return tok;
}


xToken* xToken_create(int tag, char *lexeme) 
{
    xToken* tok = xToken_createSimple(tag);
    tok->value.lexeme = xstrdup(lexeme);
    return tok;
}

/* constructs and returns a token with the tag XINT, 
   setting the value field to the integer specified by the argument.
 */

xToken* xToken_createInt(xint value) 
{
    xToken* tok = xToken_createSimple(XINT);
    tok->value.xint = value;
    return tok;
}

/* constructs and returns a token with the tag XFLOAT, 
   setting the value field to the (double precision) floating point number
   specified by the argument.
 */

xToken* xToken_createFloat(xfloat value) 
{
    xToken* tok = xToken_createSimple(XFLOAT);
    tok->value.xfloat = value;
    return tok;
}

/* constructs and returns a token with the tag XID, 
   setting the lexeme field to the string specified by the argument 
   (duplicate the original string).
 */

xToken* xToken_createId(char *lexeme) 
{
    return xToken_create(XID, lexeme);
}

/*
 문자열 buf 토큰을 문자열로 변활할 만큼 충분한 메모리를 확보하고 있어야 한다.
 */
char* xToken_toString(xToken* tok, char* buf) 
{
    if (tok->tag == XID) {
       sprintf(buf, "id = %s\n", tok->value.lexeme);
        return buf;
    }
    if (tok->tag == XINT) {
        sprintf(buf, "value = %d\n", tok->value.xint);
        return buf;
    }
    if (tok->tag == XFLOAT) {
        sprintf(buf, "value = %g\n", tok->value.xfloat);
        return buf;
    }
    sprintf(buf, "<%s>\n", tok->value.lexeme);
    return buf;
}

/*
 토큰(토큰에 할당된 메모리)을 해제한다. 
 토큰 생성시 부가적으로 할당된 메모리(lexeme...)도 해제한다.
 */
void xToken_free(xToken* tok) 
{
    if (tok->tag != XINT && tok->tag != XFLOAT) {
        xfree(tok->value.lexeme);
    }

    xfree(tok);
}
