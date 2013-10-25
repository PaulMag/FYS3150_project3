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
    /* Destructor is called all the time for some reason.
    if (outfileOpen) { // avoid closing if it was never opened
        outfile->close();
        cout << name << ".dat was closed." << endl;
    }
    */
}

vec CelObj:: getForce(CelObj other) {
    /* Find the force that act upon this body from another CelObj, not included
     * the gravity constant G.
     */
    vec r = other.position - position;
    return mass * other.mass * r / pow(norm(r,2), 3) * 0.00011854924136738324;
   // vec r = position;
   // return - 0.00011854924136738324 * 333054.253182 * r * pow(norm(r,2), -3);
}

void CelObj:: makeOutfile(string location) {
    outfile  = new ofstream(location.c_str());
}

void CelObj:: closeOutfile() {
    outfile->close();
}

void CelObj:: writeData() {
    *outfile << position(0) << "," << position(1) << "," << position(2) << endl;
}
