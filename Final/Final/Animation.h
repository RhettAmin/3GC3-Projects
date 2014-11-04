/*   
Authors: Kevin Ly, Rhett Amin, David Elsonbaty 
Student Numbers: 1144604, 1130789,   
File: Animation.h 
-----------------------------  
 Header File for the Animation Class, Contains all variables, methods and constructors. 
*/
#include "Box.h" 
  
class Animation 
{ 
public: 
    //Animation Number Input 
    int aninum; 
    //Body parts to animate 
    Box *body [6]; 
    //Fighter Position 
    point3D* fightPos; 
    //Boolean states to iterate through proper animation 
    bool first,aniFin; 
    bool anip1,anip2,anip3; 
    float limit1, limit2; 
    //Number of jumps before touching platforms 
    int *jumpnum; 
      
    //Direction Vector Modifier 
    point3D *dir; 
    //Boolean state; Direction of Face 
    bool* faceRight; 
      
    //Constructor 
    Animation(int ani, Box *body [6], point3D *fight,point3D *x, int *jumpCount, bool* facing); 
    //Methods 
    bool aniAction(); 
  
};