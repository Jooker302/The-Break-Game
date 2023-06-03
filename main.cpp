#include <GL/glut.h>
#include <stdio.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include<conio.h>
#include<stdlib.h>

using namespace std;
#define FROM_RIGHT      1 
#define FROM_LEFT       2 
#define FROM_TOP        3 
#define FROM_BOTTOM     4 

static int WINDOW_WIDTH, WINDOW_HEIGHT;

int playerResult = 0;
static float Xspeed = 0.5, Yspeed = 0.5;
static float delta = 0.5;

char string[100];

struct RECTA
{
    float left, top, right, bottom;
};

RECTA ball = { 100,100,120,120 };  // Ball Size
RECTA wall; // wall ,will declare with window size
RECTA player_1 = { 0,490,100,500 }; // Increased button bar width

bool gameStarted = false;  // check of game start
bool gameOver = false; // game end check

void DrawRectangle(RECTA  rect)
{
    glBegin(GL_QUADS);
    glVertex2f(rect.left, rect.bottom);      //Left - Bottom 
    glVertex2f(rect.right, rect.bottom);
    glVertex2f(rect.right, rect.top);
    glVertex2f(rect.left, rect.top);
    glEnd();
}

void Timer(int v)
{
    // Update ball position if the game is started and not over
    if (gameStarted && !gameOver) {
        ball.left += Xspeed;
        ball.right += Xspeed;
        ball.top += Yspeed;
        ball.bottom += Yspeed;
    }

    glutTimerFunc(1, Timer, 1); // Call the Timer function again after 1 millisecond
}

void drawText(char* string, int x, int y)
{
    char* c;
    glPushMatrix();
    glTranslatef(x, y, 0);
    glScalef(0.1, -0.1, 1);

    for (c = string; *c != '\0'; c++)
    {
        glutStrokeCharacter(GLUT_STROKE_ROMAN, *c);
    }
    glPopMatrix();
}

void stop()
{
    printf("game over\n");
    gameOver = true;
}

int Test_Ball_Wall(RECTA  ball, RECTA wall)
{
    // Check if the ball has collided with any of the walls
    if (ball.right >= wall.right)
        return FROM_RIGHT;
    if (ball.left <= wall.left)
        return FROM_LEFT;
    if (ball.top <= wall.top)
        return FROM_TOP;
    if (ball.bottom >= wall.bottom)
        stop();
    //return FROM_BOTTOM;  

    else return 0;
}

bool Test_Ball_Player(RECTA ball, RECTA player)
{
    // Check if the ball has collided with the player's bar
    if (ball.bottom >= player.top && ball.left >= player.left && ball.right <= player.right)
    {
        playerResult++;
        return true;
    }

    return false;
}

void keyboard(unsigned char key, int x, int y)
{
    switch (key) {
    case 27:
        exit(0); // Exit the game when the Escape key is pressed
        break;
    }
}

static int mouse_x = 0;
void MouseMotion(int x, int y)
{
    mouse_x = x; // Update the x-coordinate of the mouse
}

void mouse(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        if (gameOver) {
            // Restart the game
            playerResult = 0;
            Xspeed = 0.5;
            Yspeed = 0.5;
            delta = 0.5;
            ball = { 100,100,120,120 };
            player_1 = { 0,490,100,500 };
            gameStarted = true;
            gameOver = false;
        }
        else if (!gameStarted) {
            // Start the game
            gameStarted = true;
        }
    }
    else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
        // Exit the game
        exit(0);
    }
}

void Setting(void)
{
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}

void reshape(int w, int h)
{
    WINDOW_WIDTH = w;
    WINDOW_HEIGHT = h;

    glViewport(0, 0, (GLsizei)w, (GLsizei)h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, w, h, 0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

int pcResult = 0;

void Render()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    if (!gameStarted) {
        sprintf(string, "WELCOME TO THE BREAKOUT GAME");
        drawText(string, 250, 250);
        sprintf(string, "Click left mouse button to start");
        drawText(string, 250, 275);
        sprintf(string,"Presented By: ");
        drawText(string, 320, 300);
        sprintf(string, "Abdul Moeed (20-ARID-560)");
        drawText(string, 320, 320);
        sprintf(string, "M. Shahzaib (20-ARID-625)");
        drawText(string, 320, 340);
        sprintf(string, "Omer Farooq (20-ARID-635)");
        drawText(string, 320, 360);
    }
    else {
        if (gameOver) {
            sprintf(string, "Game Over! Click left mouse button to restart");
            drawText(string, 200, 250);
        }
        else {
           // sprintf(string, "PC : %d ", pcResult);
            drawText(string, 10, 80);
            sprintf(string, "Player : %d ", playerResult);
            drawText(string, 10, 80);
            //drawText(string, 10, 120);

            wall.left = wall.top = 0;
            wall.right = WINDOW_WIDTH;
            wall.bottom = WINDOW_HEIGHT;

            DrawRectangle(ball);

            if (Test_Ball_Wall(ball, wall) == FROM_RIGHT)
                Xspeed = -delta;

            if (Test_Ball_Wall(ball, wall) == FROM_LEFT)
                Xspeed = delta;

            if (Test_Ball_Wall(ball, wall) == FROM_TOP)
                Yspeed = delta;

            if (Test_Ball_Wall(ball, wall) == FROM_BOTTOM)
            {
                Yspeed = -delta;
                pcResult += 1;
            }

            DrawRectangle(player_1);
            player_1.left = mouse_x - 50;
            player_1.right = mouse_x + 50;

            if (Test_Ball_Player(ball, player_1) == true)
                Yspeed = -delta;
        }
    }

    glutSwapBuffers();
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(795, 500);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("THE BREAKOUT GAME");
    Setting();
    glutDisplayFunc(Render);
    glutIdleFunc(Render);
    glutTimerFunc(1, Timer, 1);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutPassiveMotionFunc(MouseMotion);
    glutMainLoop();
    return 0;
}

