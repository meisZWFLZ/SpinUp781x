#include "vex.h"

using namespace vex;
using signature = vision::signature;
using code = vision::code;

// A global instance of brain used for printing to the V5 Brain screen
brain  Brain;

// VEXcode device constructors
controller Controller1 = controller(primary);
inertial Inertial10 = inertial(PORT15);
limit CatapultLimitSwitch = limit(Brain.ThreeWirePort.A);
digital_out ExpansionPiston = digital_out(Brain.ThreeWirePort.H);
motor LeftDriveA = motor(PORT12, ratio36_1, true);
motor LeftDriveB = motor(PORT10, ratio6_1, true);
motor LeftDriveC = motor(PORT11, ratio6_1, true);
motor RightDriveA = motor(PORT19, ratio6_1, false);
motor RightDriveB = motor(PORT18, ratio18_1, false);
motor RightDriveC = motor(PORT20, ratio6_1, false);
rotation LeftDriveR = rotation(PORT1, false);
rotation RightDriveR = rotation(PORT2, false);
rotation HoriR = rotation(PORT3, false);
optical RollerSensor = optical(PORT13);
digital_out DiscLock1 = digital_out(Brain.ThreeWirePort.B);
digital_out CataAngler = digital_out(Brain.ThreeWirePort.E);
motor IntakeMotorA = motor(PORT4, ratio18_1, false);
motor IntakeMotorB = motor(PORT5, ratio18_1, true);
motor_group Intake = motor_group(IntakeMotorA, IntakeMotorB);

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