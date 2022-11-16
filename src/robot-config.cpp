#include "vex.h"

using namespace vex;
using signature = vision::signature;
using code = vision::code;

// A global instance of brain used for printing to the V5 Brain screen
brain  Brain;

// VEXcode device constructors
controller Controller1 = controller(primary);
inertial Inertial10 = inertial(PORT21);
limit CatapultLimitSwitch = limit(Brain.ThreeWirePort.G);
digital_out ExpansionPiston = digital_out(Brain.ThreeWirePort.F);
motor LeftDriveA = motor(PORT2, ratio18_1, true);
motor LeftDriveB = motor(PORT3, ratio18_1, true);
motor LeftDriveC = motor(PORT4, ratio18_1, false);
motor RightDriveA = motor(PORT12, ratio18_1, false);
motor RightDriveB = motor(PORT13, ratio18_1, false);
motor RightDriveC = motor(PORT14, ratio18_1, true);
motor Intake = motor(PORT19, ratio18_1, false);
motor Catapult1 = motor(PORT20, ratio18_1, false);
rotation LeftDriveR = rotation(PORT5, false);
rotation RightDriveR = rotation(PORT15, false);
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