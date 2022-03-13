#include <stdio.h>      //printf
#include <stdlib.h>     //atof
#include <string.h>     //strcmp
#include "primitivas.h" //plano, caixa

int main(int argc, char** argv) {

    char* shape = argv[1];
    char* file = argv[argc - 1];

    if (argc < 4) {
        printf("Not enough parameters!");
        return 0;
    }

    if (argc > 7) {
        printf("Too many arguments!");
        return 0;
    }

    if (strcmp(shape, "plane") == 0) { //generator plane 1 3 plane.3d
        if (argc != 5) {
            printf("Number of parameters wrong!");
            return 0;
        }
        printf("Plane file created!");
        //float unit = 10.0f * atof(argv[2]);
        plano(atof(argv[2]), atof(argv[3]), file);
    }

    if (strcmp(shape, "box") == 0) { //generator box 2 3 box.3d
        if (argc != 5) {
            printf("Number of parameters wrong!");
            return 0;
        }
        printf("Box file created!");
        caixa(atof(argv[2]), atof(argv[3]), file);
    }

    if (strcmp(shape, "sphere") == 0) { //generator sphere 1 10 10 sphere.3d
        if (argc != 6) {
            printf("Number of parameters wrong!");
            return 0;
        }
        printf("Sphere file created!");
        esfera(atof(argv[2]), atoi(argv[3]), atoi(argv[4]), file);
    }

    if (strcmp(shape, "cone") == 0) { //generator cone 1 2 4 3 cone.3d
        if (argc != 7) {
            printf("Number of parameters wrong!");
                return 0;
        }
        printf("Cone file created!");
        cone(atof(argv[2]), atof(argv[3]), atoi(argv[4]), atoi(argv[5]), file);
    }

    return 0;
}