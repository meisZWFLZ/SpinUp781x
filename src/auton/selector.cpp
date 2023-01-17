#include "auton/selector.h"
#include "auton/autons.h"
#include "robot.h"

void AutonSelector::print() {
  Brain.Screen.clearScreen();
  // team color
  Brain.Screen.setFillColor(Robot::TEAM::RED == Robot::team ? red : blue);
  Brain.Screen.drawRectangle(240, 0, 480, 240);
  
  // auton display
  Brain.Screen.setFont(monoXL);
  Brain.Screen.setFillColor(transparent);
  Brain.Screen.setCursor(1, 1);
  Brain.Screen.print(auton::autons[autonPos].first);
}
void AutonSelector::listener() {
  if (Brain.Screen.xPosition() < 240) {
    // auton
    autonPos = (autonPos + 1) % auton::autons.size();
  } else {
    // team
    Robot::team =
        Robot::TEAM::RED == Robot::team ? Robot::TEAM::BLUE : Robot::TEAM::RED;
  }
  print();
}
const void AutonSelector::start(
/*     const std::vector<std::pair<char *, std::function<void(void)>>> autonArr1 */) {
  // autonArr1[0]();
  // autonArr = autonArr1;
  if (initialized)
    return;
  initialized = 1;
  Brain.Screen.pressed(AutonSelector::listener);
  print();
  Competition.autonomous(run);
};
void AutonSelector::run() {
  Brain.Screen.setFont(monoL);
  Brain.Screen.setFillColor(transparent);
  auton::autons[autonPos].second();
};
int AutonSelector::autonPos = 0;
bool AutonSelector::initialized = false;
