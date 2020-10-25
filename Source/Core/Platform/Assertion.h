#pragma once

#include <iostream>

namespace Loops::Core
{

    #ifndef NDEBUG
    #define ASSERT_MSG(Expr, Msg) \
        AssertWithMsg(#Expr, Expr, __FILE__, __LINE__, Msg)

    #define ASSERT(Expr) \
    	Assert(#Expr, Expr, __FILE__, __LINE__)

    #else
    #define ASSERT_MSG(Expr, Msg) ;
    #define ASSERT(Expr);
    #endif

    void AssertWithMsg(const char* expr_str, bool expr, const char* file, int line, const char* msg);

    void Assert(const char* expr_str, bool expr, const char* file, int line);

}
