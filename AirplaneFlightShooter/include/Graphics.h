#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "Config.h"
#include "Entities.h"

extern GLuint dlBuilding, dlTree, dlPlane, dlPropeller;
extern GLuint terrainTex, roadTex;

extern GLfloat sunDir[4];
extern GLfloat ambLight[4];
extern GLfloat difLight[4];
extern GLfloat spcLight[4];

void drawBox(float w, float h, float d);
void buildDisplayLists();
void setupTextures();
void initGL();

void renderBuilding(int idx);
void renderTree(Tree& t);
void renderPlane(Plane& p, bool isPlayer, bool isEnemy);
void renderShadow(Plane& p);
void renderTerrain();
void renderSky();
void renderBullets();
void renderMissiles();
void renderParticles();
void renderText2D(float x, float y, const char* str, float r=1, float g=1, float b=1);
void renderHUD();
void setupCamera();

#endif
