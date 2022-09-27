#include "robot.h"

inertial Robot::inertialSensor = Inertial10;

std::vector<encoder> Robot::Encoders::encoders = {
    VertEncoder,  // left
    // rightEncoder, // right
    HoriEncoder,  // back
};
const double Robot::Dimensions::wheelRadius = 2.75;

constexpr double Ss = 4.93; // left
// constexpr double Sl = 3.59; // right
constexpr double Sr = 1.655; // back

const std::vector<double> Robot::Encoders::distanceToTrackingCenter = {Ss,/*  Sl, */ Sr};

void Robot::print(){};


// 4.93 vert
// 1.655