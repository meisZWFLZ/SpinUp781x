#include "vex.h"

using namespace vex;
using signature = vision::signature;
using code = vision::code;

// A global instance of brain used for printing to the V5 Brain screen
brain  Brain;

// VEXcode device constructors
controller Controller1 = controller(primary);
inertial Inertial10 = inertial(PORT21);
limit CatapultLimitSwitch = limit(Brain.ThreeWirePort.B);
digital_out ExpansionPiston = digital_out(Brain.ThreeWirePort.A);
motor LeftDriveA = motor(PORT11, ratio6_1, true);
motor LeftDriveB = motor(PORT13, ratio6_1, true);
motor LeftDriveC = motor(PORT12, ratio6_1, false);
motor RightDriveA = motor(PORT14, ratio6_1, false);
motor RightDriveB = motor(PORT16, ratio18_1, false);
motor RightDriveC = motor(PORT15, ratio6_1, true);
motor Intake = motor(PORT4, ratio6_1, false);
motor Catapult1 = motor(PORT20, ratio18_1, false);
rotation LeftDriveR = rotation(PORT17, false);
rotation RightDriveR = rotation(PORT18, false);
rotation HoriR = rotation(PORT10, false);
optical RollerSensor = optical(PORT9);

// VEXcode generated functions
// define variable for remote controller enable/disable
bool RemoteControlCodeEnabled = true;

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 * 
 * This should be called at the start of your int main function.
 */
void vexcodeInit( void ) {
  // nothing to initialize
}