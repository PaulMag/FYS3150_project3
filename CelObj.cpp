#include <iostream>
#include <cmath>
#include <fstream>
#include <armadillo>

using namespace std;
using namespace arma;

#include "CelObj.h"

CelObj:: CelObj() {
    position (3);
    velocity (3);
}

CelObj:: ~CelObj() {
    //delete[];
}

double CelObj:: getMass() {
    return mass;
}

vec CelObj:: getForce(CelObj other) {
    /* Find the force that act upon this body from another CelObj, not included
     * the gravity constant G.
     */
    vec r = other.position - position;
    return mass * other.mass * r / pow(norm(r,2), 3);
}
