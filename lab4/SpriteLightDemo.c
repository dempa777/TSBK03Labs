// Demo of heavily simplified sprite engine
// by Ingemar Ragnemalm 2009
// used as base for lab 4 in TSBK03.
// OpenGL 3 conversion 2013.

#ifdef __APPLE__
	#include <OpenGL/gl3.h>
	#include "MicroGlut.h"
	// uses framework Cocoa
#else
	#include <GL/gl.h>
	#include "MicroGlut.h"
#endif

#include <stdlib.h>
#include "LoadTGA.h"
#include "SpriteLight.h"
#include "GL_utilities.h"
#include <time.h>
// L�gg till egna globaler h�r efter behov.
#define nrOfSheeps 20

float randomFloat(){
				return (float)rand()/(float)RAND_MAX;
}

float Distance(float h1, float v1, float h2, float v2) {
	return sqrt((h2-h1)*(h2-h1) + (v2-v1)*(v2-v1));
}
	
float maxDist = 80.0;
float len;

void Cohesion(float cohesionC){
	float totH = 0.0, totV = 0.0, avgH = 0.0, avgV = 0.0;
	int sheepCount;

	SpritePtr sp;
	SpritePtr sp1;
	
	sp = gSpriteRoot;
	sp1 = gSpriteRoot;

	SpritePtr sheeps[nrOfSheeps];
	int i = 0;
	do 
	{
					sheeps[i] = sp1;
					i++;
					sp1 = sp1->next;
	} while (sp1 != NULL);

	do
	{
					totH = 0.0; 
					totV = 0.0;
					sheepCount = 0;	
          
          if(sp->busig){
						if(Distance(sp->busH, sp->busV, sp->position.h, sp->position.v) < 50.0){	
              sp->busV = gHeight*randomFloat(); 
              sp->busH = gWidth*randomFloat(); 
            }
            sp->speed.v = 0.01*(sp->busV-sp->position.v);
            sp->speed.h = 0.01*(sp->busH-sp->position.h);
          } else {
					for(i = 0; i < nrOfSheeps; i++){
									if(Distance(sheeps[i]->position.h, sheeps[i]->position.v, sp->position.h, sp->position.v) < maxDist){	
													totH += sheeps[i]->position.h;
													totV += sheeps[i]->position.v;
													sheepCount++;
									}
					}	

					avgH = cohesionC*(totH/sheepCount - sp->position.h);
					avgV = cohesionC*(totV/sheepCount - sp->position.v);

          len = sqrt(avgH*avgH + avgV*avgV);
          if(len>1){
            avgH /= len;
            avgV /= len;
          }

					sp->speed.h += avgH; 
					sp->speed.v += avgV; 

          }

          
          sp = sp->next;
	} while (sp != NULL);

}

void Separation(separationC) {
	SpritePtr sp1;
	
  float dirMoveH;
  float dirMoveV;

	SpritePtr sp;
	
	sp1 = gSpriteRoot;
	sp = gSpriteRoot;


	SpritePtr sheeps[nrOfSheeps];
	int i = 0;
	do 
	{
					sheeps[i] = sp1;
					i++;
					sp1 = sp1->next;
	} while (sp1 != NULL);
 
  SpritePtr closestSheep;
  float minDist = 1, curDis;
  do
  {
    minDist = 100000;
					for(i = 0; i < nrOfSheeps; i++){
                  curDis = Distance(sheeps[i]->position.h, sheeps[i]->position.v, sp->position.h, sp->position.v);
									if(curDis < maxDist && curDis < minDist && curDis > 1.0)
                  {
                      minDist = curDis;
                      closestSheep = sheeps[i];
                  }
          }
          
          
          if(!sp->busig) {


          float hLength = separationC/(sqrt(minDist));
          if(hLength>1) {hLength =1;}
          float vLength = separationC/(sqrt(minDist));
          if(vLength>1) {vLength =1;}


          dirMoveH = (sp->position.h - closestSheep->position.h);
          dirMoveV = (sp->position.v - closestSheep->position.v); 
          len = sqrt(dirMoveH*dirMoveH + dirMoveV*dirMoveV);
            
          sp->speed.h += hLength*dirMoveH/len;
          sp->speed.v += vLength*dirMoveV/len;
          printf("sepH: %f sepV: %f\n" , hLength*dirMoveH/len, vLength*dirMoveV/len);
          }
    

          sp = sp->next;
	} while (sp != NULL);

}

void Alignment(float alignC){
	float totH = 0.0, totV = 0.0, avgH = 0.0, avgV = 0.0, dist;
	int sheepCount;

	SpritePtr sp;
	SpritePtr sp1;
	
	sp = gSpriteRoot;
	sp1 = gSpriteRoot;


	SpritePtr sheeps[nrOfSheeps];
	int i = 0;
	do 
	{
					sheeps[i] = sp1;
					i++;
					sp1 = sp1->next;
	} while (sp1 != NULL);

	do
	{
					totH = 0.0; 
					totV = 0.0;
					sheepCount = 0;	

          for(i = 0; i < nrOfSheeps; i++){
            dist = Distance(sheeps[i]->position.h, sheeps[i]->position.v, sp->position.h, sp->position.v); 
            if(dist > 1.0 && dist < maxDist){	
              totH += sheeps[i]->speed.h*maxDist/dist;
              totV += sheeps[i]->speed.v*maxDist/dist;
              sheepCount++;
            }
          }

          if(!sp->busig) {

            totH = alignC*totH/sheepCount;
            totV = alignC*totV/sheepCount;
            len = sqrt(avgH*avgH + avgV*avgV);
            if(len>1){
              totH /= len;
              totV /= len;
            }

            sp->speed.h += totH; 
            sp->speed.v += totV; 

            len = sqrt(sp->speed.h*sp->speed.h + sp->speed.v*sp->speed.v);
						if(len>3){
              sp->speed.h /= len;
              sp->speed.v /= len;
            }
          }
          sp = sp->next;
  } while (sp != NULL);

}

FPoint CalcAvoidance(float diffV, float diffH){
  FPoint value;
  value.v=0.0;
  value.h=0.0;
  float len = sqrt(diffV*diffV + diffH*diffH);
	float outV, outH; 
	float absDV, absDH;
  absDV = sqrt(diffV*diffV);
  absDH = sqrt(diffH*diffH);
	
 	if(len<maxDist) {
		value.v = 1.0 - absDV/maxDist;

		value.h = 1.0 - absDH/maxDist;
		
		if(diffV > 0.0 || diffV < 0.0)
			value.v *= (absDV/diffV);
		else
			value.v = 0.0;
		if(diffH > 0.0 || diffH < 0.0)		
			value.h *= (absDH/diffH);
		else
			value.h = 0.0;
		
	}
  return value;
}

  FPoint speedDiff[nrOfSheeps];
  FPoint averagePos[nrOfSheeps];
  FPoint avoidanceVec[nrOfSheeps];
  SpritePtr food;

void FirstLoop() {
  SpritePtr ptr;
  SpritePtr ptr2;
  int i,j;
  int count;

	FPoint temp; 

  ptr = gSpriteRoot;
  for(i = 0; i < nrOfSheeps; i++) {

    count = 0;
    speedDiff[i].v = 0.0;
    speedDiff[i].h = 0.0;
    averagePos[i].v = 0.0;
    averagePos[i].h = 0.0;
    avoidanceVec[i].h = 0.0;
    avoidanceVec[i].v = 0.0;
		

    ptr2 = gSpriteRoot;
    for(j = 0; j < nrOfSheeps; j++) {

      if(i != j){
        if(Distance(ptr->position.v, ptr->position.h, ptr2->position.v, ptr2->position.h) < maxDist){

 	        // speed diff
 	        speedDiff[i].h += (ptr2->speed.h - ptr->speed.h);
 	        speedDiff[i].v += (ptr2->speed.v - ptr->speed.v);
	
         	// avg pos
         	averagePos[i].h += ptr2->position.h;
         	averagePos[i].v += ptr2->position.v;

         	temp = CalcAvoidance(-ptr2->position.v + ptr->position.v, -ptr2->position.h + ptr->position.h);
      	 
					avoidanceVec[i].v += temp.v;
					avoidanceVec[i].h += temp.h;
			
					count++;
        }
      }
      ptr2 = ptr2->next;
    }
    float speedLen = sqrt(speedDiff[i].v*speedDiff[i].v + speedDiff[i].h*speedDiff[i].h);
    float posLen = sqrt(averagePos[i].v*averagePos[i].v + averagePos[i].h*averagePos[i].h);
    float avoidLen = sqrt(avoidanceVec[i].v*avoidanceVec[i].v + avoidanceVec[i].h*avoidanceVec[i].h);
    if(count > 0) {
      if(speedLen <= 1) {
        speedLen = 1.0;
      }
      if(avoidLen <= 1) {
        avoidLen = 1.0;
      }
      if(posLen <= 1) {
        posLen = 1.0;
      }
      speedDiff[i].v /= count;//*speedLen;
      speedDiff[i].h /= count;//*speedLen;
      averagePos[i].v /= count;//*posLen;
      averagePos[i].h /= count;//*posLen;
      avoidanceVec[i].h /= count;//*avoidLen;
      avoidanceVec[i].v /= count;//*avoidLen;
      
      averagePos[i].v -= ptr->position.v;
      averagePos[i].h -= ptr->position.h;

    }
    ptr = ptr->next;
  }
}


void SecondLoop(){
  float cohesionWeight = 0.01;
  float alignWeight = 0.1;
  float avoidanceWeight = 1.5;//2.2;//0.00128;
  float foodWeight = 0.0003;
  float foodVelV;
  float foodVelH;

  float len;

  SpritePtr ptr;
  SpritePtr ptr2;
  int i,j;
  ptr = gSpriteRoot;
  for(i = 0; i < nrOfSheeps; i++) {
    if(ptr->busig) {
						if(Distance(ptr->busH, ptr->busV, ptr->position.h, ptr->position.v) < 50.0){	
              ptr->busV = gHeight*randomFloat(); 
              ptr->busH = gWidth*randomFloat(); 
            }
            ptr->speed.v = 0.01*(ptr->busV-ptr->position.v);
            ptr->speed.h = 0.01*(ptr->busH-ptr->position.h);
			
		} else {
     foodVelH = -ptr->position.h + food->position.h;
     foodVelV = -ptr->position.v + food->position.v;

    if(sqrt(foodVelH*foodVelH + foodVelV*foodVelV) < 100.0) { 
      food->position.v = gHeight*randomFloat();
      food->position.h = gWidth*randomFloat();

    }

		ptr->speed.v += speedDiff[i].v*alignWeight + averagePos[i].v*cohesionWeight + avoidanceVec[i].v*avoidanceWeight + foodWeight*foodVelV;
    ptr->speed.h += speedDiff[i].h*alignWeight + averagePos[i].h*cohesionWeight + avoidanceVec[i].h*avoidanceWeight + foodWeight*foodVelH;

		printf("coes: %f %f, avoid: %f %f, allign: %f %f \n", averagePos[i].v,averagePos[i].h , avoidanceVec[i].v, avoidanceVec[i].h, speedDiff[i].h, speedDiff[i].v);


    len = sqrt(ptr->speed.v*ptr->speed.v + ptr->speed.h*ptr->speed.h);

    if(len <= 3) {
      len = 1.0;
    }

    ptr->speed.v /= len;
    ptr->speed.h /= len;
		}
    ptr = ptr->next;
  
}

}



void SpriteBehavior() // Din kod!
{
	FirstLoop();
	SecondLoop();	
}



// Drawing routine
void Display()
{
  SpritePtr sp;

  glClearColor(0, 0, 0.2, 1);
  glClear(GL_COLOR_BUFFER_BIT+GL_DEPTH_BUFFER_BIT);
  glEnable(GL_TEXTURE_2D);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  DrawBackground();

  SpriteBehavior(); // Din kod!

  // Loop though all sprites. (Several loops in real engine.)
  sp = gSpriteRoot;
  do
  {
    HandleSprite(sp); // Callback in a real engine
    DrawSprite(sp);
    sp = sp->next;
  } while (sp != NULL);

  glutSwapBuffers();
}

void Reshape(int h, int v)
{
  glViewport(0, 0, h, v);
  gWidth = h;
  gHeight = v;
}

void Timer(int value)
{
  glutTimerFunc(20, Timer, 0);
  glutPostRedisplay();
}

// Example of user controllable parameter
float someValue = 0.0;

void Key(unsigned char key,
    __attribute__((unused)) int x,
    __attribute__((unused)) int y)
{
  switch (key)
  {
    case '+':
      someValue += 0.1;
      printf("someValue = %f\n", someValue);
      break;
    case '-':
      someValue -= 0.1;
      printf("someValue = %f\n", someValue);
      break;
    case 0x1b:
      exit(0);
  }
}

void Init()
{
  TextureData *sheepFace, *blackFace, *dogFace, *foodFace;

  LoadTGATextureSimple("bilder/leaves.tga", &backgroundTexID); // Bakgrund

  sheepFace = GetFace("bilder/sheep.tga"); // Ett f�r
  blackFace = GetFace("bilder/blackie.tga"); // Ett svart f�r
  dogFace = GetFace("bilder/dog.tga"); // En hund
  foodFace = GetFace("bilder/mat.tga"); // Mat
  int i;
  srand(time(NULL));
  
  food = NewSprite(foodFace, gWidth*randomFloat(), gHeight*randomFloat(), 0.0, 0.0, false);

for(i = 0; i < nrOfSheeps; i++) {
    if(i < 1) { 
      NewSprite(blackFace, gWidth*randomFloat(), gHeight*randomFloat(), i, i, true);
    } else {
      NewSprite(sheepFace, gWidth*randomFloat(), gHeight*randomFloat(), i, i, false);
    }
  }

 

  /*
     NewSprite(sheepFace, 100, 200, 1, 1);
     NewSprite(sheepFace, 200, 100, 1.5, -1);
     NewSprite(sheepFace, 250, 200, -1, 1.5);
     */
}

int main(int argc, char **argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
  glutInitWindowSize(800, 600);
  glutInitContextVersion(3, 2);
  glutCreateWindow("SpriteLight demo / Flocking");

  glutDisplayFunc(Display);
  glutTimerFunc(20, Timer, 0); // Should match the screen synch
  glutReshapeFunc(Reshape);
  glutKeyboardFunc(Key);

  InitSpriteLight();
  Init();

  glutMainLoop();
  return 0;
}
