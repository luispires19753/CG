#ifndef _GROUP_H_
#define _GROUP_H_

#include <vector>
#include <string>
#include <GL/glut.h>

using namespace std;
using std::vector;

class Vertice{
    private:
        float x;
        float y;
        float z;

    public:
        Vertice();
        Vertice(float xx, float yy, float zz);

        float getX();
        float getY();
        float getZ();
};



class Model {
    vector<Vertice*> vertices;

public:
    Model();
    Model(vector<Vertice*> coords);

    vector<Vertice*> getVertices();
};



class Operacoes{
    char* name;
    float x,y,z,angle;

public:
    Operacoes();
    Operacoes(char* nameO, float xx, float yy, float zz, float angleO);

    char* getName();
    float getX();
    float getY();
    float getZ();
    float getAngle();
};



class Group {
    vector<Operacoes*> ops;
    vector<Model*> models;
    vector<Group*> filhos;

public:
    Group();
    Group(vector<Operacoes*> op, vector<Model*> m, vector<Group*> c);

    vector<Operacoes*> getOps();
    vector<Model*> getModel();
    vector<Group*> getFilhos();

    void pushOps(Operacoes* o);
    void pushModel(Model* m);
    void pushFilho(Group* c);
};

#endif