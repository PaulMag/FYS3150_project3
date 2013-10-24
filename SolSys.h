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

    void addCelObj(CelObj);
    void addCelObj(string, double, vec, vec);

    void setPositions (mat);
    void setVelocities(mat);

    mat getPositions ();
    mat getVelocities();

    cube findForces();
    mat  findAccels();

    void rungeKutta4();
    void moveSystem(int stepN, double h);
    void moveSystem(double time, int stepN);
    void moveSystem(double time, double h);
};

#endif
