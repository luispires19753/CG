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
using std::vector;


struct Coords
{
	float xx;
	float yy;
	float zz;
};


vector<Coords> triangles;

//no need to change
void changeSize(int w, int h) {

	if (h == 0)
		h = 1;

	float ratio = w * 1.0 / h;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, w, h);
	gluPerspective(45.0f, ratio, 1.0f, 1000.0f);
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
	gluLookAt(5.0, 5.0, 5.0,
		0.0, 0.0, 0.0,
		0.0f, 1.0f, 0.0f);
	*/
	camera(posX, posY, posZ, lookX, lookY, lookZ, upX, upY, upZ);
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


void readFile(string fname) {
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




float posX, posY, posZ, lookX, lookY, lookZ, upX, upY, upZ;

int readXML(string f_path) {
	XMLDocument doc;

	if (doc.LoadFile(f_path) != XML_SUCCESS) {
		printf("Ocorreu um erro na leitura do ficheiro XML\n");
		return 1;
	}

	XMLNode* root = doc.FirstChild();

	if (strcmp("world", root->Value()) != 0) {
		printf("Esperado valor 'world', obtido %s\n", root->Value());
		return 1;
	}

	if (root->NoChildren()) {
		printf("Não contém dados\n");
		return 1;
	}

	XMLNode* camera = root->FirstChild();

	if (strcmp("camera", camera->Value()) != 0) {
		printf("Esperado valor 'camera', obtido %s\n", camera->Value());
		return 1;
	}

	if (camera->NoChildren()) {
		printf("Não contém definiçoes da camara\n");
		return 1;
	}

	XMLNode* setting = camera->FirstChild();

	while (setting != nullptr) {

		XMLElement* element = setting->ToElement();

		if (strcmp("positon", setting->Value()) == 0) {
			posX = atof(element->Atribute("x"));
			posY = atof(element->Atribute("y"));
			posZ = atof(element->Atribute("z"));
		}

		if (strcmp("lookAt", setting->Value()) == 0) {
			lookX = atof(element->Atribute("x"));
			lookY = atof(element->Atribute("y"));
			lookZ = atof(element->Atribute("z"));
		}

		if (strcmp("up", setting->Value()) == 0) {
			upX = atof(element->Atribute("x"));
			upY = atof(element->Atribute("y"));
			upZ = atof(element->Atribute("z"));
		}

		setting = setting->nextSibling();
	}

	XMLNode* group = root->nextSibling();

	if (strcmp("group", group->Value()) != 0) {
		printf("Esperado valor 'group', obtido %s\n", group->Value());
		return 1;
	}

	if (group->NoChildren()) {
		printf("Não há dados de models\n");
		return 1;
	}

	XMLNode* models = group.FirstChild();

	if (strcmp("models", models->Value()) != 0) {
		printf("Esperado valor 'models', obtido %s\n", models->Value());
		return 1;
	}

	if (model->NoChildren()) {
		printf("Não há dados de models\n");
		return 1;
	}

	XMLNode* model = models->FirstChild();

	while (model != nullptr) {
		XMLElement* element = model->ToElement();

		if (strcmp("model", model->Value()) != 0) {
			cerr << "Esperado valor 'model', obtido '" << model->Value() << "'" << endl;
			return 1;
		}

		string ficheiro = element->Attribute("file");
		readFile(ficheiro);

		model = model->NextSibling();
	}

	return 0;

}

int main(int argc, char** argv) {

	// init GLUT and the window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(80, 80);
	glutInitWindowSize(1500, 1000);
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

	//  OpenGL settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// enter GLUT's main cycle
	glutMainLoop();

	return 1;
}
