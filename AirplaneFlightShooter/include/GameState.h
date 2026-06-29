#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "Config.h"
#include "Entities.h"
#include <vector>

// ============================================================
// State Global Eksternal (Agar Bisa Diakses Antar Modul)
// ============================================================
extern Plane    player;
extern Plane    enemies[ENEMY_COUNT];
extern Bullet   bullets[BULLET_MAX];
extern Missile  missiles[MISSILE_MAX];
extern Particle particles[PARTICLE_MAX];
extern Crater   craters[CRATER_MAX];

extern int   craterCount, score, enemiesKilled;
extern int   scoreFromKills, scoreFromWaves, scoreFromBldg;
extern float gameTime, dt;
extern int   lastTime;
extern bool  gameOver, gameWon, gamePaused;

extern int   currentWave, killsThisWave;
extern bool  waveClearPopup, waveTransition;
extern float waveClearTimer, waveTransitionTimer;
extern char  waveClearMsg[64];

extern int   missileAmmo;
extern float missileReload;
extern float enemyBulletDmg, enemySpeedMult;

extern int   camMode;
extern float camOrbitYaw, camOrbitPitch, camOrbitDist;
extern bool  keyState[256], specState[256];
extern int   mouseBtn[3], lastMouseX, lastMouseY;

extern float propAngle, lockOnTimer;
extern int   lockedEnemy;
extern bool  aimAssistOn;
extern float crossWorldX, crossWorldY;

// ============================================================
// Fungsi Logika & Gameplay
// ============================================================
void initPlayer();
void initEnemies();
void initBullets();
void initParticles();
void spawnParticles(Vec3 pos, int n, bool fire);
void addCrater(Vec3 pos, int buildingIdx, bool onFire);

int  findBestLockTarget();
Vec3 calcLeadPoint(int enemyIdx);
bool worldToScreen(Vec3 pos, int W, int H, float& sx, float& sy);

void fireBullet(Plane& p, bool fromPlayer);
void fireMissile();

bool sphereAABB(Vec3 c, float r, float bx, float bz, float bw, float bd, float bh);
void checkBulletBuildingCollision(Bullet& bullet);
void checkBulletPlaneCollision(Bullet& bullet);

void updatePlayer(float dt_);
void updateEnemy(Plane& e, float dt_);
void updateMissiles(float dt_);
void updateGameLogic();

#endif // GAME_STATE_H
