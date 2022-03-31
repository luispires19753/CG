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
#include "group.h"

#include "tinyxml/tinyxml2.h"

using namespace std;
using namespace tinyxml2;
using std::vector;

Group* scene = new Group();


float alfa;
float beta;
float raio;

float posX, posY, posZ, lookX, lookY, lookZ, upX, upY, upZ, fov, near, far;

vector<Vertice*> readFile(string fname) {
	string line;
	ifstream file(fname);
	vector<Vertice*> vertices;

	if (file.fail()) {
		throw std::ios_base::failure(string("Couldn't find file: ") + fname);
	}

	else {
		while (getline(file, line)) {
			size_t pos;

			float x, y, z;

			x = stof(line, &pos);
			line.erase(0, pos + 1);
			y = stof(line, &pos);
			line.erase(0, pos + 1);
			z = stof(line, &pos);

			Vertice* v = new Vertice(x, y, z);
			vertices.push_back(v);
		}
	}

	file.close();
	return vertices;
}

Group* readGroup(XMLElement* element) {
	Group* g = new Group();

	for (element = element->FirstChildElement(); element; element = element->NextSiblingElement()) {
		if (strcmp(element->Value(), "transform") == 0) {
			XMLElement* elementChild;
			for (elementChild = element->FirstChildElement(); elementChild; elementChild = elementChild->NextSiblingElement()) {

				char* name = (char*)elementChild->Value();
				
				float x = 0, y = 0, z = 0, angle = 0;

				if (strcmp(elementChild->Value(), "translate") == 0) {
					x = atof(elementChild->Attribute("x"));
					y = atof(elementChild->Attribute("y"));
					z = atof(elementChild->Attribute("z"));
				}

				if (strcmp(elementChild->Value(), "rotate") == 0) {
					x = atof(elementChild->Attribute("x"));
					y = atof(elementChild->Attribute("y"));
					z = atof(elementChild->Attribute("z"));
					angle = atof(elementChild->Attribute("angle"));
				}

				if (strcmp(elementChild->Value(), "scale") == 0) {
					x = atof(elementChild->Attribute("x"));
					y = atof(elementChild->Attribute("y"));
					z = atof(elementChild->Attribute("z"));
				}
				
				Operacoes* o = new Operacoes(strdup(name), x, y, z, angle);

				g->pushOps(o);

			}
					
		}

		if (strcmp(element->Value(), "models") == 0) {
			XMLElement* elementChild;
			for (elementChild = element->FirstChildElement(); elementChild; elementChild = elementChild->NextSiblingElement()) {
				string ficheiro = elementChild->Attribute("file");
				Model* m = new Model(readFile(ficheiro));

				g->pushModel(m);
			}
		}

		if (strcmp(element->Value(), "group") == 0) {
			Group* child = readGroup(element);
			g->pushFilho(child);
		}
	}
	return g;
}

Group* readXML(const char * file) {
	XMLDocument doc;
	doc.LoadFile(file);
	Group* g = new Group();

	XMLElement* pRoot = doc.FirstChildElement("world");

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

		raio = sqrtf(powf(posX, 2) + powf(posY, 2) + powf(posZ, 2));
		beta = asin(posY / raio);
		alfa = asin(posX / (raio * cos(beta)));

	}

	XMLElement* group = pRoot->FirstChildElement("group");
	
	g = readGroup(group);

	return g;
}

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

void renderGroup(Group* g) {
	
	glPushMatrix();

	for (Operacoes* op : g->getOps()) {
		if (strcmp(op->getName(), "translate") == 0) {
			glTranslatef(op->getX(), op->getY(), op->getZ());
		}
		if (strcmp(op->getName(), "rotate") == 0) {
			glRotatef(op->getAngle(), op->getX(), op->getY(), op->getZ());
		}
		if (strcmp(op->getName(), "scale") == 0) {
			glScalef(op->getX(), op->getY(), op->getZ());
		}
	}

	glBegin(GL_TRIANGLES);

	for (Model* m : g->getModel()) {
		std::vector<Vertice*> v = m->getVertices();

		for (int i = 0; i < v.size(); i += 3) {
			glColor3f(1, 1, 1);
			glVertex3f(v[i]->getX(), v[i]->getY(), v[i]->getZ());
			glVertex3f(v[i + 1]->getX(), v[i + 1]->getY(), v[i + 1]->getZ());
			glVertex3f(v[i + 2]->getX(), v[i + 2]->getY(), v[i + 2]->getZ());
		}
	}

	glEnd();

	for (Group* child : g->getFilhos()) {
		renderGroup(child);
	}

	glPopMatrix();
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
	
	//camera(posX, posY, posZ, lookX, lookY, lookZ, upX, upY, upZ);
	camera(raio*cos(beta)*sin(alfa), raio*sin(beta), raio * cos(beta) * cos(alfa), lookX, lookY, lookZ, upX, upY, upZ);
	drawAxis();

	renderGroup(scene);

	//glEnd();
	
	glutSwapBuffers();
}


int main(int argc, char **argv) {

// init GLUT and the window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
	glutInitWindowPosition(80,80);
	glutInitWindowSize(800,800);
	glutCreateWindow("Engine - Phase 2");
	
	
	if (argc != 2) {
		printf("Invalid input");
		return 0;
	}
	else {
		scene = readXML(argv[1]);
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