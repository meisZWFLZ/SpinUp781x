#include "auton/record.h"
#include "auton/recordPaths.h"
#include <algorithm>
#include <sstream>

using namespace auton;

vex::task *Recorder::loopTask = nullptr;
std::forward_list<RecordingUnit *> Recorder::recordingUnits =
    std::forward_list<RecordingUnit *>();

// Serialization function
void RecordingUnit::serialize(const RecordingUnit &unit, std::ostream &out) {
  out.write(reinterpret_cast<const char *>(&unit), sizeof(unit));
}

// Deserialization function
void RecordingUnit::deserialize(RecordingUnit &unit, std::istream &in) {
  in.read(reinterpret_cast<char *>(&unit), sizeof(unit));
}

void Recorder::start() {
  if (Recorder::loopTask != nullptr) {
    // Recorder::stop();
    Recorder::loopTask->suspend();
    Recorder::saveNew();
    Recorder::recordingUnits = std::forward_list<RecordingUnit *>();
    Recorder::loopTask->resume();
  } else
    Recorder::loopTask = new vex::task(Recorder::recordingLoop);
}

void Recorder::stop() {
  if (Recorder::loopTask != nullptr)
    Recorder::loopTask->stop();
  loopTask = nullptr;
}

void Recorder::save(const char *path) {
  RecordingPaths::add(path);
  std::ofstream myfile(path, std::ofstream::binary);
  if (myfile.is_open()) {
    // Recorder::recordingUnits.reverse();
    for (auto unit : recordingUnits) {
      if (!myfile)
        break;
      RecordingUnit::serialize(*unit, myfile);
      // myfile.write(buffer, 8);
      // printf("%s\n", buffer);
    }
  } else {
    printf("Couldn't open file\n");
  }
}
void Recorder::saveNew() {
  std::stringstream ss;
  if (!RecordingPaths::get().empty())
    ss << std::max(RecordingPaths::get().size(),
                   (unsigned)atoi(RecordingPaths::get().back().c_str())) +
              1;
  else
    ss << RecordingPaths::get().size();
  // ss << atoi(RecordingPaths::get().back().c_str()) + 1;
  save(ss.str().c_str());
};

namespace recordingLoopStuff {

inline const int getTime() { return vex::timer::system(); }

inline const int getLeft() { return Controller1.Axis3.position(); }
inline const int getRight() { return Controller1.Axis2.position(); }
inline const bool getShoot() { return Controller1.ButtonL1.pressing(); }
inline const bool getRoller() { return Controller1.ButtonL2.pressing(); }
inline const bool getIntake() { return Controller1.ButtonR1.pressing(); }
inline const bool getOutake() { return Controller1.ButtonR2.pressing(); }
inline const bool getExpand() {
  return Controller1.ButtonA.pressing() && Controller1.ButtonX.pressing() &&
         Controller1.ButtonY.pressing();
}

struct ControllerValues {
  int left = 0;
  int right = 0;
  bool shoot = 0;  // L1
  bool roller = 0; // L2
  bool intake = 0; // R1
  bool outake = 0; // R2
  bool expand = 0;
  ControllerValues()
      : left(0), right(0), shoot(0), roller(0), intake(0), outake(0),
        expand(0){};
  ControllerValues(int left, int right, bool shoot, bool roller, bool intake,
                   bool outake, bool expand)
      : left(left), right(right), shoot(shoot), roller(roller), intake(intake),
        outake(outake), expand(expand){};

  inline const bool checkLeft(ControllerValues *last) {
    return left - last->left != 0;
  };
  inline const bool checkRight(ControllerValues *last) {
    return left - last->left != 0;
  };

  inline const bool checkShoot(ControllerValues *last) const {
    return !last->shoot && shoot;
  };
  inline const bool checkRoller(ControllerValues *last) const {
    return !last->roller && roller;
  };
  inline const bool checkIntake(ControllerValues *last) const {
    return !last->intake && intake;
  };
  inline const bool checkOutake(ControllerValues *last) const {
    return !last->outake && outake;
  };
  inline const bool checkExpand(ControllerValues *last) const {
    return !last->expand && expand;
  };

  static inline ControllerValues *get() {
    return new ControllerValues(getLeft(), getRight(), getShoot(), getRoller(),
                                getIntake(), getOutake(), getExpand());
  };
  // true -> newVal
  // false -> notNew
  static inline bool compare(ControllerValues *last, ControllerValues *curr) {
    return curr->checkLeft(last) || curr->checkRight(last) ||
           curr->checkShoot(last) || curr->checkRoller(last) ||
           curr->checkIntake(last) || curr->checkOutake(last) ||
           curr->checkExpand(last);
  };

  static RecordingUnit *toRecordingUnit(ControllerValues *last,
                                        ControllerValues *curr,
                                        int timeSinceLastUnit) {
    auto unit = new RecordingUnit();

    unit->timeSinceLastUnit = timeSinceLastUnit;
    // unit->right = curr->right - last->right;
    // unit->left = curr->left - last->left;
    unit->right = curr->right;
    unit->left = curr->left;
    unit->shoot = curr->checkShoot(last);
    unit->roller = curr->checkRoller(last);
    unit->intake = curr->checkIntake(last);
    unit->outake = curr->checkOutake(last);
    unit->expand = curr->checkExpand(last);

    return unit;
  };
};
}; // namespace recordingLoopStuff
int Recorder::recordingLoop() {
  printf("loop starting1\n");
  using namespace recordingLoopStuff;
  ControllerValues *last = new ControllerValues();
  ControllerValues *curr;
  printf("loop starting2\n");
  // int i = 0;
  // int lastI = 0; // iteration of last RecordingUnit
  int lastTime = getTime();
  while (1) {
    curr = ControllerValues::get();
    printf("loop iteration: %i\n", getTime());
    if (ControllerValues::compare(last, curr)) {
      printf("new vals\n");
      Recorder::recordingUnits.push_front(
          // ControllerValues::toRecordingUnit(last, curr, i - lastI));
          ControllerValues::toRecordingUnit(last, curr, getTime() - lastTime));
      // lastI = i;
      lastTime = getTime();
    };
    last = curr;
    // i++;
    task::sleep(Recorder::iterationLength);
  };
  return 1;
};