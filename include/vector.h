#ifndef VECTOR_H_INCLUDED
#define VECTOR_H_INCLUDED

#include <data_structures.h>

vec3f vec3f_create(float x, float y, float z);
vec3f vec3f_cross_product(vec3f a, vec3f b);
float vec3f_dot_product(vec3f a, vec3f b);
vec3f vec3f_scalar_product(vec3f a, float s);
vec3f vec3f_add(vec3f a, vec3f b);
vec3f vec3f_subtract(vec3f a, vec3f b);
float vec3f_length(vec3f a);
vec3f vec3f_normalized(vec3f a);


vec2f vec2f_create(float x, float y);
float vec2f_dot_product(vec2f a, vec2f b);
vec2f vec2f_scalar_product(vec2f a, float s);
vec2f vec2f_add(vec2f a, vec2f b);
vec2f vec2f_subtract(vec2f a, vec2f b);
float vec2f_length(vec2f a);
vec2f vec2f_normalized(vec2f a);

#endif // VECTOR_H_INCLUDED
