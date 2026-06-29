#include "GameState.h"
#include "Config.h"
#include "City.h"
#include <ctime>
#include <cstdio>
#include <cstring>

Plane     player;
Plane     enemies[ENEMY_COUNT];
Bullet    bullets[BULLET_MAX];
Missile   missiles[MISSILE_MAX];
Particle  particles[PARTICLE_MAX];
Crater    craters[CRATER_MAX];

int   craterCount = 0, score = 0, enemiesKilled = 0;
int   scoreFromKills = 0, scoreFromWaves = 0, scoreFromBldg = 0;
float gameTime = 0.f, dt = 0.016f;
int   lastTime = 0;
bool  gameOver = false, gameWon = false, gamePaused = false;

int   currentWave = 1;
int   killsThisWave = 0;
bool  waveClearPopup = false;
float waveClearTimer = 0.f;
char  waveClearMsg[64] = "";
bool  waveTransition = false;
float waveTransitionTimer = 0.f;

int   missileAmmo = 5;
float missileReload = 0.f;
float enemyBulletDmg = 18.f;
float enemySpeedMult = 1.0f;

int   camMode = 0; 
float camOrbitYaw = 0.f, camOrbitPitch = 20.f, camOrbitDist = 18.f;
bool  keyState[256] = {}, specState[256] = {};
int   mouseBtn[3] = {}, lastMouseX, lastMouseY;

float propAngle = 0.f, lockOnTimer = 0.f;
int   lockedEnemy = -1;
bool  aimAssistOn = true;
float crossWorldX = 0.f, crossWorldY = 0.f;

void initPlayer(){
    player.pos={0,12,-30}; player.vel={0,0,0};
    player.yaw=0; player.pitch=0; player.roll=0;
    player.health=100.f; player.alive=true; player.shootCooldown=0;
}

void initEnemies(){
    for(int i=0;i<ENEMY_COUNT;i++){
        enemies[i].pos={randf(-80,80),randf(10,30),randf(-80,80)};
        enemies[i].vel={0,0,0};
        enemies[i].yaw=randf(0,360); enemies[i].pitch=0; enemies[i].roll=0;
        enemies[i].health=50.f; enemies[i].alive=true;
        enemies[i].shootCooldown=randf(0.5f,3.f);
        float angle=randf(0.f,2.f*PI), dist_=randf(30.f,70.f);
        enemies[i].aiTarget=vec3(enemies[i].pos.x+cosf(angle)*dist_, randf(8.f,30.f), enemies[i].pos.z+sinf(angle)*dist_);
        enemies[i].aiTarget.x=fmaxf(-MAP_SIZE*0.8f,fminf(MAP_SIZE*0.8f,enemies[i].aiTarget.x));
        enemies[i].aiTarget.z=fmaxf(-MAP_SIZE*0.8f,fminf(MAP_SIZE*0.8f,enemies[i].aiTarget.z));
        enemies[i].aiTimer=randf(1.f,5.f);
    }
}

void initBullets(){
    for(int i=0;i<BULLET_MAX;i++) bullets[i].active=false;
    for(int i=0;i<MISSILE_MAX;i++) missiles[i].active=false;
}

void initParticles(){
    for(int i=0;i<PARTICLE_MAX;i++) particles[i].active=false;
}

void spawnParticles(Vec3 pos, int n, bool fire){
    for(int i=0;i<PARTICLE_MAX&&n>0;i++){
        if(!particles[i].active){
            particles[i].active=true; particles[i].isFire=fire;
            particles[i].pos=pos;
            particles[i].vel=vec3(randf(-4,4),randf(1,8),randf(-4,4));
            if(fire){
                particles[i].r=randf(0.8f,1.f); particles[i].g=randf(0.1f,0.5f); particles[i].b=0.f;
                particles[i].size=randf(0.3f,0.9f); particles[i].life=randf(0.4f,1.2f);
            } else {
                particles[i].r=particles[i].g=particles[i].b=randf(0.5f,0.9f);
                particles[i].size=randf(0.1f,0.4f); particles[i].life=randf(0.3f,0.8f);
            }
            particles[i].a=1.f; n--;
        }
    }
}

void addCrater(Vec3 pos, int buildingIdx, bool onFire){
    if(craterCount>=CRATER_MAX) return;
    craters[craterCount]={pos, randf(0.4f,0.9f), onFire?randf(3.f,8.f):0.f, buildingIdx, onFire};
    craterCount++;
}

int findBestLockTarget(){
    int W=glutGet(GLUT_WINDOW_WIDTH), H=glutGet(GLUT_WINDOW_HEIGHT);
    float yr=player.yaw*PI/180.f, pr=player.pitch*PI/180.f;
    Vec3 fwd={sinf(yr)*cosf(pr),-sinf(pr),cosf(yr)*cosf(pr)};
    int bestIdx=-1; float bestScore=0.f;
    for(int i=0;i<ENEMY_COUNT;i++){
        if(!enemies[i].alive) continue;
        Vec3 diff=enemies[i].pos-player.pos;
        float dist=vlen(diff); if(dist>150.f) continue;
        float dot=vdot(vnorm(diff),fwd); if(dot<0.3f) continue;
        float sb=1.f; float ex,ey;
        if(worldToScreen(enemies[i].pos,W,H,ex,ey)){
            float sd=sqrtf((crossWorldX-ex)*(crossWorldX-ex)+(crossWorldY-ey)*(crossWorldY-ey));
            sb=1.f+fmaxf(0.f,1.f-sd/200.f)*3.f;
        }
        float s=dot*sb/(dist+1.f);
        if(s>bestScore){bestScore=s;bestIdx=i;}
    }
    return bestIdx;
}

Vec3 calcLeadPoint(int idx){
    if(idx<0||!enemies[idx].alive) return {0,0,0};
    float tof=vlen(enemies[idx].pos-player.pos)/65.f;
    return enemies[idx].pos+enemies[idx].vel*tof;
}

bool worldToScreen(Vec3 pos, int W, int H, float& sx, float& sy){
    GLdouble m[16],p[16]; GLint v[4]; GLdouble wx,wy,wz;
    glGetDoublev(GL_MODELVIEW_MATRIX,m); glGetDoublev(GL_PROJECTION_MATRIX,p); glGetIntegerv(GL_VIEWPORT,v);
    if(gluProject(pos.x,pos.y,pos.z,m,p,v,&wx,&wy,&wz)==GL_FALSE||wz<0||wz>1.f) return false;
    sx=(float)wx; sy=(float)wy; return true;
}

void fireBullet(Plane& p, bool fromPlayer){
    float yr=p.yaw*PI/180.f, pr_=p.pitch*PI/180.f;
    Vec3 fwd={sinf(yr)*cosf(pr_),-sinf(pr_),cosf(yr)*cosf(pr_)};
    if(fromPlayer){
        int W=glutGet(GLUT_WINDOW_WIDTH), H=glutGet(GLUT_WINDOW_HEIGHT);
        GLdouble mv[16],pj[16]; GLint vp[4];
        glGetDoublev(GL_MODELVIEW_MATRIX,mv); glGetDoublev(GL_PROJECTION_MATRIX,pj); glGetIntegerv(GL_VIEWPORT,vp);
        GLdouble wx0,wy0,wz0,wx1,wy1,wz1;
        float cxf=crossWorldX,cyf=crossWorldY;
        if(cxf<1) cxf=1; if(cxf>W-1) cxf=(float)(W-1);
        if(cyf<1) cyf=1; if(cyf>H-1) cyf=(float)(H-1);
        if(gluUnProject(cxf,cyf,0.0,mv,pj,vp,&wx0,&wy0,&wz0)==GL_TRUE &&
           gluUnProject(cxf,cyf,1.0,mv,pj,vp,&wx1,&wy1,&wz1)==GL_TRUE){
            Vec3 rayDir=vnorm(vec3((float)(wx1-wx0),(float)(wy1-wy0),(float)(wz1-wz0)));
            fwd=vnorm(fwd*0.15f+rayDir*0.85f);
        }
        if(aimAssistOn){
            int ae=-1; float bd=9999.f;
            for(int i=0;i<ENEMY_COUNT;i++){
                if(!enemies[i].alive) continue;
                float ex,ey;
                if(worldToScreen(enemies[i].pos,W,H,ex,ey)){
                    float d=sqrtf((cxf-ex)*(cxf-ex)+(cyf-ey)*(cyf-ey));
                    if(d<40.f&&d<bd){bd=d;ae=i;}
                }
            }
            if(ae>=0){
                Vec3 toLead=vnorm(calcLeadPoint(ae)-p.pos);
                if(vdot(fwd,toLead)>0.4f) fwd=vnorm(fwd*0.5f+toLead*0.5f);
            }
        }
    }
    for(int i=0;i<BULLET_MAX;i++){
        if(!bullets[i].active){
            bullets[i].active=true; bullets[i].fromPlayer=fromPlayer;
            bullets[i].pos=p.pos+fwd*4.5f; bullets[i].vel=fwd*65.f; bullets[i].life=3.5f;
            spawnParticles(bullets[i].pos,2,true);
            break;
        }
    }
}

void fireMissile(){
    if(missileAmmo<=0||lockedEnemy<0||!enemies[lockedEnemy].alive) return;
    for(int i=0;i<MISSILE_MAX;i++){
        if(!missiles[i].active){
            float yr=player.yaw*PI/180.f, pr_=player.pitch*PI/180.f;
            Vec3 fwd={sinf(yr)*cosf(pr_),-sinf(pr_),cosf(yr)*cosf(pr_)};
            missiles[i].active=true; missiles[i].pos=player.pos+fwd*2.5f;
            missiles[i].vel=fwd*30.f; missiles[i].targetIdx=lockedEnemy;
            missiles[i].life=6.f; missiles[i].turnSpeed=3.5f;
            missileAmmo--;
            if(missileReload<=0) missileReload=MISSILE_RELOAD_TIME;
            break;
        }
    }
}

bool sphereAABB(Vec3 c, float r, float bx,float bz,float bw,float bd,float bh){
    float dx=fmaxf(bx-r-bw*0.5f,fminf(c.x,bx+bw*0.5f))-c.x;
    float dy=fmaxf(0-r,          fminf(c.y,bh))         -c.y;
    float dz=fmaxf(bz-r-bd*0.5f,fminf(c.z,bz+bd*0.5f))-c.z;
    return (dx*dx+dy*dy+dz*dz)<r*r;
}

void checkBulletBuildingCollision(Bullet& bullet){
    if(!bullet.active) return;
    for(int i=0;i<BUILDING_COUNT;i++){
        Building& b=buildings[i];
        if(b.x > 9000.f) continue;
        if(sphereAABB(bullet.pos,0.3f,b.x,b.z,b.w,b.d,b.h)){
            bullet.active=false;
            if(b.crackLevel<2) b.crackLevel++;
            b.damaged=true;
            bool onFire=(b.crackLevel==2);
            addCrater(bullet.pos,i,onFire);
            spawnParticles(bullet.pos,onFire?20:10,onFire);
            if(bullet.fromPlayer){ scoreFromBldg+=5; score=scoreFromKills+scoreFromWaves+scoreFromBldg; }
            break;
        }
    }
}

void checkBulletPlaneCollision(Bullet& bullet){
    if(!bullet.active) return;
    if(!bullet.fromPlayer&&player.alive){
        if(vlen(bullet.pos-player.pos)<1.8f){
            bullet.active=false; player.health-=enemyBulletDmg;
            spawnParticles(bullet.pos,8,false);
            if(player.health<=0){ player.alive=false; gameOver=true; spawnParticles(player.pos,60,true); }
        }
    }
    if(bullet.fromPlayer){
        for(int i=0;i<ENEMY_COUNT;i++){
            if(!enemies[i].alive) continue;
            if(vlen(bullet.pos-enemies[i].pos)<2.f){
                bullet.active=false; enemies[i].health-=25.f;
                spawnParticles(bullet.pos,12,enemies[i].health<=0);
                if(enemies[i].health<=0){
                    enemies[i].alive=false; enemiesKilled++; killsThisWave++;
                    scoreFromKills+=100+currentWave*50+(int)(gameTime/60.f)*20;
                    score=scoreFromKills+scoreFromWaves+scoreFromBldg;
                    spawnParticles(enemies[i].pos,50,true);
                }
                break;
            }
        }
    }
}

void updateMissiles(float dt_){
    for(int i=0;i<MISSILE_MAX;i++){
        if(!missiles[i].active) continue;
        Missile& m=missiles[i];
        m.life-=dt_;
        if(m.life<=0){ m.active=false; spawnParticles(m.pos,15,true); continue; }
        int ti=m.targetIdx;
        if(ti>=0&&ti<ENEMY_COUNT&&enemies[ti].alive){
            Vec3 newDir=vnorm(vnorm(m.vel)+vnorm(enemies[ti].pos-m.pos)*m.turnSpeed*dt_);
            float spd=vlen(m.vel); if(spd<50.f) spd+=20.f*dt_;
            m.vel=newDir*spd;
        }
        m.pos=m.pos+m.vel*dt_;
        if(ti>=0&&ti<ENEMY_COUNT&&enemies[ti].alive){
            if(vlen(m.pos-enemies[ti].pos)<2.5f){
                m.active=false; enemies[ti].health-=80.f;
                spawnParticles(m.pos,40,true);
                if(enemies[ti].health<=0){
                    enemies[ti].alive=false; enemiesKilled++; killsThisWave++;
                    scoreFromKills+=150+currentWave*50+(int)(gameTime/60.f)*20;
                    score=scoreFromKills+scoreFromWaves+scoreFromBldg;
                    spawnParticles(enemies[ti].pos,60,true);
                }
            }
        }
        if(m.pos.y<0.5f){ m.active=false; spawnParticles(m.pos,20,true); }
    }
}

void updateEnemy(Plane& e, float dt_){
    if(!e.alive) return;
    float distToPlayer=vlen(player.pos-e.pos);
    float margin=MAP_SIZE*0.8f;
    bool outOfBounds=(fabsf(e.pos.x)>margin||fabsf(e.pos.z)>margin);

    e.aiTimer-=dt_;
    if(outOfBounds){
        e.aiTarget=vec3(randf(-30.f,30.f),randf(10.f,25.f),randf(-30.f,30.f));
        e.aiTimer=4.f;
    } else {
        if(e.aiTimer<=0.f){
            if(distToPlayer<80.f&&player.alive){
                if(randf(0,1)<0.65f){
                    e.aiTarget=player.pos+vec3(randf(-5.f,5.f),randf(-3.f,3.f),randf(-5.f,5.f));
                    e.aiTimer=randf(2.f,4.5f);
                } else {
                    float angle=randf(0.f,2.f*PI), dist_=randf(20.f,60.f);
                    e.aiTarget=vec3(e.pos.x+cosf(angle)*dist_,randf(8.f,30.f),e.pos.z+sinf(angle)*dist_);
                    e.aiTimer=randf(3.f,6.f);
                }
            } else {
                float angle=randf(0.f,2.f*PI), dist_=randf(25.f,70.f);
                e.aiTarget=vec3(e.pos.x+cosf(angle)*dist_,randf(8.f,35.f),e.pos.z+sinf(angle)*dist_);
                e.aiTimer=randf(4.f,8.f);
            }
            e.aiTarget.x=fmaxf(-margin,fminf(margin,e.aiTarget.x));
            e.aiTarget.z=fmaxf(-margin,fminf(margin,e.aiTarget.z));
        }
    }

    Vec3 diff=e.aiTarget-e.pos; float dist_=vlen(diff);
    Vec3 dir=(dist_>0.5f)?vnorm(diff):vec3(0,0,1);
    float targetYaw=atan2f(dir.x,dir.z)*180.f/PI;
    float targetPitch=-asinf(fmaxf(-0.9f,fminf(0.9f,dir.y)))*180.f/PI;
    float dyaw=targetYaw-e.yaw;
    while(dyaw>180.f) dyaw-=360.f; while(dyaw<-180.f) dyaw+=360.f;
    float tr=(dist_>30.f)?2.5f:1.5f;
    e.yaw+=dyaw*dt_*tr;
    e.pitch+=(targetPitch-e.pitch)*dt_*1.8f;
    e.pitch=fmaxf(-50.f,fminf(50.f,e.pitch));

    float baseSpeed=(distToPlayer<80.f&&player.alive)?22.f:16.f;
    float curSpeed=baseSpeed*enemySpeedMult;
    if(dist_<10.f) curSpeed*=0.4f;
    float yr=e.yaw*PI/180.f, pr_=e.pitch*PI/180.f;
    e.vel=vec3(sinf(yr)*cosf(pr_),-sinf(pr_),cosf(yr)*cosf(pr_))*curSpeed;
    e.pos=e.pos+e.vel*dt_;
    if(e.pos.y<5.f) e.pos.y=5.f; if(e.pos.y>100.f) e.pos.y=100.f;

    e.shootCooldown-=dt_;
    Vec3 toPlayer=player.pos-e.pos;
    float dotToPlayer=vdot(vnorm(e.vel),vnorm(toPlayer));
    if(e.shootCooldown<=0&&distToPlayer<100.f&&dotToPlayer>0.55f&&player.alive){
        fireBullet(e,false);
        float si=fmaxf(0.4f,2.5f-(currentWave-1)*0.35f);
        e.shootCooldown=randf(si*0.7f,si);
    }
}

void updatePlayer(float dt_){
    if(!player.alive) return;
    float speed=22.f;
    if(keyState['z']||keyState['Z']) speed=45.f;
    if(keyState['v']||keyState['V']) speed=10.f;
    float ts=80.f,ps=60.f,rs=90.f;
    if(keyState['a']||keyState['A']) player.yaw  -=ts*dt_;
    if(keyState['d']||keyState['D']) player.yaw  +=ts*dt_;
    if(keyState['w']||keyState['W']) player.pitch-=ps*dt_;
    if(keyState['s']||keyState['S']) player.pitch+=ps*dt_;
    if(keyState['q']||keyState['Q']) player.roll -=rs*dt_;
    if(keyState['e']||keyState['E']) player.roll +=rs*dt_;
    if(player.pitch>70)  player.pitch=70;
    if(player.pitch<-70) player.pitch=-70;
    float yr=player.yaw*PI/180.f, pr_=player.pitch*PI/180.f;
    Vec3 fwd={sinf(yr)*cosf(pr_),-sinf(pr_),cosf(yr)*cosf(pr_)};
    player.vel=fwd*speed; player.pos=player.pos+player.vel*dt_;
    float M=MAP_SIZE*0.9f;
    if(player.pos.x<-M) player.pos.x=-M; if(player.pos.x>M) player.pos.x=M;
    if(player.pos.z<-M) player.pos.z=-M; if(player.pos.z>M) player.pos.z=M;
    if(player.pos.y<2.f){ player.pos.y=2.f; player.pitch=0; }
    if(player.pos.y>150.f) player.pos.y=150.f;
    player.shootCooldown-=dt_;
    if(keyState[' ']&&player.shootCooldown<=0){ fireBullet(player,true); player.shootCooldown=0.18f; }
}

void updateGameLogic(){
    lockOnTimer-=dt;
    if(lockOnTimer<=0.f){
        lockOnTimer=0.3f;
        if(lockedEnemy<0||!enemies[lockedEnemy].alive||vlen(enemies[lockedEnemy].pos-player.pos)>130.f)
            lockedEnemy=findBestLockTarget();
        else { int b=findBestLockTarget(); if(b>=0) lockedEnemy=b; }
    }

    updatePlayer(dt);
    for(int i=0;i<ENEMY_COUNT;i++) updateEnemy(enemies[i],dt);

    if(player.alive){
        bool crashed=(player.pos.y<=2.5f);
        if(!crashed){
            for(int i=0;i<BUILDING_COUNT;i++){
                Building& b=buildings[i];
                if(b.x > 9000.f) continue;
                if(sphereAABB(player.pos,1.2f,b.x,b.z,b.w,b.d,b.h)){
                    crashed=true; b.crackLevel=2; b.damaged=true;
                    addCrater(player.pos,i,true); break;
                }
            }
        }
        if(crashed){ player.health=0; player.alive=false; gameOver=true; spawnParticles(player.pos,80,true); }
    }

    if(waveClearPopup){
        waveClearTimer-=dt;
        if(waveClearTimer<=0.f) waveClearPopup=false;
    }

    if(waveTransition){
        waveTransitionTimer-=dt;
        if(waveTransitionTimer<=0.f){
            waveTransition=false;
            currentWave++;
            killsThisWave=0;
            enemySpeedMult=1.0f+(currentWave-1)*0.35f;
            enemyBulletDmg=18.f +(currentWave-1)*10.f;
            missileAmmo=5; missileReload=0.f;
            initEnemies();
            sprintf(waveClearMsg,"=== WAVE %d DIMULAI! (x%.1f Speed) ===",currentWave,enemySpeedMult);
            waveClearPopup=true; waveClearTimer=3.f;
        }
    }

    if(!waveTransition&&!gameOver&&!gameWon){
        bool allDead=true;
        for(int i=0;i<ENEMY_COUNT;i++) if(enemies[i].alive){allDead=false;break;}
        if(allDead){
            scoreFromWaves+=300+currentWave*100;
            score=scoreFromKills+scoreFromWaves+scoreFromBldg;

            if(currentWave>=TOTAL_WAVES){
                gameWon=true;
                sprintf(waveClearMsg,"=== MISSION COMPLETE! ALL WAVES CLEAR! ===");
                waveClearPopup=true; waveClearTimer=5.f;
            } else {
                sprintf(waveClearMsg,"=== WAVE %d CLEARED! Wave %d siap... ===",currentWave,currentWave+1);
                waveClearPopup=true;  waveClearTimer=WAVE_PAUSE+1.f;
                waveTransition=true;  waveTransitionTimer=WAVE_PAUSE;
            }
        }
    }

    if(missileAmmo<5&&missileReload>0){
        missileReload-=dt;
        if(missileReload<=0){ missileAmmo++; if(missileAmmo<5) missileReload=MISSILE_RELOAD_TIME; }
    }

    updateMissiles(dt);

    for(int i=0;i<BULLET_MAX;i++){
        if(!bullets[i].active) continue;
        bullets[i].pos=bullets[i].pos+bullets[i].vel*dt; bullets[i].life-=dt;
        if(bullets[i].life<=0){bullets[i].active=false;continue;}
        checkBulletBuildingCollision(bullets[i]);
        checkBulletPlaneCollision(bullets[i]);
    }

    for(int i=0;i<PARTICLE_MAX;i++){
        if(!particles[i].active) continue;
        particles[i].pos=particles[i].pos+particles[i].vel*dt;
        particles[i].vel.y-=4.f*dt; particles[i].life-=dt;
        particles[i].a=particles[i].life/1.2f;
        if(particles[i].life<=0) particles[i].active=false;
    }

    for(int i=0;i<craterCount;i++){
        if(!craters[i].onFire) continue;
        craters[i].fireTimer-=dt;
        if(craters[i].fireTimer<=0){
            craters[i].fireTimer=randf(0.1f,0.3f);
            spawnParticles(craters[i].pos,2,true);
        }
    }
}
