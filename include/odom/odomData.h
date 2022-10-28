#include "position.h"
#include "robot.h"
#include <array>

#ifndef ODOM_ODOMDATA_H
#define ODOM_ODOMDATA_H

class OdomData {
public:
  struct Data {
    double yaw;
    double leftEnc;
    double rightEnc;
    double horiEnc;

    Position pos;

    void set(Data);
    void update(Position pos);
    void noChangeUpdate();

    Data(double yaw, double leftEnc, double rightEnc, double horiEnc,
         Position pos);
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

    const std::array<double, 3> getEncoders() const;
    const std::array<double, 3> getEncodersInches() const;

    const Data getData() const;
  };
  // Position pain = {};

  Data last;
  Data curr;

  void update(Position roborPos);
  void noChangeUpdate();

  // getters
  const double getInertial() const;
  const double getEncoder(const Robot::Encoders::ENCODER) const;
  const double getEncoderInches(const Robot::Encoders::ENCODER) const;
  const Position getPosition() const;

  const std::array<double, 3> getEncoders() const;
  const std::array<double, 3> getEncodersInches() const;

  const Data getCurrData() const;
  const Data getLastData() const;

  const DeltaData delta() const;

  OdomData(const Position startPos);
};

#endif