#pragma once

#include "crtdbg.h"

/* Asserts */
/* We use !! below to ensure that any overloaded operators used to evaluate expr do not end up at operator || */
#define ATLX_ASSERT_EXPR(expr, msg) \
        (void) ((!!(expr)) || \
                (1 != _CrtDbgReportW(2, _CRT_WIDE(__FILE__), __LINE__, NULL, L"%s", msg)) || \
                (_CrtDbgBreak(), 0))

#ifndef ATLX_ASSERT
#define ATLX_ASSERT(expr)   ATLX_ASSERT_EXPR((expr), NULL)
#endif  /* _ASSERT */
