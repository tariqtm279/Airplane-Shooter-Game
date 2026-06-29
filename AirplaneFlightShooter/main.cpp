#include "Config.h"
#include "GameState.h"
#include "Graphics.h"
#include "City.h"

void reshape(int w, int h){ if(h==0)h=1; glViewport(0,0,w,h); }

void display(){
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    int W=glutGet(GLUT_WINDOW_WIDTH), H=glutGet(GLUT_WINDOW_HEIGHT);
    glMatrixMode(GL_PROJECTION); glLoadIdentity();
    gluPerspective(60.0,(double)W/H,0.3,1000.0);
    glMatrixMode(GL_MODELVIEW); glLoadIdentity();
    setupCamera();
    glLightfv(GL_LIGHT0,GL_POSITION,sunDir);
    GLfloat fc[]={0.62f,0.80f,0.97f,1.f};
    glFogfv(GL_FOG_COLOR,fc); glFogf(GL_FOG_START,80.f); glFogf(GL_FOG_END,350.f);
    
    renderSky();
    renderTerrain();
    for(int i=0;i<BUILDING_COUNT;i++) renderBuilding(i);
    for(int i=0;i<TREE_COUNT;i++)     renderTree(trees[i]);
    
    renderShadow(player);
    for(int i=0;i<ENEMY_COUNT;i++) renderShadow(enemies[i]);
    
    renderPlane(player,true,false);
    for(int i=0;i<ENEMY_COUNT;i++) renderPlane(enemies[i],false,true);
    
    renderBullets();
    renderMissiles();
    renderParticles();
    renderHUD();
    glutSwapBuffers();
}

void keyDown(unsigned char k, int, int){
    keyState[(unsigned char)k]=true;
    if(k==27) exit(0);
    if(k=='p'||k=='P') gamePaused=!gamePaused;
    if(k=='c'||k=='C') camMode=(camMode+1)%2;
    if(k=='t'||k=='T') aimAssistOn=!aimAssistOn;
    if(k=='f'||k=='F'){
        int best = findBestLockTarget();
        lockedEnemy = (best>=0)?best:-1;
    }
    if(k=='x'||k=='X') fireMissile();
    if((k=='r'||k=='R')&&(gameOver||gameWon)){
        gameOver=false; gameWon=false;
        score=0; enemiesKilled=0; gameTime=0;
        scoreFromKills=0; scoreFromWaves=0; scoreFromBldg=0;
        craterCount=0; lockedEnemy=-1; lockOnTimer=0;
        currentWave=1; killsThisWave=0;
        enemySpeedMult=1.f; enemyBulletDmg=18.f;
        missileAmmo=5; missileReload=0.f;
        waveClearPopup=false; waveTransition=false;
        initPlayer(); initBuildings(); initTrees(); initEnemies(); initBullets(); initParticles();
    }
}

void keyUp(unsigned char k, int, int){ keyState[(unsigned char)k]=false; }
void specialDown(int k, int, int){ specState[k]=true; }
void specialUp(int k, int, int){ specState[k]=false; }

void mouseClick(int btn, int state, int x, int y){
    if(btn==GLUT_LEFT_BUTTON){
        mouseBtn[0]=(state==GLUT_DOWN)?1:0;
        if(state==GLUT_DOWN&&!gameOver&&!gameWon&&!gamePaused&&player.alive&&player.shootCooldown<=0){
            fireBullet(player,true); player.shootCooldown=0.18f;
        }
    }
    if(btn==GLUT_RIGHT_BUTTON) mouseBtn[1]=(state==GLUT_DOWN)?1:0;
    if(btn==3&&state==GLUT_DOWN){ camOrbitDist-=2.f; if(camOrbitDist<5) camOrbitDist=5; }
    if(btn==4&&state==GLUT_DOWN){ camOrbitDist+=2.f; if(camOrbitDist>60) camOrbitDist=60; }
    lastMouseX=x; lastMouseY=y;
}

void mouseMotion(int x, int y){
    int H=glutGet(GLUT_WINDOW_HEIGHT);
    if(camMode==1&&mouseBtn[1]){
        camOrbitYaw  +=(x-lastMouseX)*0.5f;
        camOrbitPitch-=(y-lastMouseY)*0.5f;
        if(camOrbitPitch>80) camOrbitPitch=80;
        if(camOrbitPitch<5)  camOrbitPitch=5;
    }
    crossWorldX=(float)x; crossWorldY=(float)(H-y);
    lastMouseX=x; lastMouseY=y;
}

void mousePassive(int x, int y){
    int H=glutGet(GLUT_WINDOW_HEIGHT);
    crossWorldX=(float)x; crossWorldY=(float)(H-y);
    lastMouseX=x; lastMouseY=y;
}

void update(int val){
    if(!gamePaused && !gameOver && !gameWon){
        int now = glutGet(GLUT_ELAPSED_TIME);
        dt = (now - lastTime) * 0.001f; 
        if(dt > 0.05f) dt = 0.05f;
        lastTime = now; 
        gameTime += dt; 
        propAngle += 720.f * dt;

        updateGameLogic();
    }
    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}

int main(int argc, char** argv){
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB|GLUT_DEPTH);
    glutInitWindowSize(1024,768);
    glutCreateWindow("Airplane City Shooter - OpenGL GLUT");
    
    initGL();
    initBuildings(); 
    initTrees(); 
    initPlayer(); 
    initEnemies();
    initBullets(); 
    initParticles();
    
    crossWorldX=512.f; crossWorldY=384.f;
    
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyDown); 
    glutKeyboardUpFunc(keyUp);
    glutSpecialFunc(specialDown); 
    glutSpecialUpFunc(specialUp);
    glutMouseFunc(mouseClick); 
    glutMotionFunc(mouseMotion); 
    glutPassiveMotionFunc(mousePassive);
    
    lastTime=glutGet(GLUT_ELAPSED_TIME);
    glutTimerFunc(16,update,0);
    glutMainLoop();
    return 0;
}
