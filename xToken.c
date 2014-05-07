#include "xUtil.h"
#include "xToken.h"

/* There are four constructors for tokens, depending on whether the token 
   is an integer numeral, a floating point numeral, an identifier, or something else.
 */


/* constructs and returns a token for something other than a numeral, an identifier, or an identifier.  
   It presupposes that the given tag value is an element of the enumeration (other than XINT, XFLOAT, XID).
 */

xToken* xToken_createSimple(xTokenTag tag) 
{
    xToken* tok = xalloc(xToken);
    tok->tag = tag;
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
    xToken* tok = xToken_createSimple(XID);
    tok->value.lexeme = xstrdup(lexeme);
    return tok;
}


static char *xxxTokenString[] = {
#define x(tok, str) str,
        XTOKEN_LIST_SPEICAL_CHARS
//        XTOKEN_LIST_RESERVED_WORD     
//        XTOKEN_LIST_EXTRA
#undef x
};

/* returns an external representation of a given token. 
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
    sprintf(buf, "<%s>\n", xxxTokenString[tok->tag]);
    return buf;
}

/* The free_token function deallocates the storage associated with the
   token pointer to which its argument points.

   This function does free dynamic storage for strings in the
   lexeme field of identifier and basic-type tokens.
 */

void xToken_free(xToken* tok) 
{
    if (tok->tag == XID) {
        xfree(tok->value.lexeme);
    }

    xfree(tok);
}
