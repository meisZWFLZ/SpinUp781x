#include "odom/tracking.h"
#include "vex.h"
#include <functional>
#include <string>
#include <thread>

using namespace std;

void OdomTracking::updateSnapshot(const Position robotPos) {
  data.update(Position::sum(robotPos, data.getPosition()));
  this->deltaPos1 = data.getPosition();
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
  encoderInches = {data.delta().getEncoderInches(Robot::Encoders::LEFT),
                   data.delta().getEncoderInches(Robot::Encoders::BACK)};
  // encoderInches[Robot::Encoders::BACK] =
  // encoderInches.push_back(
  // data.delta().getEncoderInches(Robot::Encoders::BACK));
};

double OdomTracking::findDeltaX(const double deltaTheta) {
  // vector
  // return deltaTheta == 0
  //            ? encoderInches[Robot::Encoders::BACK]
  //            : ((2 * sin(deltaTheta / 2))) *
  //                  ((encoderInches[Robot::Encoders::BACK] / deltaTheta) +
  //                   Robot::Encoders::distanceToTrackingCenter
  //                       [Robot::Encoders::BACK]);
  // arc

  return 2 * sin(data.getInertial() / 2) *
         ((encoderInches[Robot::Encoders::LEFT] / deltaTheta) +
          Robot::Encoders::distanceToTrackingCenter[Robot::Encoders::LEFT]);
};
double OdomTracking::findDeltaY(const double deltaTheta) {
  // vector
  // return deltaTheta == 0
  //            ? encoderInches[Robot::Encoders::LEFT]
  //            : ((2 * sin(deltaTheta / 2))) *
  //                  ((encoderInches[Robot::Encoders::LEFT] / deltaTheta) +
  //                   Robot::Encoders::distanceToTrackingCenter
  //                       [Robot::Encoders::LEFT]);

  // arc
  return 2 * sin(data.getInertial() / 2) *
         ((encoderInches[Robot::Encoders::LEFT] / deltaTheta) +
          Robot::Encoders::distanceToTrackingCenter[Robot::Encoders::LEFT]);
};
double OdomTracking::findDeltaTheta() { return data.delta().getInertial(); };

Position OdomTracking::findDeltaPosition() {
  double deltaTheta = findDeltaTheta();
  return {findDeltaX(deltaTheta), findDeltaY(deltaTheta), deltaTheta};
};

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

  return y;
}

double OdomTracking::findTravelX(const Position &deltaPos) {
  // bad vector rotation
  double meanDTheta = data.last.yaw + (deltaPos.heading / 2);
  return (deltaPos.x * cos(-meanDTheta)) - (deltaPos.y * sin(-meanDTheta));
};
struct coordinate {
  float x;
  float y;

  coordinate(const double x, const double y) : x(x), y(y){};
};
coordinate findTravelCoord(const Position &deltaPos) {
  // desmos vector rotation
  // t = acos(a/sqrt(a^2+b^2);
  // a_I = sqrt(a^2+b^2)*cos(t + t_I)
  // b_I = sqrt(a^2+b^2)*sin(t + t_I)
  float coefficent = Q_rsqrt(pow(deltaPos.x, 2) + pow(deltaPos.y, 2));
  float t1 =
      acos(deltaPos.x / Q_rsqrt(pow(deltaPos.x, 2) + pow(deltaPos.y, 2))) +
      deltaPos.heading;
  return {(coefficent * cos(t1)), (coefficent * sin(t1))};                                                     
};

double OdomTracking::findTravelY(const Position &deltaPos) {
  double meanDTheta = data.last.yaw + (deltaPos.heading / 2);
  return (deltaPos.x * sin(-meanDTheta)) - (deltaPos.y * cos(-meanDTheta));
};

Position OdomTracking::findRobotPosition() {
  // auto coord = findTravelCoord(findDeltaPosition());
  // Position robotPos1 = {coord.x, coord.y, 0};
  auto deltaPos = findDeltaPosition();
  Position robotPos1 = {findTravelX(deltaPos), findTravelY(deltaPos), 0};

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

std::vector<OdomTracking *> trackers = {};
void trackerLoop() {
  OdomTracking *tracker = trackers[trackers.size() - 1];
  while (1) {
    tracker->updateEncoderInches();
    if (tracker->encoderInches[0] != 0 || tracker->encoderInches[1] != 0 ||
        tracker->data.delta().getInertial() != 0) {
      tracker->updateSnapshot(tracker->findRobotPosition());
    } else
      tracker->updateSnapshot({});

    wait(OdomTracking::WAIT_TIME, msec);
  }
};

OdomTracking::OdomTracking(Position startPos) : data(startPos) {
  trackers.push_back(this);
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