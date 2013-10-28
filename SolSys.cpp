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
    N++; // now there is 1 more planet in the system
}

void SolSys:: addCelObj(string n, double m, rowvec x, rowvec v) {
    CelObj newBody = CelObj(n, m, x, v);
    addCelObj(newBody);
}

void SolSys:: addCelObj(string n, double m, double x0, double x1, double x2,
                                            double v0, double v1, double v2) {
    rowvec x;
    x << x0 << x1 << x2;
    rowvec v;
    v << v0 << v1 << v2;
    CelObj newBody(n, m, x, v);
    addCelObj(newBody);
}

void SolSys:: addCelObj(string n, double m, double x0, double x1,
                                            double v0, double v1) {
    addCelObj(n, m, x0, x1, 0.0, v0, v1, 0.0);
}

void SolSys:: setPositions(mat x) {
    for (int i=0; i<N; i++) {
        bodies[i].position = x.row(i);
    }
}

void SolSys:: setVelocities(mat v) {
    for (int i=0; i<N; i++) {
        bodies[i].velocity = v.row(i);
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
    cube F = zeros<cube>(N,3,N);

    for (int i=0; i<N; i++) {
        for (int j=0; j<i; j++) {

            rowvec f = bodies[i].getForce(bodies[j]);
            for (int k=0; k<3; k++) {
                F(i,k,j) =   f(k);
                F(j,k,i) = - f(k); // same force on both in CelObj pair
            }
        }
    }
    F *= 0.00011854924136738324; // multiply with earthM scaled gravity constant
    return F;
}

mat SolSys:: findAccels() {
    cube F = findForces();
    mat a = zeros<mat>(N,3);

    for (int j=0; j<N; j++) {
        a += F.slice(j); // sum all forces on each CelObj
    }
    for (int i=0; i<N; i++) {
        a.row(i) /= bodies[i].mass;
    }
    return a;
}

void SolSys:: rungeKutta4(double h) {
    /* Forwards every Celestial Object in the Solar System one timestep with
     * the RungeKutta4 method.
     */
    double h2 = h * 0.5; // to save a few calculations
    double h6 = h / 6.0;

    /*
    // Euler Chromer:
    mat v0 = getVelocities();
    mat x0 = getPositions();
    mat a0 = findAccels();

    mat v1 = v0 + a0 * h;
    mat x1 = x0 + v1 * h;
    setVelocities(v1);
    setPositions (x1);
    // end Euler Chromer
    */

    mat v0 = getVelocities();
    mat x0 = getPositions();
    mat a0 = findAccels();

    mat v1 = v0 + a0 * h2;
    mat x1 = x0 + v0 * h2;
    setPositions(x1);
    mat a1 = findAccels();

    mat v2 = v0 + a1 * h2;
    mat x2 = x0 + v1 * h2;
    setPositions(x2);
    mat a2 = findAccels();

    mat v3 = v0 + a2 * h;
    mat x3 = x0 + v2 * h;
    setPositions(x3);
    mat a3 = findAccels();

    setVelocities(v0 + (a0 + 2*a1 + 2*a2 + a3) * h6);
    setPositions (x0 + (v0 + 2*v1 + 2*v2 + v3) * h6);
}

void SolSys:: moveSystem(double time, int stepN, string location) {
    /* Solve system for a given period of time and a number of steps.
     * Write results to files in folder location unless location = "0".
     */

    double h = time / stepN;

    if (location.compare("0") != 0) {
        cout << "Solving and writing data..." << endl;

        outfile  = new ofstream(("data/" + location + "/info.dat").c_str());
        *outfile << time << "," << stepN << "," << 3 << endl;
        for (int i=0; i<N; i++) {
            *outfile << bodies[i].name << ",";
        }
        *outfile << endl;
        outfile->close();

        for (int i=0; i<N; i++) {
            bodies[i].makeOutfile("data/" + location + "/obj" + SSTR(i) + ".dat");
        }

        for (int j=0; j<stepN; j++) {
            rungeKutta4(h);
            for (int i=0; i<N; i++) {
                bodies[i].writeData();
            }
        }
        for (int i=0; i<N; i++) {
            bodies[i].closeOutfile();
        }
    }
    else {
        cout << "Solving WITHOUT writing data..." << endl;

        for (int j=0; j<stepN; j++) {
            rungeKutta4(h);
        }
    }
}

void SolSys:: moveSystem(double time, double h, string location) {
    /* Give timestep as argument instead of number of steps. */
    int stepN = (int)(time / h + 0.5); // round to int
    moveSystem(time, stepN, location);
}

void SolSys:: moveSystem(double time, int stepN) {
    /* Solve without saving the data. */
    moveSystem(time, stepN, "0");
}

void SolSys:: moveSystem(double time, double h) {
    /* Solve without saving the data. */
    moveSystem(time, h, "0");
}
