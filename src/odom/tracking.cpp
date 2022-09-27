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
  return deltaTheta == 0
             ? encoderInches[Robot::Encoders::BACK]
             : ((2 * sin(deltaTheta / 2))) *
                   ((encoderInches[Robot::Encoders::BACK] / deltaTheta) +
                    Robot::Encoders::distanceToTrackingCenter
                        [Robot::Encoders::BACK]);
};
double OdomTracking::findDeltaY(const double deltaTheta) {
  return deltaTheta == 0
             ? encoderInches[Robot::Encoders::LEFT]
             : ((2 * sin(deltaTheta / 2))) *
                   ((encoderInches[Robot::Encoders::LEFT] / deltaTheta) +
                    Robot::Encoders::distanceToTrackingCenter
                        [Robot::Encoders::LEFT]);
};
double OdomTracking::findDeltaTheta() {
  // return (encoderInches[Robot::Encoders::LEFT] -
  //         encoderInches[Robot::Encoders::RIGHT]) /
  //        (Robot::Encoders::distanceToTrackingCenter[Robot::Encoders::LEFT] +
  //         Robot::Encoders::distanceToTrackingCenter[Robot::Encoders::RIGHT]);
  // return data.delta().getInertial(YAW);
  return data.delta().getInertial();
};

Position OdomTracking::findDeltaPosition() {
  double deltaTheta = findDeltaTheta();
  return {findDeltaX(deltaTheta) /* 0 */, findDeltaY(deltaTheta) /* 0 */,
          deltaTheta};
  // this->deltaPos1 = {findDeltaX(deltaTheta) /* 0 */,
  //                    findDeltaY(deltaTheta) /* 0 */, deltaTheta};
  // return deltaPos1;
};
// axis:
// true -> X
// false -> y
// double OdomTracking::findTravel(const bool axis, const Position &deltaPos) {
//   double meanDTheta =
//       data.getPosition().heading /* 0 */ + (deltaPos.heading / 2);
//   return (axis ? deltaPos.x : deltaPos.y) * ((cos(-meanDTheta))) -
//          (!axis ? deltaPos.x : deltaPos.y) * ((sin(-meanDTheta)));
//   // return 0;
// };
double OdomTracking::findTravelX(const double deltaX, const double deltaY,
                                 const double deltaHeading) {
  double meanDTheta = data.last.yaw /* 0 */ + (deltaHeading / 2);
  // return (deltaX * cos(meanDTheta));
  return (deltaX * cos(-meanDTheta)) - (deltaY * sin(-meanDTheta));
  // deltaPos1.x = DTx;
  // return DTx;
  // return findTravel(true, deltaPos) /* 0 */;
};
double OdomTracking::findTravelY(const double deltaX, const double deltaY,
                                 const double deltaHeading) {
  double meanDTheta = data.last.yaw /* 0 */ + (deltaHeading / 2);
  // return (deltaY * sin(meanDTheta));
  return (deltaX * sin(-meanDTheta)) - (deltaY * cos(-meanDTheta));
  // deltaPos1.y = DTy;
  // return DTy;
  // return findTravel(false, deltaPos);
};

Position OdomTracking::findRobotPosition() {
  Position deltaPos = findDeltaPosition();
  Position robotPos1 = {findTravelX(deltaPos.x, deltaPos.y, deltaPos.heading),
                        findTravelY(deltaPos.x, deltaPos.y, deltaPos.heading),
                        0};

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