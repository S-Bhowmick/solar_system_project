// 2D Animated Solar System by Surjya Bhowmick
// Computer Graphics Course Project
// Algorithms Used: DDA Line Algorithm, Midpoint Circle Algorithm
// Transformations Used: Translation, Rotation, Scaling
// Developed in C++ using OpenGL + GLUT

#ifdef _APPLE_
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define PI 3.14159265f

// globals
float venus_angle   = 0.0f;
float earth_angle   = 0.0f;
float mars_angle    = 0.0f;
float jupiter_angle = 0.0f;
float saturn_angle  = 0.0f;
float moon_angle    = 0.0f;
float sun_pulse     = 0.0f;
float twinkle       = 0.0f;

float comet_x       = -40.0f;
float comet_y       = 380.0f;

int   paused        = 0;
float speed         = 1.0f;

// Sun centered so all orbits fit in 900x500
// Max orbit radius = 210 (Saturn).
// Safe center: x >= 210, x <= 690, y >= 210, y <= 290
// Use (450, 250) — center.
#define SUN_X        450
#define SUN_Y        250

// Orbit radii — scaled so Saturn fits
#define R_VENUS       65
#define R_EARTH       95
#define R_MARS       128
#define R_JUPITER    168
#define R_SATURN     210
#define R_MOON        22

// Stars — kept inside the 900x500 window
int stars[][3] = {
    {30, 480, 2},{80, 465, 3},{140, 472, 2},{200, 488, 3},{260, 460, 2},
    {320, 475, 3},{380, 462, 2},{440, 478, 3},{500, 468, 2},{560, 485, 3},
    {620, 458, 2},{680, 472, 3},{740, 465, 2},{800, 483, 3},{860, 470, 2},
    {50, 445, 3},{120, 452, 2},{190, 440, 3},{270, 450, 2},{350, 438, 3},
    {430, 454, 2},{510, 444, 3},{590, 448, 2},{670, 435, 3},{750, 450, 2},
    {840, 442, 3},{70, 420, 2},{160, 430, 3},{250, 418, 2},{340, 428, 3},
    {420, 422, 2},{510, 432, 3},{590, 425, 2},{670, 412, 3},{750, 428, 2},
    {840, 420, 3},{100, 400, 2},{200, 410, 3},{300, 398, 2},{400, 408, 3},
    {495, 402, 2},{575, 414, 3},{655, 406, 2},{740, 395, 3},{825, 410, 2},
    {40, 388, 3},{150, 380, 2},{265, 392, 3},{375, 378, 2},{485, 388, 3},
    {585, 382, 2},{685, 374, 3},{775, 386, 2},{870, 378, 3},{110, 362, 2},
    {220, 370, 3},{335, 358, 2},{445, 368, 3},{555, 362, 2},{655, 352, 3},
    {755, 365, 2},{858, 355, 3},{60, 342, 2},{170, 352, 3},{285, 340, 2},
    {395, 348, 3},{505, 338, 2},{615, 345, 3},{715, 335, 2},{825, 342, 3}
};
#define NUM_STARS 70

// forward declarations
void drawMidpointCircle(int cx, int cy, int r);
void drawMidpointCircleOutline(int cx, int cy, int r);
void drawBresenhamLine(int x1, int y1, int x2, int y2);
void draw_stars(void);
void draw_comet(void);
void draw_orbits(void);
void draw_sun(void);
void draw_venus(int cx, int cy);
void draw_earth(int cx, int cy);
void draw_mars(int cx, int cy);
void draw_jupiter(int cx, int cy);
void draw_saturn(int cx, int cy);
void draw_hud(void);
void display(void);
void update_animation(void);
void keyboard(unsigned char key, int x, int y);
void specialKeys(int key, int x, int y);
void myInit(void);

// midpoint circle — filled
void drawMidpointCircle(int cx, int cy, int r)
{
    int x = 0, y = r, p = 1 - r;
    while (x <= y)
    {
        glBegin(GL_LINES);
            glVertex2i(cx - x, cy + y); glVertex2i(cx + x, cy + y);
            glVertex2i(cx - x, cy - y); glVertex2i(cx + x, cy - y);
            glVertex2i(cx - y, cy + x); glVertex2i(cx + y, cy + x);
            glVertex2i(cx - y, cy - x); glVertex2i(cx + y, cy - x);
        glEnd();

        x++;
        if (p < 0)
            p += 2 * x + 1;
        else
        {
            y--;
            p += 2 * (x - y) + 1;
        }
    }
}

// midpoint circle — outline only
void drawMidpointCircleOutline(int cx, int cy, int r)
{
    int x = 0, y = r, p = 1 - r;

    glBegin(GL_POINTS);
    while (x <= y)
    {
        glVertex2i(cx + x, cy + y); glVertex2i(cx - x, cy + y);
        glVertex2i(cx + x, cy - y); glVertex2i(cx - x, cy - y);
        glVertex2i(cx + y, cy + x); glVertex2i(cx - y, cy + x);
        glVertex2i(cx + y, cy - x); glVertex2i(cx - y, cy - x);

        x++;
        if (p < 0)
            p += 2 * x + 1;
        else
        {
            y--;
            p += 2 * (x - y) + 1;
        }
    }
    glEnd();
}

// Bresenham line
void drawBresenhamLine(int x1, int y1, int x2, int y2)
{
    int dx = abs(x2 - x1), dy = abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;

    glBegin(GL_POINTS);
    while (1)
    {
        glVertex2i(x1, y1);
        if (x1 == x2 && y1 == y2) break;

        int e2 = 2 * err;
        if (e2 > -dy) { err -= dy; x1 += sx; }
        if (e2 <  dx) { err += dx; y1 += sy; }
    }
    glEnd();
}

// init
void myInit(void)
{
    glClearColor(0.0f, 0.0f, 0.05f, 1.0f);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, 900.0, 0.0, 500.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glPointSize(1.0f);
}

// stars
void draw_stars(void)
{
    for (int i = 0; i < NUM_STARS; i++)
    {
        float bright = 0.65f + 0.35f * sinf(twinkle + stars[i][0] * 0.09f);
        glColor3f(bright, bright, bright);
        glPointSize((float)stars[i][2]);

        glBegin(GL_POINTS);
            glVertex2i(stars[i][0], stars[i][1]);
        glEnd();
    }
    glPointSize(1.0f);
}

// comet
void draw_comet(void)
{
    for (int i = 1; i < 9; i++)
    {
        float a = 1.0f - i / 9.0f;
        glColor3f(a * 0.8f, a * 0.8f, a);
        drawBresenhamLine(
            (int)comet_x,           (int)comet_y,
            (int)(comet_x - i * 8), (int)(comet_y + i * 3)
        );
    }

    glColor3f(1.0f, 1.0f, 1.0f);
    drawMidpointCircle((int)comet_x, (int)comet_y, 4);
}

// orbit rings
void draw_orbits(void)
{
    glColor3f(0.55f, 0.55f, 0.60f);

    for (int r = R_VENUS - 1; r <= R_VENUS + 1; r++)     drawMidpointCircleOutline(SUN_X, SUN_Y, r);
    for (int r = R_EARTH - 1; r <= R_EARTH + 1; r++)     drawMidpointCircleOutline(SUN_X, SUN_Y, r);
    for (int r = R_MARS - 1; r <= R_MARS + 1; r++)       drawMidpointCircleOutline(SUN_X, SUN_Y, r);
    for (int r = R_JUPITER - 1; r <= R_JUPITER + 1; r++) drawMidpointCircleOutline(SUN_X, SUN_Y, r);
    for (int r = R_SATURN - 1; r <= R_SATURN + 1; r++)   drawMidpointCircleOutline(SUN_X, SUN_Y, r);
}

// sun
void draw_sun(void)
{
    int sr = (int)(28.0f + 3.0f * sinf(sun_pulse));

    glColor3f(0.30f, 0.22f, 0.0f);
    drawMidpointCircle(SUN_X, SUN_Y, sr + 7);

    glColor3f(0.52f, 0.38f, 0.0f);
    drawMidpointCircle(SUN_X, SUN_Y, sr + 4);

    glColor3f(1.0f, 0.90f, 0.0f);
    drawMidpointCircle(SUN_X, SUN_Y, sr);

    glColor3f(1.0f, 0.85f, 0.20f);
    for (int i = 0; i < 12; i++)
    {
        float a = i * 30.0f * PI / 180.0f + sun_pulse * 0.3f;
        int x1 = SUN_X + (int)(cosf(a) * (sr + 8));
        int y1 = SUN_Y + (int)(sinf(a) * (sr + 8));
        int x2 = SUN_X + (int)(cosf(a) * (sr + 17));
        int y2 = SUN_Y + (int)(sinf(a) * (sr + 17));
        drawBresenhamLine(x1, y1, x2, y2);
    }
}

// venus
void draw_venus(int cx, int cy)
{
    glColor3f(0.45f, 0.26f, 0.0f);
    drawMidpointCircle(cx, cy, 9);

    glColor3f(1.0f, 0.70f, 0.0f);
    drawMidpointCircle(cx, cy, 7);
}

// earth
void draw_earth(int cx, int cy)
{
    glColor3f(0.05f, 0.22f, 0.45f);
    drawMidpointCircle(cx, cy, 10);

    glColor3f(0.18f, 0.52f, 1.0f);
    drawMidpointCircle(cx, cy, 8);

    glColor3f(0.15f, 0.60f, 0.15f);
    drawMidpointCircle(cx - 2, cy + 2, 3);
    drawMidpointCircle(cx + 3, cy - 2, 2);

    // moon orbit ring
    glColor3f(0.40f, 0.40f, 0.45f);
    drawMidpointCircleOutline(cx, cy, R_MOON);
    drawMidpointCircleOutline(cx, cy, R_MOON + 1);

    int mx = cx + (int)(R_MOON * cosf(moon_angle));
    int my = cy + (int)(R_MOON * sinf(moon_angle));

    glColor3f(0.45f, 0.45f, 0.45f);
    drawMidpointCircle(mx, my, 4);

    glColor3f(0.78f, 0.78f, 0.78f);
    drawMidpointCircle(mx, my, 2);
}

// mars
void draw_mars(int cx, int cy)
{
    glColor3f(0.38f, 0.10f, 0.06f);
    drawMidpointCircle(cx, cy, 8);

    glColor3f(0.90f, 0.35f, 0.25f);
    drawMidpointCircle(cx, cy, 6);

    glColor3f(0.90f, 0.90f, 0.90f);
    drawMidpointCircle(cx, cy + 4, 2);
}

// jupiter
void draw_jupiter(int cx, int cy)
{
    glColor3f(0.38f, 0.26f, 0.14f);
    drawMidpointCircle(cx, cy, 14);

    glColor3f(0.82f, 0.66f, 0.46f);
    drawMidpointCircle(cx, cy, 12);

    glColor3f(0.68f, 0.50f, 0.30f);
    drawBresenhamLine(cx - 11, cy + 4, cx + 11, cy + 4);
    drawBresenhamLine(cx - 11, cy + 1, cx + 11, cy + 1);
    drawBresenhamLine(cx - 11, cy - 2, cx + 11, cy - 2);
    drawBresenhamLine(cx - 11, cy - 5, cx + 11, cy - 5);

    glColor3f(0.78f, 0.22f, 0.10f);
    drawMidpointCircle(cx + 4, cy - 2, 3);
}

// saturn
void draw_saturn(int cx, int cy)
{
    // rings first
    for (int off = -4; off <= 4; off++)
    {
        if (off >= -1 && off <= 1) continue;

        if (off % 2 == 0)
            glColor3f(0.70f, 0.62f, 0.38f);
        else
            glColor3f(0.55f, 0.48f, 0.26f);

        drawBresenhamLine(cx - 22, cy + off, cx + 22, cy + off);
    }

    glColor3f(0.42f, 0.38f, 0.22f);
    drawMidpointCircle(cx, cy, 12);

    glColor3f(0.86f, 0.80f, 0.58f);
    drawMidpointCircle(cx, cy, 10);
}

// HUD
void draw_hud(void)
{
    const char *c;

    glColor3f(1.0f, 1.0f, 1.0f);
    glRasterPos2i(10, 488);
    const char *title = "2D Solar System  |  SPACE=Pause  UP/DOWN=Speed";
    for (c = title; *c != '\0'; c++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *c);

    char spdbuf[32];
    sprintf(spdbuf, "Speed: %.1fx", speed);

    glColor3f(1.0f, 0.85f, 0.20f);
    glRasterPos2i(10, 474);
    for (c = spdbuf; *c != '\0'; c++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *c);

    if (paused)
    {
        glColor3f(1.0f, 0.25f, 0.25f);
        glRasterPos2i(385, 488);
        const char *ps = "-- PAUSED --";
        for (c = ps; *c != '\0'; c++)
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
    }
}

// animation update
void update_animation(void)
{
    if (!paused)
    {
        venus_angle   += 0.0035f * speed;
        earth_angle   += 0.0020f * speed;
        mars_angle    += 0.0012f * speed;
        jupiter_angle += 0.0006f * speed;
        saturn_angle  += 0.0003f * speed;
        moon_angle    += 0.0120f * speed;
        sun_pulse     += 0.0080f * speed;
        twinkle       += 0.0050f * speed;

        comet_x += 0.40f * speed;
        comet_y -= 0.06f * speed;

        if (comet_x > 950.0f)
        {
            comet_x = -40.0f;
            comet_y = 420.0f + (float)(rand() % 60) - 30.0f;
        }
    }

    glutPostRedisplay();
}

// keyboard: normal keys
void keyboard(unsigned char key, int x, int y)
{
    (void)x;
    (void)y;

    if (key == ' ')
        paused = !paused;
}

// keyboard: special keys
void specialKeys(int key, int x, int y)
{
    (void)x;
    (void)y;

    if (key == GLUT_KEY_UP)
    {
        speed += 0.2f;
        if (speed > 5.0f) speed = 5.0f;
    }

    if (key == GLUT_KEY_DOWN)
    {
        speed -= 0.2f;
        if (speed < 0.2f) speed = 0.2f;
    }
}

// display callback
void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    draw_stars();
    draw_comet();
    draw_orbits();
    draw_sun();

    int vx  = SUN_X + (int)(R_VENUS   * cosf(venus_angle));
    int vy  = SUN_Y + (int)(R_VENUS   * sinf(venus_angle));
    int ex  = SUN_X + (int)(R_EARTH   * cosf(earth_angle));
    int ey  = SUN_Y + (int)(R_EARTH   * sinf(earth_angle));
    int mrx = SUN_X + (int)(R_MARS    * cosf(mars_angle));
    int mry = SUN_Y + (int)(R_MARS    * sinf(mars_angle));
    int jx  = SUN_X + (int)(R_JUPITER * cosf(jupiter_angle));
    int jy  = SUN_Y + (int)(R_JUPITER * sinf(jupiter_angle));
    int sx  = SUN_X + (int)(R_SATURN  * cosf(saturn_angle));
    int sy  = SUN_Y + (int)(R_SATURN  * sinf(saturn_angle));

    draw_venus(vx, vy);
    draw_earth(ex, ey);
    draw_mars(mrx, mry);
    draw_jupiter(jx, jy);
    draw_saturn(sx, sy);

    draw_hud();

    glutSwapBuffers();
}

// timer callback
void timer(int value)
{
    (void)value;
    update_animation();
    glutTimerFunc(16, timer, 0);
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(900, 500);
    glutCreateWindow("2D Animated Solar System - Surjya Bhowmick");

    myInit();

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeys);
    glutTimerFunc(16, timer, 0);

    glutMainLoop();
    return 0;
}