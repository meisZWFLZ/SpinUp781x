#include "odom/odomdata.h"
#include "conversions.h"
void OdomData::Data::set(Data data) {
  yaw = data.yaw;
  vertEnc = data.vertEnc;
  horiEnc = data.horiEnc;
  pos.x = data.pos.x;
  pos.y = data.pos.y;
  pos.heading = data.pos.heading;
};

void OdomData::Data::update(Position pos1) {
  yaw = Conversions::Degrees::toRadians(Robot::inertialSensor.heading(degrees));
  vertEnc = Conversions::Degrees::toRadians(
      Robot::Encoders::encoders[Robot::Encoders::ENCODER::LEFT].position(
          degrees));
  horiEnc = Conversions::Degrees::toRadians(
      Robot::Encoders::encoders[Robot::Encoders::ENCODER::BACK].position(
          degrees));

  pos.x = pos1.x;
  pos.y = pos1.y;
  pos.heading = yaw;
};

OdomData::Data::Data(double yaw, double vertEnc, double horiEnc, Position pos)
    : yaw(yaw), vertEnc(vertEnc), horiEnc(horiEnc), pos(pos){};

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
  return enc == 0 ? curr.vertEnc : curr.horiEnc;
};
const double
OdomData::getEncoderInches(const Robot::Encoders::ENCODER enc) const {
  return Conversions::EncoderRadians::toInches(getEncoder(enc));
};
const Position OdomData::getPosition() const {
  // printf("x: %f\n", curr.pos.x);
  // printf("y: %f\n", curr.pos.y);
  return curr.pos /* {curr.pos.x, curr.pos.y, curr.pos.heading} */;
};

const std::array<double, 2> OdomData::getEncoders() const {
  return {curr.vertEnc, curr.horiEnc};
};

const std::array<double, 2> OdomData::getEncodersInches() const {
  return {Conversions::EncoderRadians::toInches(curr.vertEnc),
          Conversions::EncoderRadians::toInches(curr.horiEnc)};
};

const OdomData::Data OdomData::getCurrData() const { return curr; };

const OdomData::Data OdomData::getLastData() const { return last; };

const OdomData::DeltaData OdomData::delta() const { return {this}; };

const double OdomData::DeltaData::getInertial() const {
  return curr.yaw - last.yaw;
};

const double
OdomData::DeltaData::getEncoder(const Robot::Encoders::ENCODER enc) const {
  return enc == 0 ? curr.vertEnc - last.vertEnc : curr.horiEnc - last.horiEnc;
};

const double OdomData::DeltaData::getEncoderInches(
    const Robot::Encoders::ENCODER enc) const {
  return Conversions::EncoderRadians::toInches(getEncoder(enc));
};

const Position OdomData::DeltaData::getPosition() const {
  return Position::difference(curr.pos, last.pos);
};

const std::array<double, 2> OdomData::DeltaData::getEncoders() const {
  return {curr.vertEnc - last.vertEnc, curr.horiEnc - last.horiEnc};
};

const std::array<double, 2> OdomData::DeltaData::getEncodersInches() const {
  return {Conversions::EncoderRadians::toInches(curr.vertEnc - last.vertEnc),
          Conversions::EncoderRadians::toInches(curr.horiEnc - last.horiEnc)};
};

OdomData::OdomData(const Position startPos) : last({{}}), curr({startPos}){};