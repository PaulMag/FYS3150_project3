#include <iostream>
#include <cmath>
#include <fstream>
#include <armadillo>

using namespace std;
using namespace arma;

#ifndef SOLSYS_H
#define SOLSYS_H

#include "CelObj.h"

class SolSys {

public:
    int N; // no of bodies
    CelObj* bodies;

    SolSys();
    ~SolSys();

    void findForces();
};

#endif
