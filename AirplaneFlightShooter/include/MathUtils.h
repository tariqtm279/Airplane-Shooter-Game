#ifndef MATH_UTILS_H
#define MATH_UTILS_H

#include <cmath>
#include <cstdlib>

struct Vec3 { float x, y, z; };

inline Vec3 vec3(float x, float y, float z){ return {x,y,z}; }
inline Vec3 operator+(Vec3 a, Vec3 b){ return {a.x+b.x, a.y+b.y, a.z+b.z}; }
inline Vec3 operator-(Vec3 a, Vec3 b){ return {a.x-b.x, a.y-b.y, a.z-b.z}; }
inline Vec3 operator*(Vec3 a, float s){ return {a.x*s, a.y*s, a.z*s}; }
inline float vlen(Vec3 v){ return sqrtf(v.x*v.x+v.y*v.y+v.z*v.z); }
inline Vec3 vnorm(Vec3 v){ float l=vlen(v); if(l<0.0001f)return{0,0,0}; return v*(1.f/l); }
inline float vdot(Vec3 a,Vec3 b){return a.x*b.x+a.y*b.y+a.z*b.z;}

inline float randf(float lo, float hi){ return lo + (hi-lo)*((float)rand()/RAND_MAX); }

#endif
