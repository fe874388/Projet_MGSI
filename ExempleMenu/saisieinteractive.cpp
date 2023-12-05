#include <iostream>
#include <new>
#include <math.h>
#include <GL/glut.h>
#include <vector>

const float PI = 3.1415926535;

// variables globales pour OpenGL
int window,width,height;
const int NMAX = 100;
int N = 0;

int mp=-1,droite=0,gauche=0;

struct Point {
	float x,y;
	Point(float a=0, float b=0) {set(a,b);}
	void set(float a, float b) {x=a;y=b;}
};

enum TypeBouton
{
action1 = 0,
action2,
action3,
action4
}bouton_action = action1;

static void menu (int item)
{
bouton_action = static_cast<TypeBouton> (item);
glutPostRedisplay ();
}

Point P[NMAX];

void ResetPoints() {
    for (int i = 0; i < NMAX; ++i) {
        P[i].set(0, 0);
    }
    N = 0;
    glutPostRedisplay();
}

// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// ATTENTION !  Les instructions tourne en boucle dans les fonctions d'appelle
static void appelaction1()
{
    // Instruction ... Fonction 1
}

static void appelaction2()
{
    // Fonction 2
}

static void appelaction3()
{
    // Fonction 3
}

static void appelaction4()
{
    // Fonction 4
}

void Trace()
{
	glBegin(GL_POINTS);
	for (int i=0;i<N;i++){
		glVertex2f(P[i].x,P[i].y);
	}
	glEnd();
}

void main_reshape(int width,  int height)
{
	GLint viewport[4];
    glViewport(0, 0, width, height);
    glGetIntegerv(GL_VIEWPORT, viewport);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	glOrtho(0.0, viewport[2], 0.0, viewport[3], -50.0, 50.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}


void main_display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glColor3f(0.0,1.0,0.0);
    glPointSize(3.0);
	glInitNames();
	glPushName(1);
	for (int i=0;i<N;i++){
		glLoadName(i);
		glBegin(GL_POINTS);
		glVertex2f(P[i].x,P[i].y);
		glEnd();
	}
	glutPostRedisplay();

	switch (bouton_action)
	{
	case action1:
        appelaction1();
        break;
    case action2:
        appelaction2();
        break;
    case action3:
        appelaction3();
        break;
    case action4:
        appelaction4();
        break;
    }
    glutSwapBuffers();
}



void Mouse(int button, int state, int x, int y) {
	GLint viewport[4];

	glutSetCursor(GLUT_CURSOR_CROSSHAIR);
	glGetIntegerv(GL_VIEWPORT, viewport);

	if(button == GLUT_LEFT_BUTTON) {
		droite = 0; gauche = 1;
			glColor3f(0.0,1.0,0.0);
			glPointSize(3.0);
			glInitNames();
			glPushName(1);

			P[N].x = x;
			P[N].y = viewport[3]-y;

			glLoadName(N);
			glBegin(GL_POINTS);
				glVertex2f(P[N].x,P[N].y);
			glEnd();

			if(state == GLUT_UP) N++;

			glutPostRedisplay();



		}

	if(button == GLUT_RIGHT_BUTTON) {
		gauche = 0; droite = 1;
		if(state == GLUT_DOWN) {
			GLuint *selectBuf = new GLuint[200];
			GLuint *ptr;
			GLint hits;

			glSelectBuffer(200, selectBuf);
			glRenderMode(GL_SELECT);

			glPushMatrix();
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			gluPickMatrix(x, viewport[3]-y, 5.0, 5.0, viewport);
			glOrtho(0.0, viewport[2], 0.0, viewport[3], -50.0, 50.0);

			glColor3f(0.0,1.0,0.0);
			glPointSize(3.0);
			glInitNames();
			glPushName(1);

			for (int i = 0;i<N;i++) {
				glLoadName(i);
				glBegin(GL_POINTS);
				glVertex2f(P[i].x,P[i].y);
				glEnd();
			}

			glPopMatrix();
			glFlush();

			hits = glRenderMode(GL_RENDER);
			if(hits) {
				ptr = (GLuint *)selectBuf;
				ptr += 3;
				mp = *ptr;
			}
		}

		if(state == GLUT_UP)
			mp = -1;

		main_reshape(viewport[2], viewport[3]);
		glutPostRedisplay();
	}
}

void Motion(int x, int y) {
	GLint viewport[4];

	glGetIntegerv(GL_VIEWPORT, viewport);

	if ((droite) &&(mp != -1)) {
		int i= mp;
		P[i].x = x;
		P[i].y = viewport[3]-y;
		glutPostRedisplay();
	}
}

int main (int argc, char** argv)
{
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(0, 0);
    glutInit(&argc, argv);
	glEnable(GL_DEPTH_TEST);

    window = glutCreateWindow("Select");
    glutReshapeFunc(main_reshape);
    glutDisplayFunc(main_display);
  	glutMouseFunc(Mouse);
	glutMotionFunc(Motion);

	//   *********************************************************************************
	// Creation MENU glut
	glutCreateMenu (menu);
	glutAddMenuEntry ("Cone", action1);
	glutAddMenuEntry ("Cube", action2);
	glutAddMenuEntry ("Pyramide", action3);
	glutAddMenuEntry ("Miroir cylindrique", action4);

	// par exemple si on veut faire apparaître le menu avec clic droit
	glutAttachMenu (GLUT_RIGHT_BUTTON);

	glutPostRedisplay();
    glutMainLoop();

    return 0;
}
