#pragma once

#include <algorithm>
#include <cmath>
#include <type_traits>

#include "util/assert.h"
#include "util/fpclassify.h"

// If we don't do this then we get the C90 fabs from the global namespace which
// is only defined for double.
using std::fabs;

#define math_max std::max
#define math_min std::min
#define math_max3(a, b, c) math_max(math_max((a), (b)), (c))
#define math_min3(a, b, c) math_min(math_min((a), (b)), (c))

// Restrict value to the range [min, max]. Undefined behavior if min > max.
template <typename T>
inline T math_clamp(T value, T min, T max) {
    // DEBUG_ASSERT compiles out in release builds so it does not affect
    // vectorization or pipelining of clamping in tight loops.
    DEBUG_ASSERT(min <= max);
    return math_max(min, math_min(max, value));
}

// NOTE(rryan): It is an error to call even() on a floating point number. Do not
// hack this to support floating point values! The programmer should be required
// to manually convert so they are aware of the conversion.
template <typename T>
inline bool even(T value) {
    return value % 2 == 0;
}

#ifdef _MSC_VER
// Ask VC++ to emit an intrinsic for fabs instead of calling std::fabs.
#pragma intrinsic(fabs)
#endif

inline int roundUpToPowerOf2(int v) {
    int power = 1;
    while (power < v && power > 0) {
        power *= 2;
    }
    // There is not a power of 2 higher than v representable by our
    // architecture's integer size.
    if (power < 0) {
        return -1;
    }
    return power;
}

inline double roundToFraction(double value, int denominator) {
    int wholePart = static_cast<int>(value);
    double fractionPart = value - wholePart;
    double numerator = std::round(fractionPart * denominator);
    return wholePart + numerator / denominator;
}

template <typename T>
inline const T ratio2db(const T a) {
    static_assert(std::is_same<float, T>::value ||
                  std::is_same<double, T>::value ||
                  std::is_same<long double, T>::value,
                  "ratio2db works only for floating point types");
    return log10(a) * 20;
}

template <typename T>
inline const T db2ratio(const T a) {
    static_assert(std::is_same<float, T>::value ||
                  std::is_same<double, T>::value ||
                  std::is_same<long double, T>::value,
                  "db2ratio works only for floating point types");
    return static_cast<T>(pow(10, a / 20));
}
