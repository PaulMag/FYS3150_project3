#include <iostream>
#include <cmath>
#include <fstream>
#include <armadillo>

using namespace std;
using namespace arma;

#include "SolSys.h"
#include "CelObj.h"

SolSys:: SolSys() {
    bodies [N];
    CelObj a = bodies[2];
    a.mass = 17;
    cout << a.mass << endl;
    //CelObj b = bodies[2];
    //cout << b.mass << endl;
}

SolSys:: ~SolSys() {
    //delete[];
}

void SolSys:: findForces() {
    /* Fills a matrix with the forces between every CelObj.
     */
    cube forces = zeros<cube>(N,N,3);
    vec a = randu<vec>(3);
    for (int i=0; i<N; i++) {
        for (int j=0; j<i; j++) {
            forces(span(i), span(j), span()) = a;
        }
    }
}
