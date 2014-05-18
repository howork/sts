#ifndef _XTOKEN_H_
#define _XTOKEN_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "xUtil.h"

/*
    Tokens are identified by tags, which are small integers (first column).
    Lexemes are denoted in second column.
    if x is defined as @code 
        #define x(tok, str) tok
     @endcode
     then, tag (first column) will be extracted, and x is defined as @code 
        #define x(tok, str) str
     @endcode
     then, lexeme (second column) will be extracted.
 */

#define XTOKEN_LIST_SPEICAL_CHARS     \
        x(      LPAREN,         "("             ) \
        x(      RPAREN,         ")"             ) \
        x(      LBRACE,         "{"             ) \
        x(      RBRACE,         "}"             ) \
        x(      LBRACKET,       "["             ) \
        x(      RBRACKET,       "]"             ) \
        x(      TILDE,          "~"             ) \
        x(      GRAVE,          "`"             ) \
        x(      EXCL,           "!"             ) \
        x(      AT,             "@"             ) \
        x(      SHARP,          "#"             ) \
        x(      DALLAR,         "$"             ) \
        x(      PERCENT,        "%"             ) \
        x(      CARET,          "^"             ) \
        x(      AMP,            "&"             ) \
        x(      STAR,           "*"             ) \
        x(      MINUS,          "-"             ) \
        x(      PLUS,           "+"             ) \
        x(      EQUAL,          "="             ) \
        x(      VBAR,           "|"             ) \
        x(      BSLASH,         "\\"            ) \
        x(      COLON,          ":"             ) \
        x(      SCOLON,         ";"             ) \
        x(      DQUOTE,         "\""            ) \
        x(      SQUOTE,         "\'"            ) \
        x(      LT,            "<"              ) \
        x(      COMMA,          ","             ) \
        x(      GT,            ">"              ) \
        x(      DOT,            "."             ) \
        x(      QMARK,          "?"             ) \
        x(      SLASH,          "/"             ) \
                                                  \
        x(      INC,            "++"            ) \
        x(      DEC,            "--"            ) \
        x(      EXP,            "**"            ) \
        x(      GE,             ">="            ) \
        x(      LE,             "<="            ) \
        x(      EQ,             "=="            ) \
        x(      NEQ,            "<>"            ) \
        x(      BAND,           "&&"            ) \
        x(      BOR,            "||"            ) \
        x(      BXOR,           "||"            ) \
        x(      SHL,            "<<"            ) \
        x(      SHR,            ">>"            ) \
        x(      ASHR,           ">>>"           ) \
        x(      PLUS_ASSIGN,    "+="            ) \
        x(      MINUS_ASSIGN,   "-="            ) \
        x(      MUL_ASSIGN,     "*="            ) \
        x(      DIV_ASSIGN,     "/="            ) \
        x(      MOD_ASSIGN,     "%="            ) \
        x(      BAND_ASSIGN,    "&&="           ) \
        x(      BOR_ASSIGN,     "||="           ) \
        x(      BXOR_ASSIGN,    "^^="           ) \
        x(      SHL_ASSIGN,     "<<="           ) \
        x(      SHR_ASSIGN,     ">>="           ) \
        x(      ASHR_ASSIGN,    ">>>="          ) \

#define XTOKEN_LIST_RESERVED_WORD \
        x(      UNDEF,          "undef"         ) \
        x(      NULL,           "null"          ) \
        x(      FALSE,          "false"         ) \
        x(      TRUE,           "ture"          ) \
                                                  \
        x(      NOT,            "not"           ) \
        x(      AND,            "and"           ) \
        x(      OR,             "or"            ) \
                                                  \
        x(      IF,             "if"            ) \
        x(      ELSE,           "else"          ) \
        x(      WHILE,          "while"         ) \
        x(      DO,             "do"            ) \
        x(      FOR,            "for"           ) \
        x(      FOREACH,        "foreach"       ) \
                                                  \
        x(      BREAK,          "break"         ) \
        x(      NEXT,           "next"          ) \
        x(      RETURN,         "return"        ) \
        x(      YIELD,          "yield"         ) 
                                                  

#define XTOKEN_LIST_EXTRA       \
        x(      ILLEGAL,        "illegal"       ) \
        x(      EOS,            "end-of-stream" ) \
        x(      EOL,            "end-of-line"   ) \
                                                  \
        x(      WCOMMENT,       "word-comment"  ) \
        x(      LCOMMENT,       "line-comment"  ) \
        x(      BCOMMENT,       "block-cpmment" ) \
                                                  \
        x(      ID,             "id"            ) \
        x(      BOOL,           "bool"          ) \
        x(      BYTE,           "byte"          ) \
        x(      CHAR,           "char"          ) \
        x(      INT,            "int"           ) \
        x(      FLOAT,          "float"         ) \
                                                  \
        x(      STRING,         "string"        ) \
        x(      XSTRING,        "ext-string"    ) \
        x(      PATTERN,        "pattern"       ) \
                                                  \
        x(      MUL=XSTAR,      "*"             ) \
        x(      DIV=XSLASH,     "/"             ) \
        x(      MOD=XPERCENT,   "%"             ) \
        x(      ASSIGN=XEQUAL,  "="             ) 

typedef enum {
            #define x(tok, str) X##tok,
                XTOKEN_LIST_SPEICAL_CHARS
                XTOKEN_LIST_RESERVED_WORD     
                XTOKEN_LIST_EXTRA
            #undef x
                    
        } xTokenTag;


/* Tokens for numerals and identifiers contain additional information besides their tags.  
   A numeral contains is value (as an int(as xint) or a double (as xfloat))
   and an identifier contains its lexeme (as a string, e.g., char* of allocated memory).
 */

/** 토큰객체
토큰은 tag에 의해서 구분되며, 숫자와 식별자는 value에 해당 값 또는 lexeme를 저장한다. 
(정수형은 xint에, 실수형은 xfloat에, 식별자의 이름은 lexeme에 저장)
 */
typedef struct _xToken {
            xTokenTag tag;
            union {
                char* lexeme;   /// 식별자의 이름 저장, 
                                /// 토큰객체를 생성할 때 메모리가 할당되고, 토큰객체 해제시 메모리가 해제된다. 
                int xint;       /// 정수 토큰일 때 정수값 저장
                double xfloat;  ///  실수 토큰일 때 실수삾 저장
            } value;
        } xToken;


/* 
 In this minimal implementation, we provide functions for constructing, displaying, and freeing tokens.
 토큰객체를 생성하는 네가지 함수와 토큰을 문자열로 변환하는 함수, 토큰객체를 해제하는 함수
 */

/// 단순 타입의 토큰객체을 생성한다.
xToken* xToken_createSimple(xTokenTag kind);
/// 
xToken* xToken_create(int kind, char *lexeme);
/// XINT 타입의 토큰객체을 생성한다.
xToken* xToken_createInt(xint value);
/// XFLOAT 타입의 토큰객체을 생성한다.
xToken* xToken_createFloat(xfloat value);
/// XID (identifier) 타입의 토큰객체를 생성한다.
xToken* xToken_createId(char* lexeme);

/// 토큰을 문자열도 변환한다.
char* xToken_toString(xToken* tok, char buf[]);

/// 토큰 객체를 해제한다.
void xToken_free(xToken* tok);

#define printToken(tok)     \
        do {                                            \
            char tmp[100];                              \
            printf("%s\n", xToken_toString(tok, tmp));  \
        } while(0)                                      //
    
#ifdef __cplusplus
}
#endif

#endif // _XTOKEN_H_
