/*   
Authors: Kevin Ly, Rhett Amin, David Elsonbaty 
Student Numbers: 1144604, 1130789,   
File: Animation.cpp 
-----------------------------  
Animation Class for the Fighter Object 
Send information in regards to fighter object through constructor 
Execute animation by inputing the animation number 
  
Handles the movement for the Fighter object, alters values accordingly 
*/
  
#include "Animation.h" 
#include <iostream> 
#include <stdio.h> 
//Enum Statements for Body parts, and animation type 
enum {leftLeg, rightLeg, cbody, leftArm, rightArm, head}; 
enum {jump, punch, kick, fireGun}; 
  
using namespace std; 
  
//Constructor 
Animation::Animation(int input, Box *bodyparts [6], point3D *fightPos, point3D *diri, int *jumpCount, bool* faceState){ 
    for(int i = 0; i < 6; i ++){ 
        body[i] = bodyparts[i]; 
    } 
    this->fightPos = fightPos; 
    dir = diri; 
    aninum = input; 
    first = true; 
    aniFin = false; 
    jumpnum = jumpCount; 
    faceRight = faceState; 
}; 
  
//Method: aniAction() 
//With the animation type stated in the constructor, Uses switch case to find out which animation to execute 
//while iteration through animation return false, when animation is finished return true. 
bool Animation:: aniAction(){ 
    switch (aninum) { 
      
  
    //Jump Animation: 
    //If Jump number is <=1, Execute Jump animation 
    //Animation ideology: 
    //Move body parts down wards, then upwards, 
    //Upon finishing animation apply the direction vector change y+ 
    case jump: 
        if (first){ 
            limit2 = 4; 
            anip1 = true; 
            anip2 = true; 
            first = false; 
            if (*jumpnum >= 1){ 
                anip1 = false; 
                anip2 = false; 
                return true; 
            } else { 
                *jumpnum = *jumpnum + 1; 
            } 
        } 
        if (anip1){ 
            if(body[cbody]->pos->y > ((fightPos->y) + 1.75)){ 
                body[cbody]->move(0,-0.2,0); 
                body[leftArm]->move(0,-0.2,0); 
                body[rightArm]->move(0,-0.2,0); 
                body[head]->move(0,-0.2,0); 
            } else { 
                anip1 = false; 
            }    
        } else if (anip2){ 
            if (limit2 > 0){ 
                body[cbody]->move(0,0.3,0); 
                body[leftArm]->move(0,0.3,0); 
                body[rightArm]->move(0,0.3,0); 
                body[head]->move(0,0.3,0); 
                limit2--; 
                dir->y = 0.75; 
                  
            } else{ 
                anip2 = false; 
                aniFin = true; 
            } 
  
        } 
        return aniFin; 
        break; 
    //Punch Animation: 
    //Animation ideology: 
    //Using quadradic function x^2 + 1 
    //Iterate through x @ 0 while x is incremented by 0.2 
    //Will cause punch to go forward and back 
  
    case punch: 
        if (first){ 
            limit1 = 0; 
            anip1 = true; 
            first = !first; 
        } 
        if(anip1){ 
            float en = -(limit1)*(limit1) + 1; 
            if(!(*faceRight)){ 
                en = en * -1; 
            } 
  
            body[leftArm]->move(0,0,en); 
            limit1 += 0.2; 
  
            if(*faceRight){ 
                if(body[leftArm]->pos->z < fightPos->z ){ 
                    anip1 = false; 
                } 
            } else if (!(*faceRight)){ 
                if(body[leftArm]->pos->z > fightPos->z ){ 
                    anip1 = false; 
                } 
            } 
  
        } else { 
            body[leftArm]->pos->z = fightPos->z; 
            aniFin = true; 
        } 
        return aniFin; 
        break; 
  
    //Kick Animation: 
    //Animation ideology: 
    //Using quadradic function x^2 + 1 
    //Iterate through x @ 0 while x is incremented by 0.2 
    //Will cause Kick to go forward and back 
    case kick: 
        if (first){ 
            first = false; 
            anip1 = true; 
            limit1 = 0.1; 
              
        } 
        if(anip1){ 
            float en = -(limit1)*(limit1) + 1.3; 
            if(!(*faceRight)){ 
                en = en * -1; 
            } 
            int negstate=1; 
            if (!(*faceRight)){ 
                negstate = -1; 
            } 
            body[rightLeg]->move(0,negstate*(en/2),en); 
            limit1 += 0.2; 
          
            if(*faceRight){ 
                if(body[rightLeg]->pos->z < fightPos->z ){ 
                    anip1 = false; 
                } 
            } else if (!(*faceRight)){ 
                if(body[rightLeg]->pos->z > fightPos->z ){ 
                    anip1 = false; 
                } 
            } 
        } else { 
            body[rightLeg]->pos->z = fightPos->z; 
            body[rightLeg]->pos->y = fightPos->y; 
            aniFin = true; 
        } 
        return aniFin; 
        break; 
    //FireGun Animation: 
    //Animation ideology: 
    //Using quadradic function x^2 + 1 
    //Iterate through x @ 0 while x is incremented by 0.2 
    //Will cause firegun to go forward and back 
    case fireGun: 
        if (first){ 
            limit1 = 0; 
            anip1 = true; 
            first = !first; 
        } 
        if(anip1){ 
            float en = -(limit1)*(limit1) + 1; 
            if(!(*faceRight)){ 
                en = en * -1; 
            } 
  
            body[leftArm]->move(0,0,en); 
            body[rightArm]->move(0,0,en); 
            limit1 += 0.2; 
  
            if(*faceRight){ 
                if(body[leftArm]->pos->z < fightPos->z ){ 
                    anip1 = false; 
                } 
            } else if (!(*faceRight)){ 
                if(body[leftArm]->pos->z > fightPos->z ){ 
                    anip1 = false; 
                } 
            } 
  
        } else { 
            body[leftArm]->pos->z = fightPos->z; 
            body[rightArm]->pos->z = fightPos->z; 
            aniFin = true; 
        } 
        return aniFin; 
        break; 
    } 
}