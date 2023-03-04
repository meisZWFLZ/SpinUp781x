#include "auton/playback.h"
#include "robot.h"

using namespace auton;

// vex::task *Player::loopTask = nullptr;
// std::forward_list<RecordingUnit *> Player::recordingUnits =
//     std::forward_list<RecordingUnit *>();

void Player::stop() {
  if (Player::loopTask != nullptr)
    Player::loopTask->stop();
}

Player::Player(const char *path)
    : loopTask(nullptr), recordingUnits(std::forward_list<RecordingUnit *>()) {
  std::ifstream loadFile(path, std::ifstream::binary);
  if (loadFile.is_open()) {
    // Player::recordingUnits.reverse();
    printf("start loading \n");
    while (loadFile) {
      RecordingUnit *currUnit = new RecordingUnit();
      RecordingUnit::deserialize(*currUnit, loadFile);
      printf("outake: %i\n", currUnit->outake);
      recordingUnits.push_front(currUnit);
    }
    // myfile.write(buffer, 8);
    // printf("%s\n", buffer);
  } else {
    printf("Couldn't open file\n");
  }
}

inline const int getTime() { return vex::timer::system(); }

inline void Player::playRecordingUnit(RecordingUnit *unit) {
  // static int left = 0;
  // static int right = 0;

  // task::sleep(unit->timeSinceLastUnit * Recorder::iterationLength);
  task::sleep(unit->timeSinceLastUnit);

  // left += unit->left;
  // right += unit->right;

  // printf("outake: %i\n", unit->outake);
  printf("time: %i\n", unit->timeSinceLastUnit);
  printf("left: %i\n", unit->left);
  printf("right: %i\n", unit->right);
  Robot::InputListeners::leftDrive(unit->left);
  Robot::InputListeners::rightDrive(unit->right);
  if (unit->shoot)
    Robot::InputListeners::shoot();
  if (unit->roller)
    Robot::InputListeners::roller();
  if (unit->intake)
    Robot::InputListeners::intake();
  if (unit->outake)
    Robot::InputListeners::outtake();
  if (unit->expand)
    Robot::InputListeners::expand();
}

std::vector<std::forward_list<RecordingUnit *> *> arrOfArrOfUnits = {};

void Player::start() {
  // printf("list empty?: %i\n", !recordingUnits.empty());
  // printf("loop exists?: %i\n", Player::loopTask == nullptr);

  if (!recordingUnits.empty() && Player::loopTask == nullptr) {
    arrOfArrOfUnits.push_back(&recordingUnits);
    Player::loopTask = new vex::task(Player::playbackLoop);
  }
}

int Player::playbackLoop() {
  std::forward_list<RecordingUnit *> myRecordingUnits = *arrOfArrOfUnits.back();
  printf("playback loop starting\n");
  for (auto unit : myRecordingUnits)
    playRecordingUnit(unit);
  Robot::Drivetrain::left(0);
  Robot::Drivetrain::right(0);
  Robot::Actions::stopIntake();
  task::yield();
  return 1;
};