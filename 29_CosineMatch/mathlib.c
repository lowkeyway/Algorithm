/* Chimera Math Library */
#include <stdio.h>
#include "math.h"
#include <limits.h>

// Still TODO:

// float nanf(const char *arg) -- not supported by Chimera
// fpclassify(arg)
// floating-point exception handling
// rounding modes

#ifndef MATHLIB_HIGH_ACCURACY
#define MATHLIB_HIGH_ACCURACY 1
#endif

#ifndef MATHLIB_STRICT
#define MATHLIB_STRICT 1
#endif

#ifndef MATHLIB_FULL_ARGUMENT_REDUCTION
#define MATHLIB_FULL_ARGUMENT_REDUCTION 0
#endif

#ifdef __CHIMERA__
//  typedef unsigned int uint16;
//  typedef int int16;
//  typedef unsigned long uint32;
//  typedef long int32;
#  ifndef MAX_UINT32
#    define MAX_UINT32 ULONG_MAX
#  endif
#else
typedef unsigned short uint16;
typedef short int16;
typedef unsigned int uint32;
typedef int int32;
typedef const uint16 rom_uint16;
# define MAX_UINT32 UINT_MAX
#endif

#ifdef _MSC_VER
#define inline __inline
#endif

#define LEN(x) (sizeof(x)/sizeof(x[0]))

static float cosq(float x);
static float sinq(float x);
static float reducesinarg(float arg, int16 *quo);
static float sinorcosf(float x, uint16 offset);
static float erfc1(float arg);
static float erfc2(float arg);
static inline float round_to_nearest(float x);
static inline int iszero(float x);

union floatbits {
	struct {
		uint32 mantissa : 23,
			exponent : 8,
			sign : 1;
	};
	float f;
	uint32 v;
};

static inline int iszero(float x)
{
	// Chimera doesn't support subnormals, so any number with a zero exponent is 0
	union floatbits f;
	f.f = x;
	return (f.exponent == 0);
}

static inline float round_to_nearest(float x)
{
#ifdef __CHIMERA__
	return x == 0.0f ? x : __builtin_roundf(x);
#else
	float n, y;
	y = fabsf(x);
	n = truncf(y + 0.5f);
	if (n - x == 0.5f) {
		n -= (float)(((uint32)n) & 1);
	}
	return copysignf(n, x);
#endif
}

// ----------------------------------------------------------------------
// Number format manipulation, rounding, routines.
// ----------------------------------------------------------------------

/**
 * Splits a number into fractional and exponent parts.
 *
 * Inputs:
 *   arg: the input value
 * Outputs:
 *   returns the fractional value
 *   exp: the unbiased exponent
 *
 * Special cases:
 *   arg == +/-0 -> return arg, exp = 0
 *   arg == +/-Inf -> return arg, exp undefined
 *   arg == NaN -> return NaN, exp undefined (N/A on Chimera)
 */
float frexpf(float arg, int *exp)
{
	union floatbits f;
	int fexp;
	f.f = arg;
	fexp = f.exponent;
	if (fexp == 0) {
		// assume fexp==0 -> arg==0 since Chimera does not support subnormals
		*exp = 0;
	}
	else if (fexp != 255) {
		*exp = fexp - 126;
		f.exponent = 126;
	}
	return f.f;
}

/**
 * Decompose a float into integral and fractional parts. The
 * fractional part has the same sign as the argument.
 *
 * Inputs:
 *   arg: the input value
 * Outputs:
 *   returns the fractional part
 *   iptr: the integral part
 *
 * Special cases:
 *   arg == +/-0 -> return +/-0, +/-0 in *iptr
 *   arg == +/-Inf -> return +/-0, +/-inf in *iptr
 *   arg == NaN -> return NaN, NaN in *iptr (unsupported on Chimera)
 */
float modff(float arg, float *iptr)
{
	float v;
	*iptr = truncf(arg);
	if (isinf(arg))
		v = 0.0f;
	else
		v = arg - *iptr;
	return copysignf(v, arg);
}

/**
 * Same as scalbnf().
 */
float ldexpf(float arg, int exp)
{
	return scalbnf(arg, exp);
}

/**
 * Multiplies arg by 2^exp.
 *
 * Inputs:
 *   arg: value to multiply
 *   exp: the exponent of 2^exp
 * Outputs:
 *   returns arg * 2^exp
 *
 * Errors:
 *   On overflow, returns +/-HUGE_VALF
 *
 * Special cases:
 *   arg == +/-0 -> return unmodified
 *   arg == +/-Inf -> return unmodified
 *   exp == 0 -> return arg unmodified
 *   arg == NaN -> return NaN (N/A on Chimera)
 */
float scalbnf(float arg, int exp)
{
	union floatbits f;
	int fexp;
	f.f = arg;
	fexp = f.exponent;
	exp += fexp;
	if (fexp > 0 && fexp < 255) {   // NOTE: no support for subnormals
		if (exp > 255)
		{
			f.mantissa = 0;
			exp = 255;
		}
		if (exp < 0)
		{
			exp = 0;
		}
		f.exponent = exp;
	}
	return f.f;
}

/**
 * Do not use. Essentially the same as scalbnf().
 */
float scalblnf(float arg, long exp)
{
	return scalbnf(arg, (int)exp);
}

/**
 * Rounds to nearest integer, rounding ties away from zero.
 *
 * Inputs:
 *   x: value to round.
 * Outputs:
 *   returns rounded value.
 *
 * Special cases:
 *   +/- Inf -> unmodified
 *   +/- 0 -> unmodified
 *   NaN -> NaN (unsupported on Chimera)
 *
 * Note: Chimera does not use __builtin_roundf() because of
 *   TL-39546
 */
float mathlib_roundf(float x)
{
	return copysignf(truncf(fabsf(x) + 0.5f), x);
}

#ifndef __CHIMERA__

/**
 * Compute |x|.
 *
 * Inputs:
 *   x: value to use
 * Outputs:
 *   returns |x|
 *
 * Special cases:
 * +/- 0 -> +0
 * +/- Inf -> +Inf
 * NaN -> NaN (unsupported on Chimera)
 *
 * Note: Chimera uses __builtin_fabsf()
 */
float fabsf(float x)
{
	union floatbits f;
	f.f = x;
	f.sign = 0;
	return f.f;
}

/**
 * Truncates to integer towards zero.
 *
 * Inputs:
 *   x: value to truncate.
 * Outputs:
 *   returns truncated value.
 *
 * Special cases:
 *   +/- Inf -> unmodified
 *   +/- 0 -> unmodified
 *   NaN -> NaN (unsupported on Chimera)
 *
 * Note: Chimera uses __builtin_truncf()
 */
float truncf(float x)
{
	float y;
	if (fabsf(x) > INT_MAX) {
		return x;
	}
	y = (float)((int32)x);
	if (y == x) {
		return x;
	}
	return y;
}

/**
 * Rounds to integer away from zero.
 *
 * Inputs:
 *   x: value to round.
 * Outputs:
 *   returns rounded value.
 *
 * Special cases:
 *   +/- Inf -> unmodified
 *   +/- 0 -> unmodified
 *   NaN -> NaN (unsupported on Chimera)
 *
 * Note: Chimera uses __builtin_ceilf()
 */
float ceilf(float x)
{
	float y;
	if (fabsf(x) > INT_MAX) {
		return x;
	}
	y = (float)((int32)x);
	if (y == x) {
		return x;
	}
	if (x < 0) {
		return y;
	}
	else {
		return y + 1.0f;
	}
}

/**
 * Rounds to integer towards -infinity.
 *
 * Inputs:
 *   x: value to round.
 * Outputs:
 *   returns rounded value.
 *
 * Special cases:
 *   +/- Inf -> unmodified
 *   +/- 0 -> unmodified
 *   NaN -> NaN (unsupported on Chimera)
 *
 * Note: Chimera uses __builtin_floorf()
 */
float floorf(float x)
{
	float y;
	if (fabsf(x) > INT_MAX) {
		return x;
	}
	y = (float)((int32)x);
	if (y == x) {
		return x;
	}
	if (x < 0) {
		return y - 1.0f;
	}
	else {
		return y;
	}
}

#endif

/**
 * Rounds to nearest, result is long integer.
 *
 * Inputs:
 *   arg: value to round
 * Outputs:
 *   returns rounded value
 *
 * Errors:
 *   If the result cannot be represented as a long integer, an
 *   implementation-defined result is returned.
 */
long lroundf(float arg)
{
	return (long)roundf(arg);
}

/**
 * Rounds to a floating-point integer using the current rounding mode.
 *
 * Inputs:
 *   arg: value to round
 * Outputs:
 *   returns rounded value
 *
 * Special Cases:
 *   +/- Inf -> unmodified
 *   +/- 0 -> unmodified
 *   NaN -> NaN (unsupported on Chimera)
 *
 * Note: We currently only support round-to-nearest mode.
 *
 * Note: Unlike rintf(), this function does not generate any
 * exceptions, but exceptions are not supported anyway.
 */
float nearbyintf(float arg)
{
	return round_to_nearest(arg);
}


/**
 * Rounds to a floating-point integer using the current rounding mode.
 *
 * Inputs:
 *   arg: value to round
 * Outputs:
 *   returns rounded value
 *
 * Special Cases:
 *   +/- Inf -> unmodified
 *   +/- 0 -> unmodified
 *   NaN -> NaN (unsupported on Chimera)
 *
 * Note: We currently only support round-to-nearest mode.
 *
 * Note: Unlike nearbyintf(), this function should generate
 * exceptions, but exceptions are not supported anyway.
 */
float rintf(float arg)
{
	return round_to_nearest(arg);
}

/**
 * Rounds to a long integer using the current rounding mode.
 *
 * Inputs:
 *   arg: value to round
 * Outputs:
 *   returns rounded value
 *
 * Special Cases:
 *   +/- Inf -> implementation defined
 *   NaN -> implementation defined (unsupported on Chimera)
 *
 * Note: We currently only support round-to-nearest mode.
 *
 * Note: This function should generate exceptions, but exceptions are
 * not supported.
 */
long lrintf(float arg)
{
#ifdef __CHIMERA__
	// TODO: avoid extra ffloat/fint instructions by doing
	// this with inline assembly
	return __builtin_roundf(arg);
#else
	return (long)roundf(arg);
#endif
}

/**
 * Extracts unbiased exponent as a signed integer
 *
 * Inputs:
 *   arg: value to extract exponent from
 * Outputs:
 *   returns exponent
 *
 * Special cases:
 *   +/-0 -> FP_ILOGB0
 *   +/-Inf -> INT_MAX
 *   NaN -> FP_ILOGBNAN (unsupported on Chimera)
 */
int ilogbf(float arg)
{
	union floatbits f;
	int fexp;
	f.f = arg;
	fexp = f.exponent;
	if (fexp == 0) {
		// assume fexp==0 -> arg==0 since Chimera does not support subnormals
		return FP_ILOGB0;
	}
	else if (fexp == 255) {
		return INT_MAX;
	}
	else {
		return fexp - 127;
	}
}

/**
 * Extracts unbiased exponent as a float.
 *
 * Inputs:
 *   arg: value to extract exponent from
 * Outputs:
 *   returns exponent
 *
 * Special cases:
 *   +/-0 -> -Inf
 *   +/-Inf -> +Inf
 *   NaN -> NaN (unsupported on Chimera)
 */
float logbf(float arg)
{
	union floatbits f;
	int fexp;
	f.f = arg;
	fexp = f.exponent;
	if (fexp == 0) {
		// assume fexp==0 -> arg==0 since Chimera does not support subnormals
		return -INFINITY;
	}
	else if (fexp == 255) {
		// no NaN, since Chimera does not support them
		return INFINITY;
	}
	else {
		return (float)(fexp - 127);
	}
}

/**
 * Returns next representable value in the direction of to.
 *
 * Inputs:
 *   from: starting value
 *   to: value to increment towards
 * Outputs:
 *   returns incremented value
 *
 */
float nextafterf(float from, float to)
{
	union floatbits f;
	int inc;
	int32 mantissa;
	int exp;

	if (from == to) return to;
	if (isinf(from))
		return from;

	f.f = from;
	if (to < from) {
		if (from < 0) {
			inc = 1;
		}
		else {
			inc = -1;
		}
	}
	else {
		if (from < 0) {
			inc = -1;
		}
		else {
			inc = 1;
		}
	}

	exp = f.exponent;
	mantissa = f.mantissa + inc;
	if (mantissa == (1UL << 23)) {
		exp++;
		mantissa -= (1UL << 23);
	}
	if (mantissa < 0) {
		exp--;
		mantissa += (1UL << 23);
	}
	if (exp >= 255) {
		exp = 255;
		mantissa = 0;
	}
	if (exp <= 0) {
		exp = 0;
		mantissa = 0;
	}
	f.mantissa = mantissa & 0x7fffff;
	f.exponent = exp;
	return f.f;
}

/**
 * Returns value with magnitude of x and sign of y.
 *
 * Inputs:
 *   x: magnitude value
 *   y: sign value
 * Outputs:
 *   returns x with y's sign
 *
 * Special cases:
 *   x is NaN -> NaN with sign of y (unsupported on Chimera)
 */
float copysignf(float x, float y)
{
#ifdef __CHIMERA__
	asm const volatile(
		"        bclr Y bit 15\n"
		"        ifset reg (%2 + 1) bit 15\n"
		"          bset Y bit 15\n"
		: "=b" (x)
		: "0" (x), "u" (y)
		: "cc");
	return x;
#else
	union floatbits f1, f2;
	f1.f = x;
	f2.f = y;
	f1.sign = f2.sign;
	return f1.f;
#endif
}

/**
 * Computes remainder of x/y as x - y*round(x/y). The remainder is not
 * guaranteed to have the same sign as x.
 *
 * Inputs:
 *   x: numerator
 *   y: denominator
 * Outputs:
 *   Returns remainder of x/y
 *
 * Special cases:
 *   x = +/-Inf, y not NaN -> NaN (unsupported on Chimera)
 *   y = +/-0, x is not NaN -> NaN (unsupported on Chimera)
 *   x or y NaN -> NaN (unsupported on Chimera)
 */
float remainderf(float x, float y)
{
	float q, n, r;
	if (iszero(y))
		return y;

	q = x / y;
	n = round_to_nearest(q);
	r = x - n * y;
	return r;
}

/**
 * Computes remainder plus sign and some bits of quotient. The
 * remainder is not guaranteed to have the same sign as x.
 *
 * Inputs:
 *   x: numerator
 *   y: denominator
 * Outputs:
 *   returns remainder of x/y
 *   quo: at least three bits of quotient, with sign of quotient
 *
 * Special cases:
 *   x = +/-Inf, y not NaN -> NaN (unsupported on Chimera)
 *   y = +/-0, x is not NaN -> NaN (unsupported on Chimera)
 *   x or y NaN -> NaN (unsupported on Chimera)
 *
 * Note: this function is only implemented for quotients 2^31 or
 * smaller.
 */
float remquof(float x, float y, int *quo)
{
	// FIXME: make this work for values over 2^31
	float q, n, r;
	if (iszero(y))
		return y;

	q = x / y;
	n = round_to_nearest(q);
	r = x - n * y;
	if (fabsf(n) < (float)0x80000000UL) {
		int32 quo32;
		quo32 = (int32)n;
		if (sizeof(int32) > sizeof(int)) {
			*quo = (int16)quo32 & 0x7FFF;
			if (quo32 < 0) *quo |= 0x8000;
		}
		else {
			*quo = quo32;
		}
	}
	else {
		*quo = 0;
	}
	return r;
}

/**
 * Computes remainder of x/y. The remainder has the same sign as x and
 * magnitude less than or equal to y.
 *
 * Inputs:
 *   x: numerator
 *   y: denominator
 * Outputs:
 *   return remainder
 *
 * Special cases:
 *   x = +/-0, y != 0 -> x
 *   x = +/-Inf, y not NaN -> NaN (unsupported by Chimera)
 *   y = +/-0, x not NaN -> NaN (unsupported by Chimera)
 *   y = +/-Inf, x finite -> x
 *   x or y is NaN -> NaN (unsupported by Chimera)
 */
float fmodf(float x, float y)
{
	float q, n, r;
	union floatbits fx, fy;
	fx.f = x;
	fy.f = y;

	if (fy.exponent == 0)
		return fy.f;
	if (fy.exponent == 255 && fx.exponent != 255)
		return fx.f;
#if MATHLIB_STRICT
	if (fx.exponent == 0)
		return fx.f;
#endif

	q = fx.f / fy.f;
	n = truncf(q);
	r = fx.f - n * fy.f;
	return r;
}


/**
 * Computes max(x,y), ignoring NaN values.
 *
 * Inputs:
 *   x, y: values to compare
 * Outputs:
 *   returns max(x,y)
 *
 * Special cases:
 *   if x is NaN -> y (unsupported by Chimera)
 *   if y is NaN -> x (unsupported by Chimera)
 *
 * Note: because Chimera does not support NaN, this is trivial.
 */
float fmaxf(float x, float y)
{
	return (y > x) ? y : x;
}

/**
 * Computes min(x,y), ignoring NaN values.
 *
 * Inputs:
 *   x, y: values to compare
 * Outputs:
 *   returns min(x,y)
 *
 * Special cases:
 *   if x is NaN -> y (unsupported by Chimera)
 *   if y is NaN -> x (unsupported by Chimera)
 *
 * Note: because Chimera does not support NaN, this is trivial.
 */
float fminf(float x, float y)
{
	return (y < x) ? y : x;
}

/**
 * Computes positive difference between x and y. If x <= y, returns
 * +0.0. Otherwise, returns x-y.
 *
 * Inputs:
 *   x, y: values to subtract
 * Outputs:
 *   returns max(+0.0, x-y)
 *
 * Special cases:
 *   if x or y is NaN -> NaN (unsupported by Chimera)
 *
 */
float fdimf(float x, float y)
{
	float z = x - y;
	if (signbit(z))
		return 0.0f;
	return z;
}

// ----------------------------------------------------------------------
// Begin numerical functions
// ----------------------------------------------------------------------


/**
 * Compute base-2 log of x.
 *
 * Algorithm:
 *
 *   Decompose x into exponent n and fractional part f in [0.5,1].
 *   Compute log2(f) with a rational approximation, then return n +
 *   log2(f).
 *
 *   Approximations are Hart 2520 (HIGH_ACCURACY = 0) with 9.63 bits
 *   of precision and Hart 2524 (HIGH_ACCURACY = 1) with 27.64 bits of
 *   precision.
 *
 * Accuracy:
 *
 *   Over (0,+inf), excluding [0.99,1.01], max relative error:
 *     HIGH_ACCURACY=1: 9.51e-06
 *     HIGH_ACCURACY=0: 6.35e-02
 *   In [0.99,1.01], relative error is very large because log(1.0) = 0.0.
 *
 * Special cases:
 *
 *   +/-0 -> -inf
 *   1 -> +0.0
 *   x < 0 -> NaN (unsupported on Chimera)
 *   +inf -> +inf
 *   NaN -> NaN (unsupported on Chimera)
 */
float log2f(float x)
{
	int exp;
	float p, q;

	// short-circuit for values that would become subnormals:
	if (x < 1.1754944e-38f)
		return -INFINITY;

	if (x == 1.0f)
		return 0.0f;

#if MATHLIB_STRICT
	// prevent division below from making inf/inf -> NaN
	if (isinf(x) && signbit(x) == 0)
		return INFINITY;
#endif

	x = frexpf(x, &exp);
#if MATHLIB_HIGH_ACCURACY == 0
	// Hart 2520
	p = -2.409094f + 2.406974f*x;
	q = 0.7071062f + x;
#else
	// Hart 2524
	p = ((4.81147460989f*x + 6.10585199015f)*x - 8.8626599391f)*x - 2.05466671951f;
	q = ((x + 6.42784209029f)*x + 4.54517087629f)*x + 0.353553425277f;
#endif
	x = p / q;
	x += exp; // FIXME: loss of significance when p/q is near -1.0 and exp == 1.
	return x;
}

/**
 * Compute natural log of x.
 *
 * Algorithm:
 *
 *   Compute log(x) = log2(x) * log(2)
 *
 * Accuracy:
 *
 *   Over (0,+inf), excluding [0.99,1.01], max relative error:
 *     HIGH_ACCURACY=1: 1.29e-05
 *     HIGH_ACCURACY=0: 6.44e-02
 *   In [0.99,1.01], relative error is very large because log(1.0) = 0.0.
 *
 * Special cases:
 *
 *   +/-0 -> -inf
 *   1 -> +0.0
 *   x < 0 -> NaN (unsupported on Chimera)
 *   +inf -> +inf
 *   NaN -> NaN (unsupported on Chimera)
 */
float logf(float x)
{
	return log2f(x) * ((float)M_LN2);
}

/**
 * Compute base-10 log of x.
 *
 * Algorithm:
 *
 *   Compute log10(x) = log2(x) * log10(2)
 *
 * Accuracy:
 *
 *   Over (0,+inf), excluding [0.99,1.01], max relative error:
 *     HIGH_ACCURACY=1: 1.26e-05
 *     HIGH_ACCURACY=0: 6.20e-02
 *   In [0.99,1.01], relative error is very large because log(1.0) = 0.0.
 *
 * Special cases:
 *
 *   +/-0 -> -inf
 *   1 -> +0.0
 *   x < 0 -> NaN (unsupported on Chimera)
 *   +inf -> +inf
 *   NaN -> NaN (unsupported on Chimera)
 */
float log10f(float x)
{
	return log2f(x) * (0.301029995663981f); // log10(2)
}

/**
 * Compute natural log of 1+x. This is useful when x is much smaller
 * than 1, since otherwise x can be truncated in the addtion.
 *
 * Algorithm:
 *
 *   If |x| >= 0.01, use logf()
 *   Otherwise, use a polynomial approximation to log(1+x) generated in Matlab with:
 *     >> f = chebfun(@(x) log(1+x), [-0.01, 0.01])
 *     >> p = poly(remez(f, 2)); % low accuracy
 *     >> p = poly(remez(f, 5)); % high accuracy
 *
 * Accuracy:
 *
 *   Over (-0.01,0.01), max relative error:
 *     HIGH_ACCURACY=1: 1.19e-07
 *     HIGH_ACCURACY=0: 2.52e-05
 *
 * Special cases:
 *
 *   +/-0 -> unmodified
 *   -1 -> -inf
 *   x < -1 -> NaN (unsupported on Chimera)
 *   +inf -> +inf
 *   NaN -> NaN (unsupported on Chimera)
 */
float log1pf(float x)
{
	if (fabsf(x) < 1e-2f) {
#if MATHLIB_HIGH_ACCURACY == 0
		return ((-0.500031252287242f)*x + 1.000025001257891f)*x;
#else
		return (((((0.200024969506602f)*x + -0.250024999824195f)*x + 0.333333332162063f)*x + -0.499999999062639f)*x + 1.000000000000011f)*x;
#endif
	}
	else {
		return logf(1.0f + x);
	}
}

/**
 * Compute 2^x.
 *
 * Algorithm:
 *   Calculate 2^x = 2^n*2^xi, where n is an integer and 0 <= xi < 1. Use
 *   a polynomial approximation to calculate 2^xi, then multiply this by
 *   the exact value 2^n.
 *
 *   Polynomials are found in Matlab with remez_series():
 *     >> h = @(x) 2.^x; dh = @(x) log(2)*2.^x; d2h = @(x) log(2)*log(2)*2.^x;
 *     >> p = remez_series(2,h,dh,d2h,[0,1],1) % low accuracy
 *     >> p = remez_series(5,h,dh,d2h,[0,1],1) % high accuracy
 *
 * Accuracy:
 *
 *   Over (-256.0,256.0), max relative error:
 *     HIGH_ACCURACY=1: 2.19e-07
 *     HIGH_ACCURACY=0: 1.72e-03
 *
 * Special cases:
 *
 *   +/-0 -> 1
 *   -inf -> +0
 *   +inf -> +inf
 *   NaN -> NaN (unsupported by Chimera)
 */
float exp2f(float x)
{
	// convert 2^x -> 2^(n+xi), where n is an integer and 0 <= xi < 1

	float n, xi, y;

	// FIXME: the approximation below doesn't quite match at x = 0.0
	if (x == 0.0f)
		return 1.0f;

	// short-circuit overflows so we can be sure
	// that n below always fits in an integer
	if (fabsf(x) >= 128) {
		if (signbit(x)) {
			return 0.0f;
		}
		else {
			return HUGE_VALF;
		}
	}

	n = floorf(x);

	xi = x - n;
	// series approximation to 2^x over [0,1]
#if MATHLIB_HIGH_ACCURACY == 0
  // 2.76-digit accuracy:
	xi = (3.371894e-01f*xi + 6.576363e-01f)*xi + 1.001725e+00f;
#else
  // ~7-digit accuracy
	xi = ((((1.877577e-03f*xi + 8.989340e-03f)*xi + 5.582632e-02f)*xi + 2.401536e-01f)*xi + 6.931531e-01f)*xi + 9.999999e-01f;
#endif
	y = ldexpf(xi, (int)n);

	return y;
}

/**
 * Compute e^x.
 *
 * Algorithm:
 *   Calculate e^x = e^(x/log(2)).
 *
 * Accuracy:
 *
 *   Over (-256.0,256.0), max relative error:
 *     HIGH_ACCURACY=1: 3.97e-06
 *     HIGH_ACCURACY=0: 1.73e-03
 *
 * Special cases:
 *
 *   +/-0 -> 1
 *   -inf -> +0
 *   +inf -> +inf
 *   NaN -> NaN (unsupported by Chimera)
 */
float expf(float x)
{
	float y;

	y = x * (float)M_LOG2E; // 1/ln(2) = log2(e)
	return exp2f(y);
}

/**
 * Compute e^x - 1. This is useful for x very close to zero, where e^x
 * gives a result very close to 1.0. In this case, calculating e^x - 1
 * results in a loss of significance that lowers accuracy.
 *
 * Algorithm:
 *   If |x| > 0.5, use expf(x)-1.
 *   Otherwise, use a rational approximation from Hart sequence 1800.
 *
 * Accuracy:
 *
 *   Over (-0.5,0.5), max relative error: 3.94e-07.
 *
 * Special cases:
 *
 *   +/-0 -> 1
 *   -inf -> -1
 *   +inf -> +inf
 *   NaN -> NaN (unsupported by Chimera)
 */
float expm1f(float x)
{
	float y, z, p, q;
	if (fabsf(x) > 0.5f) {
		return expf(x) - 1.0f;
	}

	// Hart sequence 1800
	z = x * x;
	p = x * (5.000089649149271528f + 0.08332586539095234f*z);
	q = 10.000179298301740601f + z;
	y = 2.0f*p / (q - p);
	return y;
}

/**
 * sin(x) and cos(x).
 *
 * Algorithm:
 *
 *   Reduce x to pi/2*q + xx, where q is an integer and xx is in the
 *   range [-pi/4,pi/4]. Use the last two bits of q to select which
 *   approximation to use as:
 *   0 -> sin(xx), 1-> cos(xx), 2 -> -sin(xx), 3 -> -cos(xx)
 *
 *   For each of sin(x) and cos(x), use a polynomial approximation,
 *   calculated in Matlab with:
 *
 *   >> h = @(x) sin(x); dh = @(x) cos(x); d2h = @(x) -sin(x); d3h = @(x) -cos(x);
 *   >> sin_coeffs = remez_series(8, h, dh, d2h, pi/4*[-1,1], 0);
 *   >> cos_coeffs = remez_series(7, dh, d2h, d3h, pi/4*[-1,1], 0);
 *
 *   I discarded very small coefficients.
 *
 *   With MATHLIB_FULL_ARGUMENT_REDUCTION=1, the Payne-Hanek algorithm
 *   is used to do high-precision argument reduction for large inputs.
 *   Otherwise, use 32-bit division and tolerate reduced accuracy for
 *   large inputs.
 *
 * Accuracy:
 *
 *   Over (-1000,1000), max absolute error: 8.73e-05
 *
 * Special cases:
 *
 *   +/-0 -> unmodified
 *   +/-inf -> NaN (unsupported by Chimera)
 */
static float sinq(float x)
{
	float y;
	float z = x * x;
	y = (((-1.946212e-4f * z + 8.331585e-3f)*z - 1.666664e-1f)*z + 1.0f)*x;
	return y;
}

static float cosq(float x)
{
	float y;
	float z = x * x;
	y = (((2.437993e-05f*z - 1.388662e-03f)*z + 4.166662e-02f)*z - 0.5f)*z + 1.0f;
	return y;
}

#if MATHLIB_FULL_ARGUMENT_REDUCTION
// This is 192 bits of 2/pi, starting at the binary point.
rom_uint16 two_over_pi[] = {
  0xA2F9, 0x836E, 0x4E44, 0x1529,
  0xFC27, 0x57D1, 0xF534, 0xDDC0,
  0xDB62, 0x9599, 0x3C43, 0x9041,
};
#endif

static float reducesinarg(float arg, int16 *quo)
{
#if MATHLIB_FULL_ARGUMENT_REDUCTION == 0
	float y = arg * (float)M_2_PI;
	float n = round_to_nearest(y);
	*quo = ((int16)n) & 3;
	return (y - n)*((float)M_PI_2);
#else
	float xx;
	float x[3];
	float b[7];
	float y[9];
	float sumy;
	float t;
	float n;
	float f;

	int16 i, j;
	int16 e;
	int16 word;
	int16 bit;
	int16 Mminus2;
	uint32 v;
	int16 expadj = 0;

	xx = arg;

	// divide arg into 12-bit chunks
	e = ilogbf(xx);
	if (e == FP_ILOGB0) {
		*quo = 0;
		return 0.0f;
	}
	for (i = 0; i < (int16)LEN(x); i++) {
		x[i] = scalbnf(floorf(scalbnf(xx, 10 * (i + 1) - e)), e - 10 * (i + 1));
		xx -= x[i];
	}

	Mminus2 = e - 25;
	if (Mminus2 < 0) Mminus2 = 0;

	// get bits (M-2):(M-2)+LEN(b)-1, 10 bits at a time
	word = Mminus2 + 10;
	bit = 32 - (word & 0xF);
	word = word / 16;
	if (bit > 21) {
		bit -= 16;
		word--;
	}
	// FIXME: roll this into loop below
	v = two_over_pi[word + 1];
	if (word >= 0) {
		v |= ((uint32)two_over_pi[word] << 16);
	}
	word += 2;

	// Since b's exponent is like 1/x's, scale b up for very large inputs
	// to prevent underflow
	if (Mminus2 > (int16)LEN(b) * 10) {
		expadj = Mminus2;
		Mminus2 = 0;
	}

	for (i = 0; i < (int16)LEN(b); i++) {
		int16 f;
		if (bit < 0) {
			bit += 16;
			v <<= 16;
			v |= two_over_pi[word++];
		}
		f = (uint16)((v >> bit) & 0x3FF);
		b[i] = scalbnf(f, -(Mminus2 + 10 + 10 * i));
		bit -= 10;
	}

	for (i = 0; i < (int16)LEN(y); i++) {
		y[i] = 0;
	}

	// convolve x * b = y

	for (i = 0; i < (int16)LEN(y); i++) {
		int16 idx = i - LEN(b) + 1;
		if (idx < 0) idx = 0;
		for (j = idx; j < (int16)LEN(x) && j <= i; j++) {
			y[i] += x[j] * b[i - j];
		}
	}
	sumy = 0;
	for (i = LEN(y) - 1; i >= 0; i--) {
		if (expadj != 0) {
			y[i] = scalbnf(y[i], -expadj);
		}
		y[i] = fmodf(y[i], 4);
		sumy += y[i];
	}

	t = -sumy;
	for (i = 0; i < (int16)LEN(y); i++) {
		t += y[i];
	}
	n = round_to_nearest(sumy);

	f = (sumy - n) + t;

	f *= (float)M_PI_2;

	// this is a higher-accuracy version of f *= pi/2, but I haven't seen
	// a benefit from it:
	//f = (f * (float) -4.3711388e-08) + (f * (float) 1.5707964);

	*quo = (int16)((int32)n) & 0x3;

	return f;
#endif
}

static float sinorcosf(float x, uint16 offset)
{
	uint16 quo;
	float arg;
	arg = reducesinarg(x, &quo);
	quo += offset;
	switch (quo & 0x3) {
	case 0:
		return cosq(arg);
	case 1:
		return -sinq(arg);
	case 2:
		return -cosq(arg);
	case 3:
	default:
		return sinq(arg);
	}
}

float cosf(float x)
{
	return sinorcosf(x, 0);
}

float sinf(float x)
{
	return sinorcosf(x, -1);
}

/**
 * Arctangent.
 *
 * Algorithm:
 *
 *   Reduce x to the range [0, 1] with the identities
 *     atan(-x) = -atan(x)
 *     atan(x) = pi/2 - atan(1/x)
 *
 *   Approximate using series Hart 4990.
 *
 * Accuracy:
 *
 *   Over (-inf, inf), max absolute error 5.96e-07
 *
 * Special cases:
 *
 *   +/-0 -> unmodified
 *   +inf -> +pi/2
 *   -inf -> -pi/2
 *   NaN -> NaN (unsupported by Chimera)
 */
float atanf(float x)
{
	uint16 negative1 = 0;
	uint16 negative2 = 0;
	float y = 0.0;
	float z;
	float v;

	negative1 = signbit(x);
	x = fabsf(x);

	// reduce arguments greater than 1
	if (x > 1.0f)
	{
		x = 1 / x;
		y = (float)M_PI_2;
		negative2 = 1;
	}

	z = x * x;
	v = (((0.00786337627f * z - 0.03701299998f) * z + 0.08387118962f) * z - 0.1348719133f) * z;
	v = (((v + 0.19881482434f) * z - 0.33326514917f) * z + 0.99999934782f) * x;

	if (negative2)
		y -= v;
	else
		y += v;

	if (negative1)
		return -y;
	else
		return y;
}

/**
 * Compute atan(y/x) with correct quadrant.
 *
 * Algorithm:
 *
 *   Use atan(y/x) with appropriate offset.
 *
 * Accuracy:
 *
 *   Same as atan().
 *
 * Special cases:
 *
 *   y == +/-0 && x < 0 or x == -0.0 -> +/-pi
 *   y == +/-0 && x > 0 or x == +0.0 -> +/-0
 *   y is +/-inf && x is finite -> +/- pi/2
 *   y is +/-inf && x is -inf -> +/-3*pi/4
 *   y is +/-inf && x is +inf -> +/-pi/4
 *   x == +/-0 && y < 0 -> -pi/2
 *   x == +/-0 && y > 0 -> +pi/2
 *   x is -inf && y is finite && y > 0 -> +pi
 *   x is -inf && y is finite && y < 0 -> -pi
 *   x is +inf && y is finite && y > 0 -> +0
 *   x is +inf && y is finite && y < 0 -> -0
 *   either x or y NaN -> NaN (unsupported by Chimera)
 */
float atan2f(float y, float x)
{
	float z;
	float offset = -0.0f;
	//printf("Enter %s\n", __func__);
	// note that we're using offset == 0.0f for comparisons to zero
	// to save on expensive floating-point constant loads.

	if (x == offset && y == offset) {
		z = offset;
		if (!signbit(y)) {
			z = -z;
		}
#if MATHLIB_STRICT
	}
	else if (isinf(x) && isinf(y)) {
		if (signbit(x)) {
			z = (float)(3.0f*M_PI_4);
		}
		else {
			z = (float)M_PI_4;
		}
		if (signbit(y)) {
			z = -z;
		}
		return z;
#endif
	}
	else {
		z = atanf(y / x);
	}
	if (signbit(x)) {
		offset = (float)M_PI;
		if (signbit(y)) {
			offset = -offset;
		}
	}
	return offset + z;
}

/**
 * Compute x^y.
 *
 * Algorithm:
 *
 *   Use 2^(y*log2(|x|)), with handling for negative x and y.
 *
 * Accuracy:
 *
 *   Max relative error over (0.002, 26):
 *     HIGH_ACCURACY=1: 8.10e-06
 *     HIGH_ACCURACY=0: 2.36e-02
 *
 *   That range was chosen since 26^26 is about the largest
 *   representable number, and 26^0.002 ~= 1.0.
 *
 * Special cases:
 *
 *   x == +0, y is negative odd integer -> +inf
 *   x == -0, y is negative odd integer -> -inf
 *   x == +/-0, y is negative, finite, even integer or non-integer -> +inf
 *   x == +/-0, y is -inf -> +inf
 *   x == +0, y is positive odd integer -> +0
 *   x == -0, y is positive odd integer -> -0
 *   x == +/-0, y is a positive non-integer or positive even integer -> +0
 *   x == -1, y is +/-inf -> 1
 *   x == +1, y anything including NaN -> 1 (NaN unsupported on Chimera)
 *   x is anything including NaN, y == +/-0 -> 1 (NaN unsupported on Chimera)
 *   x finite and negative, y finite non-integer -> NaN (unsupported on Chimera)
 *   |x| < 1, y is -inf -> +inf
 *   |x| > 1, y is -inf -> +0
 *   |x| < 1, y is +inf -> +0
 *   |x| > 1, y is +inf -> +inf
 *   x is -inf, y is negative odd integer -> -0
 *   x is -inf, y is negative non-integer or negative even integer -> +0
 *   x is -inf, y is positive odd integer -> -inf
 *   x is -inf, y is positive non-integer or positive even integer -> +inf
 *   x is +inf, y < 0 -> +0
 *   x is +inf, y > 0 -> +inf
 *   except as above, if x or y is NaN -> NaN (unsupported on Chimera)
 */
float powf(float x, float y)
{
	float z;
	int is_int = 1;
	int is_odd = 0;
	if (fabsf(y) < 0x7FFFFFFF) {
		int32 ei = (int32)y;
		is_int = ei - y == 0.0f; // must be exact
		is_odd = is_int && ((ei & 0x1) == 1);
	}
#if MATHLIB_STRICT
	if (x == -1.0f && isinf(y)) {
		return 1.0f;
	}
	// no NaN support on Chimera
	//if (!isinf(x) && signbit(x) && !isinf(y) && !is_int) {
	//  return nanf();
	if (isinf(x)) {
		if (signbit(x)) {
			if (signbit(y)) {
				if (is_odd) {
					return -0.0f;
				}
				else {
					return +0.0f;
				}
			}
			else {
				if (is_odd) {
					return -INFINITY;
				}
				else {
					return INFINITY;
				}
			}
		}
		else {
			if (signbit(y)) {
				return 0.0f;
			}
			else {
				return INFINITY;
			}
		}
	}
#endif
	z = exp2f(y*log2f(fabsf(x)));
	if (is_odd && signbit(x))
		z = -z;
	return z;
}


/**
 * Compute x^(1/3).
 *
 * Algorithm:
 *
 *   Reduce x to [1/8, 1] by multiplying by -2^k, then use a
 *   polynomial approximation (Hart 560) to seed the Newton iteration:
 *
 *     y_{n+1} = 1/3*(2*y_n + x/y_n^2)
 *
 *   which converges in 5 iterations. Multiply the result by 2^(k/3).
 *
 * Accuracy:
 *
 *   Max relative error over (-inf, inf): 2.38e-07
 *
 *
 * Special cases:
 *
 *   +/-0 -> unchanged
 *   +/-inf -> unchanged
 *   NaN -> NaN (unsupported on Chimera)
 */
float cbrtf(float x)
{
	int exp;
	int16 k, i, neg;
	float y;
	neg = signbit(x) != 0;
	x = fabsf(x);
	if (frexpf(x, &exp) == 0.0f) {
		if (neg) {
			return -0.0f;
		}
		else {
			return 0.0f;
		}
	}
	if (exp >= 0) exp += 2;
	k = exp / 3;
	x = ldexpf(x, -3 * k);
	// Hart 560
	y = 0.5550349f + 0.459806f*x;
	for (i = 0; i < 5; i++) {
		y = 1.0f / 3.0f * (2.0f*y + x / (y*y));
	}
	y = ldexpf(y, k);
	if (neg) y = -y;
	return y;
}

#ifndef __CHIMERA__
/**
 * Compute x^(1/2).
 *
 * Algorithm:
 *
 *   Reduce x to [1/4, 1] by multiplying by -2^k, then use a
 *   polynomial approximation (Hart 150) to seed the Newton iteration:
 *
 *     y_{n+1} = 1/2*(x/y_n + y_n)
 *
 *   which converges in 5 iterations. Multiply the result by 2^(k/2).
 *
 * Accuracy:
 *
 *   Max relative error over (-inf, inf): 1.19e-7
 *
 * Note:
 *
 *   On Chimera, sqrtf() uses __builtin_sqrtf() with max relative
 *   error of 2.30e-07.
 *
 * Special cases:
 *   +/-0 -> unchanged
 *   +/-inf -> unchanged
 *   NaN -> NaN (unsupported on Chimera)
 */
float sqrtf(float x)
{
	int exp, k, i;
	float y;
	// assume negative inputs were close to zero and return 0.
	if (signbit(x)) return 0.0f;

	x = fabsf(x);
	if (frexpf(x, &exp) == 0.0f) {
		return 0.0f;
	}
	if (exp >= 0) exp += 1;
	k = exp / 2;
	x = ldexpf(x, -2 * k);
	// Hart 150
	y = 0.41730659f + 0.59016206f*x;
	for (i = 0; i < 5; i++) {
		y = 0.5f * (x / y + y);
	}
	y = ldexpf(y, k);
	return y;
}
#endif



/**
 * Compute sqrt(x^2 + y^2).
 *
 * Algorithm:
 *
 *   Anything that won't over/underflow uses the straightforward
 *   implementation. If either argument will overflow/underflow when
 *   squared, use max(x,y) * sqrt(1.0 + (min(x,y)/max(x/y))^2). If
 *   1/max(x/y) will underflow, rescale to prevent it.
 *
 * Accuracy:
 *
 *   Max relative error over (-inf, inf): 1.72e-07
 *
 * Notes:
 *
 *   On Chimera, this uses __builtin_sqrtf() which reduces the
 *   accuracy to 2.37e-07.
 *
 * Special cases:
 *
 *   x == 0 -> |y|
 *   y == 0 -> |x|
 *   x or y is +/-inf -> +inf (even if other is NaN) (NaN unsupported by Chimera)
 *   x or y NaN, other not infinite -> NaN (unsupported by Chimera)
 */
float hypotf(float x, float y)
{
	float t;
	x = fabsf(x);
	y = fabsf(y);
	if (x < y) {
		t = x;
		x = y;
		y = t;
	}
	if (x == 0)
		return x;
	if (x > 1.304381743659671e+19f || y < 1.084202172485504e-19f) {
		if (x > 8.507059173023462e+37f) {
			x /= 2.0f;
			y /= x;
			x *= sqrtf(4.0 + y * y);
		}
		else {
			y /= x;
			x *= sqrtf(1.0 + y * y);
		}
		return x;
	}
	else {
		return sqrtf(x*x + y * y);
	}
}

/**
 * Compute arcsine.
 *
 * Algorithm:
 *
 *   Use identity
 *
 *     asin(x) = atan(x/sqrt(1-x^2))
 *
 * Accuracy:
 *
 *   Over (-1, +1), max absolute error 6.56e-07.
 *
 * Special cases:
 *
 *   +/-0 -> unmodified
 *   |x| > 1 -> NaN (unsupported by Chimera)
 *   NaN -> NaN (unsupported by Chimera)
 */
float asinf(float x)
{
	// FIXME: improve this
	return atanf(x / sqrtf(1 - x * x));
}

/**
 * Compute arccosine.
 *
 * Algorithm:
 *
 *   Use identity
 *
 *     acos(x) = pi/2 - asin(x)
 *
 * Accuracy:
 *
 *   Over (-1, +1), max absolute error 7.15e-07.
 *
 * Special cases:
 *
 *   +1 -> +0
 *   |x| > 1 -> NaN (unsupported by Chimera)
 *   NaN -> NaN (unsupported by Chimera)
 */
float acosf(float x)
{
	// FIXME: improve this
	return (float)M_PI_2 - asinf(x);
}

/**
 * Compute tan(x).
 *
 * Algorithm:
 *
 *   Use identities:
 *
 *     tan(x) = tan(x + pi*k)
 *     tan(x) = -tan(-x)
 *     tan(x) = 1/(tan(pi/2-x))
 *
 *   to reduce x to the interval [0, pi/4], then use a rational
 *   approximation (Hart 4240 or Hart 4242).
 *
 * Accuracy:
 *
 *   Over (-pi/2,pi/2), maximum relative error:
 *     HIGH_ACCURACY=1: 9.75e-06
 *     HIGH_ACCURACY=0: 1.30e-03
 *
 * Special cases:
 *
 *   +/- 0 -> unmodified
 *   +/-inf -> NaN (unsupported on Chimera)
 *   NaN -> NaN (unsupported on Chimera)
 *
 * Note:
 *
 *   Simple argument reduction method causes accuracy to drop for
 *   values outside of (-pi/2, pi/2).
 */
float tanf(float arg)
{
	float k, x, n, d, y, z;
	int recip = 0;

	if (fabsf(arg) < 5.4e-3f)
		return arg;

	x = arg / (float)M_PI;
	k = round_to_nearest(x);
	x = 4.0f*(x - k);

	if (fabsf(x) > 1.0f) {
		x = copysignf(2.0f, x) - x;
		recip = 1;
	}

	z = x * x;
#if MATHLIB_HIGH_ACCURACY == 0
	// Hart 4240
	n = 3.655026f*x;
	d = 4.659791f - z;
#else
	// Hart 4242
	n = x * (0.211849369664121e3f - 0.125288887278448e2f*z);
	d = (z - 0.714145309347748e2f)*z + 0.2697350131214121e3f;
#endif
	y = (recip) ? d / n : n / d;

	return y;
}

/**
 * Compute sinh(x)
 *
 * Algorithm:
 *
 *   Use definition
 *
 *     sinh(x) = 1/2 * (exp(arg) - exp(-arg))
 *
 * Accuracy:
 *
 *   Over (-64, 64), max relative error:
 *     HIGH_ACCURACY = 1: 7.73e-06
 *     HIGH_ACCURACY = 0: 1.76e-01
 *
 * Special cases:
 *
 *   +/-0 or +/-inf -> unmodified
 *   NaN -> NaN (unsupported by Chimera)
 */
float sinhf(float arg)
{
	// FIXME: handle loss of significance in subtraction by splitting into
	// 3 regions
#if MATHLIB_STRICT
	if (arg == 0.0f)
		return arg;
#endif
	return 0.5f * (expf(arg) - expf(-arg));
}

/**
 * Compute cosh(x)
 *
 * Algorithm:
 *
 *   Use definition
 *
 *     cosh(x) = 1/2 * (exp(arg) + exp(-arg))
 *
 * Accuracy:
 *
 *   Over (-64, 64), max relative error:
 *     HIGH_ACCURACY = 1: 3.50e-06
 *     HIGH_ACCURACY = 0: 1.73e-03
 *
 * Special cases:
 *
 *   +/-0 -> 1
 *   +/-inf -> +inf
 *   NaN -> NaN (unsupported by Chimera)
 */
float coshf(float arg)
{
	return 0.5f * (expf(arg) + expf(-arg));
}

/**
 * Compute tanh(x)
 *
 * Algorithm:
 *
 *   Use definition
 *
 *     tanh(x) = (exp(arg) - exp(-arg))/(exp(arg) + exp(-arg))
 *
 * Accuracy:
 *
 *   Over (-64, 64), max relative error:
 *     HIGH_ACCURACY = 1: 7.98e-06
 *     HIGH_ACCURACY = 0: 1.77e-01
 *
 * Special cases:
 *
 *   +/-0 -> +/-0
 *   +/-inf -> +/-1
 *   NaN -> NaN (unsupported by Chimera)
 */
float tanhf(float arg)
{
	// FIXME: handle loss of significance in subtraction by splitting into
	// 3 regions
	float p, n;
#if MATHLIB_STRICT
	if (isinf(arg))
		return copysignf(1.0f, arg);
	if (arg == 0.0f)
		return arg;
#endif
	p = expf(arg);
	n = expf(-arg);
	return (p - n) / (p + n);
}

/**
 * Compute asinh(x)
 *
 * Algorithm:
 *
 *   Use identity
 *
 *     asinh(|x|) = log(|x| + sqrt(1+x^2))
 *
 * Accuracy:
 *
 *   Over (-inf, inf), max relative error:
 *     HIGH_ACCURACY = 1: 1.42e-06
 *     HIGH_ACCURACY = 0: 5.96e-03
 *
 * Special cases:
 *
 *   +/-0 or +/-inf -> unmodified
 *   NaN -> NaN (unsupported by Chimera)
 */
float asinhf(float arg)
{
	float z;
	float x = fabsf(arg);
	if (x > 1.6777216e7f) // prevent overflow in x*x
		z = logf(2 * x);
	else
		z = logf(x + sqrtf(x*x + 1.0f));
	return copysignf(z, arg);
}

/**
 * Compute acosh(x)
 *
 * Algorithm:
 *
 *   Use identity
 *
 *     acosh(x) = log(x + sqrt(x^2-1))
 *
 * Accuracy:
 *
 *   Over [1, inf), max relative error:
 *     HIGH_ACCURACY = 1: 1.03e-06
 *     HIGH_ACCURACY = 0: 4.60e-03
 *
 * Special cases:
 *
 *   x < 1 -> NaN (unsupported on Chimera)
 *   1 -> +0
 *   +inf -> +inf
 *   NaN -> NaN (unsupported by Chimera)
 */
float acoshf(float arg)
{
	if (arg <= 1.0f) {
		return 0.0f;
	}
	if (arg > 1.6777216e7f) // prevent overflow in x*x
		return logf(2 * arg);
	else
		return logf(arg + sqrtf(arg*arg - 1.0f));
}

/**
 * Compute atanh(x)
 *
 * Algorithm:
 *
 *   Use identity
 *
 *     atanh(x) = 1/2 * log((1+x)/(1-x))
 *
 * Accuracy:
 *
 *   Over (-1, 1), max relative error:
 *     HIGH_ACCURACY = 1: 5.28e-06
 *     HIGH_ACCURACY = 0: 6.05e-03
 *
 * Special cases:
 *
 *   +/-0 or +/-inf -> unmodified
 *   NaN -> NaN (unsupported by Chimera)
 */
float atanhf(float arg)
{
	if (fabsf(arg) > 1.0f)
		return 0.0f;
#if MATHLIB_STRICT
	if (arg == 0.0f) {
		return arg;
	}
#endif
	return 0.5f*logf((1.0f + arg) / (1.0f - arg));
}

/**
 * Computes Gamma(x).
 *
 * Algorithm:
 *
 *   Reduces x to interval [2,3] using identity:
 *
 *     Gamma(z+n) = z*(z+1)*...*(z+(n-1))*Gamma(z)
 *
 *   Basically this just means we multiply or divide by a product of
 *   consecutive integers to get into range.
 *
 *   Gamma(z) in [2,3] is approximated with a rational approximation,
 *   either Hart 5201 or 5206.
 *
 * Accuracy:
 *
 *   Over (-32, 32), max relative error is:
 *     HIGH_ACCURACY=1: 1.34e-04
 *     HIGH_ACCURACY=0: 8.31e-03
 *
 *   These measurements exclude the areas around 0, 1, and negative
 *   integers, where relative accuracy is undefined. Accuracy is exact
 *   for positive integers.
 *
 * Special cases:
 *
 *  +/- 0 -> +/- inf
 *  negative integer -> NaN (unsupported on Chimera)
 *  -inf -> NaN (unsupported on Chimera)
 *  +inf -> +inf
 *  NaN -> NaN (unsupported on Chimera)
 */
float tgammaf(float arg)
{
	float x, X, G; // following Hart's notation
	int k, N;

	// short-circuit to prevent indefinite looping below
	if (fabsf(arg) > 36.0) {
		if (signbit(arg) && round_to_nearest(arg) != arg) {
			return 0.0f;
		}
		else {
			// FIXME: get sign right
			return INFINITY;
		}
	}

#if MATHLIB_STRICT
	if (arg == 0.0f) {
		return copysignf(INFINITY, arg);
	}
#endif

	x = arg - 2.0f;
	N = (int)floorf(x);
	X = x - (float)N;
#if MATHLIB_HIGH_ACCURACY == 0
	// Hart 5201
	G = (0.6896826f*X + 0.2936889f)*X + 1.008308f;
#else
	// Hart 5206
	G = ((((((0.16063028892e-2f*X + 0.5159029832e-2f)*X + 0.4451087862e-2f)*X + 0.721101941645e-1f)*X + 0.821117276973e-1f)*X + 0.4117741970939f)*X + 0.4227874604607f)*X + 0.9999999757437f;
#endif
	X += 2.0f;
	if (N >= 0) {
		for (k = 0; k < N; k++) {
			G *= X + k;
		}
	}
	else {
		float a = 1.0f;
		for (k = 1; k <= -N; k++) {
			a *= X - k;
		}
		G /= a;
	}
	return G;
}

/**
 * Computes log(Gamma(x)).
 *
 * Algorithm:
 *
 *   For small x, use logf(tgammaf(x)). For larger x, use Stirling's
 *   approximation to x!:
 *
 *     log(Gamma(x)) = (x - 0.5)*log(x) - x + log(sqrt(2*pi)) + 1/x * R(1/x^2)
 *
 *   where R is a rational function (Hart 5400).
 *
 * Accuracy:
 *
 *   Over (0, 1.3e36), max relative error is:
 *     HIGH_ACCURACY=1: 5.57e-06
 *     HIGH_ACCURACY=0: 7.41e-02
 *
 *   These measurements exclude the areas around 0, 1, and 2, where
 *   log(Gamma(x)) has a pole and zeros, since the relative error
 *   blows up at those points.
 *
 * Special cases:
 *
 *  1 or 2 -> +0
 *  +/-0 -> +inf
 *  negative integer -> +inf
 *  +/-inf -> +inf
 *  NaN -> NaN (unsupported on Chimera)
 *
 * Note:
 *
 *  Negative arguments probably don't work right.
 */

float lgammaf(float arg)
{
	float P, y;

	// handle small and negative integers with normal tgammaf(). This
	// breaks negative values a bit but I'm not sure they're useful
	// anyway, as Matlab's gammaln() doesn't even work with negative
	// values.
#ifdef MATHLIB_STRICT
	if (isinf(arg)) {
		return INFINITY;
	}
#endif
	if (arg < 8.0f) {
		return logf(tgammaf(arg));
	}
	// Hart 5400
	P = 1.0f / arg * (0.83333273853e-1f - 0.2762472e-2f / (arg*arg));
	// Stirling approximation
	y = (arg - 0.5f) * logf(arg) - arg + 0.9189385f + P;
	return y;
}

static float erfc1(float arg)
{
	float p, q;

	if (arg > 10.0f) return 0.0f;

#if MATHLIB_HIGH_ACCURACY == 0
	// Hart 5600
	p = 0.30214f;
	q = 0.26543f + arg;
#else
	// Hart 5620
	p = ((-0.26637445e0f*arg + 0.29441993e1f)*arg - 0.10741699e2f)*arg + 0.12986981e2f;
	q = (((((((arg - 0.34369391e1f)*arg + 0.67569795e1f)*arg - 0.479192e1f)*arg + 0.50736012e1f)*arg + 0.23361795e1f)*arg + 0.74658056e1f)*arg + 0.39073962e1f)*arg + 0.12987021e2f;
#endif
	return p / q;
}

static float erfc2(float arg)
{
	float p, q;

	if (arg > 10.0f) return 0.0f;

#if MATHLIB_HIGH_ACCURACY == 0
	// Hart 5660
	p = 0.7942561f;
	q = 0.7894589f + arg;
#else
	// Hart 5662
	p = (0.57404837033548f * arg + 0.2153997718487e1f)*arg + 0.353221659121e1f;
	q = ((arg + 0.39690911552337e1f)*arg + 0.6139719523819e1f)*arg + 0.353221617055e1f;
#endif
	return expf(-arg * arg)*p / q;
}

/**
 * Computes error function.
 *
 * Algorithm:
 *
 *   Uses 1-erfcf(x), choosing between two approximations for erfc
 *   depending on the value of x. See erfcf() for details.
 *
 * Accuracy:
 *
 *   Over (-127, 127), max absolute error:
 *     HIGH_ACCURACY=1: 3.25e-06
 *     HIGH_ACCURACY=0: 1.38e-01
 *
 * Special cases:
 *
 *   +/-0 -> +/-0
 *  +/-inf -> +/-1
 *  NaN -> NaN (unsupported on Chimera)
 */
float erff(float arg)
{
	float x;
	float y;
#if MATHLIB_STRICT
	if (arg == 0.0f) return arg;
#endif

	x = fabsf(arg);
	if (x < 0.47)
		y = 1.0f - erfc1(x);
	else
		y = 1.0f - erfc2(x);

	if (signbit(arg))
		return -y;
	else
		return y;
}

/**
 * Computes complementary error function.
 *
 * Algorithm:
 *
 *   First reduces x to [0,inf). Then chooses between two rational
 *   approximations for erfc:
 *
 *     x < 0.47: Hart 5600/5620
 *     x >= 0.47: Hart 5660/5662
 *
 * Accuracy:
 *
 *   Over (-127, 127), max absolute error:
 *     HIGH_ACCURACY=1: 3.34e-06
 *     HIGH_ACCURACY=0: 1.38e-01
 *
 * Special cases:
 *
 *  +inf -> +0
 *  -inf -> 2
 *  NaN -> NaN (unsupported on Chimera)
 */
float erfcf(float arg)
{
	float x = fabsf(arg);
	float y;
	if (x < 0.47)
		y = erfc1(x);
	else
		y = erfc2(x);

	if (signbit(arg))
		return 2.0f - y;
	else
		return y;
}

// It's better to remove fmaf() than to give people false hope that it
// works correctly.

///**
// * Fused multiply-add x*y+z.
// *
// * Inputs:
// *   x, y: numbers to multiply
// *   z: number to add
// * Outputs:
// *   returns x*y+z
// *
// * Note:
// *   This does not actually do a fused multiply-accumulate.
// */
//float fmaf(float x, float y, float z)
//{
//  return x*y + z;
//}
