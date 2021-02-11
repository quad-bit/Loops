#pragma once

#include <iostream>

#ifndef NDEBUG
    #define ASSERT_MSG(Expr, Msg) \
        AssertWithMsg(#Expr, Expr, __FILE__, __LINE__, Msg)

    #define ASSERT(Expr) \
        Assert(#Expr, Expr, __FILE__, __LINE__)

    #define DEPRECATED ASSERT_MSG(0, "Deprecated") 

#else
    #define ASSERT_MSG(Expr, Msg) ;
    #define ASSERT(Expr);
    #define DEPRECATED ; 
#endif

void AssertWithMsg(const char* expr_str, bool expr, const char* file, int line, const char* msg);

void Assert(const char* expr_str, bool expr, const char* file, int line);

