/*********
   CTIS164 - Template Source Program
----------
STUDENT : Hüseyin Deniz ULU 22102749
SECTION : 01
HOMEWORK: 03
----------
PROBLEMS: -
----------
ADDITIONAL FEATURES: 
-Multiple scenes
-Two rotatable complex objects(cop has color switch in its emergency lightning)
-A storyline(sort of)
-Two different endings
-Player based movable object(thief)
*********/

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>
#include "vec.h"

#define WINDOW_WIDTH  1000
#define WINDOW_HEIGHT 800

#define TIMER_PERIOD    17 // Period for the timer.
#define TIMER_ON         1 // 0:disable timer, 1:enable timer

#define D2R 0.0174532

#define THIEVE_SPEED 4
#define COP_SPEED 3


/* Global Variables for Template File */
bool up = false, down = false, right = false, left = false;
int  winWidth, winHeight; // current Window width and height

typedef struct {
	vec_t pos;
	bool  visibility;
	float angle;
	vec_t vel;  //velocity
} player_t;

player_t cop = { {-2000 , -200}, 0, 90};
player_t thief = { {0 , 400} , 1};

typedef struct {
	
	vec_t pos;
	float size;
	float alpha;

} symbol_t;



int gamemode = 0;
double time = 20;

bool space = 0;
int won = -1;//0=cop 1=thieve
polar_t temp;

//
// to draw circle, center at (x,y)
// radius r
//
void circle(int x, int y, int r)
{
#define PI 3.1415
	float angle;
	glBegin(GL_POLYGON);
	for (int i = 0; i < 100; i++)
	{
		angle = 2 * PI * i / 100;
		glVertex2f(x + r * cos(angle), y + r * sin(angle));
	}
	glEnd();
}

void circle_wire(int x, int y, int r)
{
#define PI 3.1415
	float angle;

	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < 100; i++)
	{
		angle = 2 * PI * i / 100;
		glVertex2f(x + r * cos(angle), y + r * sin(angle));
	}
	glEnd();
}

void print(int x, int y, const char* string, void* font)
{
	int len, i;

	glRasterPos2f(x, y);
	len = (int)strlen(string);
	for (i = 0; i < len; i++)
	{
		glutBitmapCharacter(font, string[i]);
	}
}

// display text with variables.
// vprint(-winWidth / 2 + 10, winHeight / 2 - 20, GLUT_BITMAP_8_BY_13, "ERROR: %d", numClicks);
void vprint(int x, int y, void* font, const char* string, ...)
{
	va_list ap;
	va_start(ap, string);
	char str[1024];
	vsprintf_s(str, string, ap);
	va_end(ap);

	int len, i;
	glRasterPos2f(x, y);
	len = (int)strlen(str);
	for (i = 0; i < len; i++)
	{
		glutBitmapCharacter(font, str[i]);
	}
}

// vprint2(-50, 0, 0.35, "00:%02d", timeCounter);
void vprint2(int x, int y, float size, const char* string, ...) {
	va_list ap;
	va_start(ap, string);
	char str[1024];
	vsprintf_s(str, string, ap);
	va_end(ap);
	glPushMatrix();
	glTranslatef(x, y, 0);
	glScalef(size, size, 1);

	int len, i;
	len = (int)strlen(str);
	for (i = 0; i < len; i++)
	{
		glutStrokeCharacter(GLUT_STROKE_ROMAN, str[i]);
	}
	glPopMatrix();
}

void vertex(vec_t P, vec_t Tr, double angle) {
	double xp = (P.x * cos(angle) - P.y * sin(angle)) + Tr.x;
	double yp = (P.x * sin(angle) + P.y * cos(angle)) + Tr.y;
	glVertex2d(xp, yp);
}

void drawCop()
{
	glColor3ub(255, 255, 255);
	glBegin(GL_POLYGON);
	vertex({ 40,20 }, cop.pos, cop.angle * D2R);
	vertex({ -40,20 }, cop.pos, cop.angle * D2R);
	vertex({ -40,-20 }, cop.pos, cop.angle * D2R);
	vertex({ 40, -20 }, cop.pos, cop.angle * D2R);
	glEnd();

	if( (int)(2*time) % 2 == 1){//
		glColor3ub(255, 0, 0);
		glBegin(GL_POLYGON);
		vertex({ 5,15 }, cop.pos, cop.angle * D2R);
		vertex({ -5, 15 }, cop.pos, cop.angle * D2R);
		vertex({ -5, 0 }, cop.pos, cop.angle * D2R);
		vertex({ 5, 0 }, cop.pos, cop.angle * D2R);
		glEnd();
		glColor3ub(196, 196, 196);
		glBegin(GL_POLYGON);
		vertex({ 5,-15 }, cop.pos, cop.angle * D2R);
		vertex({ -5, -15 }, cop.pos, cop.angle * D2R);
		vertex({ -5, 0 }, cop.pos, cop.angle * D2R);
		vertex({ 5, 0 }, cop.pos, cop.angle * D2R);
		glEnd();
	}
	else{
		glColor3ub(0, 0, 255);
		glBegin(GL_POLYGON);
		vertex({ 5,-15 }, cop.pos, cop.angle * D2R);
		vertex({ -5, -15 }, cop.pos, cop.angle * D2R);
		vertex({ -5, 0 }, cop.pos, cop.angle * D2R);
		vertex({ 5, 0 }, cop.pos, cop.angle * D2R);
		glEnd();

		glColor3ub(196, 196, 196);
		glBegin(GL_POLYGON);
		vertex({ 5,15 }, cop.pos, cop.angle * D2R);
		vertex({ -5, 15 }, cop.pos, cop.angle * D2R);
		vertex({ -5, 0 }, cop.pos, cop.angle * D2R);
		vertex({ 5, 0 }, cop.pos, cop.angle * D2R);
		glEnd();

	}

	glColor3ub(0, 204, 204);
	glBegin(GL_POLYGON);//front window
	vertex({ 30,15 }, cop.pos, cop.angle * D2R);
	vertex({ 7,13 }, cop.pos, cop.angle * D2R);
	vertex({ 7,-13 }, cop.pos, cop.angle * D2R);
	vertex({ 30,-15 }, cop.pos, cop.angle * D2R);
	glEnd();

	glColor3ub(0, 204, 204);
	glBegin(GL_POLYGON);//rear window
	vertex({ -35,15 }, cop.pos, cop.angle * D2R);
	vertex({ -7,13 }, cop.pos, cop.angle * D2R);
	vertex({ -7,-13 }, cop.pos, cop.angle * D2R);
	vertex({ -35,-15 }, cop.pos, cop.angle * D2R);
	glEnd();

	glColor3ub(255, 255, 0);
	glBegin(GL_POLYGON);//right light
	vertex({ 37,5 }, cop.pos, cop.angle * D2R);
	vertex({ 37,15 }, cop.pos, cop.angle * D2R);
	vertex({ 42,15 }, cop.pos, cop.angle * D2R);
	vertex({ 42,5 }, cop.pos, cop.angle * D2R);
	glEnd();


	glColor3ub(255, 255, 0);
	glBegin(GL_POLYGON);//left light
	vertex({ 37,-5 }, cop.pos, cop.angle * D2R);
	vertex({ 37,-15 }, cop.pos, cop.angle * D2R);
	vertex({ 42,-15 }, cop.pos, cop.angle * D2R);
	vertex({ 42,-5 }, cop.pos, cop.angle * D2R);
	glEnd();
	
}

void drawThief()
{
	glColor3ub(255, 0, 0);
	glBegin(GL_POLYGON);//base
	vertex({ 40,20 }, thief.pos, thief.angle * D2R);
	vertex({ -40,20 }, thief.pos, thief.angle * D2R);
	vertex({ -40,-20 }, thief.pos, thief.angle * D2R);
	vertex({ 40, -20 }, thief.pos, thief.angle * D2R);
	glEnd();


	glColor3ub(128, 128, 128);
	glBegin(GL_POLYGON);//front window
	vertex({ 30,15}, thief.pos, thief.angle * D2R);
	vertex({ 7,13 }, thief.pos, thief.angle * D2R);
	vertex({ 7,-13 }, thief.pos, thief.angle * D2R);
	vertex({ 30,-15 }, thief.pos, thief.angle * D2R);
	glEnd();

	glColor3ub(128,128,128);
	glBegin(GL_POLYGON);//front window
	vertex({ -35,15 }, thief.pos, thief.angle * D2R);
	vertex({ -7,13 }, thief.pos, thief.angle * D2R);
	vertex({ -7,-13 }, thief.pos, thief.angle * D2R);
	vertex({ -35,-15 }, thief.pos, thief.angle * D2R);
	glEnd();

	glColor3ub(255, 255, 0);
	glBegin(GL_POLYGON);//right light
	vertex({ 37,5 }, thief.pos, thief.angle * D2R);
	vertex({ 37,15 }, thief.pos, thief.angle * D2R);
	vertex({ 42,15 }, thief.pos, thief.angle * D2R);
	vertex({ 42,5 }, thief.pos, thief.angle * D2R);
	glEnd();


	glColor3ub(255, 255, 0);
	glBegin(GL_POLYGON);//left light
	vertex({ 37,-5 }, thief.pos, thief.angle * D2R);
	vertex({ 37,-15 }, thief.pos, thief.angle * D2R);
	vertex({ 42,-15 }, thief.pos, thief.angle * D2R);
	vertex({ 42,-5 }, thief.pos, thief.angle * D2R);
	glEnd();

}

void drawBaton(int x,int y, int sizeB)
{
	glColor3ub(0, 0, 0);
	glBegin(GL_POLYGON);
	glVertex2f(x,y);
	glVertex2f(x+(-0.4*sizeB),y+(0.4*sizeB));
	glVertex2f(x+(-0.7*sizeB),y+(0.2*sizeB));
	glVertex2f(x+(-0.2*sizeB),y+(-0.3*sizeB));
	glVertex2f(x+(-0.4*sizeB),y+(-0.6*sizeB));
	glVertex2f(x,y+(-1*sizeB));
	glVertex2f(x+(sizeB),y+(0.6*sizeB));
	glVertex2f(x+(0.7*sizeB),y+(sizeB));
	glEnd();

}

void drawMask(int x, int y, int sizeM)
{
	glColor3ub(0, 0, 0);
	circle(x, y, 2*sizeM / 3);
	glRectf(x+ 2 * sizeM / 3,y,x- 2 * sizeM / 3,y-sizeM);
	glColor3ub(96,96,96);
	circle(x+sizeM/3,y,sizeM/4);
	circle(x-sizeM/3,y,sizeM/4);
	circle(x,y- 5* sizeM / 8,sizeM/3);
	
}

void introBackground()
{
	glColor3ub(64,64,64);
	glRectf(-500, -400, 500, 400);
	glColor3ub(0,0,0);
	glRectf(-100, -400, 100, 400);

	glColor3ub(255, 255, 255);
	int i;
	for(i= 420;i>-700;i-=100)
		glRectf(10, i, -10, i-50);

	glColor3ub(204,153,153);
	glRectf(-350, 470, -120, 350);
	glColor3ub(255, 204, 153);
	glRectf(-400, 310, -120, 120);
	glColor3ub(204, 153, 153);
	glRectf(-300, 90, -120, -140);
	glColor3ub(255, 204, 153);
	glRectf(-395, -170, -120, -500);

	glColor3ub(204, 153, 153);
	glRectf(300, 470, 120, 300);
	glColor3ub(255, 204, 153);
	glRectf(230, 270, 120, 170);
	glColor3ub(204, 153, 153);
	glRectf(380, 140, 120, -200);
	glColor3ub(255, 204, 153);
	glRectf(435, -230, 120, -400);

}

void introMessage()
{
	glColor4ub(196,196,196,245);
	glRectf(-350,-150,350,-350);
	glColor3ub(0, 0, 0);
	vprint2(-325, -200, 0.18, "Attention all units! A local officer reported a bank heist.");
	vprint2(-195, -245, 0.2, "Near units are being waited!");
	vprint2(-125, -330, 0.15, "Press <space> to skip.");
	drawBaton(-300, -300, 50);
}

void mainBackground()
{
	glColor3ub(64, 64, 64);
	glRectf(-500, -400, 500, 400);
	glColor3ub(0, 0, 0);
	glRectf(-450, -400, 450, 400);

	glColor3ub(255, 255, 255);
	int i;
	for (i = 420; i > -700; i -= 100)
	{
		glRectf(385, i, 365, i - 50);
		glRectf(235, i, 215, i - 50);
		glRectf(85, i, 65, i - 50);
		glRectf(-385, i, -365, i - 50);
		glRectf(-235, i, -215, i - 50);
		glRectf(-85, i, -65, i - 50);
	}
}

void copMessage()
{
	glColor4ub(196, 196, 196, 224);
	glRectf(-350, 375, 350, 275);
	glColor3ub(0, 0, 0);
	vprint2(-250, 335, 0.2, "Click green area to summon a unit.");
	vprint2(-200, 305, 0.2, "Then press <space> to skip ");
	drawBaton(-310, 315, 30);


	glColor4ub(0, 196, 0, 196);
	glRectf(-250, -250, 250, -350);
}

void meanwhile() {
	glColor3ub(255, 255, 255);
	vprint2(-310, -200, 1, "Meanwhile...");
}

void thiefMessage() {
	glColor3ub(196, 196, 196);
	glRectf(-350, -100, 350, -300);
	glColor3ub(0, 0, 0);
	vprint2(-245, -130, 0.2, "Oh @!$# They are going to block us!");
	vprint2(-230, -175, 0.2, "We should bypass them to escape.");
	vprint2(-125, -260, 0.15, "Press <space> to skip.");
	drawMask(-300,-225,50);

}

void winnerSide() {
	
	glColor3ub(196,196,196);
	glRectf(-500, -400, 500, 400);
	
	if (won==0){
		drawBaton(0, 0, 200);
		glColor3ub(0, 0, 0);
		vprint2(-200, -300, 0.6, "Cops Won");
	}
	if (won==1){
		drawMask(0, 50, 200);
		glColor3ub(0, 0, 0);
		vprint2(-225, -300, 0.6, "Thieves Won");
	}

	vprint2(-190, -375, 0.2, "Press <space> to try again.");

}

void resetLocs()
{
	cop = { {-2000 , -200}, 0, 90 };
	thief = { {0 , 400} , 1 };


}

void drawSwitch() {

	switch (gamemode) {
	case 0:
		
		introBackground();
		drawThief();
		introMessage();
		
		break;
	case 1:

		mainBackground();
		copMessage();
		drawCop();
		
		break;

	case 2:
		meanwhile();
		break;
	case 3:
		mainBackground();
		drawCop();
		drawThief();
		thiefMessage();
		break;
	case 4:mainBackground();
		drawCop();
		drawThief();
		break;
		
	case 5:
		winnerSide();
		break;
	}


}

//
// To display onto window using OpenGL commands
//
void display() {
	//
	// clear window to black
	//
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT);

	drawSwitch();

	glutSwapBuffers();
}

//
// key function for ASCII charachters like ESC, a,b,c..,A,B,..Z
//
void onKeyDown(unsigned char key, int x, int y)
{
	// exit when ESC is pressed.
	if (key == 27)
		exit(0);
	if (key == ' ' && gamemode == 0)
		gamemode = 1;
	else if (key == ' ' && gamemode == 1 && cop.visibility==1)
		gamemode = 2;	
	else if (key == ' ' && gamemode == 2)
		gamemode = 3;
	else if (key == ' ' && gamemode == 3)
		gamemode = 4;
	else if (key == ' ' && gamemode == 5)
		gamemode = 0;


	// to refresh the window it calls display() function
	glutPostRedisplay();
}

void onKeyUp(unsigned char key, int x, int y)
{
	// exit when ESC is pressed.
	if (key == 27)
		exit(0);



	// to refresh the window it calls display() function
	glutPostRedisplay();
}

//
// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
//
void onSpecialKeyDown(int key, int x, int y)
{
	// Write your codes here.
	switch (key) {
	case GLUT_KEY_UP:
		up = true;
		break;
	case GLUT_KEY_DOWN:
		down = true;
		break;
	case GLUT_KEY_LEFT:
		left = true;
		break;
	case GLUT_KEY_RIGHT:
		right = true;
		break;
	}

	// to refresh the window it calls display() function
	glutPostRedisplay();
}

//
// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
//
void onSpecialKeyUp(int key, int x, int y)
{
	// Write your codes here.
	switch (key) {
	case GLUT_KEY_UP:
		up = false;
		break;
	case GLUT_KEY_DOWN:
		down = false;
		break;
	case GLUT_KEY_LEFT:
		left = false;
		break;
	case GLUT_KEY_RIGHT:
		right = false;
		break;
	}

	// to refresh the window it calls display() function
	glutPostRedisplay();
}

//
// When a click occurs in the window,
// It provides which button
// buttons : GLUT_LEFT_BUTTON , GLUT_RIGHT_BUTTON
// states  : GLUT_UP , GLUT_DOWN
// x, y is the coordinate of the point that mouse clicked.
//
// GLUT to OpenGL coordinate conversion:
//   x2 = x1 - winWidth / 2
//   y2 = winHeight / 2 - y1
void onClick(int button, int stat, int x, int y)
{
	// Write your codes here.

	int mx = x - winWidth / 2,
		my = winHeight / 2 - y;//glut to gl
	

	if (mx > -250 && mx < 250 && my > -350 && my < -250 && gamemode==1  && button == GLUT_LEFT_BUTTON && stat == GLUT_DOWN) {
		cop.pos.x = mx;
		cop.pos.y = my;
		cop.visibility = 1;
	}

	// to refresh the window it calls display() function
	glutPostRedisplay();
}

// This function is called when the window size changes.
// w : is the new width of the window in pixels.
// h : is the new height of the window in pixels.

void onResize(int w, int h)
{
	winWidth = w;
	winHeight = h;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-w / 2, w / 2, -h / 2, h / 2, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	display(); // refresh window.
}

void onMoveDown(int x, int y) {
	// Write your codes here.



	// to refresh the window it calls display() function   
	glutPostRedisplay();
}

// GLUT to OpenGL coordinate conversion:
//   x2 = x1 - winWidth / 2
//   y2 = winHeight / 2 - y1
void onMove(int x, int y) {
	// Write your codes here.



	// to refresh the window it calls display() function
	glutPostRedisplay();
}

#if TIMER_ON == 1
void onTimer(int v) {

	glutTimerFunc(TIMER_PERIOD, onTimer, 0);
	// Write your codes here.

	switch (gamemode) {
	case 0:
		thief.angle = 270;
		if (thief.pos.y > -800)
			thief.pos.y -=THIEVE_SPEED;
		break;
	case 1:
		if (cop.visibility && space)
			gamemode++;
		break;
	case 3:
		thief.pos.x = 0;
		thief.pos.y = 375;
		if (space)
			gamemode++;
		break;
	case 4:

		cop.pos = addV(cop.pos, cop.vel);
		cop.vel = mulV(COP_SPEED, unitV(subV(thief.pos, cop.pos)));
		temp = rec2pol(cop.vel);
		cop.angle = temp.angle;
		if (time > 0)
			time -= TIMER_PERIOD / 1000.;
		if (left)
			thief.angle -= 2;
		if (right)
			thief.angle += 2;
		if (up && thief.pos.x <= 400 && thief.pos.x >= -400 && thief.pos.y <= 550)
			thief.pos = addV(thief.pos, pol2rec({ THIEVE_SPEED, thief.angle }));
		if (down && thief.pos.x <= 400 && thief.pos.x >= -400 && thief.pos.y <= 550)
			thief.pos = subV(thief.pos, pol2rec({ THIEVE_SPEED, thief.angle }));

		if (fabs(cop.pos.x - thief.pos.x) < 20 && fabs(cop.pos.y - thief.pos.y) < 40)
		{
			won = 0;
			gamemode++;
		}
		if(thief.pos.y < -420)
		{
			won = 1;
			gamemode++;
		}
		break;
	case 5:
		resetLocs();
	}
	
	
	// to refresh the window it calls display() function
	glutPostRedisplay(); // display()

}
#endif

void Init() {

	// Smoothing shapes
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


}

void main(int argc, char* argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	//glutInitWindowPosition(100, 100);
	glutCreateWindow("Hot Pursuit by H.Deniz ULU");

	glutDisplayFunc(display);
	glutReshapeFunc(onResize);

	//
	// keyboard registration
	//
	glutKeyboardFunc(onKeyDown);
	glutSpecialFunc(onSpecialKeyDown);

	glutKeyboardUpFunc(onKeyUp);
	glutSpecialUpFunc(onSpecialKeyUp);

	//
	// mouse registration
	//
	glutMouseFunc(onClick);
	glutMotionFunc(onMoveDown);
	glutPassiveMotionFunc(onMove);

#if  TIMER_ON == 1
	// timer event
	glutTimerFunc(TIMER_PERIOD, onTimer, 0);
#endif

	Init();

	glutMainLoop();
}