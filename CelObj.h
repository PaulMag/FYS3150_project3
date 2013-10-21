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
    double mass;
    vec position;
    vec velocity;
    vec force;

    CelObj();
    ~CelObj();

    double getMass();
    vec getForce(CelObj);
};

#endif
