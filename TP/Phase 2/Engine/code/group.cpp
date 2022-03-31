#include "group.h"

using namespace std;
using std::vector;

Vertice::Vertice() {
    x = 0;
    y = 0;
    z = 0;
};

Vertice::Vertice(float xx, float yy, float zz) {
    x = xx;
    y = yy;
    z = zz;
};

float Vertice::getX() {
    return x;
};

float Vertice::getY() {
    return y;      
};

float Vertice::getZ() {
    return z;
};

//////////////////////////////// Model ////////////////////////////////////////

Model::Model() {
};

Model::Model(vector<Vertice*> coords) {
    vertices = coords;
};

vector<Vertice*> Model::getVertices() {
    return vertices;
};

//////////////////////////////////// Operações ////////////////////////////////////

Operacoes::Operacoes() {
    name = "";
    x = 0;
    y = 0;
    z = 0;
    angle = 0;
};

Operacoes::Operacoes(char* nameO, float xx, float yy, float zz, float angleO) {
    name = nameO;
    x = xx;
    y = yy;
    z = zz;
    angle = angleO;
};

char* Operacoes::getName() {
    return name;
};

float Operacoes::getX() {
    return x;
};

float Operacoes::getY() {
    return y;
};

float Operacoes::getZ() {
    return z;
};

float Operacoes::getAngle() {
    return angle;
};

////////////////////////////////////// Group //////////////////////////////////

Group::Group() {

};

Group::Group(vector<Operacoes*> op, vector<Model*> m, vector<Group*> c) {
    ops = op;
    models = m;
    filhos = c;
};

vector<Operacoes*> Group::getOps() {
    return ops;
};

vector<Model*> Group::getModel() {
    return models;
};

vector<Group*> Group::getFilhos() {
    return filhos;
};

void Group::pushOps(Operacoes* o) {
    ops.push_back(o);
};

void Group::pushModel(Model* m) {
    models.push_back(m);
};

void Group::pushFilho(Group* c) {
    filhos.push_back(c);
};