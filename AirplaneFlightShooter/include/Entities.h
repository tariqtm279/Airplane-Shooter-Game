#ifndef ENTITIES_H
#define ENTITIES_H

#include "MathUtils.h"

struct Building { float x, z, w, d, h, r, g, b; bool damaged; int crackLevel; };
struct Tree     { float x, z, height, trunkR; };
struct Plane    { Vec3 pos, vel, aiTarget; float yaw, pitch, roll, health, shootCooldown, aiTimer; bool alive; };
struct Bullet   { Vec3 pos, vel; bool active, fromPlayer; float life; };
struct Missile  { Vec3 pos, vel; bool active; int targetIdx; float life, turnSpeed; };
struct Particle { Vec3 pos, vel; float r, g, b, a, life, size; bool active, isFire; };
struct Crater   { Vec3 pos; float radius, fireTimer; int buildingIdx; bool onFire; };

#endif
