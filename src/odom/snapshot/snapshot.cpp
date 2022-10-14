// #include "odom/snapshot/snapshot.h"
// #include "conversions.h"
// #include "robot.h"

// #ifndef SENSORSNAPSHOT_CPP
// #define SENSORSNAPSHOT_CPP

// double SensorSnapshot::getEncoder(const Robot::Encoders::ENCODER encoderEnum) {
//   return encoders[encoderEnum];
// };

// double SensorSnapshot::getInertial(const INERTIAL inertialEnum) {
//   return inertials[inertialEnum];
// };

// double
// SensorSnapshot::getEncoderInches(const Robot::Encoders::ENCODER encoderEnum) {
//   return Conversions::EncoderRadians::toInches(this->getEncoder(encoderEnum));
// };

// void SensorSnapshot::updateEncoders() {
//   for (int i = 0 /* encoder */; i < Robot::Encoders::encoders.size(); i++) {
//     encoders[i] = Conversions::Degrees::toRadians(
//         Robot::Encoders::encoders[i].position(degrees));
//     // Controller1.Screen.setCursor(1, 1);
//     // Controller1.Screen.clearLine();

//     //     Brain.Screen.print(
//     // "yo momma");
//     // Robot::Encoders::encoders[Robot::Encoders::LEFT].position(degrees));
//   };
//   // Brain.Screen.setCursor(1, 2);
//   // Brain.Screen.print(encoders[0]);
//   // Brain.Screen.print(", ");
//   // Brain.Screen.print(encoders[1]);
// };

// SensorSnapshot snap = {};
// SensorSnapshot SensorSnapshot::root(const Position &startPosition) {
//   return SensorSnapshot(&snap, startPosition);
// };
// // SensorSnapshot SensorSnapshot::root(const Position &startPosition) {
// //   return SensorSnapshot(startPosition);
// // };

// DeltaSnapshot SensorSnapshot::delta() { return {this}; };

// double getInertialAxis(orientationType axis) {
//   return Conversions::Degrees::toRadians(
//       Robot::inertialSensor.orientation(axis, degrees));
// }

// void SensorSnapshot::updateInertials() {
//   inertials[INERTIAL::YAW] = getInertialAxis(yaw);
//   inertials[INERTIAL::ROLL] = getInertialAxis(roll);
//   inertials[INERTIAL::PITCH] = getInertialAxis(pitch);
// };

// void SensorSnapshot::updateData() {
//   updateInertials();
//   updateEncoders();
// }

// Position SensorSnapshot::getPosition() { return robotPos; };

// SensorSnapshot *SensorSnapshot::getLast() { return last; };

// std::vector<double> SensorSnapshot::getInertials() { return inertials; };
// std::vector<double> SensorSnapshot::getEncoders() { return encoders; };

// SensorSnapshot::SensorSnapshot(SensorSnapshot *last, Position robotPosition)
//     : robotPos(robotPosition), last(last) {
//   updateData();
// };

// SensorSnapshot::SensorSnapshot(Position startPos)
//     : robotPos(startPos), last(nullptr), first(true) {

//   updateData();
// };
// SensorSnapshot::SensorSnapshot() : robotPos(), last(nullptr), first(true) {
//   updateData();
// }

// void SensorSnapshot::lastUpdateInertials(std::vector<double> lastInertials) {
//   this->inertials = lastInertials;
// };
// void SensorSnapshot::lastUpdateEncoders(std::vector<double> lastEncoders) {
//   this->encoders = lastEncoders;
// };
// void SensorSnapshot::lastUpdatePosition(Position lastPos) {
//   robotPos.x = lastPos.x;
//   robotPos.y = lastPos.y;
//   robotPos.heading = lastPos.heading;
      
//   Brain.Screen.clearLine();
//   Brain.Screen.print("(");
//   Brain.Screen.print(lastPos.x);
//   Brain.Screen.print(",");
//   Brain.Screen.print(lastPos.y);
//   Brain.Screen.print(",");
//   Brain.Screen.print(lastPos.heading);
//   Brain.Screen.print(")");
//   Brain.Screen.newLine();
// };

// void SensorSnapshot::lastUpdate(SensorSnapshot *lastSnap) {
//   lastUpdateInertials(lastSnap->getInertials());
//   lastUpdateEncoders(lastSnap->getEncoders());
//   lastUpdatePosition(lastSnap->getPosition());
// }

// // SensorSnapshot last = SensorSnapshot::root({});
// void SensorSnapshot::update(const Position robotPosition) {

//   // Brain.Screen.setCursor(1, 1);
//   // Brain.Screen.clearLine();
//   // Brain.Screen.print("(");
//   // Brain.Screen.print(robotPosition.x);
//   // Brain.Screen.print(",");
//   // Brain.Screen.print(robotPosition.y);
//   // Brain.Screen.print(",");
//   // Brain.Screen.print(robotPosition.heading);
//   // Brain.Screen.print(")");
//   // Brain.Screen.newLine();


//   // SensorSnapshot lastThing = {this, robotPosition};
//   last->lastUpdate(this);
//   updateData();

//   // Controller1.Screen.setCursor(1, 1);
//   // Controller1.Screen.clearLine();

//   // Controller1.Screen.print(robotPosition.x);
//   // Controller1.Screen.print(", ");
//   // Controller1.Screen.print(robotPosition.y);

//   return;
// };

// #endif