#include <iostream>
#include <cmath>
#include <fstream>
#include <armadillo>

using namespace std;
using namespace arma;

#include "SolSys.h"
#include "CelObj.h"

SolSys:: SolSys() {
    N = 0; // number of celestial bodies
    //bodies[N];
    //bodies = new CelObj[N];
}

SolSys:: ~SolSys() {
    vector<CelObj>().swap(bodies);
}

void SolSys:: addCelObj(CelObj body) {
    /* Add a new CelObj to the SolSys. This is done by copying the old CelObj
     * list "bodies" into a new list that is 1 longer, and deleting the old
     * one. This isn't very effective, but it only needs to be done a few times
     * at the beginning of the program, before the simulations starts.
     */
    bodies.push_back(body);
    N++;              // now there is 1 more planet in the system
}

void SolSys:: addCelObj(string n, double m, vec x, vec v) {
    CelObj newBody = CelObj(n, m, x, v);
    addCelObj(newBody);
}

void SolSys:: addCelObj(string n, double m, double x0, double x1, double x2,
                                            double v0, double v1, double v2) {
    vec x;
    x << x0 << x1 << x2;
    vec v;
    v << v0 << v1 << v2;
    CelObj newBody(n, m, x, v);
    addCelObj(newBody);
}

void SolSys:: addCelObj(string n, double m, double x0, double x1,
                                            double v0, double v1) {
    addCelObj(n, m, x0, x1, 0, v0, v1, 0);
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
    double G = 1.0; // TODO: Implement gravity constant G properly!!!
    F *= G; // multiply with gravity constant
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

void SolSys:: rungeKutta4(double h) {
    /* Forwards every Celestial Object in the Solar System one timestep with
     * the RungeKutta4 method.
     */
    double h2 = h * 0.5; // too save a few calculations
    double h6 = h / 6.0;

    mat v0 = getVelocities();
    mat x0 = getPositions();
    mat a0 = findAccels();

    mat v1 = v0 + a0 * h2;
    mat x1 = x0 + v0 * h2;
    setPositions(x1);
    mat a1 = findAccels();

    mat v2 = v0 + a1 * h2;
    mat x2 = v0 + v1 * h2;
    setPositions(x2);
    mat a2 = findAccels();

    mat v3 = v0 + a2 * h;
    mat x3 = x0 + v2 * h;
    setPositions(x3);
    mat a3 = findAccels();

    setVelocities(v0 + (a0 + 2*a1 + 2*a2 + a3) * h6);
    setPositions (x0 + (v0 + 2*v1 + 2*v2 + v3) * h6);
}

void SolSys:: makeDataFiles(string location) {
    for (int i=0; i<N; i++) {
        bodies[i].makeOutfile(location);
    }
}

void SolSys:: moveSystem(double time, int stepN, bool output) {
    /* Solve system for a given no of steps and a timestep.
     */
    double h = time / stepN;
    if (output) {
        for (int j=0; j<stepN; j++) {
            rungeKutta4(h);
            for (int i=0; i<N; i++) {
                bodies[i].writeData();
            }
        }
    }
    else {
        for (int j=0; j<stepN; j++) {
            rungeKutta4(h);
        }
    }
}

void SolSys:: moveSystem(double time, double h, bool output) {
    int stepN = (int)(time / h + 0.5); // round to int
    moveSystem(time, stepN, output);
}

void SolSys:: moveSystem(double time, int stepN) {
    moveSystem(time, stepN, true); // default value is true
}

void SolSys:: moveSystem(double time, double h) {
    moveSystem(time, h, true); // default value is true
}
