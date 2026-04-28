// 2D Animated Solar System using OpenGL Transformations
// Uses: glTranslatef(), glRotatef(), glScalef()
// C++ + OpenGL + GLUT

#ifdef _APPLE_
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define PI 3.14159265f

// -------------------- Global Animation Variables --------------------
float venus_angle   = 0.0f;
float earth_angle   = 0.0f;
float mars_angle    = 0.0f;
float jupiter_angle = 0.0f;
float saturn_angle  = 0.0f;
float moon_angle    = 0.0f;

float sun_pulse     = 0.0f;
float twinkle       = 0.0f;

float comet_x       = -80.0f;
float comet_y       = 390.0f;

int   paused        = 0;
float speed         = 1.0f;

// -------------------- Scene Constants --------------------
#define SUN_X        450
#define SUN_Y        250

#define R_VENUS       65
#define R_EARTH       95
#define R_MARS       128
#define R_JUPITER    168
#define R_SATURN     210
#define R_MOON        22

// -------------------- Star Data --------------------
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

// -------------------- Utility: Draw Filled Circle --------------------
void drawFilledCircle(float radius)
{
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(0.0f, 0.0f);
    for (int i = 0; i <= 100; i++)
    {
        float angle = 2.0f * PI * i / 100.0f;
        glVertex2f(cosf(angle) * radius, sinf(angle) * radius);
    }
    glEnd();
}

// -------------------- Utility: Draw Circle Outline --------------------
void drawCircleOutline(float radius)
{
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 100; i++)
    {
        float angle = 2.0f * PI * i / 100.0f;
        glVertex2f(cosf(angle) * radius, sinf(angle) * radius);
    }
    glEnd();
}

// -------------------- Text Drawing --------------------
void drawText(float x, float y, void *font, const char *str)
{
    glRasterPos2f(x, y);
    for (const char *c = str; *c != '\0'; c++)
        glutBitmapCharacter(font, *c);
}

// -------------------- Initialization --------------------
void myInit(void)
{
    glClearColor(0.0f, 0.0f, 0.05f, 1.0f);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, 900.0, 0.0, 500.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

// -------------------- Stars --------------------
void draw_stars(void)
{
    for (int i = 0; i < NUM_STARS; i++)
    {
        float bright = 0.65f + 0.35f * sinf(twinkle + stars[i][0] * 0.09f);
        float scale  = 0.8f + 0.25f * sinf(twinkle + stars[i][1] * 0.07f);

        glPushMatrix();
        glTranslatef((float)stars[i][0], (float)stars[i][1], 0.0f);
        glScalef(scale, scale, 1.0f);

        glColor3f(bright, bright, bright);
        glPointSize((float)stars[i][2]);

        glBegin(GL_POINTS);
            glVertex2f(0.0f, 0.0f);
        glEnd();

        glPopMatrix();
    }
    glPointSize(1.0f);
}

// -------------------- Comet --------------------
void draw_comet(void)
{
    glPushMatrix();
    glTranslatef(comet_x, comet_y, 0.0f);

    for (int i = 1; i < 9; i++)
    {
        float a = 1.0f - i / 9.0f;
        glColor3f(a * 0.8f, a * 0.8f, a);

        glBegin(GL_LINES);
            glVertex2f(0.0f, 0.0f);
            glVertex2f(-i * 8.0f, i * 3.0f);
        glEnd();
    }

    glColor3f(1.0f, 1.0f, 1.0f);
    drawFilledCircle(4.0f);

    glPopMatrix();
}

// -------------------- Orbit Rings --------------------
void draw_orbits(void)
{
    glPushMatrix();
    glTranslatef(SUN_X, SUN_Y, 0.0f);

    glColor3f(0.55f, 0.55f, 0.60f);
    drawCircleOutline(R_VENUS);
    drawCircleOutline(R_EARTH);
    drawCircleOutline(R_MARS);
    drawCircleOutline(R_JUPITER);
    drawCircleOutline(R_SATURN);

    glPopMatrix();
}

// -------------------- Sun --------------------
void draw_sun(void)
{
    float pulse_scale = 1.0f + 0.08f * sinf(sun_pulse);

    glPushMatrix();
    glTranslatef(SUN_X, SUN_Y, 0.0f);
    glScalef(pulse_scale, pulse_scale, 1.0f);

    glColor3f(0.30f, 0.22f, 0.0f);
    drawFilledCircle(35.0f);

    glColor3f(0.52f, 0.38f, 0.0f);
    drawFilledCircle(32.0f);

    glColor3f(1.0f, 0.90f, 0.0f);
    drawFilledCircle(28.0f);

    glColor3f(1.0f, 0.85f, 0.20f);
    for (int i = 0; i < 12; i++)
    {
        glPushMatrix();
        glRotatef(i * 30.0f + sun_pulse * 8.0f, 0.0f, 0.0f, 1.0f);

        glBegin(GL_LINES);
            glVertex2f(36.0f, 0.0f);
            glVertex2f(48.0f, 0.0f);
        glEnd();

        glPopMatrix();
    }

    glPopMatrix();
}

// -------------------- Venus --------------------
void draw_venus(void)
{
    glColor3f(0.45f, 0.26f, 0.0f);
    drawFilledCircle(9.0f);

    glColor3f(1.0f, 0.70f, 0.0f);
    drawFilledCircle(7.0f);
}

// -------------------- Earth + Moon --------------------
void draw_earth(void)
{
    glColor3f(0.05f, 0.22f, 0.45f);
    drawFilledCircle(10.0f);

    glColor3f(0.18f, 0.52f, 1.0f);
    drawFilledCircle(8.0f);

    glPushMatrix();
    glTranslatef(-2.0f, 2.0f, 0.0f);
    glColor3f(0.15f, 0.60f, 0.15f);
    drawFilledCircle(3.0f);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(3.0f, -2.0f, 0.0f);
    glColor3f(0.15f, 0.60f, 0.15f);
    drawFilledCircle(2.0f);
    glPopMatrix();

    glColor3f(0.40f, 0.40f, 0.45f);
    drawCircleOutline(R_MOON);

    glPushMatrix();
    glRotatef(moon_angle, 0.0f, 0.0f, 1.0f);
    glTranslatef(R_MOON, 0.0f, 0.0f);

    glColor3f(0.45f, 0.45f, 0.45f);
    drawFilledCircle(4.0f);

    glColor3f(0.78f, 0.78f, 0.78f);
    drawFilledCircle(2.0f);

    glPopMatrix();
}

// -------------------- Mars --------------------
void draw_mars(void)
{
    glColor3f(0.38f, 0.10f, 0.06f);
    drawFilledCircle(8.0f);

    glColor3f(0.90f, 0.35f, 0.25f);
    drawFilledCircle(6.0f);

    glPushMatrix();
    glTranslatef(0.0f, 4.0f, 0.0f);
    glColor3f(0.90f, 0.90f, 0.90f);
    drawFilledCircle(2.0f);
    glPopMatrix();
}

// -------------------- Jupiter --------------------
void draw_jupiter(void)
{
    glColor3f(0.38f, 0.26f, 0.14f);
    drawFilledCircle(14.0f);

    glColor3f(0.82f, 0.66f, 0.46f);
    drawFilledCircle(12.0f);

    glColor3f(0.68f, 0.50f, 0.30f);
    for (float y = 4.0f; y >= -5.0f; y -= 3.0f)
    {
        glBegin(GL_LINES);
            glVertex2f(-11.0f, y);
            glVertex2f( 11.0f, y);
        glEnd();
    }

    glPushMatrix();
    glTranslatef(4.0f, -2.0f, 0.0f);
    glColor3f(0.78f, 0.22f, 0.10f);
    drawFilledCircle(3.0f);
    glPopMatrix();
}

// -------------------- Saturn --------------------
void draw_saturn(void)
{
    glPushMatrix();
    glRotatef(-20.0f, 0.0f, 0.0f, 1.0f);

    glColor3f(0.70f, 0.62f, 0.38f);
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 100; i++)
    {
        float a = 2.0f * PI * i / 100.0f;
        glVertex2f(cosf(a) * 24.0f, sinf(a) * 8.0f);
    }
    glEnd();

    glColor3f(0.55f, 0.48f, 0.26f);
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 100; i++)
    {
        float a = 2.0f * PI * i / 100.0f;
        glVertex2f(cosf(a) * 18.0f, sinf(a) * 6.0f);
    }
    glEnd();

    glPopMatrix();

    glColor3f(0.42f, 0.38f, 0.22f);
    drawFilledCircle(12.0f);

    glColor3f(0.86f, 0.80f, 0.58f);
    drawFilledCircle(10.0f);
}

// -------------------- HUD --------------------
void draw_hud(void)
{
    char spdbuf[32];
    sprintf(spdbuf, "Speed: %.1fx", speed);

    glColor3f(1.0f, 1.0f, 1.0f);
    drawText(10.0f, 488.0f, GLUT_BITMAP_HELVETICA_12,
             "2D Solar System  |  SPACE=Pause  UP/DOWN=Speed");

    glColor3f(1.0f, 0.85f, 0.20f);
    drawText(10.0f, 474.0f, GLUT_BITMAP_HELVETICA_12, spdbuf);

    if (paused)
    {
        glColor3f(1.0f, 0.25f, 0.25f);
        drawText(385.0f, 488.0f, GLUT_BITMAP_HELVETICA_18, "-- PAUSED --");
    }
}

// -------------------- Display --------------------
void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    draw_stars();
    draw_comet();
    draw_orbits();
    draw_sun();

    // Venus
    glPushMatrix();
    glTranslatef(SUN_X, SUN_Y, 0.0f);
    glRotatef(venus_angle, 0.0f, 0.0f, 1.0f);
    glTranslatef(R_VENUS, 0.0f, 0.0f);
    draw_venus();
    glPopMatrix();

    // Earth
    glPushMatrix();
    glTranslatef(SUN_X, SUN_Y, 0.0f);
    glRotatef(earth_angle, 0.0f, 0.0f, 1.0f);
    glTranslatef(R_EARTH, 0.0f, 0.0f);
    draw_earth();
    glPopMatrix();

    // Mars
    glPushMatrix();
    glTranslatef(SUN_X, SUN_Y, 0.0f);
    glRotatef(mars_angle, 0.0f, 0.0f, 1.0f);
    glTranslatef(R_MARS, 0.0f, 0.0f);
    draw_mars();
    glPopMatrix();

    // Jupiter
    glPushMatrix();
    glTranslatef(SUN_X, SUN_Y, 0.0f);
    glRotatef(jupiter_angle, 0.0f, 0.0f, 1.0f);
    glTranslatef(R_JUPITER, 0.0f, 0.0f);
    draw_jupiter();
    glPopMatrix();

    // Saturn
    glPushMatrix();
    glTranslatef(SUN_X, SUN_Y, 0.0f);
    glRotatef(saturn_angle, 0.0f, 0.0f, 1.0f);
    glTranslatef(R_SATURN, 0.0f, 0.0f);
    draw_saturn();
    glPopMatrix();

    draw_hud();

    glutSwapBuffers();
}

// -------------------- Animation Update --------------------
void update_animation(void)
{
    if (!paused)
    {
        venus_angle   += 0.20f * speed;
        earth_angle   += 0.12f * speed;
        mars_angle    += 0.08f * speed;
        jupiter_angle += 0.04f * speed;
        saturn_angle  += 0.02f * speed;
        moon_angle    += 0.70f * speed;

        sun_pulse     += 0.08f * speed;
        twinkle       += 0.05f * speed;

        comet_x += 0.40f * speed;
        comet_y -= 0.06f * speed;

        if (comet_x > 950.0f)
        {
            comet_x = -80.0f;
            comet_y = 420.0f + (float)(rand() % 60) - 30.0f;
        }
    }

    glutPostRedisplay();
}

// -------------------- Timer --------------------
void timer(int value)
{
    (void)value;
    update_animation();
    glutTimerFunc(16, timer, 0);
}

// -------------------- Keyboard --------------------
void keyboard(unsigned char key, int x, int y)
{
    (void)x;
    (void)y;

    if (key == ' ')
        paused = !paused;
}

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

// -------------------- Main --------------------
int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(900, 500);
    glutCreateWindow("2D Solar System with Transformations");

    myInit();

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeys);
    glutTimerFunc(16, timer, 0);

    glutMainLoop();
    return 0;
}