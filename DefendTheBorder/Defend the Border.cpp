/*********
   CTIS164 - Template Source Program
----------
STUDENT : Hüseyin Deniz ULU
SECTION : 01
HOMEWORK: 02
----------
PROBLEMS: -
----------
ADDITIONAL FEATURES:
Cannon is rotatable and can do shots in angle
Can see multiple shots in the same frame
Starting and finishing messages
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
#include <time.h>

#define WINDOW_WIDTH  1000
#define WINDOW_HEIGHT 800

#define TIMER_PERIOD  17 // Period for the timer.
#define TIMER_ON       1 // 0:disable timer, 1:enable timer

#define D2R 0.0174532

#define BULMAX 10
#define FIRE_PERIOD 20
#define MAXENM 5
#define TIME 20

/* Global Variables for Template File */
bool up = false, down = false, right = false, left = false;
int  winWidth = WINDOW_WIDTH,
winHeight = WINDOW_HEIGHT; // current Window width and height

double unit = 30;//for the integrity of the project
double angle = 0;//angle of player and bullet

typedef struct p {
	double x, y;
} point_t;

typedef struct player {
	point_t pos;
}player_t;

double initX = -winWidth / 2 + 3.5 * unit;
double initBx = -winWidth / 2 + 3.5 * unit - 11;

player_t p = { {initX , 0} };//intializing the start point and angle of the player

typedef struct bullet {
	point_t bulPos = { initBx, 0 };
	double bulRad = unit / 2;
	double bulAng = angle;
	bool moving = false;

}bullet_t;

bullet_t b[BULMAX];

typedef struct tank {
	point_t pos;
}tank_t;

tank_t t[MAXENM] = { winWidth - 3 * unit,-250 };

int currentEnemy = 1;
int totalEnemy = 0;
int fireSpeed = 0;
bool space = false;
double remTime = TIME;
int hit = 0;

bool animation = 0;
int mode = 0;
double perc;

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

void drawBackground() {
	glColor3ub(192, 192, 192);
	glRectf(-winWidth / 2, -winHeight / 2, winWidth / 2, winHeight / 2);//grass


}

void drawTower() {
	glColor3ub(204, 102, 0);
	glRectf(-winWidth / 2 + 2 * unit, -winHeight / 2 + unit, -winWidth / 2 + 2.4 * unit, winHeight / 2 - 1.3 * unit);//left foot of tower
	glRectf(-winWidth / 2 + 4.5 * unit, -winHeight / 2 + unit, -winWidth / 2 + 4.9 * unit, winHeight / 2 - 1.3 * unit);//right foot of tower


	for (int i = -winHeight / 2 + 2 * unit; i < winHeight / 2 - 3 * unit; i += 4 * unit) {//ladder
		glLineWidth(4);
		glBegin(GL_LINES);
		glVertex2f(-winWidth / 2 + 2.4 * unit, i);
		glVertex2f(-winWidth / 2 + 4.5 * unit, i + 2.3 * unit);
		glVertex2f(-winWidth / 2 + 2.4 * unit, i + 2.3 * unit);
		glVertex2f(-winWidth / 2 + 4.5 * unit, i);
		glEnd();
		glLineWidth(1);
	}
}

void vertex(point_t P, point_t pos, double angle) {
	double xp = (P.x * cos(angle) - P.y * sin(angle)) + -winWidth / 2 + 3.2 * unit;
	double yp = (P.x * sin(angle) + P.y * cos(angle)) + pos.y;
	glVertex2d(xp, yp);
}

void drawPlayer() {

	glColor3ub(128, 128, 128);
	glLineWidth(10);
	glBegin(GL_LINES);
	glVertex2f(-winWidth / 2 + 2.8 * unit, p.pos.y - 45);
	glVertex2f(-winWidth / 2 + 3.4 * unit, p.pos.y - 12);
	glVertex2f(-winWidth / 2 + 3.4 * unit, p.pos.y - 12);
	glVertex2f(-winWidth / 2 + 4 * unit, p.pos.y - 45);
	glEnd();//stand of cannon

	glColor3ub(0, 0, 0);

	glBegin(GL_POLYGON);
	vertex({ 7,18 }, p.pos, angle * D2R);
	vertex({ 54,12 }, p.pos, angle * D2R);
	vertex({ 54,-12 }, p.pos, angle * D2R);
	vertex({ 7, -18 }, p.pos, angle * D2R);
	glEnd();//turning cannon
	circle(-winWidth / 2 + 3.2 * unit, p.pos.y, 20);//cannon

	glColor3ub(128, 128, 128);
	glRectf(-winWidth / 2 + 2 * unit, p.pos.y - 40, -winWidth / 2 + 4.9 * unit, p.pos.y - 55);

	glColor3ub(75, 75, 75);
	circle(-winWidth / 2 + 2.2 * unit, p.pos.y - 47.5, 10);
	circle(-winWidth / 2 + 4.7 * unit, p.pos.y - 47.5, 10);//base


}

void drawTank(tank_t t)//int rnd)
{
	int tUnit = 30;

	glColor3ub(0, 51, 0);
	glRectf(t.pos.x - 3 * tUnit, t.pos.y - tUnit / 2, t.pos.x + 3 * tUnit, t.pos.y + tUnit / 2);
	glBegin(GL_POLYGON);
	glVertex2f(t.pos.x - 2 * tUnit, t.pos.y + tUnit / 2);
	glVertex2f(t.pos.x - tUnit, t.pos.y + 2 * tUnit);
	glVertex2f(t.pos.x + tUnit, t.pos.y + 2 * tUnit);
	glVertex2f(t.pos.x + 2 * tUnit, t.pos.y + tUnit / 2);
	glEnd();
	glRectf(t.pos.x - 3.5 * tUnit, t.pos.y + 1.3 * tUnit, t.pos.x, t.pos.y + 1.7 * tUnit);
	glColor3ub(0, 0, 0);
	circle(t.pos.x - 2.5 * tUnit, t.pos.y - 1.2 * tUnit, 2 * tUnit / 3);
	circle(t.pos.x + 2.5 * tUnit, t.pos.y - 1.2 * tUnit, 2 * tUnit / 3);
	glRectf(t.pos.x + 2.5 * tUnit, t.pos.y - 1.9 * tUnit, t.pos.x - 2.5 * tUnit, t.pos.y - 0.5 * tUnit);

}

int canFire() {
	int i;
	for (i = 0; i < BULMAX; i++) {
		if (b[i].moving == false)
			return (i);
	}
	return (-1);
}

void drawBullet(int i) {
	glColor3ub(128, 0, 0);
	circle(b[i].bulPos.x, b[i].bulPos.y, b[i].bulRad);
}


void initTank(tank_t* t)
{
	int rnd = rand();
	t->pos.x = winWidth / 2;
	t->pos.y = -winHeight / 2 + 5 * unit + rand() % 15 * unit;
}
void finInfo()
{
	glColor3ub(64, 64, 64);
	glRectf(-winWidth / 3, -winHeight / 3, winWidth / 3, winHeight / 3);
	glColor3ub(255, 255, 255);

	perc = hit / (totalEnemy * 1.0);

	if (perc == 0) {
		vprint2(-winWidth / 3 + 90, winHeight / 3 - 70, 0.2, "Your percentage was  %.1f/100", perc * 100);
		vprint2(-winWidth / 3 + 90, winHeight / 3 - 120, 0.2, "WHAT ARE YOU DOING SOLDIER!?");
	}
	else if (perc < 0.5 && perc > 0) {
		vprint2(-winWidth / 3 + 90, winHeight / 3 - 70, 0.2, "Your percentage was  %.1f/100", perc * 100);
		vprint2(-winWidth / 3 + 90, winHeight / 3 - 120, 0.2, "COME ON YOU CAN DO BETTER");
	}
	else if (perc < 1 && perc >= 0.5) {
		vprint2(-winWidth / 3 + 90, winHeight / 3 - 70, 0.2, "Your percentage was  %.1f/100", perc * 100);
		vprint2(-winWidth / 3 + 90, winHeight / 3 - 120, 0.2, "NOT BAD SOLDIER. NOT BAD");
	}
	else if (perc == 1) {
		vprint2(-winWidth / 3 + 90, winHeight / 3 - 70, 0.2, "Your percentage was  %.1f/100", perc * 100);
		vprint2(-winWidth / 3 + 90, winHeight / 3 - 120, 0.2, "A PERFECT ONE SOLDIER. BULLSEYE!");
	}
	else if (perc > 1) {
		vprint2(-winWidth / 3 + 90, winHeight / 3 - 70, 0.2, "Your percentage was  %.1f/100", perc * 100);
		vprint2(-winWidth / 3 + 120, winHeight / 3 - 120, 0.2, "HOW DID YOU DO THAT??");
	}

	vprint2(-winWidth / 3 + 90, -winHeight / 3 + 100, 0.2, "PRESS <F1> TO GIVE ANOTHER TRY");

}

void startInfo()
{
	glColor3ub(64, 64, 64);
	glRectf(-winWidth / 3, -winHeight / 3, winWidth / 3, winHeight / 3);
	glColor3ub(255, 255, 255);


	vprint2(-winWidth / 3 + 90, winHeight / 3 - 60, 0.2, "WAKE UP SOLDIER TIME HAS COME!");
	vprint2(-winWidth / 3 + 150, -winHeight / 3 + 100, 0.2, "PRESS <F1> TO START");




}

void resetAll()
{
	currentEnemy = 1;
	totalEnemy = 0;
	fireSpeed = 0;
	space = false;
	remTime = TIME;
	hit = 0;
	angle = 0;

	for (int i = 0; i < MAXENM; i++)
		t[i] = { winWidth - 3 * unit,-250 };
	for (int j = 0; j < BULMAX; j++)
	{
		b[j].bulAng = angle;
		b[j].bulRad = unit / 2;
		b[j].moving = false;
		b[j].bulPos.x = initBx;
		b[j].bulPos.y = 0;
	}
	p = { {initX , 0} };
}

void drawInfo()
{
	glColor3ub(96, 96, 96);
	glRectf(winWidth / 2, winHeight / 2, -winWidth / 2, winHeight / 2 - 100);
	glLineWidth(8);
	glColor3ub(0, 0, 0);
	glBegin(GL_LINE_LOOP);
	glVertex2f(winWidth / 2, winHeight / 2);
	glVertex2f(winWidth / 2, winHeight / 2 - 100);
	glVertex2f(-winWidth / 2, winHeight / 2 - 100);
	glVertex2f(-winWidth / 2, winHeight / 2);
	glEnd();
	glBegin(GL_LINES);
	glVertex2f(winWidth / 2 - 333, winHeight / 2);
	glVertex2f(winWidth / 2 - 333, winHeight / 2 - 100);
	glVertex2f(-winWidth / 2 + 333, winHeight / 2);
	glVertex2f(-winWidth / 2 + 333, winHeight / 2 - 100);

	glEnd();
	glLineWidth(1);

	vprint2(-winWidth / 2 + 60, winHeight / 2 - 40, 0.2, "Remaining Time");
	vprint2(-winWidth / 2 + 120, winHeight / 2 - 80, 0.2, "%.2f", remTime);
	vprint2(-winWidth / 2 + 410, winHeight / 2 - 40, 0.2, "Total Enemies");
	vprint2(-winWidth / 2 + 490, winHeight / 2 - 80, 0.2, "%d", totalEnemy);
	vprint2(-winWidth / 2 + 750, winHeight / 2 - 40, 0.2, "Hit Enemies", hit);
	vprint2(-winWidth / 2 + 820, winHeight / 2 - 80, 0.2, "%d", hit);

	glColor3ub(0, 0, 0);
	glRectf(winWidth / 2, -winHeight / 2, -winWidth / 2, -winHeight / 2 + 40);
	glColor3ub(255, 255, 255);
	vprint2(-winWidth / 2 + 10, -winHeight / 2 + 10, 0.15, "<Space> for shooting,  <up> or <down> or <right> or <left> for aiming, <F1> to give a break");
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

	drawBackground();
	drawTower();

	for (int i = 0; i < BULMAX; i++)
		if (b[i].moving)
			drawBullet(i);
	drawPlayer();

	for (int i = 0; i < currentEnemy; i++)
		drawTank(t[i]);

	drawInfo();
	if (mode == 0)
		startInfo();

	if (remTime <= 0)
	{
		finInfo();
	}

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
	if (key == ' ')
		space = true;



	// to refresh the window it calls display() function
	glutPostRedisplay();
}

void onKeyUp(unsigned char key, int x, int y)
{
	// exit when ESC is pressed.
	if (key == 27)
		exit(0);
	if (key == ' ')
		space = false;


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
	case GLUT_KEY_F1:
		animation = !animation;
		if (remTime <= 0)
			resetAll();
		mode = 1;
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



	// to refresh the window it calls display() function
	glutPostRedisplay();
}

//
// This function is called when the window size changes.
// w : is the new width of the window in pixels.
// h : is the new height of the window in pixels.
//
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
	if (remTime < 0)
	{
		animation = false;
		remTime = 0;
	}
	if (animation && mode)
	{
		if (remTime > 0)
			remTime -= TIMER_PERIOD / 1000.;

		if (right && angle > -30) {
			angle -= 2;
		}

		if (left && angle < 50) {
			angle += 2;
		}

		if (up && p.pos.y < winHeight / 2 - 5 * unit) {
			p.pos.y += 2;
		}


		if (down && p.pos.y > -winHeight / 2 + 6 * unit) {
			p.pos.y -= 2;
		}

		if (space && fireSpeed == 0)
		{
			int canFired = canFire();
			if (canFired != -1) {
				b[canFired].bulPos.x = initX - 11;
				b[canFired].bulPos.y = p.pos.y;
				b[canFired].bulAng = angle;
				b[canFired].moving = true;
				fireSpeed = FIRE_PERIOD;
			}
		}
		if (fireSpeed > 0)
			fireSpeed--;


		for (int i = 0; i < BULMAX; i++) {
			if (b[i].moving)
			{
				b[i].bulPos.x += 15 * cos(b[i].bulAng * D2R);
				b[i].bulPos.y += 15 * sin(b[i].bulAng * D2R);

				if (!(b[i].bulPos.x < winWidth / 2 + b[i].bulRad && b[i].bulPos.y < winHeight / 2 + b[i].bulRad && b[i].bulPos.y > -winHeight / 2 - b[i].bulRad))
					b[i].moving = false;

				int k;
				for (k = 0; k < MAXENM; k++)
					if (fabs(b[i].bulPos.x - t[k].pos.x) < 3 * unit && fabs(b[i].bulPos.y - t[k].pos.y) < 2 * unit)
					{
						t[k].pos.y += 50 * unit;
						hit++;
					}
			}
		}

		for (int i = 0; i < currentEnemy; i++) { //for t

			if (t[i].pos.x > -winWidth / 2 - 3 * unit)
				t[i].pos.x -= 5;

			else {
				initTank(&t[i]);
				totalEnemy++;
			}

			if (currentEnemy < MAXENM && t[i].pos.x < winWidth / 2 - 200 * (currentEnemy)) {
				currentEnemy++;
				initTank(&t[currentEnemy - 1]);
				totalEnemy++;
			}


		}
	}
	// to refresh the window it calls display() function
	glutPostRedisplay(); // display()
}
#endif

void Init() {
	srand(time(NULL));

	// Smoothing shapes
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}

void main(int argc, char* argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	//glutInitWindowPosition(100, 100);
	glutCreateWindow("Defend the Border by H.Deniz ULU");




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