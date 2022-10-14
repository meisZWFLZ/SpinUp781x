// #include "odom/snapshot/delta.h"
// #include "conversions.h"

// #ifndef DELTASNAPSHOT_CPP
// #define DELTASNAPSHOT_CPP

// double DeltaSnapshot::getEncoder(Robot::Encoders::ENCODER encoderEnum) {
//   return snapshot->getLast()->getEncoder(encoderEnum) -
//          snapshot->getEncoder(encoderEnum);
// };

// double DeltaSnapshot::getInertial(INERTIAL inertialEnum) {
//   return snapshot->getLast()->getInertial(inertialEnum) -
//          snapshot->getInertial(inertialEnum);
// }

// double
// DeltaSnapshot::getEncoderInches(const Robot::Encoders::ENCODER encoderEnum) {
//   return Conversions::EncoderRadians::toInches(this->getEncoder(encoderEnum));
// };

// Position DeltaSnapshot::getPosition() {
//   Position posVar = Position::difference(snapshot->getLast()->getPosition(),
//                                          snapshot->getPosition());
//   return posVar;
// };

// DeltaSnapshot *DeltaSnapshot::getLast() {
//   DeltaSnapshot lastDelta = {snapshot->getLast()};
//   return &lastDelta;
// }
// DeltaSnapshot::DeltaSnapshot(SnapshotGetters *snapshot) : snapshot(snapshot){};

// #endif