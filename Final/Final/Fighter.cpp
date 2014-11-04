/*   
Authors: Kevin Ly, Rhett Amin, David Elsonbaty 
Student Numbers: 1144604, 1130789,   
File: Fighter.cpp 
-----------------------------  
Class for fighter objects 
Enables the creation of fighter objects 
Methods include: 
- Drawing (wire, solid, and no rotation); 
- Animation 
- update fighter location 
- action 
- move fighter 
- change location 
- collision checking 
- displaying/animating bullets 
*/
  
#include "Fighter.h" 
#include <iostream> 
  
using namespace std; 
  
//ENUM for body parts 
enum {leftLeg, rightLeg, body, leftArm, rightArm, head}; 
//Constructor 
Fighter::Fighter(){ 
    point3D *x = new point3D(0,1,0); 
    posFighter = x; 
} 
  
Fighter::Fighter(point3D* d,bool* faceRig){ 
    posFighter = d; 
    dirVec = new point3D(0,4.5,0); 
    hitPoints = new float(100); 
    //Set the distance of each body part away from posFighter 
    posll = new point3D(posFighter->x + 0.75, posFighter->y, posFighter->z); 
    posrl = new point3D(posFighter->x - 0.75, posFighter->y, posFighter->z); 
    posb = new point3D(posFighter->x, posFighter->y + 3, posFighter->z); 
    posla = new point3D(posFighter->x + 1.5, posFighter->y + 3.25, posFighter->z); 
    posra = new point3D(posFighter->x - 1.5, posFighter->y + 3.25, posFighter->z); 
    posh = new point3D(posFighter->x, posFighter->y + 5.25, posFighter->z); 
  
    //make new box object at each position 
    bodyParts[leftLeg] = new Box(1,1,posll,faceRig); 
    bodyParts[rightLeg] = new Box(1,1,posrl,faceRig); 
    bodyParts[body] = new Box(1.5,3,posb,faceRig); 
    bodyParts[leftArm] = new Box(1,1,posla,faceRig); 
    bodyParts[rightArm] = new Box(1,1,posra,faceRig); 
    bodyParts[head] = new Box(1,1,posh,faceRig); 
  
    //initalize the properties of fighter class 
    aniState = nullptr; 
    idle = 20; 
    idle2 = 9; 
    gravity = -0.075; 
    faceRight = faceRig; 
    jumpCount = new int (0); 
    coliHit = new bool(false); 
    bulletState = new bool(false); 
      
    Victory = new bool(false); 
    color = new colourStruc(0,0,0); 
} 
  
Fighter::Fighter(point3D* d){ 
    posFighter = d; 
  
    posll = new point3D(posFighter->x + 0.75, posFighter->y, posFighter->z); 
    posrl = new point3D(posFighter->x - 0.75, posFighter->y, posFighter->z); 
    posb = new point3D(posFighter->x, posFighter->y + 3, posFighter->z); 
    posla = new point3D(posFighter->x + 1.5, posFighter->y + 3.25, posFighter->z); 
    posra = new point3D(posFighter->x - 1.5, posFighter->y + 3.25, posFighter->z); 
    posh = new point3D(posFighter->x, posFighter->y + 5.25, posFighter->z); 
  
    bodyParts[leftLeg] = new Box(1,1,posll); 
    bodyParts[rightLeg] = new Box(1,1,posrl); 
    bodyParts[body] = new Box(1.5,3,posb); 
    bodyParts[leftArm] = new Box(1,1,posla); 
    bodyParts[rightArm] = new Box(1,1,posra); 
    bodyParts[head] = new Box(1,1,posh); 
  
    //set idle 
    aniState = nullptr; 
    idle = 20; 
    idle2 = 9; 
    gravity = -0.075; 
    jumpCount = new int (0); 
    coliHit = new bool(false); 
    bulletState = new bool(false); 
    color = new colourStruc(0,0,0); 
} 
  
//Methods: 
  
//changeLocation 
//sets the location of a fighter manually 
  
void Fighter::changeLocation(point3D* point){ 
      
    posFighter = point; 
      
    //create initial positions 
    posll = new point3D(posFighter->x + 0.75, posFighter->y, posFighter->z); 
    posrl = new point3D(posFighter->x - 0.75, posFighter->y, posFighter->z); 
    posb = new point3D(posFighter->x, posFighter->y + 3, posFighter->z); 
    posla = new point3D(posFighter->x + 1.5, posFighter->y + 3.25, posFighter->z); 
    posra = new point3D(posFighter->x - 1.5, posFighter->y + 3.25, posFighter->z); 
    posh = new point3D(posFighter->x, posFighter->y + 5.25, posFighter->z); 
      
    bodyParts[leftLeg] = new Box(1,1,posll); 
    bodyParts[rightLeg] = new Box(1,1,posrl); 
    bodyParts[body] = new Box(1.5,3,posb); 
    bodyParts[leftArm] = new Box(1,1,posla); 
    bodyParts[rightArm] = new Box(1,1,posra); 
    bodyParts[head] = new Box(1,1,posh); 
      
      
} 
  
//drawBox 
//draw body parts based on wire/solid/no angle properties 
void Fighter::drawBox(){ 
    for(int i = 0; i < 6; i++){ 
        bodyParts[i]->draw(); 
    } 
} 
  
void Fighter::drawWire(){ 
    for(int i = 0; i < 6; i++){ 
        bodyParts[i]->drawWire(); 
    } 
} 
  
void Fighter::drawBoxNoAngle(){ 
    for(int i = 0; i < 6; i++){ 
        bodyParts[i]->drawNoAngle(); 
    } 
} 
  
void Fighter::drawWireNoAngle(){ 
    for(int i = 0; i < 6; i++){ 
        bodyParts[i]->drawWireNoAngle(); 
    } 
} 
  
//animate 
//Checks if the animation object is null, if it is null the procede to do idle animation 
//if animation is not null then do animation, until animation method returns true; which means finished animation 
//idle animation: moves torse,head,arms up and down, legs forward and backwards 
void Fighter::animate(){ 
    bool finish; 
    applyGrav(); 
      
    if (aniState == nullptr){ 
        if (idle < 20 && idle >= 0){; 
            bodyParts[body]->move(0,-0.02,0); 
            bodyParts[leftArm]->move(0,-0.05,0); 
            bodyParts[rightArm]->move(0,-0.05,0); 
            bodyParts[head]->move(0,-0.02,0); 
  
            idle--; 
        } else { 
            if (idle < 0){ 
                idle = 39; 
            } 
              
            bodyParts[body]->move(0,0.02,0); 
            bodyParts[leftArm]->move(0,0.05,0); 
            bodyParts[rightArm]->move(0,0.05,0); 
            bodyParts[head]->move(0,0.02,0); 
            idle--; 
        } 
  
        if(idle2 <= 9 && idle2 >=0){ 
            bodyParts[leftLeg]->move(0,0,-0.1); 
            bodyParts[rightLeg]->move(0,0, 0.1); 
            idle2--; 
        } else { 
  
            if(idle2 < 0){ 
                idle2 = 19; 
            } 
            bodyParts[leftLeg]->move(0,0,0.1); 
            bodyParts[rightLeg]->move(0,0,-0.1); 
            idle2--; 
        } 
    } else { 
        bool finish = aniState->aniAction(); 
      
        if (finish){ 
            aniState = nullptr; 
        } 
    } 
  
      
} 
  
//applyGrav 
//applies gravity to position ofifghter, and all body parts. 
//direction vector is affected by gravity 
//check against platform 
void Fighter::applyGrav(){ 
    //Update direction vector based on colisions and platforms 
    posFighter->x += dirVec->x; 
    posFighter->y += dirVec->y; 
    posFighter->z += dirVec->z; 
    updateBodyParts(); 
      
    dirVec->y += gravity; 
  
    //Platform Checking 
    if (dirVec->z > 0){ 
        dirVec->z -= 0.1; 
    } else if (dirVec->z < 0) { 
        dirVec->z += 0.1; 
    } 
  
    if (dirVec->z  < 0.1 && dirVec->z > -0.1){ 
        dirVec->z = 0; 
    } 
  
    //If character is not on platform apply gravity. 
    if (posFighter->z < 28 && posFighter->z > -28) { 
        if (posFighter->y < 1.5 && posFighter->y > 0) { 
            *jumpCount = 0; 
            dirVec->y = 0.2; 
        } 
    } 
} 
  
//updateBodyParts 
// for each body part modify by gravity 
void Fighter::updateBodyParts(){ 
    for(int i = 0; i < 6; i++){ 
        bodyParts[i]->pos->x += dirVec->x; 
        bodyParts[i]->pos->y += dirVec->y; 
        bodyParts[i]->pos->z += dirVec->z; 
    }    
} 
  
//action 
//create a new animation based on d 
void Fighter::action(int d){ 
    if (aniState == nullptr){ 
        aniState = new Animation(d, bodyParts, posFighter,dirVec, jumpCount, faceRight); 
    } 
} 
  
//moveFighter 
//move fighter based on x,y,z inputs 
void Fighter::moveFighter(float x, float y, float z){ 
        posFighter->x += x; 
        posFighter->y += y; 
        posFighter->z += z; 
  
        for(int i = 0; i < 6; i++){ 
            bodyParts[i]->pos->x += x; 
            bodyParts[i]->pos->y += y; 
            bodyParts[i]->pos->z += z; 
        }    
} 
  
//collisionCheck 
//Checks the distance between arms/legs/bullet to the torso of the body 
// if it is less than the distance there then a hit has happened. 
//affect enemy hp, direction vector, shield 
void Fighter::collisionCheck(){ 
    //Check during animation ONLY 
    if(aniState != nullptr){ 
        //Calculate Distance between body + enemy torso 
        float dPunch = MathLib3D::pDistance(bodyParts[leftArm]->pos,enefig->bodyParts[body]->pos); 
        float dKick = MathLib3D::pDistance(bodyParts[rightLeg]->pos,enefig->bodyParts[body]->pos); 
  
        if(!(*coliHit)){ 
            if(dPunch <= 2){ 
                *coliHit = true; 
                //Shield state checking 
                if(*enefig->shieldState == 0){ 
                    if(*faceRight){ 
                        enefig->dirVec->z += 0.4; 
                    } else { 
                        enefig->dirVec->z -= 0.4; 
                    } 
                    //Affect hitpoints 
                    *enefig->hitPoints = *enefig->hitPoints - 2.5; 
                } else { 
                    *enefig->shieldSize = *enefig->shieldSize - 0.25; 
                    if (*enefig->shieldSize <= 0){ 
                        *enefig->shieldState = 0; 
                    } 
                } 
            } 
            if(dKick <= 2) { 
                *coliHit = true; 
                if(*enefig->shieldState == 0){ 
                    if(*faceRight){ 
                        enefig->dirVec->z += 0.7; 
                        enefig->dirVec->y += 0.5; 
                    } else { 
                        enefig->dirVec->z -= 0.7; 
                        enefig->dirVec->y += 0.5; 
                    } 
                    *enefig->hitPoints = *enefig->hitPoints - 3; 
                } else { 
                    *enefig->shieldSize = *enefig->shieldSize - 0.25; 
                    if (*enefig->shieldSize <= 0){ 
                        *enefig->shieldState = 0; 
                    } 
                } 
            } 
  
        } else { 
            *coliHit = false; 
        } 
    } 
  
    //If there exist a bullet execute this code, when bullet hits: erase bullet 
    if (*bulletState){ 
        float dbullet = MathLib3D::pDistance(bullet->pos,enefig->bodyParts[rightLeg]->pos); 
  
        if(dbullet <= 4.8) { 
            if(*enefig->shieldState == 0){ 
                if(*faceRight){ 
                    enefig->dirVec->z += 0.5; 
                } else { 
                    enefig->dirVec->z -= 0.5; 
                } 
                *enefig->hitPoints = *enefig->hitPoints - 5; 
            } else { 
                *enefig->shieldSize = *enefig->shieldSize - 0.25; 
                if (*enefig->shieldSize <= 0){ 
                    *enefig->shieldState = 0; 
                } 
            } 
            *bulletState = false; 
            bullet = nullptr; 
        } 
    } 
} 
  
//Update info required for fighter to fighter interaction 
void Fighter::updateInfo(Fighter* asdf, int* shieldstate, float* shieldsize){ 
    enefig = asdf; 
    shieldSize = shieldsize; 
    shieldState = shieldstate; 
} 
  
//Animate Bullet: 
//Move bullet pased on its direction vector, if it exceeds z + or z- then delete 
void Fighter::aniBullet(){ 
    if(*bulletState){ 
        bullet->pos->z += bullet->dirVec->z; 
  
        if(bullet->pos->z > 40 && bullet->pos->z < -40){ 
            *bulletState = false; 
            bullet = nullptr; 
        } 
    } 
} 
  
//DisplayBullet 
//Basic draw bullet function 
void Fighter::displaybullet(){ 
    if(*bulletState){ 
		if(!*Victory){
			bullet->drawBullet(color); 
		}
    } 
} 
  
//activateBullet() 
//Meant to be used for keyboard input. 
//when activated: make new bullet at fighter posistion, set the properties in the fighter object 
void Fighter::activateBullet(){ 
    float* x = new float(posFighter->x); 
    float* y = new float(posFighter->y + 3); 
    float* z = new float(posFighter->z); 
    posbullet = new point3D(*x,*y,*z); 
    bullet = new Box(1,1,posbullet,faceRight); 
      
    if(*faceRight){ 
        bullet->dirVec->z = 1; 
    } else { 
        bullet->dirVec->z = -1; 
    } 
  
    *bulletState = true; 
} 