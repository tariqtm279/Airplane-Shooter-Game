#ifndef CONFIG_H
#define CONFIG_H

#ifdef _WIN32
  #include <windows.h>
#endif

#include <GL/glut.h>

// Konstanta Map & Objek
#define MAP_SIZE        200.0f
#define BUILDING_COUNT   96     
#define TREE_COUNT      160     
#define ENEMY_COUNT       5
#define BULLET_MAX       30
#define MISSILE_MAX       6
#define PARTICLE_MAX    200
#define CRATER_MAX       50

static const float PI = 3.14159265f;
const int TOTAL_WAVES = 3;
const int KILLS_PER_WAVE = 5;   
const float WAVE_PAUSE = 3.0f;
const float MISSILE_RELOAD_TIME = 8.f;

// Konstanta Jalan Kota
static const float ROAD_MAIN    = 8.0f;   
static const float ROAD_SEC     = 5.5f;   
static const float BLOCK_STRIDE = 26.0f;  

#endif
