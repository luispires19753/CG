#include<stdio.h>
#include<stdlib.h>
#include <time.h>

#define _USE_MATH_DEFINES
#include <math.h>
#include <vector>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif


using std::vector;

GLuint buffers[2] , verticeCount1, verticeCount2;

float camX = 0.0, camY = 30.0, camZ = 40.0;
int startX, startY, tracking = 0;
float alfa = 0.0f, beta = 0.5f, radius = 200.0f;

float i = 0.0f, j = 0.0f;

typedef struct {
	float fst;
	float snd;
} Coord;

Coord arvores[200];

void spherical2Cartesian() {

	camX = radius * cos(beta) * sin(alfa);
	camY = radius * sin(beta);
	camZ = radius * cos(beta) * cos(alfa);
}

void arvores_tempo() {

	Coord a;

	for (int k = 0; k < 200; k++) {

		float f = rand() % 200 - 100;
		float s = rand() % 200 - 100;

		// 1º Quadrante
		if (f >= 0 && f < 50 && s >= 0 && s < 50) {

			f += 50;
			s += 50;
		}
		// 2º Quadrante
		else if (f > -50 && f <= 0 && s >= 0 && s < 50) {

			f -= 50;
			s += 50;
		}
		// 3º Quadrante
		else if (f > -50 && f <= 0 && s > -50 && s <= 0) {

			f -= 50;
			s -= 50;
		}
		// 4º Quadrante
		else if (f >= 0 && f < 50 && s > -50 && s <= 0) {

			f += 50;
			s -= 50;
		}

		a.fst = f;
		a.snd = s;

		arvores[k] = a;
	}
}

void changeSize(int w, int h) {

	// Prevent a divide by zero, when window is too short
	// (you cant make a window with zero width).
	if(h == 0)
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
	gluPerspective(45.0f ,ratio, 1.0f ,1000.0f);

	// return to the model view matrix mode
	glMatrixMode(GL_MODELVIEW);
}

void processKeys(unsigned char c, int xx, int yy) {

	time_t t;
	// put code to process regular keys in here
	switch (c) {

	case 'd':
		alfa -= 0.1; break;

	case 'a':
		alfa += 0.1; break;

	case 'w':
		beta += 0.1f;
		if (beta > 1.5f)
			beta = 1.5f;
		break;

	case 's':
		beta -= 0.1f;
		if (beta < -1.5f)
			beta = -1.5f;
		break;
	case ' ':
		srand((unsigned)time(&t));
		arvores_tempo();
		break;

	}

	spherical2Cartesian();
	glutPostRedisplay();
}


void processSpecialKeys(int key, int xx, int yy) {

	switch (key) {

	case GLUT_KEY_RIGHT:
		alfa -= 0.1; break;

	case GLUT_KEY_LEFT:
		alfa += 0.1; break;

	case GLUT_KEY_UP:
		beta += 0.1f;
		if (beta > 1.5f) {
			beta = 1.5f;
		}
		break;

	case GLUT_KEY_DOWN:
		beta -= 0.1f;
		if (beta < -1.5f) {
			beta = -1.5f;
		}
		break;

	case GLUT_KEY_PAGE_DOWN: radius -= 5.0f;
		if (radius < 1.0f) {
			radius = 1.0f;
		}
		break;

	case GLUT_KEY_PAGE_UP: radius += 5.0f; break;
	}
	spherical2Cartesian();
	glutPostRedisplay();

}

void processMouseButtons(int button, int state, int xx, int yy) {

	if (state == GLUT_DOWN) {
		startX = xx;
		startY = yy;
		if (button == GLUT_LEFT_BUTTON)
			tracking = 1;
		else if (button == GLUT_RIGHT_BUTTON)
			tracking = 2;
		else
			tracking = 0;
	}
	else if (state == GLUT_UP) {
		if (tracking == 1) {
			alfa += (xx - startX);
			beta += (yy - startY);
		}
		else if (tracking == 2) {

			radius -= yy - startY;
			if (radius < 3)
				radius = 3.0;
		}
		tracking = 0;
	}
}

void processMouseMotion(int xx, int yy) {

	int deltaX, deltaY;
	int alphaAux, betaAux;
	int rAux;

	if (!tracking)
		return;

	deltaX = xx - startX;
	deltaY = yy - startY;

	if (tracking == 1) {


		alphaAux = alfa + deltaX;
		betaAux = beta + deltaY;

		if (betaAux > 85.0)
			betaAux = 85.0;
		else if (betaAux < -85.0)
			betaAux = -85.0;

		rAux = radius;
	}
	else if (tracking == 2) {

		alphaAux = alfa;
		betaAux = beta;
		rAux = radius - deltaY;
		if (rAux < 3)
			rAux = 3;
	}
	camX = rAux * sin(alphaAux * 3.14 / 180.0) * cos(betaAux * 3.14 / 180.0);
	camZ = rAux * cos(alphaAux * 3.14 / 180.0) * cos(betaAux * 3.14 / 180.0);
	camY = rAux * sin(betaAux * 3.14 / 180.0);
}

void drawAxis() {

	glBegin(GL_LINES);

	// X Axis in Red
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

void drawCowboys() {

	if (i == 8) {
		i = 0;
	}

	glColor3f(0.0f, 0.0f, 1.0f);

	for (float ii = i; ii < 8+i; ii++) {
		glPushMatrix();

		glRotatef(-45.0f * ii, 0.0f, 1.0f, 0.0f);
		glTranslatef(10.0f, 1.5f, 0.0f);
		glutSolidTeapot(2);

		glPopMatrix();
	}

	i += 0.01f;
}

void drawEnemies() {

	if (j == 16) {
		j = 0;
	}

	glColor3f(1.0f, 0.0f, 0.0f);

	for (float jj = j; jj < 16 + j; jj++) {
		glPushMatrix();

		glRotatef(22.5f * jj, 0.0f, 1.0f, 0.0f);
		glTranslatef(30.0f, 1.5f, 0.0f);
		glRotatef(90, 0.0f, 1.0f, 0.0f);
		glutSolidTeapot(2);

		glPopMatrix();
	}

	j += 0.01f;
}

void drawTree() {

	for (int k = 0; k < 200; k++) {

		glPushMatrix();

		glColor3f(0.36f, 0.25f, 0.2f);
		glTranslatef(arvores[k].fst, 0, arvores[k].snd);
		
		glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
		glVertexPointer(3, GL_FLOAT, 0, 0);
		glDrawArrays(GL_TRIANGLES, 0, verticeCount1);

		
		glColor3f(0.0f, 0.39f, 0.0f);
		glTranslatef(0.0f, 4.0, 0.0f);
	
		glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
		glVertexPointer(3, GL_FLOAT, 0, 0);
		glDrawArrays(GL_TRIANGLES, 0, verticeCount2);
		

		glPopMatrix();
	}
	
}

void renderScene(void) {

	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// set the camera
	glLoadIdentity();
	gluLookAt(camX, camY, camZ,
		0.0, 0.0, 0.0,
		0.0f, 1.0f, 0.0f);

	glColor3f(0.2f, 0.8f, 0.2f);
	glBegin(GL_TRIANGLES);
		glVertex3f(100.0f, 0, -100.0f);
		glVertex3f(-100.0f, 0, -100.0f);
		glVertex3f(-100.0f, 0, 100.0f);

		glVertex3f(100.0f, 0, -100.0f);
		glVertex3f(-100.0f, 0, 100.0f);
		glVertex3f(100.0f, 0, 100.0f);
	glEnd();

	//drawAxis();

	glColor3f(0.96f, 0.26f, 0.88f);
	glutSolidTorus(1.0f, 2.5f, 50, 50);

	drawCowboys();
	drawEnemies();

	drawTree();

	// End of frame
	glutSwapBuffers();
}

void cone1(int r, int height, int slices, int stacks, vector<float>& p) {

		float sl = 2 * M_PI / float(slices);
		float st = float(r) / float(stacks);
		float alt = float(height) / float(stacks);

		float a, b;

		for (a = 0; a <= 2 * M_PI; a += sl) {
			//face de baixo
			//glVertex3f(0.0f, 0.0f, 0.0f);
			p.push_back(0.0);
			p.push_back(0.0);
			p.push_back(0.0);

			//glVertex3f(radius * sin(a + sl), 0.0f, radius * cos(a + sl));
			p.push_back(float(r) * sin(a + sl));
			p.push_back(0.0);
			p.push_back(float(r) * cos(a + sl));

			//glVertex3f(radius * sin(a), 0.0f, radius * cos(a));
			p.push_back(float(r) * sin(a));
			p.push_back(0.0);
			p.push_back(float(r) * cos(a));

			float aux = 0.0f;
			for (b = float(r); b > st; b -= st) {
				//glVertex3f(b * sin(a), aux, b * cos(a));
				p.push_back(b * sin(a));
				p.push_back(aux);
				p.push_back(b * cos(a));

				//glVertex3f(b * sin(a + sl), aux, b * cos(a + sl));
				p.push_back(b * sin(a + sl));
				p.push_back(aux);
				p.push_back(b * cos(a + sl));

				//glVertex3f((b - st) * sin(a), aux + alt, (b - st) * cos(a));
				p.push_back((b - st) * sin(a));
				p.push_back(aux + alt);
				p.push_back((b - st) * cos(a));

				//glVertex3f(b * sin(a + sl), aux, b * cos(a + sl));
				p.push_back(b * sin(a + sl));
				p.push_back(aux);
				p.push_back(b * cos(a + sl));

				//glVertex3f((b - st) * sin(a + sl), aux + alt, (b - st) * cos(a + sl));
				p.push_back((b - st) * sin(a + sl));
				p.push_back(aux+alt);
				p.push_back((b - st) * cos(a + sl));

				//glVertex3f((b - st) * sin(a), aux + alt, (b - st) * cos(a));
				p.push_back((b - st) * sin(a));
				p.push_back(aux + alt);
				p.push_back((b - st) * cos(a));

				aux += alt;
			}

			//glVertex3f(0.0f, float(height), 0.0f);
			p.push_back(0.0);
			p.push_back(float(height));
			p.push_back(0.0);

			//glVertex3f(radius * sin(a), 0, radius * cos(a));
			p.push_back(float(r) * sin(a));
			p.push_back(0.0);
			p.push_back(float(r) * cos(a));
			
			//glVertex3f(radius * sin(a + sl), 0, radius * cos(a + sl));
			p.push_back(float(r) * sin(a + sl));
			p.push_back(0.0);
			p.push_back(float(r) * cos(a + sl));

		}

}

void init() {

	// 	Build the vertex arrays
	vector<float> p;
	vector<float> p2;

	cone1(1, 7, 50, 50, p);
	cone1(3, 12, 50, 50, p2);
	
	
	verticeCount1 = p.size() / 3;
	verticeCount2 = p2.size() / 3;

	printf("%d %d\n", p.size(), p2.size());
	
	glGenBuffers(2, buffers);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float) * p.size(), p.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float) * p2.size(), p2.data(), GL_STATIC_DRAW);
	
}

void printInfo() {

	printf("Vendor: %s\n", glGetString(GL_VENDOR));
	printf("Renderer: %s\n", glGetString(GL_RENDERER));
	printf("Version: %s\n", glGetString(GL_VERSION));

	printf("\nUse Arrows to move the camera up/down and left/right\n");
	printf("Home and End control the distance from the camera to the origin");
}


int main(int argc, char **argv) {

// init GLUT and the window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(800,800);
	glutCreateWindow("CG@DI-UM");

// Required callback registry
	arvores_tempo();
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);
	glutIdleFunc(renderScene);
	
// Callback registration for keyboard processing
	glutKeyboardFunc(processKeys);
	glutSpecialFunc(processSpecialKeys);
	glutMouseFunc(processMouseButtons);
	glutMotionFunc(processMouseMotion);

//  OpenGL settings
#ifndef __APPLE__
	glewInit();
#endif

	init();
	//  OpenGL settings
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	spherical2Cartesian();

	printInfo();

// enter GLUT's main cycle
	glutMainLoop();
	
	return 1;
}
