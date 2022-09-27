#include "position.h"
#include "robot.h"
#include <array>

#ifndef ODOM_ODOMDATA_H
#define ODOMDATA_H

class OdomData {
public:
  struct Data {
    double yaw;
    double vertEnc;
    double horiEnc;

    Position pos;

    void set(Data);
    void update(Position pos);

    Data(double yaw, double vertEnc, double horiEnc, Position pos);
    Data(Position pos);
    Data();
  };
  class DeltaData {
  public:
    Data last;
    Data curr;

    DeltaData(const OdomData *odomData);

    // getters
    const double getInertial() const;
    const double getEncoder(const Robot::Encoders::ENCODER) const;
    const double getEncoderInches(const Robot::Encoders::ENCODER) const;
    const Position getPosition() const;

    const std::array<double, 2> getEncoders() const;
    const std::array<double, 2> getEncodersInches() const;

    const Data getData() const;
  };
  Position pain = {};

  Data last;
  Data curr;

  void update(Position roborPos);

  // getters
  const double getInertial() const;
  const double getEncoder(const Robot::Encoders::ENCODER) const;
  const double getEncoderInches(const Robot::Encoders::ENCODER) const;
  const Position getPosition() const;

  const std::array<double, 2> getEncoders() const;
  const std::array<double, 2> getEncodersInches() const;

  const Data getCurrData() const;
  const Data getLastData() const;

  const DeltaData delta() const;

  OdomData(const Position startPos);
};

#endif