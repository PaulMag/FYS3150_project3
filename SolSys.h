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

    void makeDataFiles(string);

    void rungeKutta4();
    void rungeKutta4(double h);

    void moveSystem(double, int,    bool);
    void moveSystem(double, double, bool);
    void moveSystem(double, int);
    void moveSystem(double, double);
};

#endif
