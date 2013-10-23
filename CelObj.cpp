#include <iostream>
#include <cmath>
#include <fstream>
#include <armadillo>

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
    //delete[];
}

vec CelObj:: getForce(CelObj other) {
    /* Find the force that act upon this body from another CelObj, not included
     * the gravity constant G.
     */
    vec r = other.position - position;
    return mass * other.mass * r / pow(norm(r,2), 3);
}
