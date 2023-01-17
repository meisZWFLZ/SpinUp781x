#include "auton/record.h"

#ifndef PLAYER_H
#define PLAYER_H

namespace auton {
class Player {
private:
  static void playRecordingUnit(RecordingUnit *);
  static int playbackLoop();
  static vex::task *loopTask;

  static std::forward_list<RecordingUnit *> recordingUnits;

public:
  static constexpr int iterationLength = 50; // ms

  Player() = delete;
  static void start();
  static void stop();
  static void load(char *path);
};

} // namespace auton
#endif