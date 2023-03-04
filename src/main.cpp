#include "auton/action.h"
#include "auton/elements.h"
#include "auton/playback.h"
#include "auton/record.h"
#include "auton/recordPaths.h"
#include "auton/selector.h"
#include "conversions.h"
#include "odom/tracking.h"
#include "pid_controller.h"
#include "position.h"
#include "vex.h"
#include <atomic>
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <thread>
#include <vector>

// #include <nlohmann/json.hpp>

// intake failsafe (disables limit switch check)
// 0 = safe
// 1 = danger (limit switch check disabled)
bool failsafe = 0;

// for reversing drive
int driveCoefficient = 1;

int getDriveCoefficient() { return driveCoefficient; };

/** intake failsafe (disables limit switch check)
 @param failsafe 0 = safe
 @param failsafe 1 = danger (limit switch check disabled) */
bool getFailSafe() { return failsafe; }

// drivetrain
void leftDriveSubscriber() {
  Robot::Drivetrain::left(driveCoefficient *
                          pow((float)Controller1.Axis3.position() / 100, 5));
}
void rightDriveSubscriber() {
  Robot::Drivetrain::right(driveCoefficient *
                           pow((float)Controller1.Axis2.position() / 100, 5));
}
// shoot
void shootListener() { Robot::Actions::shoot((Robot::GOAL)(int)Robot::team); }
// void angleshootListener() {
//   Robot::Actions::AngleShoot((Robot::GOAL)(int)Robot::team);
// }
void motorSetup() {
  // retract expansion
  ExpansionPiston.set(false);

  // disc lock
  // Robot::DiscLock::lock();

  // cata
  Intake.setStopping(coast);
  Intake.setVelocity(100, pct);

  // drive groups
  Robot::Drivetrain::Left = new motor_group(LeftDriveA, LeftDriveB, LeftDriveC);
  Robot::Drivetrain::Right =
      new motor_group(RightDriveA, RightDriveB, RightDriveC);

  // set drive to coast
  Robot::Drivetrain::Left->setStopping(coast);
  Robot::Drivetrain::Right->setStopping(coast);

  // optical light
  RollerSensor.setLightPower(100, pct);
}

void expansionCheck() {
  // checks that all letter buttons are pressed
  if (Controller1.ButtonB.pressing() && Controller1.ButtonLeft.pressing())
    Robot::InputListeners::expand();
};

// declaration
void controllerDisplay();

void driverControl() {
  // thread([] { controllerDisplay(); });

  // drivetrain
  // Controller1.Axis2.changed(&rightDriveSubscriber);
  // Controller1.Axis3.changed(&leftDriveSubscriber);
  Controller1.Axis2.changed([]() {
    Robot::InputListeners::rightDrive(Controller1.Axis2.position());
  });
  Controller1.Axis3.changed(
      []() { Robot::InputListeners::leftDrive(Controller1.Axis3.position()); });

  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~
  //    START CATA TEST
  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~

  // disc lock
  // Controller1.ButtonLeft.pressed([]() {
  //   static bool state = false;
  //   state = !state;
  //   DiscLock1.set(state);
  // });

  // // angler
  // Controller1.ButtonRight.pressed([]() {
  //   static bool state = false;
  //   state = !state;
  //   CataAngler.set(state);
  // });

  // cata
  Controller1.ButtonUp.pressed([]() {
    Intake.spin(fwd, 100, pct);
    while (Controller1.ButtonUp.pressing()) {
      wait(10, msec);
    }
    Intake.stop();
  });
  Controller1.ButtonDown.pressed([]() {
    Intake.spin(reverse, 100, pct);
    while (Controller1.ButtonDown.pressing()) {
      wait(10, msec);
    }
    Intake.stop();
  });

  // intake failsafe
  // WARNING: might cause expansion not to fire (api may only call first
  // listener)

  Controller1.ButtonX.pressed([]() { failsafe = !failsafe; });

  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~
  //    END CATA TEST
  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~

  // R1 -> intake in (toggle)
  // R2 -> intake out
  // L1 -> shoot
  // R2 L2 -> flywheel toggle
  // L2 -> roller
  // a b -> expansion

  // shoot pressed
  Controller1.ButtonL1.pressed(Robot::InputListeners::shoot);
  // Controller1.ButtonL1.pressed(shootListener);

  // intake toggle
  Controller1.ButtonR1.pressed(Robot::Actions::intake);
  Controller1.ButtonR1.released(Robot::InputListeners::intake);

  // Controller1.ButtonR1.pressed([]() {
  //   if (Intake.voltage(volt) > 0.1)
  //     Robot::Actions::stopIntake();
  //   else
  //     Robot::Actions::intake();
  // });

  // outtake toggle
  // Controller1.ButtonR2.pressed(Robot::InputListeners::outake);
  // Controller1.ButtonR2.pressed([]() {
  //   if (Intake.voltage(volt) < -0.1)
  //     Robot::Actions::stopIntake();
  //   else
  //     Robot::Actions::outtake();
  // });

  // change direction
  // Controller1.ButtonB.pressed([]() { driveCoefficent *= -1; });

  // roller
  // Controller1.ButtonL2.pressed(Robot::InputListeners::roller);
  Controller1.ButtonR2.pressed(Robot::InputListeners::roller);
  // Controller1.ButtonL2.pressed([] { Robot::Actions::roller(); });

  Controller1.ButtonL2.pressed(
      []() { Robot::Actions::pistonShoot(Robot::GOAL::MY_TEAM); });
  // expansion
  Controller1.ButtonA.pressed([] {
    Robot::Catapult::retract();
    // Robot::DiscLock::lock();
  });
  // Controller1.ButtonX.pressed(expansionCheck);
  Controller1.ButtonLeft.pressed(expansionCheck);
  Controller1.ButtonB.pressed(expansionCheck);

  // while (1) {
  //   if (!CatapultLimitSwitch.pressing() && !getFailSafe()) {
  //     Robot::Catapult::retract();
  //     Robot::DiscLock::lock();
  //     // Catapult1.spin(fwd);
  //     // while (!CatapultLimitSwitch.pressing())
  //     //   wait(50, msec);
  //     // Catapult1.stop();
  //   }
  //   wait(50, msec);
  // }
}

void preAuton() { AutonSelector::start(); }
void record() {
  static int index = 0;

  Controller1.ButtonLeft.pressed([] {
    printf("left\n");
    --index %= RecordingPaths::get().size() + 1;
  });
  Controller1.ButtonRight.pressed([] {
    printf("right\n");
    ++index %= RecordingPaths::get().size() + 1;
  });
  // static bool startDriver;
  // Controller1.ButtonX.pressed([] {
  //   printf("x\n");

  //   // ++index %= RecordingPaths::get().size() + 1;
  // });
  // bool stopped = false;

  bool started = false;
  bool auton1 = false;
  auton::Player *player = nullptr;

  for (auto item : RecordingPaths::get())
    printf("%s\n", item.c_str());

  while (1) {
    Controller1.Screen.setCursor(1, 1);
    Controller1.Screen.clearLine();
    if (RecordingPaths::get().size() != index)
      Controller1.Screen.print(RecordingPaths::get()[index].c_str());
    else
      Controller1.Screen.print("new");
    if (!started && Competition.isEnabled()) {
      // printf("starting");
      if (Controller1.ButtonX.pressing()) {
        RecordingPaths::remove(index);
        index %= RecordingPaths::get().size() + 1;
        continue;
      }

      if (Competition.isAutonomous()) {
        printf("starting player\n");
        if (!RecordingPaths::get().empty()) {
          player = new auton::Player(RecordingPaths::get()[index].c_str());
          player->start();
          auton1 = true;
          started = true;
        } else
          printf("paths empty\n");
      } else if (/* Competition.isDriverControl() */ Controller1.ButtonY
                     .pressing()) {
        printf("starting recorder\n");
        auton::Recorder::start();
        auton1 = false;
        started = true;
      }

      // printf("starting recorder\n");
    }
    if (started && !Competition.isEnabled()) {
      printf("stopping\n");
      if (auton1) {
        printf(" player\n");
        player->stop();
        // delete &player;
        player = nullptr;
      } else {
        printf(" recorder\n");
        auton::Recorder::stop();
        if (RecordingPaths::get().size() != index)
          auton::Recorder::save(RecordingPaths::get()[index].c_str());
        else
          auton::Recorder::saveNew();
      }
      started = false;
      // auton::Recorder::stop();
      // auton::Recorder::save(path);
    }
    wait(100, msec);
  }
}
// void playback(char *path) {
//   bool started = false;
//   bool stopped = false;
//   auton::Player::load(path);
//   while (1) {
//     if (!started && !stopped && Competition.isEnabled()) {
//       printf("starting player\n");
//       printf("started: &i\n", started);
//       started = true;
//       auton::Player::start();
//     }
//     if (!stopped && started && !Competition.isEnabled()) {
//       printf("stopping player\n");
//       stopped = true;
//       auton::Player::stop();
//     }
//     wait(100, msec);
//   }
// }

int main() {
  vexcodeInit();
  // return 1;

  // std::ifstream myfile;
  // myfile.open("a.txt");
  // std::string myline;
  // if (myfile.is_open()) {
  //   while (myfile) { // equivalent to myfile.good()
  //     std::getline(myfile, myline);
  //     printf("%s\n", myline.c_str());
  //   }
  // } else {
  //   printf("Couldn't open file\n");
  // }
  // return 0;

  motorSetup();

  Robot::team = Robot::TEAM::RED;

  Inertial10.calibrate(2);
  // wait until inertial finished calibrating
  while (Inertial10.isCalibrating())
    wait(20, msec);

  OdomTracking tracker1 = {{}};

  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~
  //    START COMPETITION
  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~
  Competition.drivercontrol(driverControl);
  // record();

  preAuton();

  thread give_me_a_name([] { controllerDisplay(); });

  while (1) {
    wait(1000, msec);
  }
  return 1;

  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~
  //    END COMPETITION
  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~
}

void controllerDisplay() {
  Brain.Screen.clearScreen();
  while (1) {
    // reset
    Controller1.Screen.setCursor(0, 0);

    // drive temp
    // std::stringstream sstm1;
    // sstm1 << std::string("drive: ")
    //       << (Robot::Drivetrain::Left->temperature(fahrenheit) +
    //           Robot::Drivetrain::Right->temperature(fahrenheit)) /
    //              2
    //       << std::string(" F");
    Controller1.Screen.clearLine();
    Controller1.Screen.print("drive: ");
    Controller1.Screen.print(
        (Robot::Drivetrain::Left->temperature(fahrenheit) +
         Robot::Drivetrain::Right->temperature(fahrenheit)) /
        2);
    Controller1.Screen.print(" F");
    Controller1.Screen.newLine();

    // cata temp
    // std::stringstream sstm2;
    // sstm2 << std::string("cata: ") << Catapult1.temperature(fahrenheit)
    //       << std::string(" F");
    Controller1.Screen.clearLine();
    Controller1.Screen.print("cata/in: ");
    Controller1.Screen.print(Intake./*power(watt) */ temperature(fahrenheit));
    Controller1.Screen.print(" F");
    Controller1.Screen.newLine();

    // intake failsafe
    Controller1.Screen.clearLine();
    Controller1.Screen.print(getFailSafe() ? "INTAKE: DANGER"
                                           : "INTAKE: SAFE  ");
    Controller1.Screen.newLine();

    // POSITION
    Position robotPos1 = Robot::getPosition();
    Brain.Screen.setCursor(1, 1);
    Brain.Screen.clearLine();
    // Brain.Screen.print((IntakeMotorA.power(watt)+IntakeMotorB.power(watt))/2);
    Brain.Screen.print("(");
    Brain.Screen.print(robotPos1.x);
    Brain.Screen.print(",");
    Brain.Screen.print(robotPos1.y);
    Brain.Screen.print(",");
    Brain.Screen.print(robotPos1.heading);
    Brain.Screen.print(")");
    Brain.Screen.newLine();

    // Controller1.Screen.clearLine();
    // Controller1.Screen.print("(");
    // Controller1.Screen.print(robotPos1.x);
    // Controller1.Screen.print(",");
    // Controller1.Screen.print(robotPos1.y);
    // Controller1.Screen.print(",");
    // Controller1.Screen.print(robotPos1.heading);
    // Controller1.Screen.print(")");
    // Controller1.Screen.newLine();
    printf("nail.com\n");
    wait(50, msec);
  }
}