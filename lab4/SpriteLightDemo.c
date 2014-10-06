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
// Lägg till egna globaler här efter behov.
int nrOfSheeps = 20;


float Distance(float h1, float v1, float h2, float v2) {
	return sqrt((h2-h1)*(h2-h1) + (v2-v1)*(v2-v1));
}

void SpriteBehavior() // Din kod!
{
	float totH = 0.0, totV = 0.0, avgH = 0.0, avgV = 0.0;
	int sheepCount;

	SpritePtr sp;
	SpritePtr sp1;
	
	sp = gSpriteRoot;
	sp1 = gSpriteRoot;

	float maxDist = 200.0;

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
					totH = sp->position.h; 
					totV = sp->position.v;
					sheepCount = 1;	

					for(i = 0; i < nrOfSheeps; i++){
									if(Distance(sheeps[i]->position.h, sheeps[i]->position.v, sp->position.h, sp->position.v) < maxDist){	
													totH += sheeps[i]->position.h;
													totV += sheeps[i]->position.v;
													sheepCount++;
									}
					}	

					avgH = totH/sheepCount;
					avgV = totV/sheepCount;
					sp->speed.h = 0.01*(avgH - sp->position.h);
					sp->speed.v = 0.01*(avgV - sp->position.v);
					sp = sp->next;
	} while (sp != NULL);

	// Lägg till din labbkod här. Det går bra att ändra var som helst i
	// koden i övrigt, men mycket kan samlas här. Du kan utgå från den
	// globala listroten, gSpriteRoot, för att kontrollera alla sprites
	// hastigheter och positioner, eller arbeta från egna globaler.
}


float randomFloat(){
				return (float)rand()/(float)RAND_MAX;
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

				sheepFace = GetFace("bilder/sheep.tga"); // Ett får
				blackFace = GetFace("bilder/blackie.tga"); // Ett svart får
				dogFace = GetFace("bilder/dog.tga"); // En hund
				foodFace = GetFace("bilder/mat.tga"); // Mat
				int i;
				srand(time(NULL));
				for(i = 0; i < nrOfSheeps; i++) {
								if(i < 4) { 
									NewSprite(blackFace, gWidth*randomFloat(), gHeight*randomFloat(), i*0.1, i*0.1);
								} else {
									NewSprite(sheepFace, gWidth*randomFloat(), gHeight*randomFloat(), i*0.1, i*0.1);
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
