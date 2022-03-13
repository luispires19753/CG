#include <stdlib.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <vector>
#include <string>
#include <math.h>
#include <fstream>
#include <iostream>

#include "tinyxml/tinyxml2.h"

using namespace std;
using namespace tinyxml2;
using std::vector;

//struct para guardar os vertices de triangulos
struct Coords
{
	float xx;
	float yy;
	float zz;
};


vector<Coords> triangles;

float alfa;
float beta;
float raio;

float posX, posY, posZ, lookX, lookY, lookZ, upX, upY, upZ, fov, near, far;

//no need to change
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
		alfa -= 0.2f;
		glutPostRedisplay();
		break;

	case 's':
		if (beta > -1.5f) {
			beta -= 0.25f;
		}
		glutPostRedisplay();
		break;

	case 'd':
		alfa += 0.2f;
		glutPostRedisplay();
		break;

	}
}

void changeSize(int w, int h) {

	if(h == 0)
		h = 1;

	float ratio = w * 1.0 / h;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
    glViewport(0, 0, w, h);
	gluPerspective(fov ,ratio, near ,far);
	glMatrixMode(GL_MODELVIEW);
}

// desenha o eixo
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

void camera(float pX, float pY, float pZ, float lX, float lY, float lZ, float uX, float uY, float uZ) {
	gluLookAt(pX, pY, pZ,
				lX, lY, lZ,
				uX, uY, uZ);
}


void renderScene(void) {

	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// set the camera
	glLoadIdentity();
	
	/*
	gluLookAt(raio * cos(beta) * sin(alpha), raio * sin(beta), raio * cos(beta) * cos(alpha),
				0.0, 0.0, 0.0,
				0.0f, 1.0f, 0.0f);
	*/
	
	camera(raio*cos(beta)*sin(alfa), raio*sin(beta), raio * cos(beta) * cos(alfa), lookX, lookY, lookZ, upX, upY, upZ);
	drawAxis();

	glBegin(GL_TRIANGLES);

	for (int i = 0; i < triangles.size(); i += 3) {
		glColor3f(1.0f, 1.0f, 1.0f);
		glVertex3f(triangles[i].xx, triangles[i].yy, triangles[i].zz);
		glVertex3f(triangles[i + 1].xx, triangles[i + 1].yy, triangles[i + 1].zz);
		glVertex3f(triangles[i + 2].xx, triangles[i + 2].yy, triangles[i + 2].zz);
	}

	glEnd();
	
	glutSwapBuffers();
}

//funçao que vai ler os ficheiros .3d e desenhar as figuras
void readFile(const char* fname) {
	string line;
	ifstream file(fname);

	if (file.fail()) {
		throw std::ios_base::failure(string("Ficheiro não encontrado: ") + fname);
	}

	else {
		while (getline(file, line)) {
			size_t pos;

			float x, y, z;

			x = std::stof(line, &pos);
			line.erase(0, pos + 1);
			y = std::stof(line, &pos);
			line.erase(0, pos + 1);
			z = std::stof(line, &pos);

			struct Coords Aux {};
			Aux.xx = x;
			Aux.yy = y;
			Aux.zz = z;

			triangles.push_back(Aux);
		}
	}
	file.close();
}

//funçao que vai ler o ficheiro XML e extrair informação sobre a camara, assim como
//quais os ficheiros 3d a ler

void readXML(const char* file) {
	//const char* path = "C:\Users\pires\Desktop\Phase 1\code";
	XMLDocument doc;
	doc.LoadFile(file);

	XMLElement* pRoot = doc.FirstChildElement("world");

	//cout << " Root Element is : " << pRootElement->Value();
	if (pRoot == nullptr){
		cout << "Root empty";
		return;
	}

	XMLElement* camara = pRoot->FirstChildElement("camera");
	if (camara != NULL) {
		XMLElement* position = camara->FirstChildElement("position");
		posX = atof(position->Attribute("x"));
		posY = atof(position->Attribute("y"));
		posZ = atof(position->Attribute("z"));
		XMLElement* lookAt = camara->FirstChildElement("lookAt");
		lookX = atof(lookAt->Attribute("x"));
		lookY = atof(lookAt->Attribute("y"));
		lookZ = atof(lookAt->Attribute("z"));
		XMLElement* up = camara->FirstChildElement("up");
		upX = atof(up->Attribute("x"));
		upY = atof(up->Attribute("y"));
		upZ = atof(up->Attribute("z"));
		XMLElement* projection = camara->FirstChildElement("projection");
		fov = atof(projection->Attribute("fov"));
		near = atof(projection->Attribute("near"));
		far = atof(projection->Attribute("far"));

		raio = sqrtf(powf(posX,2) + powf(posY,2) + powf(posZ,2));
		beta = asin(posY / raio);
		alfa = asin(posX / (raio * cos(beta)));

	}
	XMLElement* group = pRoot->FirstChildElement("group");
	if (group != NULL) {
		XMLElement* models = group->FirstChildElement("models");
		XMLElement* model = models->FirstChildElement("model");
		for (; model != NULL; model = model->NextSiblingElement()) {
			const char* ficheiro = model->Attribute("file");
			readFile(ficheiro);
		}
	}
}

int main(int argc, char **argv) {

// init GLUT and the window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
	glutInitWindowPosition(80,80);
	glutInitWindowSize(800,800);
	glutCreateWindow("Engine - Phase 1");
	
	
	if (argc != 2) {
		printf("Invalid input");
		return 0;
	}
	else {
		readXML(argv[1]);
	}
	
// Required callback registry 
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);

	glutKeyboardFunc(processKeys);

//  OpenGL settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	
// enter GLUT's main cycle
	glutMainLoop();
	
	return 1;
}