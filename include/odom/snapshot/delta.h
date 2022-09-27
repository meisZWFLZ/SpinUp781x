#pragma once

#ifndef DELTASNAPSHOT_H
#define DELTASNAPSHOT_H

#include "odom/snapshot/getters.h"

class DeltaSnapshot : public SnapshotGetters {
public:
  SnapshotGetters *snapshot;
  DeltaSnapshot(SnapshotGetters *snapshot);

  DeltaSnapshot *getLast() override;

  double getEncoder(const Robot::Encoders::ENCODER) override;
  double getEncoderInches(const Robot::Encoders::ENCODER) override;

  double getInertial(const INERTIAL) override;

  Position getPosition() override;

  virtual ~DeltaSnapshot(){};
};

#endif