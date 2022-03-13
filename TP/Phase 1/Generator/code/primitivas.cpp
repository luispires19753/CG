#include <stdio.h>        //FILE*, fopen, fprintf
#define _USE_MATH_DEFINES
#include <cmath>          //M_PI
#include "primitivas.h"   //plano, caixa

void plano(float length, float divisions, char* fname) {
    FILE* f;
    fopen_s(&f, fname, "w");

    //ponto de partida dos eixos x e z
    float x = -length / 2;
    float z = length / 2;

    //comprimento do lado de cada quadrado pequeno
    float lq = length / divisions;

    while (z != -length / 2) {
        while (x != length / 2) {

            //guardar tri�ngulos
            fprintf(f, "%f %f %f\n", x, 0.0, z);
            fprintf(f, "%f %f %f\n", x + lq, 0.0, z);
            fprintf(f, "%f %f %f\n", x, 0.0, z - lq);

            fprintf(f, "%f %f %f\n", x + lq, 0.0, z);
            fprintf(f, "%f %f %f\n", x + lq, 0.0, z - lq);
            fprintf(f, "%f %f %f\n", x , 0.0, z - lq);

            //tri�ngulos inversos
            fprintf(f, "%f %f %f\n", x, 0.0, z);
            fprintf(f, "%f %f %f\n", x, 0.0, z - lq);
            fprintf(f, "%f %f %f\n", x + lq, 0.0, z);

            fprintf(f, "%f %f %f\n", x + lq, 0.0, z);
            fprintf(f, "%f %f %f\n", x, 0.0, z - lq);
            fprintf(f, "%f %f %f\n", x + lq, 0.0, z-lq);



            x += lq; //incrementar x para o pr�ximo ponto
        }
        x = -length / 2; //reinicializar eixo dos x
        z -= lq; //decrementar z para o pr�ximo ponto
    }

    fclose(f);
}


void caixa(float length, float divisions, char* fname) {
    FILE* f;
    fopen_s(&f, fname, "w");

    //diferen�a entre cada ponto
    float lq = length / divisions;
    
    //ponto de partida nos extremos de y
    float x = -length / 2.0f;
    float z = length / 2.0f;
    float y = length / 2.0f;
    
    //c�lculo dos tri�ngulos nas duas faces nos extremos de y
    while (z > -length / 2) {
        while (x < length / 2) {
        //guardar tri�ngulos
            fprintf(f, "%f %f %f\n", x, y, z);
            fprintf(f, "%f %f %f\n", x + lq, y, z);
            fprintf(f, "%f %f %f\n", x, y, z - lq);

            fprintf(f, "%f %f %f\n", x + lq, y, z);
            fprintf(f, "%f %f %f\n", x + lq, y, z - lq);
            fprintf(f, "%f %f %f\n", x, y, z - lq);

                //tri�ngulos da face sim�trica
            fprintf(f, "%f %f %f\n", x, -y, z);
            fprintf(f, "%f %f %f\n", x, -y, z - lq);
            fprintf(f, "%f %f %f\n", x + lq, -y, z);

            fprintf(f, "%f %f %f\n", x + lq, -y, z);
            fprintf(f, "%f %f %f\n", x, -y, z - lq);
            fprintf(f, "%f %f %f\n", x + lq, -y, z - lq);

            x += lq; //incrementar x para o pr�ximo ponto
        }
        x = -length / 2; //reinicializar eixo dos x
        z -= lq; //decrementar z para o pr�ximo ponto
    }

    
    //ponto de partida nos extremos de z
    x = -length / 2;
    z = length / 2;
    y = length / 2;

    //c�lculo dos tri�ngulos nas duas faces nos extremos de z
    while (y > -length / 2) {
        while (x < length / 2) {
            //guardar tri�ngulos
            fprintf(f, "%f %f %f\n", x, y, z);
            fprintf(f, "%f %f %f\n", x, y - lq, z);
            fprintf(f, "%f %f %f\n", x + lq, y - lq, z);

            fprintf(f, "%f %f %f\n", x, y, z);
            fprintf(f, "%f %f %f\n", x + lq, y - lq, z);
            fprintf(f, "%f %f %f\n", x + lq, y, z);

            //tri�ngulos da face oposta
            fprintf(f, "%f %f %f\n", x, y, -z);
            fprintf(f, "%f %f %f\n", x + lq, y - lq, -z);
            fprintf(f, "%f %f %f\n", x, y - lq, -z);

            fprintf(f, "%f %f %f\n", x, y, -z);
            fprintf(f, "%f %f %f\n", x + lq, y, -z);
            fprintf(f, "%f %f %f\n", x + lq, y - lq, -z);

            x += lq; //incrementar x para o pr�ximo ponto
        }
        x = -length / 2; //reinicializar eixo dos x
        y -= lq; //incrementar y para o pr�ximo ponto
    }
   
    //ponto de partida nos extremos de x
    x = length / 2;
    z = length / 2;
    y = length / 2;

    //c�lculo dos tri�ngulos nas duas faces nos extremos de x

    while (y > -length / 2.0f) {
        while (z > -length / 2.0f) {
            //guardar tri�ngulos
            fprintf(f, "%f %f %f\n", x, y, z);
            fprintf(f, "%f %f %f\n", x, y-lq, z);
            fprintf(f, "%f %f %f\n", x, y - lq, z - lq);

            fprintf(f, "%f %f %f\n", x, y, z);
            fprintf(f, "%f %f %f\n", x, y - lq, z - lq);
            fprintf(f, "%f %f %f\n", x, y, z-lq);

            //tri�ngulos da face sim�trica
            fprintf(f, "%f %f %f\n", -x, y, z);
            fprintf(f, "%f %f %f\n", -x, y - lq, z - lq);
            fprintf(f, "%f %f %f\n", -x, y-lq, z);

            fprintf(f, "%f %f %f\n", -x, y, z);
            fprintf(f, "%f %f %f\n", -x, y, z-lq);
            fprintf(f, "%f %f %f\n", -x, y - lq, z - lq);

            z -= lq; //decrementar z para o pr�ximo ponto
        }
        z = length / 2; //reinicializar eixo do z
        y -= lq; //decrementar y para o pr�ximo ponto
    }
    
    fclose(f);
}

void esfera(float r, int slices, int stacks, char* fname) {
    FILE* f;
    fopen_s(&f, fname, "w");

    float a, b;
    float sl = 2.0f * M_PI / float(slices), st = M_PI / float(stacks);
    float px1, px2, px3, px4, py1, py2, pz1, pz2, pz3, pz4;

    //alfa (angulo formado por xx e zz) vai de 0 a 2*pi
    for (a = 0.0f; a <= 2 * M_PI; a += sl) {

        //beta (angulo formado por plano xz e yy) vai de -pi/2 a pi/2
        for (b = -M_PI / 2.0f; b < M_PI / 2.0f; b += st) {

            //triangulos que nao se veem
            px1 = r * cos(b) * sin(a);
            px2 = r * cos(b) * sin(a + sl);
            px3 = r * cos(b + st) * sin(a);
            px4 = r * cos(b + st) * sin(a + sl);

            py1 = r * sin(b);
            py2 = r * sin(b + st);

            pz1 = r * cos(b) * cos(a);
            pz2 = r * cos(b) * cos(a + sl);
            pz3 = r * cos(b + st) * cos(a);
            pz4 = r * cos(b + st) * cos(a + sl);

            fprintf(f, "%f %f %f\n", px1, py1, pz1);
            fprintf(f, "%f %f %f\n", px2, py1, pz2);
            fprintf(f, "%f %f %f\n", px3, py2, pz3);

            fprintf(f, "%f %f %f\n", px2, py1, pz2);
            fprintf(f, "%f %f %f\n", px4, py2, pz4);
            fprintf(f, "%f %f %f\n", px3, py2, pz3);

        }
    }

    fclose(f);
}


void cone(float radius, float height, int slices, int stacks, char* fname) {
    FILE* f;
    fopen_s(&f, fname, "w");

    float sl = 2 * M_PI / float(slices);
    float st = radius / float(stacks);
    float alt = height / float(stacks);

	float a, b;

	for (a = 0; a <= 2 * M_PI; a += sl) {
		//face de baixo
        float xb1 = radius * sin(a + sl);
        float zb1 = radius * cos(a + sl);

        float xb2 = radius * sin(a);
        float zb2 = radius * cos(a);

        fprintf(f,"%f %f %f\n",0.0,0.0,0.0);
        fprintf(f,"%f %f %f\n",xb1,0.0,zb1);
        fprintf(f,"%f %f %f\n",xb2,0.0,zb2);


		float aux = 0.0f;
		for (b = float(radius); b > st; b -= st) {

            float x1 = b * sin(a);
            float x2 = b * sin(a + sl);
            float x3 = (b - st) * sin(a);
            float x4 = (b - st) * sin(a + sl);

            float z1 = b * cos(a);
            float z2 = b * cos(a + sl);
            float z3 = (b - st) * cos(a);
            float z4 = (b - st) * cos(a + sl);

            fprintf(f,"%f %f %f\n",x1,aux,z1);
            fprintf(f,"%f %f %f\n",x2,aux,z2);
            fprintf(f,"%f %f %f\n",x3,aux+alt,z3);

            fprintf(f,"%f %f %f\n",x2,aux,z2);
            fprintf(f,"%f %f %f\n",x4,aux+alt,z4);
            fprintf(f,"%f %f %f\n",x3,aux+alt,z3);

			aux += alt;
		}

        fprintf(f,"%f %f %f\n",0.0,height,0.0);
        fprintf(f,"%f %f %f\n",xb2,0.0,zb2);
        fprintf(f,"%f %f %f\n",xb1,0.0,zb1);

	}
    fclose(f);

}