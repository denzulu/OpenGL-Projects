/*********
   CTIS164 - Homework01
----------
STUDENT : Hüseyin Deniz ULU 22102749
SECTION : 1
HOMEWORK: 1
----------
PROBLEMS: -
----------
ADDITIONAL FEATURES: Pressing left and right key will change the scene between day and night.
                     Pressing up and down key will change the scene between plane and ship.
                     Clicking the curtains will close or open them.
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

#define TIMER_PERIOD  25 // Period for the timer.
#define TIMER_ON       1 // 0:disable timer, 1:enable timer

#define D2R 0.0174532

/* Global Variables for Template File */
bool up = false, down = false, right = false, left = false;
int  winWidth, winHeight; // current Window width and height

/* Global Variables for This Project */
bool actTimer = true;
int wf_Height = 2 * WINDOW_HEIGHT / 3; //inner window(frame) locations
int wf_Width = 2 * WINDOW_WIDTH / 3;

int xWin = 0;//OpenGL x of center
int yWin = 0;//OpenGL y of center
int unit = 30;//measure unit for the integrity of the project(width of the frame width etc.)

int locX = xWin - wf_Width / 2 + 50;//initial locations of the main objects(car-plane-ship)
int locY = yWin;

int cloud_x[3] = { xWin - 10 * unit ,xWin ,xWin + 15 * unit };
int cloud_y = yWin;

#define S_PLANE 0
#define S_SHIP 1

#define S_DAY 10
#define S_NIGHT 11

#define CUR_CLOSE 20
#define CUR_OPEN 21

int scene = S_PLANE;
int dayNight = S_DAY;
int curtain_st = CUR_CLOSE;

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

//Functions


void pattern() {
    int y;
    //pattern outer window
    glLineWidth(1);
    glColor3f(1, 1, 1);
    glBegin(GL_LINES);

    for (y = winHeight / 2.; y > -winHeight / 2.; y -= unit)
    {
        glVertex2f(-winWidth / 2., y);
        glVertex2f(winWidth / 2., y);

    }
    glEnd();
}

void frame() {

    glColor3ub(102, 51, 0);//frame
    glRectf(xWin - wf_Width / 2, yWin + wf_Height / 2, xWin + wf_Width / 2, yWin + wf_Height / 2 - unit);//top
    glRectf(xWin - wf_Width / 2, yWin - wf_Height / 2, xWin + wf_Width / 2, yWin - wf_Height / 2 + unit);//bottom
    glRectf(xWin - wf_Width / 2, yWin + wf_Height / 2, xWin - wf_Width / 2 + unit, yWin - wf_Height / 2);//left
    glRectf(xWin + wf_Width / 2, yWin + wf_Height / 2, xWin + wf_Width / 2 - unit, yWin - wf_Height / 2);//right
    glRectf(xWin + unit, yWin - wf_Height / 2, xWin - unit, yWin + wf_Height / 2);//mid
    glEnd();

    glLineWidth(3);//outer lines of frame
    glColor3ub(0, 0, 0);

    //left inner
    glBegin(GL_LINE_LOOP);
    glVertex2f(xWin - wf_Width / 2 + unit, yWin + wf_Height / 2 - unit);
    glVertex2f(xWin - wf_Width / 2 + unit, yWin - wf_Height / 2 + unit);
    glVertex2f(xWin - unit, yWin - wf_Height / 2 + unit);
    glVertex2f(xWin - unit, yWin + wf_Height / 2 - unit);
    glEnd();

    //right inner
    glBegin(GL_LINE_LOOP);
    glVertex2f(xWin + wf_Width / 2 - unit, yWin + wf_Height / 2 - unit);
    glVertex2f(xWin + wf_Width / 2 - unit, yWin - wf_Height / 2 + unit);
    glVertex2f(xWin + unit, yWin - wf_Height / 2 + unit);
    glVertex2f(xWin + unit, yWin + wf_Height / 2 - unit);
    glEnd();

    //outer
    glBegin(GL_LINE_LOOP);
    glVertex2f(xWin - wf_Width / 2, yWin + wf_Height / 2);
    glVertex2f(xWin - wf_Width / 2, yWin - wf_Height / 2);
    glVertex2f(xWin + wf_Width / 2, yWin - wf_Height / 2);
    glVertex2f(xWin + wf_Width / 2, yWin + wf_Height / 2);
    glEnd();

    //mid
    glBegin(GL_LINES);
    glVertex2f(xWin, yWin + wf_Height / 2);
    glVertex2f(xWin, yWin - wf_Height / 2);
    glEnd();

    //handles
    circle(unit / 2, yWin, 10);
    circle(-unit / 2, yWin, 10);
}

void cloud(int c_x, int c_y)
{
    glColor3ub(255, 255, 255);
    circle(c_x, c_y, 3 * unit / 2);
    circle(c_x + unit, c_y - unit / 2, unit);
    circle(c_x - unit, c_y - unit / 2, unit);
}

void clouds() {
    //displaying clouds depending on the scene
    switch (scene) {

    case S_PLANE:
        cloud(cloud_x[0], cloud_y + unit - WINDOW_HEIGHT / 4);//plane clouds
        cloud(cloud_x[1], cloud_y + 3 * unit - WINDOW_HEIGHT / 4);
        cloud(cloud_x[2], cloud_y + (1. / 2) * unit - WINDOW_HEIGHT / 4);
        break;

    case S_SHIP:
        cloud(cloud_x[0], cloud_y + unit + WINDOW_HEIGHT / 4 - 3 * unit);//ship clouds
        cloud(cloud_x[1], cloud_y + 3 * unit + WINDOW_HEIGHT / 4 - 3 * unit);
        cloud(cloud_x[2], cloud_y + (1. / 2) * unit + WINDOW_HEIGHT / 4 - 3 * unit);
        break;
    }


}

void plane()
{
    glBegin(GL_POLYGON);
    glColor3ub(192, 192, 192);
    glVertex2f(locX - 2 * unit, locY);
    glVertex2f(locX + 2 * unit, locY);
    glVertex2f(locX + 1.6 * unit, locY + 0.6 * unit);
    glVertex2f(locX - 1.6 * unit, locY + 0.6 * unit);
    glVertex2f(locX - 2 * unit, locY + unit);
    glEnd();


    glLineWidth(0.5);
    glColor3ub(100, 100, 100);
    glBegin(GL_LINES);
    glVertex2f(locX + 1.2 * unit, locY);
    glVertex2f(locX + 1.2 * unit, locY + 0.6 * unit);
    glEnd();

    glLineWidth(0.6);
    glColor3ub(0, 0, 0);
    glBegin(GL_LINE_LOOP);
    glVertex2f(locX - 2 * unit, locY);
    glVertex2f(locX + 2 * unit, locY);
    glVertex2f(locX + 1.6 * unit, locY + 0.6 * unit);
    glVertex2f(locX - 1.6 * unit, locY + 0.6 * unit);
    glVertex2f(locX - 2 * unit, locY + unit);
    glEnd();

    glBegin(GL_POLYGON);
    glColor3ub(210, 210, 210);
    glVertex2f(locX + unit, locY + 0.3 * unit);
    glVertex2f(locX - unit, locY - 0.3 * unit);
    glVertex2f(locX - unit, locY + 0.3 * unit);
    glEnd();



    glLineWidth(0.8);
    glColor3ub(0, 0, 0);
    glBegin(GL_LINES);
    glVertex2f(locX + unit, locY + 0.3 * unit);
    glVertex2f(locX - unit, locY - 0.3 * unit);
    glVertex2f(locX - unit, locY - 0.3 * unit);
    glVertex2f(locX - unit, locY + 0.3 * unit);
    glEnd();

    glColor3ub(225, 225, 225);
    for (int i = 0; i < 8; i++)
        circle(locX + (i - 5) * unit * 0.3, locY + 0.4 * unit, 0.08 * unit);

    glBegin(GL_POLYGON);
    glColor3ub(240, 240, 240);;
    glVertex2f(locX + 1.4 * unit, locY + 0.55 * unit);
    glVertex2f(locX + 1.4 * unit, locY + 0.2 * unit);
    glVertex2f(locX + 1.85 * unit, locY + 0.2 * unit);
    glVertex2f(locX + 1.6 * unit, locY + 0.55 * unit);
    glEnd();
}

void ship()
{
    glBegin(GL_POLYGON);
    glColor3ub(170, 0, 0);
    glVertex2f(locX - 2 * unit, locY);
    glVertex2f(locX + 2 * unit, locY);
    glVertex2f(locX + 2.5 * unit, locY + 0.8 * unit);
    glVertex2f(locX - 2.2 * unit, locY + 0.8 * unit);
    glEnd();

    glBegin(GL_POLYGON);
    glColor3ub(0, 76, 153);
    glVertex2f(locX + 1.4 * unit, locY + 0.8 * unit);
    glVertex2f(locX - 1.7 * unit, locY + 0.8 * unit);
    glVertex2f(locX - 1.7 * unit, locY + 1.5 * unit);
    glVertex2f(locX + 1.4 * unit, locY + 1.5 * unit);
    glEnd();

    glColor3ub(96, 96, 96);//frame
    glRectf(locX - 1.5 * unit, locY + 1.5 * unit, locX - 1.0 * unit, locY + 1.8 * unit);
    glEnd();

    glColor3ub(225, 225, 225);
    for (int i = 0; i < 6; i++)
        circle(locX + (i - 3) * unit * 0.4, locY + 1.15 * unit, 0.1 * unit);

}

void sun() {
    glColor3ub(255, 153, 51);
    circle(-wf_Width / 2 + 4 * unit, wf_Height / 2 - 4 * unit, 2.3 * unit);
    glEnd();
    glColor3ub(255, 255, 0);
    circle(-wf_Width / 2 + 4 * unit, wf_Height / 2 - 4 * unit, 2 * unit);
    glEnd();

}

void moon() {
    int x = -wf_Width / 2 + 4 * unit;
    int y = wf_Height / 2 - 4 * unit;
    glColor3ub(224, 224, 224);
    circle(x, y, 2 * unit);
    glEnd();
    glColor3ub(160, 160, 160);
    circle(x + unit, y + unit / 3, unit / 2);
    circle(x - 1.3 * unit, y + 1.5 * unit / 3, unit / 4);
    circle(x - 0.3 * unit, y - 2 * unit / 3, unit / 3);
    glEnd();

}

void weather() {
    if (dayNight == S_DAY) {
        glColor3ub(102, 255, 255);
        glRectf(wf_Width / 2, wf_Height / 2, -wf_Width / 2, -wf_Height / 2);
        glEnd();
        if (scene == S_PLANE)//day & plane
            sun();
    }
    else {
        glColor3ub(0, 0, 51);
        glRectf(wf_Width / 2, wf_Height / 2, -wf_Width / 2, -wf_Height / 2);
        glEnd();
        if (scene == S_PLANE)//day & plane
            moon();
    }


}

void plane_scene() {
    plane();//plane design
}

void ship_scene() {
    glColor3ub(0, 128, 255);//sea color
    glRectf(wf_Width / 2, yWin, -wf_Width / 2, -wf_Height / 2);//sea
    glEnd();
    ship();//ship design
}

void curtains() {

    glColor3ub(96, 96, 96);
    glRectf(wf_Width, wf_Height / 2 + 1.5 * unit, -wf_Width, wf_Height / 2 + unit);
    glEnd();//curtain rod

    int x;

    if (curtain_st == CUR_CLOSE) {

        glColor3ub(153, 51, 255);
        glRectf(wf_Width - 2 * unit, wf_Height / 2 + 1.5 * unit, wf_Width / 2, -wf_Height / 2 - unit);
        glEnd();//right curtain

        glColor3ub(153, 51, 255);
        glRectf(-wf_Width + 2 * unit, wf_Height / 2 + 1.5 * unit, -wf_Width / 2, -wf_Height / 2 - unit);
        glEnd();//left curtain

        glLineWidth(1);
        glColor3f(0, 0, 0);
        glBegin(GL_LINES);
        for (x = -wf_Width / 2; x > -wf_Width + 2 * unit; x -= unit / 2)//left closed curtain pattern
        {
            glVertex2f(x, wf_Height / 2 + 1.5 * unit);
            glVertex2f(x, -wf_Height / 2 - unit);

        }
        for (x = wf_Width - 4 * unit / 2; x > wf_Width / 2; x -= unit / 2)//right closed curtain pattern
        {
            glVertex2f(x, wf_Height / 2 + 1.5 * unit);
            glVertex2f(x, -wf_Height / 2 - unit);

        }
        glEnd();
    }

    if (curtain_st == CUR_OPEN) {
        glColor3ub(153, 51, 255);
        glRectf(-wf_Width + 2 * unit, wf_Height / 2 + 1.5 * unit, wf_Width - 2 * unit, -wf_Height / 2 - unit);
        glEnd();

        glLineWidth(1);
        glColor3f(0, 0, 0);
        glBegin(GL_LINES);
        for (x = wf_Width - 3 * unit; x > -wf_Width + 2 * unit; x -= unit)//open curtain pattern
        {
            glVertex2f(x, wf_Height / 2 + 1.5 * unit);
            glVertex2f(x, -wf_Height / 2 - unit);
        }
        glEnd();
    }



}

void info()
{
    glColor3ub(224, 224, 224);
    glRectf(-10 * unit, -wf_Height / 2 - 1.4 * unit, 10 * unit, -wf_Height / 2 - 4 * unit);
    glEnd();
    glColor3f(0, 0, 0);
    vprint2(-6.8 * unit, -wf_Height / 2 - 2.0 * unit, 0.13, "Pressing left and right key will change the time");
    vprint2(-6.95 * unit, -wf_Height / 2 - 2.8 * unit, 0.13, "Pressing up and down key will change the scene");
    vprint2(-6.75 * unit, -wf_Height / 2 - 3.6 * unit, 0.13, "Clicking the curtains will close or open them");
    glEnd();
}

void label()
{
    glColor3ub(224, 224, 224);
    glRectf(-7 * unit, +wf_Height / 2 + 1.8 * unit, 7 * unit, wf_Height / 2 + 4 * unit);
    glColor3f(0, 0, 0);
    vprint2(-5 * unit, +wf_Height / 2 + 2.9 * unit, 0.2, "LOOK UP LOOK DOWN");
    vprint2(-3 * unit, +wf_Height / 2 + 2.0 * unit, 0.2, "H.Deniz ULU");
    glEnd();


}

//
// To display onto window using OpenGL commands
//
void display() {
    glClearColor(212 / 255., 100 / 255., 44 / 255., 0);//background
    glClear(GL_COLOR_BUFFER_BIT);
    pattern();//pattern of background

    weather();//weather(day/night)



    switch (scene) { //scenes
    case S_PLANE:
        plane_scene();
        break;

    case S_SHIP:
        ship_scene();
        break;

    }

    clouds();//clouds function

    frame();//frame of window

    curtains();//curtains

    info();

    label();

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

    //to pause when pressed space
    if (key == ' ')
        actTimer = !actTimer;


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

    if (key == GLUT_KEY_DOWN)
        if (scene == S_PLANE)
            scene = S_SHIP;

    if (key == GLUT_KEY_UP)
        if (scene == S_SHIP)
            scene = S_PLANE;

    if (key == GLUT_KEY_RIGHT)
        if (dayNight == S_DAY)
            dayNight = S_NIGHT;

    if (key == GLUT_KEY_LEFT)
        if (dayNight == S_NIGHT)
            dayNight = S_DAY;

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

    int flag = 0;

    if (curtain_st == CUR_CLOSE) {
        if (mx < wf_Width - 2 * unit && mx > wf_Width / 2 && my > -wf_Height / 2 - unit && my < wf_Height / 2 + 1.5 * unit && button == GLUT_LEFT_BUTTON && stat == GLUT_DOWN) {
            curtain_st = CUR_OPEN;//right curtain
            flag = 1;//a flag to decide whether the curtain is open or close
        }
        if (mx > -wf_Width + 2 * unit && mx < -wf_Width / 2 && my > -wf_Height / 2 - unit && my < wf_Height / 2 + 1.5 * unit && button == GLUT_LEFT_BUTTON && stat == GLUT_DOWN) {
            curtain_st = CUR_OPEN;//left curtain
            flag = 1;
        }
    }


    if (curtain_st == CUR_OPEN && flag == 0)
        if (mx < wf_Width - 2 * unit && mx > -wf_Width + 2 * unit && my > -wf_Height / 2 - unit && my < wf_Height / 2 + 1.5 * unit && button == GLUT_LEFT_BUTTON && stat == GLUT_DOWN)
        {
            flag = 0;
            curtain_st = CUR_CLOSE;
        }

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

    if (actTimer) { //space button
        if (locX < WINDOW_WIDTH / 2 - 4.1 * unit) {
            locX++;

        }
        if (locX == WINDOW_WIDTH / 2 - 4.1 * unit) {
            locX = -WINDOW_WIDTH / 2 + 4.1 * unit;

        }

        for (int i = 0; i < 3; i++)
        {
            if (cloud_x[i] > -WINDOW_WIDTH / 2) {
                cloud_x[i]--;

            }
            if (cloud_x[i] <= -WINDOW_WIDTH / 2) {
                cloud_x[i] = +WINDOW_WIDTH / 2;

            }
        }
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
    glutInitWindowPosition(400, 50);
    glutCreateWindow("Look Up Look Down by H.Deniz ULU");

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