class CelObj {

public:
    double mass;
    double position;
    double velocity;
    double force;

    CelObj(double m) {
        mass = m;
    }

    ~CelObj()
    {
        //delete[] ;
    }

    double getMass() {
        return mass;
    }


};
