#include "odom/tracking.h"
#include "conversions.h"
#include "robot.h"
#include "vex.h"
#include "vex_units.h"
#include <cmath>
#include <functional>
#include <string>
#include <thread>

using namespace std;

void OdomTracking::updateSnapshot(const Position robotPos) {
  data.update(Position::sum(robotPos, data.getPosition()));
  this->deltaPos1 = data.getPosition();
  // printf("(%f, %f, %f)\n", deltaPos1.x, deltaPos1.y, deltaPos1.heading);
  //  Brain.Screen.setCursor(1, 1);
  //   Brain.Screen.clearLine();
  //   Brain.Screen.print("(");
  //   Brain.Screen.print(robotPos.x);
  //   Brain.Screen.print(",");
  //   Brain.Screen.print(robotPos.y);
  //   Brain.Screen.print(",");
  //   Brain.Screen.print(robotPos.heading);
  //   Brain.Screen.print(")");
  //   Brain.Screen.newLine();
};
void OdomTracking::updateEncoderInches() {
  // printf("y-enc:%f\n", data.getEncoder(Robot::Encoders::LEFT));
  encoderInches = {data.delta().getEncoderInches(Robot::Encoders::LEFT),
                   data.delta().getEncoderInches(Robot::Encoders::RIGHT),
                   data.delta().getEncoderInches(Robot::Encoders::BACK)};
  // encoderInches[Robot::Encoders::BACK] =
  // encoderInches.push_back(
  // data.delta().getEncoderInches(Robot::Encoders::BACK));
};

double OdomTracking::findDeltaX(const double deltaTheta) {
  // vector
  // printf(
  //     "<x>, enc1:%f, enc2:%f theta:%f, sin:%f, quo:%f, prod:%f\n",
  //     Conversions::EncoderRadians::toInches(Conversions::Degrees::toRadians(
  //         Robot::Encoders::encoders[Robot::Encoders::BACK].position(deg))),
  //     Conversions::Degrees::toRadians(
  //         Robot::Encoders::encoders[Robot::Encoders::BACK].position(deg)),
  //     deltaTheta, sin(deltaTheta / 2),
  //     (encoderInches[Robot::Encoders::BACK] / deltaTheta),
  //     ((2 * sin(deltaTheta / 2))) *
  //         ((encoderInches[Robot::Encoders::LEFT] / deltaTheta) +
  //          Robot::Encoders::distanceToTrackingCenter[Robot::Encoders::BACK]));
  // wait(3, msec);
  //   const float a =
  //     (encoderInches[Robot::Encoders::BACK] / deltaTheta) +
  //     Robot::Encoders::distanceToTrackingCenter[Robot::Encoders::BACK];
  // printf("radius:%f\n", a);
  return deltaTheta == 0
             ? encoderInches[Robot::Encoders::BACK]
             : ((2 * sin(deltaTheta / 2))) * /*a*/
                   ((encoderInches[Robot::Encoders::BACK] / deltaTheta) +
                    Robot::Encoders::distanceToTrackingCenter
                        [Robot::Encoders::BACK]);

  // // arc
  // return 2 * sin(data.getInertial() / 2) *
  //        ((encoderInches[Robot::Encoders::LEFT] / deltaTheta) +
  //         Robot::Encoders::distanceToTrackingCenter[Robot::Encoders::LEFT]);
};
double OdomTracking::findDeltaY(const double deltaTheta) {
  // vector
  // printf(
  //     "<y>, enc1:%f, enc2:%f theta:%f, sin:%f, quo:%f, prod:%f\n",
  //     Conversions::EncoderRadians::toInches(Conversions::Degrees::toRadians(
  //         Robot::Encoders::encoders[Robot::Encoders::LEFT].position(deg))),
  //     Conversions::Degrees::toRadians(
  //         Robot::Encoders::encoders[Robot::Encoders::LEFT].position(deg)),
  //     deltaTheta, sin(deltaTheta / 2),
  //     (encoderInches[Robot::Encoders::LEFT] / deltaTheta),
  //     ((2 * sin(deltaTheta / 2))) *
  //         ((encoderInches[Robot::Encoders::LEFT] / deltaTheta) +
  //          Robot::Encoders::distanceToTrackingCenter[Robot::Encoders::LEFT]));
  // wait(3, msec);
  // if (encoderInches[Robot::Encoders::LEFT])
  //   printf("y-enc:%f\n", encoderInches[Robot::Encoders::LEFT]);
  return deltaTheta == 0 ? encoderInches[Robot::Encoders::LEFT]
                         : 2 * sin(deltaTheta / 2) *
                               ((encoderInches[Robot::Encoders::LEFT] /
                                 deltaTheta) /*  +
Robot::Encoders::distanceToTrackingCenter
[Robot::Encoders::LEFT] */);

  // // arc
  // return 2 * sin(data.getInertial() / 2) *
  //        ((encoderInches[Robot::Encoders::LEFT] / deltaTheta) +
  //         Robot::Encoders::distanceToTrackingCenter[Robot::Encoders::LEFT]);
};
double OdomTracking::findDeltaTheta() {
  // return (encoderInches[Robot::Encoders::LEFT] -
  //         encoderInches[Robot::Encoders::RIGHT]) /
  //        (Robot::Encoders::distanceToTrackingCenter[Robot::Encoders::LEFT] +
  //         Robot::Encoders::distanceToTrackingCenter[Robot::Encoders::RIGHT]);
  return data.delta().getInertial();
};

Position OdomTracking::findDeltaPosition() {
  const double deltaTheta = findDeltaTheta();
  return {findDeltaX(deltaTheta), findDeltaY(deltaTheta), deltaTheta};
};

// double OdomTracking::findTravelX(const Position &deltaPos) {
//   // bad vector rotation
//   double meanDTheta = data.last.yaw + (deltaPos.heading / 2);
//   return (deltaPos.x * cos(-meanDTheta)) - (deltaPos.y * sin(-meanDTheta));
// };

// https://en.wikipedia.org/wiki/Fast_inverse_square_root
float Q_rsqrt(float number) {
  long i;
  float x2, y;
  const float threehalfs = 1.5F;

  x2 = number * 0.5F;
  y = number;
  i = *(long *)&y;           // evil floating point bit level hacking
  i = 0x5f3759df - (i >> 1); // what the fuck?
  y = *(float *)&i;
  y = y * (threehalfs - (x2 * y * y)); // 1st iteration (3.4% error)
  y = y * (threehalfs -
           (x2 * y * y)); // 2nd iteration, this can be removed (0.17% error)
  y = y * (threehalfs - (x2 * y * y)); // 3rd iteration, for fun
  return y;
}
OdomTracking::coordinate
OdomTracking::findTravelCoord(const Position &deltaPos) {
  // ~~~~~~~~~~~~~~~~~~~~
  // Weird Sqrt Rotation
  // ~~~~~~~~~~~~~~~~~~~~

  // desmos vector rotation: https://www.desmos.com/calculator/txqjvlc2db
  // t = acos(a/sqrt(a^2+b^2);
  // a_I = sqrt(a^2+b^2)*cos(t + t_I)
  // b_I = sqrt(a^2+b^2)*sin(t + t_I)
  // printf("==(%f, %f, %f)", deltaPos.x, deltaPos.y, deltaPos.heading);
  // float invPyth = 1 / sqrt(pow(deltaPos.x, 2) + pow(deltaPos.y, 2));
  // float pythagorean = 1 / invPyth;
  // double t1 =
  //     ((/*sign*/ deltaPos.y / abs(deltaPos.y)) * acos(deltaPos.x * invPyth) +
  //      (data.last.yaw + (deltaPos.heading / 2)) /*avg orientation*/);
  // if (t1 != t1) // if(t1 == nan)
  //   return {pythagorean, 0};
  // return {(pythagorean * cos(t1)), (pythagorean * sin(t1))};

  // ~~~~~~~~~~~~~~~~~~~~~~~~~
  // Polar Conversion (atan2)
  // ~~~~~~~~~~~~~~~~~~~~~~~~~

  // const float newAngle =
  //     atan2(deltaPos.x, deltaPos.y) - (data.last.yaw + (deltaPos.heading /
  //     2));
  // const float magnitude = Position::distance(deltaPos);
  // return {magnitude * sin(newAngle), magnitude * cos(newAngle)};

  // stole from pilons:

  // https://github.com/nickmertin/5225A-2017-2018/blob/master/src/auto.c
  const float globalAngle =
      data.curr.yaw + (deltaPos.heading / 2); // dont get rid of that you silly
  float cosP = cos(globalAngle);
  float sinP = sin(globalAngle);
  return {(deltaPos.y * sinP) + (deltaPos.x * cosP),
          (deltaPos.y * cosP) - (deltaPos.x * sinP)};
};

double OdomTracking::findTravelY(const Position &deltaPos) {
  double meanDTheta = data.last.yaw + (deltaPos.heading / 2);
  return (deltaPos.x * sin(-meanDTheta)) - (deltaPos.y * cos(-meanDTheta));
};

Position OdomTracking::findRobotPosition() {
  auto coord = findTravelCoord(findDeltaPosition());
  // printf("y:%f\n", coord.y);
  // printf(" -> (%f, %f)\n", coord.x, coord.y);
  Position robotPos1 = {coord.x, coord.y, 0};
  // auto deltaPos = findDeltaPosition();
  // Position robotPos1 = {findTravelX(deltaPos), findTravelY(deltaPos), 0};

  // Brain.Screen.setCursor(1, 1);
  // Brain.Screen.clearLine();
  // Brain.Screen.print("(");
  // Brain.Screen.print(robotPos.x);
  // Brain.Screen.print(",");
  // Brain.Screen.print(robotPos.y);
  // Brain.Screen.print(",");
  // Brain.Screen.print(robotPos.heading);
  // Brain.Screen.print(")");
  // Brain.Screen.newLine();

  return robotPos1;
  // return {};
};

void displayPositon(Position pos) {
  Brain.Screen.setCursor(1, 1);
  Brain.Screen.clearLine();
  Brain.Screen.print("(");
  Brain.Screen.print(pos.x);
  Brain.Screen.print(",");
  Brain.Screen.print(pos.y);
  Brain.Screen.print(")");
};

// void OdomTracking::trackingLoop(OdomTracking *tracker) {
//   displayPositon(tracker->data.getPosition());
//   while (1) {
//     tracker->updateSnapshot(tracker->findRobotPosition());
//     displayPositon(tracker->data.getPosition());
//     wait(waitTime, msec);
//   }
// };

// OdomTracking *tracker = nullptr;
// int waitTime = 15;
// void trackingLoop1() {
//   // displayPositon(tracker->data.getPosition());
//   while (1) {
//   Brain.Screen.print("1");
//     // tracker->updateSnapshot(tracker->findRobotPosition());
//     // displayPositon(tracker->data.getPosition());
//     wait(15, msec);
//   }
//   // return 1;
// };

Position OdomTracking::getRobotPosition() {
  // snapshot.getLast()->getPosition();

  // displayPositon(snapshot.getPosition());
  // Brain.Screen.print(/*
  // Robot::Encoders::encoders[Robot::Encoders::LEFT].position(deg)
  //                     */
  //                    snapshot.getLast()->getEncoder(Robot::Encoders::LEFT));
  // Brain.Screen.print(", ");
  // Brain.Screen
  //     .print(/*
  //     Robot::Encoders::encoders[Robot::Encoders::BACK].position(deg)
  //             */
  //            snapshot.getEncod
  return data.getPosition();
};
//
inline constexpr bool changeCheck(const float diff, const float limit) {
  return abs(diff) > limit;
}

std::vector<OdomTracking *> trackers = {};
// Position absPos = {};
const Position Robot::getPosition() {
  return trackers[0]->getRobotPosition(); /* absPos ;*/
};

void trackerLoop() {
  OdomTracking *tracker = trackers[trackers.size() - 1];
  // printf("aaaaaaah: %d\n", trackers.size() - 1);
  // int lastMsec = 0 /* timer::system() */;
  // int time = 0;
  // int index = 0;
  // float runningAve = 0;
  // float lastVertEnc = 0, currVertEnc = 0, deltaVertEnc = 0, lastHoriEnc = 0,
  //       deltaHoriEnc = 0, currHoriEnc = 0, lastHeading = 0, currHeading = 0,
  //       deltaHeading = 0, arcRadiusVert = 0, arcRadiusHori = 0, cosP = 0,
  //       sinP = 0;
  // Position relDeltaPos = {};
  // Position absPos = {};
  while (1) {
    // currVertEnc =
    //     Conversions::EncoderRadians::toInches(Conversions::Degrees::toRadians(
    //         Robot::Encoders::encoders[Robot::Encoders::LEFT].position(deg)));
    // // currHoriEnc =
    // // Conversions::EncoderRadians::toInches(Conversions::Degrees::toRadians(
    // // Robot::Encoders::encoders[Robot::Encoders::RIGHT].position(deg)));
    // currHeading =
    //     Conversions::Degrees::toRadians(Robot::inertialSensor.heading(deg));

    // deltaVertEnc = lastVertEnc - currVertEnc;
    // // deltaHoriEnc = lastHoriEnc - currHoriEnc;

    // if (deltaVertEnc) {
    //   deltaHeading = lastHeading - currHeading;

    //   arcRadiusVert = deltaHeading / deltaVertEnc;
    //   // arcRadiusHori = deltaHeading / deltaHoriEnc;

    //   relDeltaPos.x = -arcRadiusVert * (1 - cos(deltaHeading));
    //   relDeltaPos.y = arcRadiusVert * sin(deltaHeading);

    //   cosP = cos(lastHeading);
    //   sinP = sin(lastHeading);

    //   absPos.x += (relDeltaPos.y * cosP) - (relDeltaPos.x * sinP);
    //   absPos.y += (relDeltaPos.y * cosP) - (relDeltaPos.x * sinP);
    // }
    // absPos.heading = currHeading;

    // printf("(x:%f,y:%f,yaw:%f)\n", relDeltaPos.x, relDeltaPos.y,
    //        absPos.heading);

    // lastVertEnc = currVertEnc;
    // lastHoriEnc = currHoriEnc;
    // lastHeading = currHeading;

    tracker->updateEncoderInches();
    // if (changeCheck(tracker->encoderInches[0], 0.01) ||
    //     changeCheck(tracker->encoderInches[1], 0.01) ||
    //     changeCheck(tracker->data.delta().getInertial(), 0.0001)) {
    // printf("what did you do neil!? %f\n",
    // Robot::getPosition().heading);
    // lastMsec = timer::system();
    tracker->updateSnapshot(tracker->findRobotPosition());
    // time = timer::system();
    // runningAve = runningAve * index / (index + 1) + (time - lastMsec)
    // /
    // index; index++; printf("%d\n", time - lastMsec);

    // } else
    //   tracker->data.noChangeUpdate();

    wait(OdomTracking::WAIT_TIME, msec);
  }
};

void OdomTracking::reset(const Position pos) {
  data.curr.pos = pos;
  data.last.pos = pos;
  Inertial10.setHeading(Conversions::Radians::toDegrees(pos.heading), deg);
};

OdomTracking::OdomTracking(Position startPos) : data(startPos) {
  trackers.push_back(this);
  displayPositon(data.curr.pos);
  (thread(trackerLoop));
  // displayPositon(snapshot.getPosition());
  // snapshot = SensorSnapshot::root(startPos);
  // tracker = this;
  // thread(trackingLoop1);
  // while (1) {
  //   // snapshot.getLast()->getPosition();
  //   updateEncoderInches();

  //   updateSnapshot(findRobotPosition());
  //   // displayPositon(snapshot.getPosition());
  //   // Brain.Screen.print(/*
  //   // Robot::Encoders::encoders[Robot::Encoders::LEFT].position(deg)
  //   //                     */
  //   // snapshot.getLast()->getEncoder(Robot::Encoders::LEFT));
  //   // Brain.Screen.print(", ");
  //   // Brain.Screen
  //   //     .print(/*
  //   // Robot::Encoders::encoders[Robot::Encoders::BACK].position(deg)
  //   //             */
  //   //            snapshot.getEncoder(Robot::Encoders::LEFT));
  //   wait(OdomTracking::waitTime, msec);
  // }
};