#ifndef _MATH_H
#define _MATH_H

#ifndef MATHLIB_STRICT
#define MATHLIB_STRICT 1
#endif

 // FIXME: these are intended to be called only from the calculation thread.

#ifdef __CHIMERA__
#define _math_h_uint32 unsigned long
#define _math_h_inline inline
#else
#define _math_h_uint32 unsigned int
#define _math_h_inline __inline
#endif // __CHIMERA__

#define INFINITY ((float) (1e38*10.0))
#define HUGE_VALF INFINITY

#define M_E 2.718281828459046
#define M_LOG2E 1.442695040888963
#define M_LOG10E 0.434294481903252
#define M_LN2 0.693147180559945
#define M_LN10 2.302585092994046
#define M_PI 3.141592653589793
#define M_PI_2 1.570796326794897
#define M_PI_4 0.785398163397448
#define M_1_PI 0.318309886183791
#define M_2_PI 0.636619772367581
#define M_2_SQRT_PI 1.128379167095513
#define M_SQRT2 1.414213562373095
#define M_SQRT1_2 0.707106781186547

#define FP_ILOGB0 INT_MIN

#ifdef __CHIMERA__

// These functions are supposed to exist as XYZ and __builtin_XYZ, but
// we force the use of __builtin_XYZ() to be extra clear about where
// they are implemented and also they should still work with
// -fno-builtin. See discussion at TL-39155.

float __builtin_fabsf(float);
#define fabsf(x) __builtin_fabsf(x)
float __builtin_sqrtf(float);
#define sqrtf(x) ((x) == 0.0f ? (x) : __builtin_sqrtf(x))
float __builtin_ceilf(float);
float __builtin_floorf(float);
float __builtin_truncf(float);

#if MATHLIB_STRICT
#  define ceilf(x) (((x) == 0.0f) ? (x) : __builtin_ceilf(x))
#  define floorf(x) (((x) == 0.0f) ? (x) : __builtin_floorf(x))
#  define truncf(x) (((x) == 0.0f) ? (x) : __builtin_truncf(x))
#else
#  define ceilf(x) __builtin_ceilf(x)
#  define floorf(x) __builtin_floorf(x)
#  define truncf(x) __builtin_truncf(x)
#endif

#else
// These are gcc builtins on Chimera but implemented explicitly
// otherwise:
float fabsf(float);
float sqrtf(float);
float ceilf(float);
float floorf(float);
float truncf(float);
#endif

// Use mathlib_roundf until TL-39546 is fixed
float mathlib_roundf(float x);
static _math_h_inline float roundf(float x) { return mathlib_roundf(x); }

float fmodf(float x, float y);
float remainderf(float x, float y);
float remquof(float x, float y, int *quo);
// FIXME: implement fmaf()
//float fmaf(float x, float y, float z);
float fmaxf(float x, float y);
float fminf(float x, float y);
float fdimf(float x, float y);
// float nanf(const char *arg);
float expf(float x);
float exp2f(float x);
float expm1f(float x);
float logf(float x);
float log2f(float x);
float log10f(float x);
float log1pf(float x);
float powf(float x, float y);
float cbrtf(float x);
float hypotf(float x, float y);
float sinf(float x);
float cosf(float x);
float tanf(float x);
float asinf(float arg);
float acosf(float arg);
float atanf(float x);
float atan2f(float y, float x);
float sinhf(float x);
float coshf(float x);
float tanhf(float x);
float asinhf(float arg);
float acoshf(float arg);
float atanhf(float arg);
float erff(float x);
float erfcf(float x);
float tgammaf(float x);
float lgammaf(float x);
long lroundf(float x);
float nearbyintf(float);
float rintf(float);
long lrintf(float);
float frexpf(float arg, int *exp);
float ldexpf(float arg, int exp);
float modff(float arg, float *iptr);
float scalbnf(float, int);
float scalblnf(float, long);
int ilogbf(float);
float logbf(float);
float nextafterf(float, float);
float copysignf(float, float);

#define isfinite(arg) _isfinite(arg)
static _math_h_inline int _isfinite(float arg)
{
#ifdef __CHIMERA__
	int rv;
	asm volatile(
		"        sext A,Y,#8,#7\n"
		"        inc A\n"
		"        snz A\n"
		: "=a" (rv)
		: "b" (arg)
		: "cc");
	return rv;
#else
	union {
		struct {
			_math_h_uint32 mantissa : 23,
				exponent : 8,
				sign : 1;
		};
		float f;
		_math_h_uint32 v;
	} f;
	f.f = arg;
	return f.exponent != 255;
#endif
}

#define isinf(arg) _isinf(arg)
static _math_h_inline int _isinf(float arg)
{
#ifdef __CHIMERA__
	int rv;
	asm volatile(
		"        and Y, #$7fff\n"
		"        cmp Y, #$7f80\n"
		"        ifset z\n"
		"          cmp A, #$0000\n"
		"        sz A\n"
		: "=a" (rv)
		: "b" (arg)
		: "cc");
	return rv;
#else
	union {
		struct {
			_math_h_uint32 mantissa : 23,
				exponent : 8,
				sign : 1;
		};
		float f;
		_math_h_uint32 v;
	} f;
	f.f = arg;
	return f.exponent == 255 && f.mantissa == 0;
#endif
}

// Chimera does not support NaN
#define isnan(arg) 0

#define isnormal(arg) _isnormal(arg)
static _math_h_inline int _isnormal(float arg)
{
#ifdef __CHIMERA__
	int rv;
	asm volatile(
		"        uext A, Y, #8, #7\n"
		"        ifclr z\n"
		"          cmp A, #$00FF\n"
		"        snz A\n"
		: "=a" (rv)
		: "b" (arg)
		: "cc");
	return rv;
#else
	union {
		struct {
			_math_h_uint32 mantissa : 23,
				exponent : 8,
				sign : 1;
		};
		float f;
		_math_h_uint32 v;
	} f;
	f.f = arg;
	return f.exponent < 255 && f.exponent > 0;
#endif
}

#define signbit(arg) _signbit(arg)
static _math_h_inline int _signbit(float arg)
{
#ifdef __CHIMERA__
	int rv;
	asm volatile(
		"        uext A, Y, #1, #15\n"
		: "=a" (rv)
		: "b" (arg)
		: "cc");
	return rv;
#else
	union {
		struct {
			_math_h_uint32 mantissa : 23,
				exponent : 8,
				sign : 1;
		};
		float f;
		_math_h_uint32 v;
	} f;
	f.f = arg;
	return f.sign;
#endif
}

#ifdef __CHIMERA__
#define isgreater(x, y) __builtin_isgreater(x, y)
#define isgreaterequal(x, y) __builtin_isgreaterequal(x, y)
#define isless(x, y) __builtin_isless(x, y)
#define islessequal(x, y) __builtin_islessequal(x, y)
#define islessgreater(x, y) __builtin_islessgreater(x, y)
#define isunordered(x, y) __builtin_isunordered(x, y)
#else
// These are supposed to disable exceptions but Chimera doesn't have
// exceptions so we're ignoring them:
#define isgreater(x, y) (x > y)
#define isgreaterequal(x, y) (x >= y)
#define isless(x, y) (x < y)
#define islessequal(x, y) (x <= y)
#define islessgreater(x, y) (x < y || x > y)
#define isunordered(x, y) (isnan(x) || isnan(y))
#endif

#undef _math_h_uint32
#undef _math_h_inline
#endif
