#include <iostream>
#include <cmath>
#include <fstream>
#include <armadillo>

#include <sstream>

using namespace std;
using namespace arma;

#include "CelObj.h"

CelObj:: CelObj() {
    position = vec(3);
    velocity = vec(3);
}

CelObj:: CelObj(string n, double m, vec x, vec v) {
    name = n;
    mass = m;
    position = x;
    velocity = v;
}

CelObj:: ~CelObj() {
    outfile->close();
}

vec CelObj:: getForce(CelObj other) {
    /* Find the force that act upon this body from another CelObj, not included
     * the gravity constant G.
     */
    vec r = other.position - position;
    return mass * other.mass * r / pow(norm(r,2), 3);
}

void CelObj:: makeOutfile(string location) {
    ostringstream oss;
    oss << location << name << ".dat";
    outfile->open(oss.str().c_str()); // convert string to array of chars
}

void CelObj:: writeData() {
    *outfile << position(0) << "," << position(1) << "," << position(2) << endl;
}
