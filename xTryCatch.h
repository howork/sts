/* Copyleft (L) 2014 Munho Choi
 *
 * The original file is try_throw_catch.h from
 *   http://www.di.unipi.it/~nids/docs/longjump_try_trow_catch.html
 *
 * Additional functions are:
 *      - changed names
 *      - cross functional try catch
 *      - extended to insert enum declaration
 *      - uncaught exceptions are handled
 * Please see the try_catch_example.c
 */

#ifndef __XTRY_CATCH_H__
#define __XTRY_CATCH_H__

#include <setjmp.h>

#define DECL_XTRY_CATCH(ex...)  \
                    enum { _EX_EX_BUF_OVERFLOW = -1,                        \
                           _EX_EX_BUF_UNDERFLOW = -2,                       \
                           _EX_EX_UNCATCHABLE_ = 0, ex };                   \
                    extern jmp_buf _ex_jmp_buf_[];                          \
                    const int _ex_jmp_buf_depth_;                           \
                    extern int _ex_jmp_buf_top_;                            \
                    extern int _ex_uncaught_;                               \
                    extern int _ex_thrown_value_;                           \
                    extern const int _trying_local_try_ ;                   \
                    extern int _local_ex_uncaught_;                         \
                    extern const int _local_ex_thrown_value_ ;              //

#define DEF_XTRY_CATCH(depth)  \
                    jmp_buf _ex_jmp_buf_[depth];                            \
                    const int _ex_jmp_buf_depth_ = depth;                   \
                    int _ex_jmp_buf_top_ = -1;                              \
                    int _ex_uncaught_ = 0;                                  \
                    int _ex_thrown_value_  = _EX_EX_UNCATCHABLE_;           \
                    const int _trying_local_try_ = 0;                       \
                    int _local_ex_uncaught_ = 0;                            \
                    const int _local_ex_thrown_value_ = _EX_EX_UNCATCHABLE_;//

#define xtry        \
                    do {                                                    \
                        if (_ex_jmp_buf_top_+1 >= _ex_jmp_buf_depth_) {     \
                            longjmp(_ex_jmp_buf_[0], _EX_EX_BUF_OVERFLOW);  \
                        }                                                   \
                        _ex_jmp_buf_top_++;                                 \
                        switch ( setjmp(_ex_jmp_buf_[_ex_jmp_buf_top_]) ) { \
                        case 0:                 while (1) {                 //

#define localtry(ex...) \
                    do {                                                    \
                        enum { _EX_EX_UNCATCHABLE_, ex };                   \
                        jmp_buf _local_ex_jmp_buf_;                         \
                        int _local_ex_uncaught_ = 0;                        \
                        int _local_ex_thrown_value_ = _EX_EX_UNCATCHABLE_;  \
                        const int _trying_local_try_ = 1;                   \
                        switch ( setjmp(_local_ex_jmp_buf_) ) {             \
                        case 0:                 while (1) {                 //

#define xcatch(x)   \
                        break;                                              \
                        case x: {                                           \
                                    if (_trying_local_try_) {               \
                                        _local_ex_uncaught_ = 0;            \
                                    }                                       \
                                    else {                                  \
                                        _ex_uncaught_ = 0;                  \
                                    }                                       \
                                }                                           //

#define xfinally    \
                        break;                  }                           \
                        default: {}                                         //

#define xendtry     \
                        }                                                   \
                        if (_trying_local_try_) {                           \
                            if (!_local_ex_uncaught_) break;                \
                            fprintf(stderr, "Uncaught exception %d :"       \
                                            " at file %s, line %d\n",       \
                                            _local_ex_thrown_value_,        \
                                            __FILE__, __LINE__);            \
                            exit(-1);                                       \
                        }                                                   \
                        else {                                              \
                            _ex_jmp_buf_top_--;                             \
                            if (!_ex_uncaught_) break;                      \
                            if (_ex_jmp_buf_top_ < 0) {                     \
                                fprintf(stderr, "Uncaught exception %d :"   \
                                                " at file %s, line %d\n",   \
                                                _ex_thrown_value_,          \
                                                __FILE__, __LINE__);        \
                                exit(-1);                                   \
                            }                                               \
                            longjmp(_ex_jmp_buf_[_ex_jmp_buf_top_],         \
                                                _ex_thrown_value_);         \
                        }                                                   \
                    } while(0)                                              //

#define xthrow(x)    \
                    do {                                                    \
                        _ex_uncaught_ = 1;                                  \
                        _ex_thrown_value_ = x;                              \
                        longjmp(_ex_jmp_buf_[_ex_jmp_buf_top_], x);         \
                    } while (0)                                             //

#define xrethrow(x)  \
                    do {                                                    \
                        _ex_jmp_buf_top_--;                                 \
                        _ex_uncaught_ = 1;                                  \
                        _ex_thrown_value_ = x;                              \
                        if (_ex_jmp_buf_top_ < 0) {                         \
                            fprintf(stderr, "Uncaught exception %d :"       \
                                            " at file %s, line %d\n",       \
                                            _ex_thrown_value_,              \
                                            __FILE__, __LINE__);            \
                            exit(-1);                                       \
                        }                                                   \
                        longjmp(_ex_jmp_buf_[_ex_jmp_buf_top_], x);         \
                    } while (0)                                             //

#define localthrow(x)   \
                    do {                                                    \
                        _local_ex_uncaught_ = 1;                            \
                        _local_ex_thrown_value_ = x;                        \
                        longjmp(_local_ex_jmp_buf_, x);                     \
                    } while (0)                                             //

#endif // __XTRY_CATCH_H__
