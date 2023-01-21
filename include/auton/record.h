#include "vex.h"
#include <forward_list>
#include <fstream>

#ifndef RECORD_H
#define RECORD_H

namespace auton {
struct RecordingUnit {
  unsigned int timeSinceLastUnit : 16;
  signed int left : 8;
  signed int right : 8;
  unsigned int shoot : 1;  // L1
  unsigned int roller : 1; // L2
  unsigned int intake : 1; // R1
  unsigned int outake : 1; // R2
  unsigned int expand : 1; // A & X & Y

  static char *toChars(RecordingUnit *);

  // Serialization function
  static void serialize(const RecordingUnit &unit, std::ostream &out);

  // Deserialization function
  static void deserialize(RecordingUnit &unit, std::istream &in);
};

class Recorder {
private:
  static int recordingLoop();
  static vex::task *loopTask;

  static std::forward_list<RecordingUnit *> recordingUnits;

public:
  static constexpr int iterationLength = 50; // ms

  Recorder() = delete;
  static void start();
  static void stop();
  static void save(const char *path);
  static void saveNew();
};
} // namespace auton

#endif