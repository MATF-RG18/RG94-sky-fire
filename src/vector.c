#include <math.h>
#include <stdlib.h>

#include <vector.h>


inline vec3f vec3f_create(float x, float y, float z)
{
    return (vec3f) {x, y, z};
}

inline vec3f vec3f_cross_product(vec3f a, vec3f b)
{
    return (vec3f) {a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.z};
}

inline float vec3f_dot_product(vec3f a, vec3f b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

inline vec3f vec3f_scalar_product(vec3f a, float s)
{
    return (vec3f) {a.x * s, a.y * s, a.z * s};
}

inline vec3f vec3f_add(vec3f a, vec3f b)
{
    return (vec3f) {a.x + b.x, a.y + b.y, a.z + b.z};
}

inline vec3f vec3f_subtract(vec3f a, vec3f b)
{
    return (vec3f) {a.x - b.x, a.y - b.y, a.z - b.z};
}

inline float vec3f_length(vec3f a)
{
    return sqrtf(a.x * a.x + a.y * a.y + a.z * a.z);
}

vec3f vec3f_normalized(vec3f a)
{
    float l = vec3f_length(a);
    return (vec3f) {a.x / l, a.y / l, a.z / l};
}



inline vec2f vec2f_create(float x, float y)
{
    return (vec2f) {x, y};
}

inline float vec2f_dot_product(vec2f a, vec2f b)
{
    return a.x * b.x + a.y * b.y;
}

inline vec2f vec2f_scalar_product(vec2f a, float s)
{
    return (vec2f) {a.x * s, a.y * s};
}

inline vec2f vec2f_add(vec2f a, vec2f b)
{
    return (vec2f) {a.x + b.x, a.y + b.y};
}

inline vec2f vec2f_subtract(vec2f a, vec2f b)
{
    return (vec2f) {a.x - b.x, a.y - b.y};
}

inline float vec2f_length(vec2f a)
{
    return sqrtf(a.x * a.x + a.y * a.y );
}

inline vec2f vec2f_normalized(vec2f a)
{
    float l = vec2f_length(a);
    return (vec2f) {a.x / l, a.y / l};
}
