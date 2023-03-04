#include "vex.h"

using namespace vex;
using signature = vision::signature;
using code = vision::code;

// A global instance of brain used for printing to the V5 Brain screen
brain Brain;

// VEXcode device constructors
controller Controller1 = controller(primary);
inertial Inertial10 = inertial(PORT21);
limit CatapultLimitSwitch = limit(Brain.ThreeWirePort.H);
digital_out ExpansionPiston = digital_out(Brain.ThreeWirePort.F);
motor LeftDriveA = motor(PORT13, ratio36_1, true);
motor LeftDriveB = motor(PORT11, ratio6_1, true);
motor LeftDriveC = motor(PORT12, ratio6_1, false);
motor RightDriveA = motor(PORT18, ratio6_1, false);
motor RightDriveB = motor(PORT20, ratio18_1, false);
motor RightDriveC = motor(PORT19, ratio6_1, true);
rotation LeftDriveR = rotation(PORT4, true);
rotation HoriR = rotation(PORT5, false);
rotation RightDriveR = rotation(PORT6, false);
optical RollerSensor = optical(PORT8);
digital_out DiscLock1 = digital_out(Brain.ThreeWirePort.B);
digital_out PistonBooster = digital_out(Brain.ThreeWirePort.E);
motor IntakeMotorA = motor(PORT2, ratio18_1, false);
motor IntakeMotorB = motor(PORT3, ratio18_1, true);
motor_group Intake = motor_group(IntakeMotorA, IntakeMotorB);

// VEXcode generated functions
// define variable for remote controller enable/disable
bool RemoteControlCodeEnabled = true;

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 *
 * This should be called at the start of your int main function.
 */
void vexcodeInit(void) {
  // nothing to initialize
}