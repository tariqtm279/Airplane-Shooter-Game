#include "City.h"
#include "Config.h"
#include <cmath>

Building buildings[BUILDING_COUNT];
Tree trees[TREE_COUNT];

bool isOnRoad(float px, float pz){
    if(fabsf(px) < (ROAD_MAIN + 2.0f)) return true;
    if(fabsf(pz) < (ROAD_MAIN + 2.0f)) return true;
    
    for(int i = -6; i <= 6; i++) {
        float rCoord = i * BLOCK_STRIDE;
        if(fabsf(rCoord) < 1.0f) continue;
        if(fabsf(px - rCoord) < (ROAD_SEC * 0.5f + 1.8f)) return true;
        if(fabsf(pz - rCoord) < (ROAD_SEC * 0.5f + 1.8f)) return true;
    }
    return false;
}

void initBuildings(){
    srand(42);
    int idx = 0;
    for(int gi = -3; gi <= 3 && idx < BUILDING_COUNT; gi++){
        for(int gj = -3; gj <= 3 && idx < BUILDING_COUNT; gj++){
            float bcenterX = gi * BLOCK_STRIDE;
            float bcenterZ = gj * BLOCK_STRIDE;
            float subOffsets[4][2] = {
                {-6.5f, -6.5f}, {6.5f, -6.5f},
                {-6.5f,  6.5f}, {6.5f,  6.5f}
            };
            for(int b = 0; b < 4 && idx < BUILDING_COUNT; b++) {
                float bx = bcenterX + subOffsets[b][0];
                float bz = bcenterZ + subOffsets[b][1];
                
                if(isOnRoad(bx, bz)) continue;
                if(fabsf(bx) > MAP_SIZE * 0.88f || fabsf(bz) > MAP_SIZE * 0.88f) continue;
                
                float distFromCenter = sqrtf(bx*bx + bz*bz);
                bool isCBD = (distFromCenter < 55.0f);
                
                buildings[idx].x = bx;
                buildings[idx].z = bz;
                
                if(isCBD){
                    buildings[idx].w = randf(6.0f, 8.0f); 
                    buildings[idx].d = randf(6.0f, 8.0f); 
                    buildings[idx].h = randf(25.0f, 60.0f);
                    float base = randf(0.45f, 0.65f); 
                    buildings[idx].r = base; 
                    buildings[idx].g = base + randf(0.0f, 0.05f); 
                    buildings[idx].b = base + randf(0.1f, 0.25f);
                } else {
                    buildings[idx].w = randf(5.0f, 7.0f); 
                    buildings[idx].d = randf(5.0f, 7.0f); 
                    buildings[idx].h = randf(8.0f, 22.0f);
                    float base = randf(0.4f, 0.6f); 
                    buildings[idx].r = base + randf(0.05f, 0.15f); 
                    buildings[idx].g = base + randf(0.0f, 0.05f); 
                    buildings[idx].b = base;
                }
                buildings[idx].damaged = false; 
                buildings[idx].crackLevel = 0; 
                idx++;
            }
        }
    }
    for(; idx < BUILDING_COUNT; idx++) {
        buildings[idx].x = 9999.f; buildings[idx].z = 9999.f; buildings[idx].h = 0.f;
    }
}

void initTrees(){
    srand(123);
    int placed = 0;
    for(int i = -3; i <= 3 && placed < (TREE_COUNT * 0.65f); i++) {
        float roadCoord = i * BLOCK_STRIDE;
        if(fabsf(roadCoord) < 1.0f) continue;
        
        for(float step = -MAP_SIZE * 0.8f; step <= MAP_SIZE * 0.8f; step += 14.0f) {
            if(placed >= TREE_COUNT) break;
            float sides[2] = {-ROAD_SEC * 0.5f - 1.5f, ROAD_SEC * 0.5f + 1.5f};
            for(int s = 0; s < 2; s++) {
                float tx1 = roadCoord + sides[s];
                float tz1 = step + randf(-0.5f, 0.5f);
                if(!isOnRoad(tx1, tz1) && fabsf(tz1) > (ROAD_MAIN + 3.f)) {
                    trees[placed].x = tx1; trees[placed].z = tz1;
                    trees[placed].height = randf(3.0f, 4.8f); trees[placed].trunkR = 0.18f;
                    placed++;
                }
                float tx2 = step + randf(-0.5f, 0.5f);
                float tz2 = roadCoord + sides[s];
                if(!isOnRoad(tx2, tz2) && fabsf(tx2) > (ROAD_MAIN + 3.f) && placed < TREE_COUNT) {
                    trees[placed].x = tx2; trees[placed].z = tz2;
                    trees[placed].height = randf(3.0f, 4.8f); trees[placed].trunkR = 0.18f;
                    placed++;
                }
            }
        }
    }
    
    int attempts = 0;
    while(placed < TREE_COUNT && attempts < 5000) {
        attempts++;
        float tx = randf(-MAP_SIZE * 0.85f, MAP_SIZE * 0.85f);
        float tz = randf(-MAP_SIZE * 0.85f, MAP_SIZE * 0.85f);
        if(isOnRoad(tx, tz)) continue;
        
        bool hitBuilding = false;
        for(int b = 0; b < BUILDING_COUNT; b++) {
            if(buildings[b].x > 9000.f) continue;
            float dx = fabsf(tx - buildings[b].x) - (buildings[b].w * 0.5f + 1.5f);
            float dz = fabsf(tz - buildings[b].z) - (buildings[b].d * 0.5f + 1.5f);
            if(dx < 0 && dz < 0) { hitBuilding = true; break; }
        }
        if(hitBuilding) continue;
        
        bool hitTree = false;
        for(int t = 0; t < placed; t++) {
            if(fabsf(tx - trees[t].x) < 3.0f && fabsf(tz - trees[t].z) < 3.0f) { hitTree = true; break; }
        }
        if(hitTree) continue;
        
        trees[placed].x = tx; 
        trees[placed].z = tz; 
        trees[placed].height = randf(2.5f, 4.2f); 
        trees[placed].trunkR = 0.16f;
        placed++;
    }
}
