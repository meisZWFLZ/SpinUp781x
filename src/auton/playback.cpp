#include "auton/playback.h"

using namespace auton;

vex::task *Player::loopTask = nullptr;
std::forward_list<RecordingUnit *> Player::recordingUnits =
    std::forward_list<RecordingUnit *>();

void Player::start() {
  if (!recordingUnits.empty() && Player::loopTask == nullptr)
    Player::loopTask = new vex::task(Player::playbackLoop);
}

void Player::stop() {
  if (Player::loopTask != nullptr)
    Player::loopTask->stop();
}

void Player::load(char *path) {
  std::ifstream loadFile(path, std::ofstream::binary);
  if (loadFile.is_open()) {
    // Player::recordingUnits.reverse();
    while (!loadFile) {
      RecordingUnit currUnit = {};
      RecordingUnit::deserialize(currUnit, loadFile);
      recordingUnits.push_front(&currUnit);
    }
    // myfile.write(buffer, 8);
    // printf("%s\n", buffer);
  } else {
    printf("Couldn't open file\n");
  }
}

inline const int getTime() { return vex::timer::system(); }

void Player::playRecordingUnit(RecordingUnit *unit) {
  
}

int Player::playbackLoop() {
  printf("loop starting1\n");
  for (auto unit : recordingUnits) {
  }
  return 1;
};