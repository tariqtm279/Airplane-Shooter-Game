#include <GL/glut.h>  // Jika menggunakan GLUT
#include <cstdio>     // <--- TAMBAHKAN BARIS INI untuk memperbaiki error sprintf
#include <cmath>

#include "Graphics.h"
#include "GameState.h"
#include "City.h"

GLuint dlBuilding, dlTree, dlPlane, dlPropeller;
GLuint terrainTex, roadTex;

GLfloat sunDir[4]   = { 0.6f, 1.0f, 0.4f, 0.0f };
GLfloat ambLight[4] = { 0.25f,0.25f,0.28f,1.f };
GLfloat difLight[4] = { 0.95f,0.90f,0.80f,1.f };
GLfloat spcLight[4] = { 1.f,  1.f,  0.95f,1.f };

void drawBox(float w, float h, float d){
    float hw=w*0.5f, hd=d*0.5f;
    glBegin(GL_QUADS);
    glNormal3f(0,0,1);  glVertex3f(-hw,0,hd);  glVertex3f(hw,0,hd);  glVertex3f(hw,h,hd);  glVertex3f(-hw,h,hd);
    glNormal3f(0,0,-1); glVertex3f(hw,0,-hd);  glVertex3f(-hw,0,-hd);glVertex3f(-hw,h,-hd);glVertex3f(hw,h,-hd);
    glNormal3f(-1,0,0); glVertex3f(-hw,0,-hd); glVertex3f(-hw,0,hd); glVertex3f(-hw,h,hd); glVertex3f(-hw,h,-hd);
    glNormal3f(1,0,0);  glVertex3f(hw,0,hd);   glVertex3f(hw,0,-hd); glVertex3f(hw,h,-hd); glVertex3f(hw,h,hd);
    glNormal3f(0,1,0);  glVertex3f(-hw,h,hd);  glVertex3f(hw,h,hd);  glVertex3f(hw,h,-hd); glVertex3f(-hw,h,-hd);
    glNormal3f(0,-1,0); glVertex3f(-hw,0,-hd); glVertex3f(hw,0,-hd); glVertex3f(hw,0,hd);  glVertex3f(-hw,0,hd);
    glEnd();
}

void buildDisplayLists(){
    dlPlane=glGenLists(1);
    glNewList(dlPlane,GL_COMPILE);
    {
        GLfloat matSpec[]={1.f,1.f,1.f,1.f}; GLfloat matShine[]={100.f};
        glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,matSpec);
        glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,matShine);
        glColor3f(0.3f,0.4f,0.2f);
        GLfloat mat[]={0.3f,0.4f,0.2f,1.f}; glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE,mat);
        glPushMatrix(); glScalef(0.6f,0.5f,3.f); drawBox(1,1,1); glPopMatrix();
        GLfloat wm[]={0.25f,0.35f,0.15f,1.f}; glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE,wm);
        glPushMatrix(); glTranslatef(-1.8f,0,0); glScalef(3.2f,0.1f,1.2f); drawBox(1,1,1); glPopMatrix();
        glPushMatrix(); glTranslatef(1.8f,0,0);  glScalef(3.2f,0.1f,1.2f); drawBox(1,1,1); glPopMatrix();
        glPushMatrix(); glTranslatef(0,0,-1.2f);  glScalef(1.6f,0.08f,0.7f); drawBox(1,1,1); glPopMatrix();
        glPushMatrix(); glTranslatef(0,0.3f,-1.3f); glScalef(0.08f,0.8f,0.7f); drawBox(1,1,1); glPopMatrix();
        GLfloat glass[]={0.4f,0.6f,1.f,0.7f}; glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE,glass);
        glPushMatrix(); glTranslatef(0,0.3f,0.6f); glScalef(0.4f,0.35f,0.6f); drawBox(1,1,1); glPopMatrix();
    }
    glEndList();

    dlPropeller=glGenLists(1);
    glNewList(dlPropeller,GL_COMPILE);
    {
        GLfloat pm[]={0.2f,0.2f,0.2f,1.f}; glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE,pm);
        for(int i=0;i<4;i++){
            glPushMatrix();
            glRotatef(i*90.f,0,0,1); glTranslatef(0,0.8f,0); glScalef(0.12f,1.6f,0.15f); drawBox(1,1,1);
            glPopMatrix();
        }
        glPushMatrix(); glutSolidSphere(0.18f,12,8); glPopMatrix();
    }
    glEndList();
}

void renderBuilding(int idx){
    Building& b=buildings[idx];
    if(b.x > 9000.f) return;
    
    glPushMatrix(); glTranslatef(b.x,0,b.z);
    GLfloat noSpec[]={0.f,0.f,0.f,1.f}; GLfloat noShine[]={0.f};
    glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,noSpec);
    glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,noShine);
    if(b.crackLevel==2) glColor3f(0.4f+0.2f*(float)(rand()%10)/10.f,0.15f,0.05f);
    else                glColor3f(b.r,b.g,b.b);
    drawBox(b.w,b.h,b.d);
    if(b.h>5.f){
        glColor3f(0.5f,0.7f,1.f);
        int floors=(int)(b.h/3.f);
        for(int fl=0;fl<floors;fl++){
            float fy=0.8f+fl*3.f;
            for(int w=0;w<2;w++){
                float fx=(w==0)?-b.w*0.25f:b.w*0.25f;
                glPushMatrix(); glTranslatef(fx,fy,b.d*0.501f); glScalef(b.w*0.18f,1.f,0.02f); drawBox(1,1,1); glPopMatrix();
            }
        }
    }
    if(b.crackLevel>=1){
        glDisable(GL_LIGHTING); glColor3f(0.05f,0.05f,0.05f); glLineWidth(2.f);
        srand(idx*777);
        glBegin(GL_LINES);
        for(int c=0;c<4;c++){
            float cx_=randf(-b.w*0.4f,b.w*0.4f), cz_=b.d*0.505f, cy_=randf(b.h*0.2f,b.h*0.9f);
            glVertex3f(cx_,cy_,cz_); glVertex3f(cx_+randf(-0.8f,0.8f),cy_+randf(-2.f,2.f),cz_);
        }
        glEnd(); glEnable(GL_LIGHTING);
    }
    glPopMatrix();
}

void renderTree(Tree& t){
    glPushMatrix(); glTranslatef(t.x,0,t.z);
    GLfloat noSpec[]={0.f,0.f,0.f,1.f}; GLfloat noShine[]={0.f};
    glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,noSpec);
    glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,noShine);
    glColor3f(0.4f,0.25f,0.1f);
    glPushMatrix(); glScalef(t.trunkR*2,t.height*0.45f,t.trunkR*2); drawBox(1,1,1); glPopMatrix();
    glColor3f(0.11f,0.52f,0.14f);
    for(int i=0;i<3;i++){
        glPushMatrix(); glTranslatef(0,t.height*0.4f+i*t.height*0.18f,0);
        glutSolidCone(t.height*0.24f*(1.f-i*0.15f),t.height*0.38f,8,4); glPopMatrix();
    }
    glPopMatrix();
}

void renderPlane(Plane& p, bool isPlayer, bool isEnemy){
    if(!p.alive) return;
    glPushMatrix();
    glTranslatef(p.pos.x,p.pos.y,p.pos.z);
    glRotatef(p.yaw,0,1,0); glRotatef(p.pitch,1,0,0); glRotatef(p.roll,0,0,1);
    if(isEnemy){
        GLfloat em[]={0.7f,0.1f,0.1f,1.f};
        glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE,em);
    }
    glCallList(dlPlane);
    glPushMatrix(); glTranslatef(0,0,1.55f);
    glRotatef(propAngle*(isEnemy?-1.3f:1.f),0,0,1);
    glCallList(dlPropeller); glPopMatrix();
    if(isPlayer){
        GLfloat trail[]={0.9f,0.5f,0.1f,1.f};
        glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE,trail);
        glPushMatrix(); glTranslatef(0,0,-1.6f); glutSolidSphere(0.12f,6,4); glPopMatrix();
    }
    glPopMatrix();
}

void renderShadow(Plane& p){
    if(!p.alive) return;
    glDisable(GL_LIGHTING);
    glPushMatrix();
    glTranslatef(p.pos.x,0.15f,p.pos.z);
    glRotatef(p.yaw,0,1,0);
    glScalef(1.f,0.01f,1.f);
    glEnable(GL_FOG);
    GLfloat sc[]={0.1f,0.1f,0.1f,0.8f};
    glFogfv(GL_FOG_COLOR,sc); glFogf(GL_FOG_START,0.f); glFogf(GL_FOG_END,0.1f);
    glCallList(dlPlane);
    GLfloat sky[]={0.62f,0.80f,0.97f,1.f};
    glFogfv(GL_FOG_COLOR,sky); glFogf(GL_FOG_START,80.f); glFogf(GL_FOG_END,350.f);
    glPopMatrix();
    glEnable(GL_LIGHTING);
}

void renderTerrain(){
    float s=MAP_SIZE;
    glEnable(GL_TEXTURE_2D); glBindTexture(GL_TEXTURE_2D,terrainTex);
    GLfloat dm[]={0.8f,0.6f,0.4f,1.f}; glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE,dm);
    glBegin(GL_QUADS); glNormal3f(0,1,0);
    glTexCoord2f(0.f,0.f);    glVertex3f(-s,0,-s);
    glTexCoord2f(s/4.f,0.f);  glVertex3f(s,0,-s);
    glTexCoord2f(s/4.f,s/4.f);glVertex3f(s,0,s);
    glTexCoord2f(0.f,s/4.f);  glVertex3f(-s,0,s);
    glEnd(); glDisable(GL_TEXTURE_2D);

    GLfloat rm[]={0.32f,0.32f,0.32f,1.f}; glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE,rm);
    glEnable(GL_TEXTURE_2D); glBindTexture(GL_TEXTURE_2D,roadTex);
    glBegin(GL_QUADS); glNormal3f(0,1,0);
    glTexCoord2f(0,0);  glVertex3f(-ROAD_MAIN, 0.05f, -s); glTexCoord2f(4,0);  glVertex3f(ROAD_MAIN, 0.05f, -s);
    glTexCoord2f(4,50); glVertex3f(ROAD_MAIN,  0.05f, s);  glTexCoord2f(0,50); glVertex3f(-ROAD_MAIN, 0.05f, s);
    glEnd();
    glBegin(GL_QUADS); glNormal3f(0,1,0);
    glTexCoord2f(0,0);  glVertex3f(-s, 0.05f, -ROAD_MAIN); glTexCoord2f(50,0); glVertex3f(s, 0.05f, -ROAD_MAIN);
    glTexCoord2f(50,4); glVertex3f(s, 0.05f, ROAD_MAIN);  glTexCoord2f(0,4);  glVertex3f(-s, 0.05f, ROAD_MAIN);
    glEnd();
    
    for(int i = -6; i <= 6; i++){
        float rPos = i * BLOCK_STRIDE;
        if(fabsf(rPos) < 1.0f) continue; 
        
        glBegin(GL_QUADS); glNormal3f(0,1,0);
        glTexCoord2f(0,0);  glVertex3f(rPos - ROAD_SEC*0.5f, 0.04f, -s);  glTexCoord2f(3,0);  glVertex3f(rPos + ROAD_SEC*0.5f, 0.04f, -s);
        glTexCoord2f(3,50); glVertex3f(rPos + ROAD_SEC*0.5f, 0.04f, s);   glTexCoord2f(0,50); glVertex3f(rPos - ROAD_SEC*0.5f, 0.04f, s);
        glEnd();
        glBegin(GL_QUADS); glNormal3f(0,1,0);
        glTexCoord2f(0,0);  glVertex3f(-s, 0.04f, rPos - ROAD_SEC*0.5f);  glTexCoord2f(50,0); glVertex3f(s, 0.04f, rPos - ROAD_SEC*0.5f);
        glTexCoord2f(50,3); glVertex3f(s, 0.04f, rPos + ROAD_SEC*0.5f);   glTexCoord2f(0,3);  glVertex3f(-s, 0.04f, rPos + ROAD_SEC*0.5f);
        glEnd();
    }
    glDisable(GL_TEXTURE_2D);
    
    GLfloat lm[]={0.9f,0.9f,0.9f,1.f}; glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE,lm);
    glBegin(GL_QUADS);
    for(float z=-s;z<s;z+=12.f){
        glVertex3f(-0.2f,0.09f,z);   glVertex3f(0.2f,0.09f,z);   glVertex3f(0.2f,0.09f,z+6.f); glVertex3f(-0.2f,0.09f,z+6.f);
        glVertex3f(z,0.09f,-0.2f);   glVertex3f(z+6.f,0.09f,-0.2f); glVertex3f(z+6.f,0.09f,0.2f); glVertex3f(z,0.09f,0.2f);
    }
    glEnd();
    
    GLfloat sw[]={0.55f,0.55f,0.52f,1.f}; glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE,sw);
    for(int i = -6; i <= 6; i++){
        float rPos = i * BLOCK_STRIDE;
        if(fabsf(rPos) < 1.0f) continue;
        glBegin(GL_QUADS); glNormal3f(0,1,0);
        glVertex3f(rPos + ROAD_SEC*0.5f,       0.025f, -s); glVertex3f(rPos + ROAD_SEC*0.5f + 1.2f, 0.025f, -s);
        glVertex3f(rPos + ROAD_SEC*0.5f + 1.2f, 0.025f,  s); glVertex3f(rPos + ROAD_SEC*0.5f,       0.025f,  s); glEnd();
        glBegin(GL_QUADS); glNormal3f(0,1,0);
        glVertex3f(rPos - ROAD_SEC*0.5f - 1.2f, 0.025f, -s); glVertex3f(rPos - ROAD_SEC*0.5f,       0.025f, -s);
        glVertex3f(rPos - ROAD_SEC*0.5f,       0.025f,  s); glVertex3f(rPos - ROAD_SEC*0.5f - 1.2f, 0.025f,  s); glEnd();
    }
}

void renderSky(){
    glDisable(GL_LIGHTING); glDisable(GL_DEPTH_TEST);
    float s=MAP_SIZE*2.f, h=MAP_SIZE;
    glBegin(GL_QUADS); glColor3f(0.25f,0.52f,0.95f);
    glVertex3f(-s,h,-s); glVertex3f(s,h,-s); glVertex3f(s,h,s); glVertex3f(-s,h,s); glEnd();
    float hc[3]={0.62f,0.80f,0.97f}, sc[3]={0.25f,0.52f,0.95f};
    float sx[5]={-s,s,s,-s,-s}, sz[5]={-s,-s,s,s,-s};
    for(int i=0;i<4;i++){
        glBegin(GL_QUADS);
        glColor3fv(hc); glVertex3f(sx[i],-2,sz[i]);   glVertex3f(sx[i+1],-2,sz[i+1]);
        glColor3fv(sc); glVertex3f(sx[i+1],h,sz[i+1]); glVertex3f(sx[i],h,sz[i]); glEnd();
    }
    glColor3f(1.f,0.95f,0.6f);
    glPushMatrix(); glTranslatef(80,120,-180); glutSolidSphere(12,16,12); glPopMatrix();
    glColor3f(1.f,1.f,1.f);
    float clouds[5][3]={{60,60,-80},{-50,70,30},{20,80,100},{-80,65,-40},{0,55,-120}};
    for(int i=0;i<5;i++){
        glPushMatrix(); glTranslatef(clouds[i][0],clouds[i][1],clouds[i][2]);
        glutSolidSphere(10,8,6); glTranslatef(8,0,0); glutSolidSphere(8,8,6); glTranslatef(-16,0,0); glutSolidSphere(7,8,6);
        glPopMatrix();
    }
    glEnable(GL_DEPTH_TEST); glEnable(GL_LIGHTING); glColor3f(1,1,1);
}

void renderBullets(){
    glDisable(GL_LIGHTING);
    for(int i=0;i<BULLET_MAX;i++){
        if(!bullets[i].active) continue;
        if(bullets[i].fromPlayer) glColor3f(1.f,0.95f,0.f); else glColor3f(1.f,0.1f,0.f);
        for(int step=0;step<4;step++){
            glPushMatrix();
            Vec3 tp=bullets[i].pos-vnorm(bullets[i].vel)*(step*1.5f);
            glTranslatef(tp.x,tp.y,tp.z);
            glutSolidSphere(0.5f-step*0.1f,8,6);
            glPopMatrix();
        }
    }
    glEnable(GL_LIGHTING);
}

void renderMissiles(){
    glDisable(GL_LIGHTING);
    for(int i=0;i<MISSILE_MAX;i++){
        if(!missiles[i].active) continue;
        Missile& m=missiles[i];
        glPushMatrix(); glTranslatef(m.pos.x,m.pos.y,m.pos.z);
        glColor3f(0.95f,0.95f,0.95f); glutSolidSphere(0.22f,6,4);
        glColor3f(1.f,0.4f,0.f);
        Vec3 nd=vnorm(m.vel);
        glBegin(GL_LINES); glVertex3f(0,0,0); glVertex3f(-nd.x*1.2f,-nd.y*1.2f,-nd.z*1.2f); glEnd();
        glPopMatrix();
    }
    glEnable(GL_LIGHTING);
}

void renderParticles(){
    glDisable(GL_LIGHTING); glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA); glDepthMask(GL_FALSE);
    for(int i=0;i<PARTICLE_MAX;i++){
        if(!particles[i].active) continue;
        Particle& p=particles[i];
        glColor4f(p.r,p.g,p.b,p.a);
        glPushMatrix(); glTranslatef(p.pos.x,p.pos.y,p.pos.z);
        glutSolidSphere(p.size,5,3); glPopMatrix();
    }
    glDepthMask(GL_TRUE); glDisable(GL_BLEND); glEnable(GL_LIGHTING);
}

void renderText2D(float x, float y, const char* str, float r,float g,float b){
    glColor3f(r,g,b); glRasterPos2f(x,y);
    for(int i=0;str[i];i++) glutBitmapCharacter(GLUT_BITMAP_9_BY_15,str[i]);
}

void renderHUD(){
    int W=glutGet(GLUT_WINDOW_WIDTH), H=glutGet(GLUT_WINDOW_HEIGHT);
    glMatrixMode(GL_PROJECTION); glPushMatrix(); glLoadIdentity();
    gluOrtho2D(0,W,0,H);
    glMatrixMode(GL_MODELVIEW); glPushMatrix(); glLoadIdentity();
    glDisable(GL_LIGHTING); glDisable(GL_DEPTH_TEST);

    char buf[128];
    float hp=player.health/100.f;
    glColor4f(0.2f,0,0,0.7f);
    glBegin(GL_QUADS); glVertex2f(10,H-30); glVertex2f(210,H-30); glVertex2f(210,H-10); glVertex2f(10,H-10); glEnd();
    glColor4f(0.1f+0.9f*(1-hp),hp*0.9f,0.f,0.85f);
    glBegin(GL_QUADS); glVertex2f(10,H-30); glVertex2f(10+200*hp,H-30); glVertex2f(10+200*hp,H-10); glVertex2f(10,H-10); glEnd();
    sprintf(buf,"HP: %.0f%%",player.health); renderText2D(15,H-25,buf);

    sprintf(buf,"SCORE: %d",score); renderText2D(W-170,H-20,buf,1,1,0);
    sprintf(buf,"WAVE: %d / %d",currentWave,TOTAL_WAVES); renderText2D(W-170,H-40,buf,1,0.5f,0);
    sprintf(buf,"KILLS: %d / 15  (wave: %d/%d)",enemiesKilled,killsThisWave,KILLS_PER_WAVE);
    renderText2D(W-260,H-60,buf,0.4f,1.f,0.4f);
    sprintf(buf,"TIME: %.1fs",gameTime); renderText2D(W-170,H-80,buf,0.8f,0.8f,1);

    float wkProg=fminf(1.f,(float)killsThisWave/KILLS_PER_WAVE);
    glColor4f(0.1f,0.1f,0.1f,0.6f);
    glBegin(GL_QUADS); glVertex2f(W-170,H-100); glVertex2f(W-10,H-100); glVertex2f(W-10,H-92); glVertex2f(W-170,H-92); glEnd();
    glColor4f(0.2f+0.8f*wkProg,1.f-0.7f*wkProg,0.f,0.85f);
    glBegin(GL_QUADS); glVertex2f(W-170,H-100); glVertex2f(W-170+160*wkProg,H-100); glVertex2f(W-170+160*wkProg,H-92); glVertex2f(W-170,H-92); glEnd();

    const char* camNames[]={"3rd Person Follow","Free Orbit"};
    sprintf(buf,"CAM: %s [C]",camNames[camMode]); renderText2D(10,H-55,buf,0.7f,0.9f,1);
    sprintf(buf,"ALT: %.1fm",player.pos.y); renderText2D(10,H-75,buf,0.6f,1,0.8f);
    float spd=vlen(player.vel); sprintf(buf,"SPD: %.1fm/s",spd); renderText2D(10,H-95,buf,0.6f,1,0.8f);
    if(aimAssistOn) renderText2D(10,H-115,"AIM ASSIST: ON  [T]",0.f,1.f,0.5f);
    else            renderText2D(10,H-115,"AIM ASSIST: OFF [T]",0.5f,0.5f,0.5f);

    renderText2D(10,116,"Z/V       : Gas/Rem Pesawat",0.6f,0.6f,0.6f);
    renderText2D(10,98, "WASD      : Gerak Pesawat",  0.6f,0.6f,0.6f);
    renderText2D(10,80, "QE        : Roll Kiri/Kanan",0.6f,0.6f,0.6f);
    renderText2D(10,62, "KLIK/SPASI: Tembak ke Crosshair",0.6f,0.6f,0.6f);
    renderText2D(10,44, "MOUSE GERAK: Arahkan Bidikan",0.8f,0.8f,0.f);
    renderText2D(10,26, "F=Lock  X=Rudal  T/C/P=Aim/Cam/Pause",0.6f,0.6f,0.6f);

    if(lockedEnemy>=0&&enemies[lockedEnemy].alive){
        glColor4f(1.f,0.1f,0.1f,0.85f);
        glBegin(GL_QUADS); glVertex2f(W/2-70,H-45); glVertex2f(W/2+70,H-45); glVertex2f(W/2+70,H-28); glVertex2f(W/2-70,H-28); glEnd();
        renderText2D(W/2-60,H-41,"[ F: LOCKED ON ] X=RUDAL",1.f,1.f,0.f);
    } else {
        glColor4f(0.f,0.f,0.f,0.45f);
        glBegin(GL_QUADS); glVertex2f(W/2-60,H-45); glVertex2f(W/2+60,H-45); glVertex2f(W/2+60,H-28); glVertex2f(W/2-60,H-28); glEnd();
        renderText2D(W/2-50,H-41,"[ F: Lock-On Target ]",0.5f,0.8f,0.5f);
    }

    int mby=H-140; renderText2D(10,mby+3,"RUDAL:",1.f,0.7f,0.f);
    for(int i=0;i<5;i++){
        if(i<missileAmmo) glColor4f(1.f,0.5f,0.f,0.9f); else glColor4f(0.3f,0.3f,0.3f,0.6f);
        float bx=55+i*18.f;
        glBegin(GL_QUADS); glVertex2f(bx,mby); glVertex2f(bx+14,mby); glVertex2f(bx+14,mby+12); glVertex2f(bx,mby+12); glEnd();
    }
    if(missileAmmo<5&&missileReload>0){
        float prog=1.f-(missileReload/MISSILE_RELOAD_TIME);
        glColor4f(0.f,0.8f,1.f,0.7f);
        glBegin(GL_QUADS); glVertex2f(55,mby-6); glVertex2f(55+90*prog,mby-6); glVertex2f(55+90*prog,mby-2); glVertex2f(55,mby-2); glEnd();
    }

    sprintf(buf,"WAVE DIFFICULTY: x%.1f",enemySpeedMult); renderText2D(W-230,H-120,buf,1.f,0.5f,1.f);

    float cxf=crossWorldX, cyf=crossWorldY;
    if(cxf<20) cxf=20; if(cxf>W-20) cxf=(float)(W-20);
    if(cyf<20) cyf=20; if(cyf>H-20) cyf=(float)(H-20);

    int aimEnemy=-1; float aimDist=9999.f;
    for(int i=0;i<ENEMY_COUNT;i++){
        if(!enemies[i].alive) continue;
        float ex,ey;
        if(worldToScreen(enemies[i].pos,W,H,ex,ey)){
            float d=sqrtf((cxf-ex)*(cxf-ex)+(cyf-ey)*(cyf-ey));
            if(d<40.f&&d<aimDist){aimDist=d;aimEnemy=i;}
        }
    }
    float crR=0.9f,crG=0.9f,crB=0.9f,crA=0.95f;
    if(aimEnemy>=0){crR=1.f;crG=0.15f;crB=0.15f;}
    else if(lockedEnemy>=0&&enemies[lockedEnemy].alive){crR=0.f;crG=1.f;crB=0.3f;}

    glLineWidth(2.f); glColor4f(crR,crG,crB,crA);
    float csz=20.f,gap=7.f;
    glBegin(GL_LINES);
    glVertex2f(cxf-csz,cyf); glVertex2f(cxf-gap,cyf);
    glVertex2f(cxf+gap,cyf); glVertex2f(cxf+csz,cyf);
    glVertex2f(cxf,cyf-csz); glVertex2f(cxf,cyf-gap);
    glVertex2f(cxf,cyf+gap); glVertex2f(cxf,cyf+csz);
    glEnd();
    glBegin(GL_LINE_LOOP);
    for(int a=0;a<20;a++){
        float ang=a*2.f*PI/20.f; glVertex2f(cxf+cosf(ang)*5.f,cyf+sinf(ang)*5.f);
    }
    glEnd();

    for(int i=0;i<ENEMY_COUNT;i++){
        if(!enemies[i].alive) continue;
        Vec3 lead=calcLeadPoint(i);
        float lsx,lsy; if(!worldToScreen(lead,W,H,lsx,lsy)) continue;
        float ex2,ey2; if(!worldToScreen(enemies[i].pos,W,H,ex2,ey2)) continue;
        float ds=5.f; glColor4f(1.f,0.8f,0.f,0.85f);
        glBegin(GL_LINE_LOOP);
        glVertex2f(lsx,lsy+ds); glVertex2f(lsx+ds,lsy); glVertex2f(lsx,lsy-ds); glVertex2f(lsx-ds,lsy);
        glEnd();
        glColor4f(1.f,0.8f,0.f,0.3f); glLineWidth(1.f);
        glBegin(GL_LINES); glVertex2f(ex2,ey2); glVertex2f(lsx,lsy); glEnd();
        glLineWidth(2.f);
    }

    for(int i=0;i<ENEMY_COUNT;i++){
        if(!enemies[i].alive) continue;
        float ex,ey; if(!worldToScreen(enemies[i].pos,W,H,ex,ey)) continue;
        float dist3d=vlen(enemies[i].pos-player.pos);
        float rs=fmaxf(14.f,60.f-dist3d*0.3f), gap2=rs*0.35f;
        bool isLocked=(lockedEnemy==i);
        if(isLocked){glColor4f(1.f,0.2f,0.2f,0.9f);glLineWidth(2.5f);}
        else         {glColor4f(0.9f,0.7f,0.f,0.55f);glLineWidth(1.2f);}
        glBegin(GL_LINES);
        glVertex2f(ex-rs,ey-rs+gap2);glVertex2f(ex-rs,ey-rs);glVertex2f(ex-rs,ey-rs);glVertex2f(ex-rs+gap2,ey-rs);
        glVertex2f(ex+rs-gap2,ey-rs);glVertex2f(ex+rs,ey-rs);glVertex2f(ex+rs,ey-rs);glVertex2f(ex+rs,ey-rs+gap2);
        glVertex2f(ex-rs,ey+rs-gap2);glVertex2f(ex-rs,ey+rs);glVertex2f(ex-rs,ey+rs);glVertex2f(ex-rs+gap2,ey+rs);
        glVertex2f(ex+rs-gap2,ey+rs);glVertex2f(ex+rs,ey+rs);glVertex2f(ex+rs,ey+rs);glVertex2f(ex+rs,ey+rs-gap2);
        glEnd();
        float ehp=enemies[i].health/50.f;
        glColor4f(0.2f,0.f,0.f,0.7f);
        glBegin(GL_QUADS);glVertex2f(ex-rs,ey+rs+5);glVertex2f(ex+rs,ey+rs+5);glVertex2f(ex+rs,ey+rs+12);glVertex2f(ex-rs,ey+rs+12);glEnd();
        glColor4f(isLocked?1.f:0.8f,0.15f,0.15f,0.9f);
        glBegin(GL_QUADS);glVertex2f(ex-rs,ey+rs+5);glVertex2f(ex-rs+rs*2*ehp,ey+rs+5);glVertex2f(ex-rs+rs*2*ehp,ey+rs+12);glVertex2f(ex-rs,ey+rs+12);glEnd();
        char db[32]; sprintf(db,"%.0fm",dist3d);
        renderText2D(ex+rs+4,ey-4,db,isLocked?1.f:0.8f,isLocked?0.4f:0.7f,isLocked?0.4f:0.f);
    }
    glLineWidth(1.5f);

    renderText2D(10,H-135,"MOUSE: Geser Bidikan",0.7f,0.9f,1.f);
    renderText2D(10,H-155,aimEnemy>=0?"[KLIK] TEMBAK TARGET!":"[KLIK] Tembak",aimEnemy>=0?1.f:0.6f,aimEnemy>=0?0.2f:0.6f,0.3f);

    if(waveClearPopup){
        float alpha=fminf(1.f,waveClearTimer/1.5f);
        if(waveClearTimer<1.f) alpha=waveClearTimer;
        int bw=360, bh=50;
        int bx=W/2-bw/2, by=H/2+40;
        glColor4f(0.f,0.f,0.f,alpha*0.75f);
        glBegin(GL_QUADS);glVertex2f(bx,by);glVertex2f(bx+bw,by);glVertex2f(bx+bw,by+bh);glVertex2f(bx,by+bh);glEnd();
        glColor4f(0.2f,0.7f,1.f,alpha*0.5f);
        glBegin(GL_LINE_LOOP);glVertex2f(bx,by);glVertex2f(bx+bw,by);glVertex2f(bx+bw,by+bh);glVertex2f(bx,by+bh);glEnd();
        glColor3f(1.f,1.f,0.f);
        renderText2D(W/2-(float)strlen(waveClearMsg)*4.5f, by+18, waveClearMsg, 1.f,1.f,0.f);

        if(waveTransition){
            float prog=1.f-waveTransitionTimer/WAVE_PAUSE;
            glColor4f(0.2f,0.7f,1.f,alpha*0.8f);
            glBegin(GL_QUADS);
            glVertex2f(bx+10,by-14); glVertex2f(bx+10+(bw-20)*prog,by-14);
            glVertex2f(bx+10+(bw-20)*prog,by-6); glVertex2f(bx+10,by-6); glEnd();
            glColor4f(0.5f,0.5f,0.5f,alpha*0.5f);
            glBegin(GL_LINE_LOOP);
            glVertex2f(bx+10,by-14);glVertex2f(bx+bw-10,by-14);glVertex2f(bx+bw-10,by-6);glVertex2f(bx+10,by-6);glEnd();
        }
    }

    if(gamePaused){
        glColor4f(0,0,0,0.55f); glBegin(GL_QUADS); glVertex2i(0,0);glVertex2i(W,0);glVertex2i(W,H);glVertex2i(0,H); glEnd();
        renderText2D(W/2-50,H/2+10,"[PAUSED]",1,1,0);
        renderText2D(W/2-60,H/2-10,"Tekan P untuk lanjut",0.8f,0.8f,0.8f);
    }

    if(gameOver){
        glColor4f(0,0,0,0.75f); glBegin(GL_QUADS); glVertex2i(0,0);glVertex2i(W,0);glVertex2i(W,H);glVertex2i(0,H); glEnd();
        int px=W/2-200, py=H/2-130;
        glColor4f(0.08f,0.05f,0.12f,0.95f); glBegin(GL_QUADS); glVertex2i(px,py);glVertex2i(px+400,py);glVertex2i(px+400,py+260);glVertex2i(px,py+260); glEnd();
        glColor4f(0.6f,0.1f,0.1f,1.f); glBegin(GL_LINE_LOOP); glVertex2i(px,py);glVertex2i(px+400,py);glVertex2i(px+400,py+260);glVertex2i(px,py+260); glEnd();
        renderText2D(W/2-55,py+230,"=== GAME OVER ===",1.f,0.15f,0.15f);
        sprintf(buf,"Waktu Bertahan : %02d:%02d",(int)(gameTime/60),(int)gameTime%60); renderText2D(px+20,py+195,buf,1.f,1.f,0.4f);
        renderText2D(px+20,py+168,"--- Rincian Skor ---",0.7f,0.7f,1.f);
        sprintf(buf,"Kill Musuh     : +%d  (%d kills)",scoreFromKills,enemiesKilled); renderText2D(px+20,py+148,buf,0.9f,0.9f,0.9f);
        sprintf(buf,"Selesai Wave   : +%d  (wave %d/%d)",scoreFromWaves,currentWave-1,TOTAL_WAVES); renderText2D(px+20,py+128,buf,0.9f,0.9f,0.9f);
        sprintf(buf,"Hancurkan Gedung: +%d",scoreFromBldg); renderText2D(px+20,py+108,buf,0.9f,0.9f,0.9f);
        sprintf(buf,"TOTAL SKOR     : %d",score); renderText2D(px+20,py+75,buf,1.f,0.85f,0.f);
        renderText2D(px+90,py+8,"Tekan R untuk restart",0.6f,0.6f,0.6f);
    }

    if(gameWon){
        glColor4f(0,0,0,0.75f); glBegin(GL_QUADS); glVertex2i(0,0);glVertex2i(W,0);glVertex2i(W,H);glVertex2i(0,H); glEnd();
        int px=W/2-220, py=H/2-145;
        glColor4f(0.04f,0.10f,0.06f,0.97f);
        glBegin(GL_QUADS);glVertex2i(px,py);glVertex2i(px+440,py);glVertex2i(px+440,py+290);glVertex2i(px,py+290);glEnd();
        glLineWidth(2.5f); glColor4f(1.f,0.85f,0.f,1.f);
        glBegin(GL_LINE_LOOP);glVertex2i(px,py);glVertex2i(px+440,py);glVertex2i(px+440,py+290);glVertex2i(px,py+290);glEnd();
        glLineWidth(1.5f);

        renderText2D(W/2-80,py+260,"=== MISSION COMPLETE ===",1.f,0.9f,0.f);
        renderText2D(W/2-55,py+235,"  *   *   *  ",1.f,1.f,0.f);
        renderText2D(W/2-75,py+218," *** *** *** ",1.f,1.f,0.f);
        renderText2D(W/2-55,py+201,"  *   *   *  ",1.f,1.f,0.f);

        renderText2D(px+30,py+178,"Semua 3 wave berhasil diselesaikan!",0.4f,1.f,0.5f);
        sprintf(buf,"Waktu: %02d:%02d",(int)(gameTime/60),(int)gameTime%60);
        renderText2D(px+30,py+155,buf,0.8f,0.9f,1.f);

        glColor4f(0.3f,0.3f,0.6f,0.6f);
        glBegin(GL_QUADS);glVertex2f(px+20,py+100);glVertex2f(px+420,py+100);glVertex2f(px+420,py+145);glVertex2f(px+20,py+145);glEnd();
        renderText2D(px+30,py+130,"--- Rincian Skor ---",0.7f,0.8f,1.f);
        sprintf(buf,"Kill Musuh (15/15): +%d",scoreFromKills); renderText2D(px+30,py+112,buf,0.9f,0.9f,0.9f);
        sprintf(buf,"Selesai Wave (3/3): +%d",scoreFromWaves); renderText2D(px+30,py+96,buf,0.9f,0.9f,0.9f);
        sprintf(buf,"Hancurkan Gedung  : +%d",scoreFromBldg);  renderText2D(px+30,py+80,buf,0.9f,0.9f,0.9f);

        glColor4f(1.f,0.85f,0.f,0.25f);
        glBegin(GL_QUADS);glVertex2f(px+20,py+55);glVertex2f(px+420,py+55);glVertex2f(px+420,py+74);glVertex2f(px+20,py+74);glEnd();
        sprintf(buf,"TOTAL SKOR: %d",score); renderText2D(px+30,py+60,buf,1.f,1.f,0.f);

        renderText2D(px+100,py+10,"Tekan R untuk main lagi",0.6f,0.9f,0.6f);
    }

    glEnable(GL_DEPTH_TEST); glEnable(GL_LIGHTING);
    glMatrixMode(GL_PROJECTION); glPopMatrix();
    glMatrixMode(GL_MODELVIEW);  glPopMatrix();
}

void setupCamera(){
    float yr=player.yaw*PI/180.f, pr=player.pitch*PI/180.f;
    Vec3 fwd={sinf(yr)*cosf(pr),-sinf(pr),cosf(yr)*cosf(pr)};
    if(camMode==0){
        Vec3 cp=player.pos-fwd*12.f+vec3(0,4,0);
        Vec3 la=player.pos+fwd*3.f;
        gluLookAt(cp.x,cp.y,cp.z,la.x,la.y,la.z,0,1,0);
    } else {
        float oy=camOrbitYaw*PI/180.f, op=camOrbitPitch*PI/180.f;
        float cx_=player.pos.x+sinf(oy)*cosf(op)*camOrbitDist;
        float cy_=player.pos.y+sinf(op)*camOrbitDist;
        float cz_=player.pos.z+cosf(oy)*cosf(op)*camOrbitDist;
        gluLookAt(cx_,cy_,cz_,player.pos.x,player.pos.y,player.pos.z,0,1,0);
    }
}

void setupTextures(){
    const int ts=64;
    unsigned char td[ts][ts][3], rd[ts][ts][3];
    for(int i=0;i<ts;i++) for(int j=0;j<ts;j++){
        int n=rand()%30;
        td[i][j][0]=75+n; td[i][j][1]=115+n; td[i][j][2]=40+n/2;
        int a=45+rand()%20; rd[i][j][0]=rd[i][j][1]=rd[i][j][2]=a;
    }
    glGenTextures(1,&terrainTex); glBindTexture(GL_TEXTURE_2D,terrainTex);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,ts,ts,0,GL_RGB,GL_UNSIGNED_BYTE,td);
    glGenTextures(1,&roadTex); glBindTexture(GL_TEXTURE_2D,roadTex);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,ts,ts,0,GL_RGB,GL_UNSIGNED_BYTE,rd);
}

void initGL(){
    glClearColor(0.25f,0.52f,0.95f,1.f);
    glEnable(GL_DEPTH_TEST); glDepthFunc(GL_LEQUAL);
    glEnable(GL_LIGHTING); glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0,GL_AMBIENT,ambLight);
    glLightfv(GL_LIGHT0,GL_DIFFUSE,difLight);
    glLightfv(GL_LIGHT0,GL_SPECULAR,spcLight);
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,GL_FALSE);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_FOG); glFogi(GL_FOG_MODE,GL_LINEAR);
    glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    setupTextures();
    buildDisplayLists();
}
