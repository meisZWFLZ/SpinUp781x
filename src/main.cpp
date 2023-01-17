#include "auton/action.h"
#include "auton/elements.h"
#include "auton/record.h"
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

// intake failsafe (disables limit switch check)
// 0 = safe
// 1 = danger (limit switch check disabled)
bool failsafe = 0;

// for reversing drive
int driveCoefficent = 1;

int getDriveCoefficent() { return driveCoefficent; };
bool getFailSafe() { return failsafe; }

// drivetrain
void leftDriveSubscriber() {
  Robot::Drivetrain::left(driveCoefficent *
                          pow((float)Controller1.Axis3.position() / 100, 5));
}
void rightDriveSubscriber() {
  Robot::Drivetrain::right(driveCoefficent *
                           pow((float)Controller1.Axis2.position() / 100, 5));
}
// shoot
void shootListener() { Robot::Actions::shoot((Robot::GOAL)(int)Robot::team); }
void angleshootListener() {
  Robot::Actions::AngleShoot((Robot::GOAL)(int)Robot::team);
}
void motorSetup() {
  // retract expansion
  ExpansionPiston.set(false);

  // disc lock
  Robot::DiscLock::lock();

  // cata
  Catapult1.setStopping(coast);
  Catapult1.setVelocity(100, pct);

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
  if (Controller1.ButtonA.pressing() && Controller1.ButtonB.pressing() &&
      Controller1.ButtonX.pressing() && Controller1.ButtonY.pressing())
    Robot::Actions::expand();
};

// declaration
void controllerDisplay();

void driverControl() {
  thread([] { controllerDisplay(); });

  // drivetrain
  Controller1.Axis2.changed(&rightDriveSubscriber);
  Controller1.Axis3.changed(&leftDriveSubscriber);

  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~
  //    START CATA TEST
  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~

  // disc lock
  Controller1.ButtonLeft.pressed([]() {
    static bool state = false;
    state = !state;
    DiscLock1.set(state);
  });

  // angler
  Controller1.ButtonRight.pressed([]() {
    static bool state = false;
    state = !state;
    CataAngler.set(state);
  });

  // cata
  Controller1.ButtonUp.pressed([]() {
    Catapult1.spin(fwd, 100, pct);
    while (Controller1.ButtonUp.pressing()) {
      wait(10, msec);
    }
    Catapult1.stop();
  });
  Controller1.ButtonDown.pressed([]() {
    Catapult1.spin(reverse, 100, pct);
    while (Controller1.ButtonDown.pressing()) {
      wait(10, msec);
    }
    Catapult1.stop();
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
  Controller1.ButtonL1.pressed(shootListener);
  // intake toggle
  Controller1.ButtonR1.pressed([]() {
    if (Intake.voltage(volt) > 0.1)
      Robot::Actions::stopIntake();
    else
      Robot::Actions::intake();
  });

  // outtake toggle
  Controller1.ButtonR2.pressed([]() {
    if (Intake.voltage(volt) < -0.1)
      Robot::Actions::stopIntake();
    else
      Robot::Actions::outtake();
  });

  // change direction
  Controller1.ButtonB.pressed([]() { driveCoefficent *= -1; });

  // roller
  Controller1.ButtonL2.pressed([] { Robot::Actions::roller(); });

  // expansion
  Controller1.ButtonA.pressed(expansionCheck);
  Controller1.ButtonX.pressed(expansionCheck);
  Controller1.ButtonY.pressed(expansionCheck);
}

void preAuton() { AutonSelector::start(); }

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
  // wait until inertial finsished calibrating
  while (Inertial10.isCalibrating())
    wait(20, msec);

  OdomTracking tracker1 = {{}};

  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~
  //    START COMPETITION
  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~

  Competition.drivercontrol(driverControl);
  preAuton();

  // while (1) {
  //   wait(1000, msec);
  // }

  bool started = false;
  bool stopped = false;
  while (1) {
    if (!started && !stopped && Competition.isEnabled()) {
      printf("starting recorder\n");
      started = true;
      auton::Recorder::start();
    }
    if (!stopped && started && !Competition.isEnabled()) {
      printf("stopping recorder\n");
      stopped = true;
      auton::Recorder::stop();
      auton::Recorder::save("a.txt");
    }
    wait(100, msec);
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
    Controller1.Screen.print("cata: ");
    Controller1.Screen.print(Catapult1.temperature(fahrenheit));
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

    wait(50, msec);
  }
}