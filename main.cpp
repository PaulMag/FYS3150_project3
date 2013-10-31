#include <iostream>
#include <cmath>
#include <fstream>
#include <armadillo>

#include <ctime>

#include "CelObj.h"
#include "SolSys.h"

using namespace std;
using namespace arma;

/* Stuff to remember:
 *
 * mat A = zeros<mat>(n, n);
 * vec a(n);
 */

int main(int argc, char* argv[]) {

    clock_t start, finish;
    start = clock();

    SolSys mysun = SolSys(2);

    /* Data collected from:
     * http://nssdc.gsfc.nasa.gov/planetary/factsheet/planet_table_ratio.html
     * Planets initial positions relative to eachother are arbitrary,
     * but position and velocity relative to Sun should be correct.
     */

    mysun.addCelObj("Mercury", 0.0553  ,   0.313, 0, 0,  58.98);
    mysun.addCelObj("Venus"  , 0.815   , - 0.731, 0, 0, -35.26);
    mysun.addCelObj("Earth"  , 1       ,   1    , 0, 0,  30.29);
    mysun.addCelObj("Mars"   , 0.107   , - 1.41 , 0, 0, -26.50);
    mysun.addCelObj("Jupiter", 317.8   ,   5.03 , 0, 0,  13.72);
    mysun.addCelObj("Saturn" ,  95.2   , - 9.20 , 0, 0, -10.18);
    mysun.addCelObj("Uranus" ,  15.5   ,  18.64 , 0, 0,   7.11);
    mysun.addCelObj("Neptune",  17.1   , -30.22 , 0, 0, - 5.50);
    mysun.addCelObj("Pluto"  ,   0.0022,  30.15 , 0, 0,   6.10);

    for (int i=0; i<mysun.N; i++) {
        /* Set correct scaling. */
        mysun.bodies[i].position *= 0.98323592;  // [earth perihelions] to [au]
        mysun.bodies[i].velocity *= 0.210945021; // [km/s] to [au/year]
    }
    mysun.addCelObj("Sun", 333054.253182, 0, 0, 0, 0);

    /*
    cout << "CM_before: " << mysun.getCenterOfMass();
    mysun.setCenterOfMass();
    cout << "CM_after:  " << mysun.getCenterOfMass();
    cout << "momentum_before: " << mysun.getTotalMomentum();
    mysun.setTotalMomentum();
    cout << "momentum_after : " << mysun.getTotalMomentum();
    */

    istringstream timeS(argv[1]);
    istringstream hS   (argv[2]);
    double time, h;
    timeS >> time;
    hS    >> h;
    string location = argv[3];

    mysun.moveSystem(time, h, location);
    finish = clock();

    double compTime = double(finish - start) / CLOCKS_PER_SEC;
    cout << "Total computation time:  " << compTime  << " seconds." << endl;
}
