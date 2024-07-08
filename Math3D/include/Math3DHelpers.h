#pragma once

#ifdef APPLE
#include <simd/simd.h>
#define IFAPPLE(expr) expr
#else
#define IFAPPLE(expr)
#endif // APPLE

#ifdef WIN32
#include <DirectXMath.h>
#define IFDX(expr) expr
#else
#define IFDX(expr)
#endif // WIN32

