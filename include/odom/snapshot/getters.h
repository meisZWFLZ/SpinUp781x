#pragma once

#ifndef SNAPSHOTGETTERS_H
#define SNAPSHOTGETTERS_H

#include "position.h"
#include "robot.h"

enum INERTIAL { YAW, PITCH, ROLL };

class SnapshotGetters {
public:
  virtual SnapshotGetters *getLast() = 0;

  // get encoder value
  virtual double getEncoder(const Robot::Encoders::ENCODER) = 0;
  // get encoder value in inches
  virtual double getEncoderInches(const Robot::Encoders::ENCODER) = 0;

  virtual double getInertial(const INERTIAL) = 0;

  virtual Position getPosition() = 0;
  // ~SnapshotGetters();
};

#endif