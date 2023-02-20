#include "odom/odomdata.h"
#include "conversions.h"
void OdomData::Data::set(Data data) {
  yaw = data.yaw;
  leftEnc = data.leftEnc;
  rightEnc = data.rightEnc;
  horiEnc = data.horiEnc;
  pos.x = data.pos.x;
  pos.y = data.pos.y;
  pos.heading = data.pos.heading;
};

void OdomData::Data::update(Position pos1) {
  // static constexpr float driveFactor =
  //     Robot::Dimensions::driveWheelRadius * ;
  // static constexpr float encoderFactor =
  // Robot::Dimensions::encoderWheelRadius;
  yaw = Conversions::Degrees::toRadians(Robot::inertialSensor.heading(degrees));
  leftEnc = Conversions::Degrees::toRadians(
      Robot::Encoders::encoders[Robot::Encoders::ENCODER::LEFT].position(
          degrees));
  rightEnc = Conversions::Degrees::toRadians(
      Robot::Encoders::encoders[Robot::Encoders::ENCODER::RIGHT].position(
          degrees));
  horiEnc = Conversions::Degrees::toRadians(
      Robot::Encoders::encoders[Robot::Encoders::ENCODER::BACK].position(
          degrees));
  leftEnc += rightEnc;
  leftEnc /= 4;

  pos.x = pos1.x;
  pos.y = pos1.y;
  pos.heading = yaw;
};

OdomData::Data::Data(double yaw, double leftEnc, double rightEnc,
                     double horiEnc, Position pos)
    : yaw(yaw), leftEnc(leftEnc), rightEnc(rightEnc), horiEnc(horiEnc),
      pos(pos){};

OdomData::Data::Data(Position pos1) { update(pos1); };
OdomData::Data::Data() { update({}); };

OdomData::DeltaData::DeltaData(const OdomData *odomData)
    : last(odomData->getLastData()), curr(odomData->getCurrData()){};

void OdomData::update(Position roborPos) {
  last.set(curr);
  curr.update(roborPos);
};

const double OdomData::getInertial() const { return curr.yaw; };
const double OdomData::getEncoder(const Robot::Encoders::ENCODER enc) const {
  switch (enc) {
  case Robot::Encoders::LEFT:
    return curr.leftEnc;
  case Robot::Encoders::RIGHT:
    return curr.rightEnc;
  case Robot::Encoders::BACK:
    return curr.horiEnc;
  }
  return 0;
};
const double
OdomData::getEncoderInches(const Robot::Encoders::ENCODER enc) const {
  switch (enc) {
  case Robot::Encoders::ENCODER::BACK:
    return getEncoder(enc) * Robot::Dimensions::encoderWheelRadius;
  default:
    return getEncoder(enc) * Robot::Dimensions::driveWheelRadius *
           Robot::Dimensions::driveGearRatio;
  };
};
const Position OdomData::getPosition() const {
  // printf("x: %f\n", curr.pos.x);
  // printf("y: %f\n", curr.pos.y);
  return curr.pos /* {curr.pos.x, curr.pos.y, curr.pos.heading} */;
};

const std::array<double, 3> OdomData::getEncoders() const {
  return {curr.leftEnc, curr.rightEnc, curr.horiEnc};
};

const std::array<double, 3> OdomData::getEncodersInches() const {
  return {getEncoderInches(Robot::Encoders::ENCODER::LEFT),
          getEncoderInches(Robot::Encoders::ENCODER::RIGHT),
          getEncoderInches(Robot::Encoders::ENCODER::BACK)};
};

const OdomData::Data OdomData::getCurrData() const { return curr; };

const OdomData::Data OdomData::getLastData() const { return last; };

const OdomData::DeltaData OdomData::delta() const { return {this}; };

const double OdomData::DeltaData::getInertial() const {
  return curr.yaw - last.yaw;
};

const double
OdomData::DeltaData::getEncoder(const Robot::Encoders::ENCODER enc) const {
  switch (enc) {
  case Robot::Encoders::LEFT:
    return last.leftEnc - curr.leftEnc;
  case Robot::Encoders::RIGHT:
    return last.rightEnc - curr.rightEnc;
  case Robot::Encoders::BACK:
    return last.horiEnc - curr.horiEnc;
  }
  return 0;
};

const double OdomData::DeltaData::getEncoderInches(
    const Robot::Encoders::ENCODER enc) const {
  switch (enc) {
  case Robot::Encoders::ENCODER::BACK:
    return getEncoder(enc) * Robot::Dimensions::encoderWheelRadius;
  default:
    return getEncoder(enc) * Robot::Dimensions::driveWheelRadius *
           Robot::Dimensions::driveGearRatio;
  };
};

const Position OdomData::DeltaData::getPosition() const {
  return Position::difference(curr.pos, last.pos);
};

const std::array<double, 3> OdomData::DeltaData::getEncoders() const {
  return {curr.leftEnc - last.leftEnc, curr.rightEnc - last.rightEnc,
          curr.horiEnc - last.horiEnc};
};

const std::array<double, 3> OdomData::DeltaData::getEncodersInches() const {
  return {getEncoderInches(Robot::Encoders::ENCODER::LEFT),
          getEncoderInches(Robot::Encoders::ENCODER::RIGHT),
          getEncoderInches(Robot::Encoders::ENCODER::BACK)};
};

OdomData::OdomData(const Position startPos) : last({{}}), curr({startPos}){};

void OdomData::Data::noChangeUpdate() {
  yaw = Conversions::Degrees::toRadians(Robot::inertialSensor.heading(degrees));
  leftEnc = Conversions::Degrees::toRadians(
      Robot::Encoders::encoders[Robot::Encoders::ENCODER::LEFT].position(
          degrees));
  rightEnc = Conversions::Degrees::toRadians(
      Robot::Encoders::encoders[Robot::Encoders::ENCODER::RIGHT].position(
          degrees));
  horiEnc = Conversions::Degrees::toRadians(
      Robot::Encoders::encoders[Robot::Encoders::ENCODER::BACK].position(
          degrees));

  pos.heading = yaw;
};

void OdomData::noChangeUpdate() { curr.noChangeUpdate(); };