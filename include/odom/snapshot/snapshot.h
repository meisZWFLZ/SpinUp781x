#pragma once

#ifndef SENSORSNAPSHOT_H
#define SENSORSNAPSHOT_H

#include "odom/snapshot/delta.h"
#include "odom/snapshot/getters.h"
#include <vector>

class SensorSnapshot : public SnapshotGetters {
public:
  std::vector<double> encoders = {0, 0};
  std::vector<double> inertials = {0, 0, 0};
  Position robotPos;

  SensorSnapshot *last;
  bool first = false; // tell whether there is a last to prevent errors

  void updateEncoders();
  void updateInertials();

  void updateData();

  SensorSnapshot(Position startPosition); // root
  SensorSnapshot(SensorSnapshot *last,
                 Position robotPosition); // gets encoders and inertialSensors

  void lastUpdateInertials(std::vector<double> lastInertials);
  void lastUpdateEncoders(std::vector<double> lastEncoders);
  void lastUpdatePosition(Position lastPos);

  void lastUpdate(SensorSnapshot* lastSnap);

public:
  SensorSnapshot();

  // updates data and returns new snapshot
  static SensorSnapshot root(const Position &startPosition);
  // SensorSnapshot(SensorSnapshot &);
  void update(Position robotPosition);
  DeltaSnapshot delta(); // gets delta version of snapshot

  SensorSnapshot *getLast() override;

  double getEncoder(const Robot::Encoders::ENCODER) override;
  double getEncoderInches(const Robot::Encoders::ENCODER) override;

  double getInertial(const INERTIAL) override;

  Position getPosition() override;

  std::vector<double> getEncoders();
  std::vector<double> getInertials();

  // virtual SensorSnapshot(){};
};
#endif