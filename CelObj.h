#include <iostream>
#include <cmath>
#include <fstream>
#include <armadillo>

using namespace std;
using namespace arma;

#ifndef CELOBJ_H
#define CELOBJ_H

class CelObj {

public:
    string name;
    double mass;

    vec position;
    vec velocity;

    ofstream* outfile;

    CelObj();
    CelObj(string, double, vec, vec);
    ~CelObj();

    vec getForce(CelObj);

    void makeOutfile(string);
    void closeOutfile();
    void writeData();
};

#endif
