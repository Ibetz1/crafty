#ifndef _CVEC3_HPP
#define _CVEC3_HPP

#include <cstdint>
#include <cmath>

extern "C" {
    #include "base_types.h"
}

/*
    broad utilities defined here for convenience
    also they have to be implimented in a header
    since other headers use them, super annoying
*/

// clamp a number
template <typename T>
T clamp(T val, T min, T max) {
    return (T) fmin(fmax(val, min), max);
}

// take the modulus of any type
template <typename T>
T mod(T val, T wrap) {
    return (T) fmod(val, wrap);
}

/*
    basic vector2 without real vector math I.E dot product
*/
#define cvec2_count 2

template <typename TYPE>
union cvec2 {
    struct { TYPE x, y; };
    TYPE data[cvec2_count];

    // add vector b to vector a
    static cvec2<TYPE> add(cvec2<TYPE> a, cvec2<TYPE> b) { 
        return { 
            (TYPE) (a.x + b.x), 
            (TYPE) (a.y + b.y), 
        }; 
    }

    // subtract vector b from vector a
    static cvec2<TYPE> sub(cvec2<TYPE> a, cvec2<TYPE> b) { 
        return { 
            (TYPE) (a.x - b.x), 
            (TYPE) (a.y - b.y), 
        }; 
    }

    // multiple vector b with vector a
    static cvec2<TYPE> mul(cvec2<TYPE> a, cvec2<TYPE> b) { 
        return { 
            (TYPE) (a.x * b.x), 
            (TYPE) (a.y * b.y), 
        }; 
    }

    // divide vector b from vector a
    static cvec2<TYPE> div(cvec2<TYPE> a, cvec2<TYPE> b) { 
        return { 
            (TYPE) (a.x / b.x), 
            (TYPE) (a.y / b.y), 
        }; 
    }

    // add vector b to vector a
    static cvec2<TYPE> add(cvec2<TYPE> a, TYPE b) { 
        return { 
            (TYPE) (a.x + b), 
            (TYPE) (a.y + b), 
        }; 
    }

    // subtract value b from vector a
    static cvec2<TYPE> sub(cvec2<TYPE> a, TYPE b) { 
        return { 
            (TYPE) (a.x - b), 
            (TYPE) (a.y - b), 
        }; 
    }

    // multiple vector a with value b
    static cvec2<TYPE> mul(cvec2<TYPE> a, TYPE b) { 
        return { 
            (TYPE) (a.x * b), 
            (TYPE) (a.y * b), 
        }; 
    }

    // divide value b from vector a
    static cvec2<TYPE> div(cvec2<TYPE> a, TYPE b) { 
        return { 
            (TYPE) (a.x / b), 
            (TYPE) (a.y / b), 
        }; 
    }

    // cast the type of a color vector
    template <typename NEW_TYPE>
    static cvec2<NEW_TYPE> cast(cvec2<TYPE> v) {
        return { 
            (NEW_TYPE) v.x, 
            (NEW_TYPE) v.y, 
        }; 
    }

    // will clamp vector a to bounds set in b and c
    static cvec2<TYPE> vclamp(cvec2<TYPE> a, cvec2<TYPE> b, cvec2<TYPE> c) {
        return {
            clamp<TYPE>(a.x, b.x, c.x),
            clamp<TYPE>(a.y, b.y, c.y),
        };
    }

    // will clamp vector a to bounds set in b and c
    static cvec2<TYPE> vclamp(cvec2<TYPE> a, TYPE b, TYPE c) {
        return {
            clamp<TYPE>(a.x, b, c),
            clamp<TYPE>(a.y, b, c),
        };
    }

    // convert one value into a vector
    static cvec2<TYPE> splat(TYPE val) {
        return {val, val};
    }

    // return  a vector filled with 0's
    static cvec2<TYPE> zero() {
        return cvec2<TYPE>::splat(0);
    }

    // generate a new vector
    static cvec2<TYPE> xy(TYPE x, TYPE y) {
        return {x, y};
    }

    // length of vector
    static TYPE length(cvec2<TYPE> vec) {
        return (TYPE) sqrt(powf(vec.x, 2) + powf(vec.y, 2));
    }
};


/*
    basic vector3 without real vector math I.E dot product
*/
#define cvec3_count 3

template <typename TYPE>
union cvec3 {

    // vector base components
    struct { TYPE x, y, z; };
    TYPE data[cvec3_count];

    // add vector b to vector a
    static cvec3<TYPE> add(cvec3<TYPE> a, cvec3<TYPE> b) { 
        return { 
            (TYPE) (a.x + b.x), 
            (TYPE) (a.y + b.y), 
            (TYPE) (a.z + b.z)
        }; 
    }

    // subtract vector b from vector a
    static cvec3<TYPE> sub(cvec3<TYPE> a, cvec3<TYPE> b) { 
        return { 
            (TYPE) (a.x - b.x), 
            (TYPE) (a.y - b.y), 
            (TYPE) (a.z - b.z)
        }; 
    }

    // multiple vector b with vector a
    static cvec3<TYPE> mul(cvec3<TYPE> a, cvec3<TYPE> b) { 
        return { 
            (TYPE) (a.x * b.x), 
            (TYPE) (a.y * b.y), 
            (TYPE) (a.z * b.z)
        }; 
    }

    // divide vector b from vector a
    static cvec3<TYPE> div(cvec3<TYPE> a, cvec3<TYPE> b) { 
        return { 
            (TYPE) (a.x / b.x), 
            (TYPE) (a.y / b.y), 
            (TYPE) (a.z / b.z)
        }; 
    }

    // add vector b to vector a
    static cvec3<TYPE> add(cvec3<TYPE> a, TYPE b) { 
        return { 
            (TYPE) (a.x + b), 
            (TYPE) (a.y + b), 
            (TYPE) (a.z + b)
        }; 
    }

    // subtract value b from vector a
    static cvec3<TYPE> sub(cvec3<TYPE> a, TYPE b) { 
        return { 
            (TYPE) (a.x - b), 
            (TYPE) (a.y - b), 
            (TYPE) (a.z - b)
        }; 
    }

    // multiple vector a with value b
    static cvec3<TYPE> mul(cvec3<TYPE> a, TYPE b) { 
        return { 
            (TYPE) (a.x * b), 
            (TYPE) (a.y * b), 
            (TYPE) (a.z * b)
        }; 
    }

    // divide value b from vector a
    static cvec3<TYPE> div(cvec3<TYPE> a, TYPE b) { 
        return { 
            (TYPE) (a.x / b), 
            (TYPE) (a.y / b), 
            (TYPE) (a.z / b)
        }; 
    }

    // cast the type of a color vector
    template <typename NEW_TYPE>
    static cvec3<NEW_TYPE> cast(cvec3<TYPE> v) {
        return { 
            (NEW_TYPE) v.x, 
            (NEW_TYPE) v.y, 
            (NEW_TYPE) v.z, 
        }; 
    }

    // will clamp vector a to bounds set in b and c
    static cvec3<TYPE> vclamp(cvec3<TYPE> a, cvec3<TYPE> b, cvec3<TYPE> c) {
        return {
            clamp<TYPE>(a.x, b.x, c.x),
            clamp<TYPE>(a.y, b.y, c.y),
            clamp<TYPE>(a.z, b.z, c.y),
        };
    }

    // will clamp vector a to bounds set in b and c
    static cvec3<TYPE> vclamp(cvec3<TYPE> a, TYPE b, TYPE c) {
        return {
            clamp<TYPE>(a.x, b, c),
            clamp<TYPE>(a.y, b, c),
            clamp<TYPE>(a.z, b, c),
        };
    }

    // convert one value into a vector
    static cvec3<TYPE> splat(TYPE val) {
        return {val, val, val};
    }

    // return  a vector filled with 0's
    static cvec3<TYPE> zero() {
        return cvec3<TYPE>::splat(0);
    }

    // generate a new vector
    static cvec3<TYPE> xyz(TYPE x, TYPE y, TYPE z) {
        return {x, y, z};
    }

    // length of vector
    static TYPE length(cvec3<TYPE> vec) {
        return (TYPE) sqrt(powf(vec.x, 2) + powf(vec.y, 2) + powf(vec.z, 2));
    }
};

/*
    base types
*/
typedef cvec2<U8>  vec2_u8;
typedef cvec2<U64> vec2_u64;
typedef cvec2<F32> vec2_f32;

typedef cvec3<U8>  vec3_u8;
typedef cvec3<U64> vec3_u64;

typedef cvec3<S64> vec3_s64;

typedef cvec3<F32> vec3_f32;

#endif