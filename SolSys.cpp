#include <iostream>
#include <cmath>
#include <fstream>
#include <armadillo>

using namespace std;
using namespace arma;

#include "SolSys.h"
#include "CelObj.h"

SolSys:: SolSys() {
    bodies[N];
    //CelObj a = bodies[2];
    //a.mass = 17;
    //cout << a.mass << endl;
    //CelObj b = bodies[2];
    //cout << b.mass << endl;
}

SolSys:: ~SolSys() {
    delete [] bodies;
}

void SolSys:: setPositions(mat x) {
    for (int i=0; i<N; i++) {
        bodies[i].position = x.row(i);
    }
}

void SolSys:: setVelocities(mat v) {
    for (int i=0; i<N; i++) {
        bodies[i].position = v.row(i);
    }
}

mat SolSys:: getPositions() {
    mat x(N,3);
    for (int i=0; i<N; i++) {
        x.row(i) = bodies[i].position;
    }
    return x;
}

mat SolSys:: getVelocities() {
    mat v(N,3);
    for (int i=0; i<N; i++) {
        v.row(i) = bodies[i].velocity;
    }
    return v;
}

cube SolSys:: findForces() {
    /* Fills a matrix with the forces between every CelObj.
     */
    cube F = zeros<cube>(N,N,3);
    for (int i=0; i<N; i++) {
        for (int j=0; j<i; j++) {
            F(span(i), span(j), span()) = bodies[i].getForce(bodies[j]);
        }
    }
    F.slice(0) -= F.slice(0).t(); // mirror forces in the matrix
    F.slice(1) -= F.slice(1).t();
    F.slice(2) -= F.slice(2).t();
    F *= G; // multiply with gravity constant. TODO: Add gravity constant G
    return F;
}

mat SolSys:: findAccels() {
    cube F = findForces();
    mat a = zeros<mat>(N,3);

    for (int i=0; i<N; i++) {
        for (int j=0; j<N; j++) {
            for (int k=0; k<N; k++) {
                a(i,k) += F(i,j,k); // manually loop through cube
            }
        }
    }
    return a;
}

void SolSys:: rungeKutta4() {
    /* Forwards every Celestial Object in the Solar System one timestep with
     * the RungeKutta4 method.
     * TODO: Implement timestep h somehow.
     */

    mat v0 = getVelocities();
    mat x0 = getPositions();
    mat a0 = findAccels();

    mat v1 = v0 + a0 * h2;
    mat x1 = x0 + v0 * h2;
    setPositions(x1);
    mat a1 = findAccels();

    mat v2 = v1 + a1 * h2;
    mat x2 = v1 + v1 * h2;
    setPositions(x2);
    mat a2 = findAccels();

    mat v3 = v2 + a2 * h;
    mat x3 = x2 + x2 * h;
    setPositions(x3);
    mat a3 = findAccels();

    setVelocities(v0 + (1./6) * (a0 + 2*a1 + 2*a2 + a3) * h);
    setPositions (x0 + (1./6) * (v0 + 2*v1 + 2*v2 + v3) * h);
}
