#include<stdio.h>
#include<stdlib.h>

#define _USE_MATH_DEFINES
#include <math.h>
#include <vector>

#include <IL/il.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif

#include <vector>

using std::vector;

GLuint buffers[2], vC;

typedef struct {
	float fst;
	float snd;
} Coord;

Coord arvores[200];

unsigned int tw, th;
unsigned char* imageData;

float camX = 0.0, camY = 30.0, camZ = 40.0;
int startX, startY, tracking = 0;

float alpha = 0, beta = 45, r = 50;

void changeSize(int w, int h) {

	// Prevent a divide by zero, when window is too short
	// (you cant make a window with zero width).
	if(h == 0)
		h = 1;

	// compute window's aspect ratio 
	float ratio = w * 1.0 / h;

	// Reset the coordinate system before modifying
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	// Set the viewport to be the entire window
    glViewport(0, 0, w, h);

	// Set the correct perspective
	gluPerspective(45,ratio,1,1000);

	// return to the model view matrix mode
	glMatrixMode(GL_MODELVIEW);
}

float getHeight(int i, int j) {

	return float(imageData[i * tw + j])*100.0f/255.0f;
}


void drawTree() {

	glPushMatrix();

	glColor3f(0.36f, 0.25f, 0.2f);
	glRotatef(180, 1.0f, 0.0f, 0.0f);
	glTranslatef(0.0f, -12.0f, 0.0f);

	glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
	glVertexPointer(3, GL_FLOAT, 0, 0);
	glDrawArrays(GL_TRIANGLES, 0, vC);

	glPopMatrix();


	glPushMatrix();

	glColor3f(0.0f, 0.0f, 1.0f);
	glRotatef(180, 1.0f, 0.0f, 0.0f);
	glTranslatef(55.0f, -(12.0 + getHeight(55.0, 55.0)), 55.0f);

	glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
	glVertexPointer(3, GL_FLOAT, 0, 0);
	glDrawArrays(GL_TRIANGLES, 0, vC);

	glPopMatrix();

	//printf("%f\n", getHeight(55.0, 55.0));


	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	for (int k = 0; k < 200; k++) {

		glPushMatrix();

		glColor3f(0.36f, 0.25f, 0.2f);

		int i = arvores[k].fst;
		int j = arvores[k].snd;

		float aux_h = float(imageData[i * tw + j]);

		glTranslatef(i, 0, j);
		glRotatef(180, 1.0f, 0.0f, 0.0f);

		glTranslatef(0.0f, -12.0f, 0.0f);
		
		glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
		glVertexPointer(3, GL_FLOAT, 0, 0);
		glDrawArrays(GL_TRIANGLES, 0, vC);

		glPopMatrix();
	}

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

}


void drawTerrain() {

	glPushMatrix();

	glColor3f(1.0, 0.45, 0.0);
	
	glBindBuffer(GL_ARRAY_BUFFER, buffers[0]); 
	glVertexPointer(3, GL_FLOAT, 0, 0);

	for (int i = 0; i < th - 1; i++) { 
		glDrawArrays(GL_TRIANGLE_STRIP, tw * 2 * i, tw * 2); 
	}

	glPopMatrix();
}


void drawAxis() {

	glBegin(GL_LINES);

	// X axis in red
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(-300.0f, 0.0f, 0.0f);
	glVertex3f(300.0f, 0.0f, 0.0f);

	// Y Axis in Green
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.0f, -300.0f, 0.0f);
	glVertex3f(0.0f, 300.0f, 0.0f);

	// Z Axis in Blue
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, -300.0f);
	glVertex3f(0.0f, 0.0f, 300.0f);

	glEnd();
}


void renderScene(void) {

	float pos[4] = {-1.0, 1.0, 1.0, 0.0};

	//glClearColor(0.0f,0.0f,0.0f,0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	gluLookAt(camX, camY, camZ, 
		      0.0,0.0,0.0,
			  0.0f,1.0f,0.0f);

	drawAxis();
	drawTerrain();
	drawTree();



// End of frame
	glutSwapBuffers();
}

void spherical2Cartesian() {

	camX = r * cos(beta) * sin(alpha);
	camY = r * sin(beta);
	camZ = r * cos(beta) * cos(alpha);
}

void processKeys(unsigned char key, int xx, int yy) {

	switch (key) {

	case 'd':
		alpha -= 0.1; break;

	case 'a':
		alpha += 0.1; break;

	case 'w':
		beta += 0.1;
		if (beta > 1.5)
			beta = 1.5;
		break;

	case 's':
		beta -= 0.1;
		if (beta < -1.5)
			beta = -1.5;
		break;

	case GLUT_KEY_PAGE_DOWN: r -= 0.1;
		if (r < 0.1)
			r = 0.1;
		break;

	case GLUT_KEY_PAGE_UP: r += 0.1; break;
	}
	spherical2Cartesian();
	glutPostRedisplay();

}


void processMouseButtons(int button, int state, int xx, int yy) {
	
	if (state == GLUT_DOWN)  {
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
			alpha += (xx - startX);
			beta += (yy - startY);
		}
		else if (tracking == 2) {
			
			r -= yy - startY;
			if (r < 3)
				r = 3.0;
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


		alphaAux = alpha + deltaX;
		betaAux = beta + deltaY;

		if (betaAux > 85.0)
			betaAux = 85.0;
		else if (betaAux < -85.0)
			betaAux = -85.0;

		rAux = r;
	}
	else if (tracking == 2) {

		alphaAux = alpha;
		betaAux = beta;
		rAux = r - deltaY;
		if (rAux < 3)
			rAux = 3;
	}
	camX = rAux * sin(alphaAux * 3.14 / 180.0) * cos(betaAux * 3.14 / 180.0);
	camZ = rAux * cos(alphaAux * 3.14 / 180.0) * cos(betaAux * 3.14 / 180.0);
	camY = rAux * 							     sin(betaAux * 3.14 / 180.0);
}

void arvores_tempo() {

	Coord a;

	for (int k = 0; k < 200; k++) {

		float f = rand() % 256 - 128;
		float s = rand() % 256 - 128;

		/*
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
		*/

		a.fst = f;
		a.snd = s;

		arvores[k] = a;
	}
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
			p.push_back(aux + alt);
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

	unsigned int t;
// 	Load the height map "terreno.jpg"
	ilGenImages(1, &t);
	ilBindImage(t);
	ilLoadImage((ILstring)"terreno.jpg");
	ilConvertImage(IL_LUMINANCE, IL_UNSIGNED_BYTE);

	tw = ilGetInteger(IL_IMAGE_WIDTH);
	th = ilGetInteger(IL_IMAGE_HEIGHT);
	// imageData is a LINEAR array with the pixel values
	imageData = ilGetData();

// 	Build the vertex arrays
	vector<float> p;

	float step_h = (th - 1) / 2.0f;
	float step_w = (tw - 1) / 2.0f;

	for (int i = 0; i < th - 1; i++) {

		for (int j = 0; j < tw; j++) {

			p.push_back(float(j) - step_w);
			p.push_back(getHeight(i,j));
			p.push_back(float(i) - step_h);

			p.push_back(float(j) - step_w);
			p.push_back(getHeight(i+1, j));
			p.push_back(float(i) - step_h + 1);

		}
	}
	
	
	printf("%f\n%d\n", getHeight(55.0, 55.0), p.size());

	glGenBuffers(2, buffers);
	glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
	glBufferData(GL_ARRAY_BUFFER, 
					sizeof(float) * p.size(), 
					p.data(), 
					GL_STATIC_DRAW);

	vector<float> p2;

	cone1(3.0, 12, 50, 50, p2);
	vC = p2.size() / 3;
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float) * p2.size(), p2.data(), GL_STATIC_DRAW);

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
	glutIdleFunc(renderScene);
	glutReshapeFunc(changeSize);

// Callback registration for keyboard processing
	glutKeyboardFunc(processKeys);
	glutMouseFunc(processMouseButtons);
	glutMotionFunc(processMouseMotion);

	#ifndef __APPLE__
		glewInit();
	#endif

	ilInit();
	init();	

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

// enter GLUT's main cycle
	glColor3f(0.0, 0.0, 0.0);
	glClearColor(0.9, 0.9, 0.9, 1.0);
	glutMainLoop();
	
	return 0;
}

