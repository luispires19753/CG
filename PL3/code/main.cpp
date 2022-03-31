#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#define _USE_MATH_DEFINES
#include <math.h>

float alpha = 0.25;
float beta = 0.5;
float raio = 7.5;

bool mouseDown = false;
float xrot = 0.0f;
float yrot = 0.0f;
float xdiff = 0.0f;
float ydiff = 0.0f;

void changeSize(int w, int h) {

	// Prevent a divide by zero, when window is too short
	// (you cant make a window with zero width).
	if (h == 0)
		h = 1;

	// compute window's aspect ratio 
	float ratio = w * 1.0 / h;

	// Set the projection matrix as current
	glMatrixMode(GL_PROJECTION);
	// Load Identity Matrix
	glLoadIdentity();

	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);

	// Set perspective
	gluPerspective(45.0f, ratio, 1.0f, 1000.0f);

	// return to the model view matrix mode
	glMatrixMode(GL_MODELVIEW);

}


void drawCone(float radius, int height, int slices, int stacks) {
	float sl = 2 * M_PI / float(slices);
	float st = float(radius) / float(stacks);
	float alt = float(height) / float(stacks);

	float a, b;

	glBegin(GL_TRIANGLES);

	for (a = 0; a <= 2 * M_PI; a += sl) {
		//face de baixo
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(radius * sin(a + sl), 0.0f, radius * cos(a + sl));
		glVertex3f(radius * sin(a), 0.0f, radius * cos(a));

		float aux = 0.0f;
		for (b = float(radius); b > st; b -= st) {
			glVertex3f(b * sin(a), aux, b * cos(a));
			glVertex3f(b * sin(a + sl), aux, b * cos(a + sl));
			glVertex3f((b - st) * sin(a), aux + alt, (b - st) * cos(a));

			glVertex3f(b * sin(a + sl), aux, b * cos(a + sl));
			glVertex3f((b - st) * sin(a + sl), aux + alt, (b - st) * cos(a + sl));
			glVertex3f((b - st) * sin(a), aux + alt, (b - st) * cos(a));

			aux += alt;
		}

		glVertex3f(0.0f, float(height), 0.0f);
		glVertex3f(radius * sin(a), 0, radius * cos(a));
		glVertex3f(radius * sin(a + sl), 0, radius * cos(a + sl));

	}
	glEnd();
}


void drawCylinder(float radius, float height, int slices) {
	float a;
	a = (2.0f * M_PI) / float(slices);
	// put code to draw cylinder in her


	glBegin(GL_TRIANGLES);

	for (float i = 0.0f; i <= 2.0f * M_PI; i += a) {

		glVertex3f(radius * sin(i), height, radius * cos(i));
		glVertex3f(radius * sin(i), 0.0f, radius * cos(i));
		glVertex3f(radius * sin(i + a), height, radius * cos(i + a));

		glVertex3f(radius * sin(i + a), height, radius * cos(i + a));
		glVertex3f(radius * sin(i), 0.0f, radius * cos(i));
		glVertex3f(radius * sin(i + a), 0.0f, radius * cos(i + a));

		
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(radius * sin(i+a), 0.0f, radius * cos(i+a));
		glVertex3f(radius*sin(i), 0.0f, radius * cos(i));
		

		glVertex3f(0.0f, height, 0.0f);
		glVertex3f(radius * sin(i), height, radius * cos(i));
		glVertex3f(radius * sin(i + a), height, radius * cos(i + a));

	}
	glEnd();

}

void drawSphere(float r, int slices, int stacks) {
	float a, b;
	float sl = 2.0f * M_PI / float(slices), st = M_PI / float(stacks);

	glBegin(GL_TRIANGLES);
	for (a = 0.0f; a <= 2 * M_PI; a += sl) {
		for (b = -M_PI / 2.0f; b < M_PI / 2.0f; b += st) {

				glVertex3f(r * cos(b) * sin(a), r * sin(b), r * cos(b) * cos(a));
				glVertex3f(r * cos(b) * sin(a + sl), r * sin(b), r * cos(b) * cos(a + sl));
				glVertex3f(r * cos(b + st) * sin(a), r * sin(b + st), r * cos(b + st) * cos(a));

				glVertex3f(r * cos(b) * sin(a + sl), r * sin(b), r * cos(b) * cos(a + sl));
				glVertex3f(r * cos(b + st) * sin(a + sl), r * sin(b + st), r * cos(b + st) * cos(a + sl));
				glVertex3f(r * cos(b + st) * sin(a), r * sin(b + st), r * cos(b + st) * cos(a));

		}
	}
	glEnd();
}


void drawAxis() {

	glBegin(GL_LINES);

	// X axis in red
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(-100.0f, 0.0f, 0.0f);
	glVertex3f(100.0f, 0.0f, 0.0f);

	// Y Axis in Green
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.0f, -100.0f, 0.0f);
	glVertex3f(0.0f, 100.0f, 0.0f);

	// Z Axis in Blue
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, -100.0f);
	glVertex3f(0.0f, 0.0f, 100.0f);

	glEnd();
}

void renderScene(void) {

	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// set the camera
	glLoadIdentity();

	gluLookAt(raio * cos(beta) * sin(alpha), raio * sin(beta), raio * cos(beta) * cos(alpha),
				0.0, 0.0, 0.0,
				0.0f, 1.0f, 0.0f);
	
	drawAxis();

	//glRotatef(45, 0.0, 0.0, 1.0);
	glTranslatef(cos(M_PI/4.0f), sin(M_PI/4.0f), 0.0f);
	glRotatef(45, 0.0f, 0.0f, 1.0f);

	























	//glRotatef(45, 0.0f, 0.0f, 1.0f);
	//glTranslatef(1.0f, 0.0f, 0.0f);

	glBegin(GL_TRIANGLES);
	glColor3f(0.0f, 1.0f, 1.0f);
	glVertex3f(-0.5f, 0.5, 0.0f);
	glVertex3f(-0.5f, -0.5f, 0.0f);
	glVertex3f(0.5f, 0.5f, 0.0f);

	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(-0.5f, -0.5f, 0.0f);
	glVertex3f(0.5f, -0.5f, 0.0f);
	glVertex3f(0.5f, 0.5f, 0.0f);

	
	glEnd();



	//drawSphere(1, 10, 10);
	//glTranslatef(-4.0f, 0.0f, 0.0f);
	//drawCylinder(1,2,10);
	//drawCone(1, 2, 20, 20);

	// End of frame
	glutSwapBuffers();
}


void processKeys(unsigned char c, int xx, int yy) {

	// put code to process regular keys in here
	switch (c) {
	case 'w':
		if (beta < 1.5f) {
			beta += 0.25f;
		}
		glutPostRedisplay();
		break;

	case 'a':
		alpha -= 0.2f;
		glutPostRedisplay();
		break;

	case 's':
		if (beta > -1.5f) {
			beta -= 0.25f;
		}
		glutPostRedisplay();
		break;

	case 'd':
		alpha += 0.2f;
		glutPostRedisplay();
		break;

	}
	
}


void processSpecialKeys(int key, int xx, int yy) {

	// put code to process special keys in here

}

void processMouse(int button, int state, int xx, int yy) {

}

void mouse2(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		mouseDown = true;

		xdiff = x - yrot;
		ydiff = -y + xrot;
	}
	else
		mouseDown = false;
}

void mouseMotion(int x, int y)
{
	if (mouseDown)
	{
		yrot = x - xdiff;
		xrot = y + ydiff;

		glutPostRedisplay();
	}
}
int main(int argc, char** argv) {

	// init GLUT and the window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(1500, 1000);
	glutCreateWindow("CG@DI-UM");

	// Required callback registry 
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);

	// Callback registration for keyboard processing
	glutKeyboardFunc(processKeys);
	
	glutMouseFunc(mouse2);
	glutMotionFunc(mouseMotion);

	//  OpenGL settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// enter GLUT's main cycle
	glutMainLoop();

	return 1;
}
