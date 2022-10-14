#include "vex.h"

using namespace vex;
using signature = vision::signature;
using code = vision::code;

// A global instance of brain used for printing to the V5 Brain screen
brain  Brain;

// VEXcode device constructors
controller Controller1 = controller(primary);
motor PTOLeft = motor(PORT2, ratio6_1, false);
motor PTORight = motor(PORT3, ratio6_1, true);
digital_out PTOPiston = digital_out(Brain.ThreeWirePort.H);
motor LeftDriveA = motor(PORT4, ratio6_1, false);
motor LeftDriveB = motor(PORT5, ratio6_1, false);
motor RightDriveA = motor(PORT6, ratio18_1, true);
motor RightDriveB = motor(PORT11, ratio18_1, true);
motor FlyWheelMotorA = motor(PORT8, ratio36_1, true);
motor FlyWheelMotorB = motor(PORT9, ratio36_1, false);
motor_group FlyWheel = motor_group(FlyWheelMotorA, FlyWheelMotorB);
encoder HoriEncoder = encoder(Brain.ThreeWirePort.A);
encoder VertEncoder = encoder(Brain.ThreeWirePort.C);
inertial Inertial10 = inertial(PORT1);
/*vex-vision-config:begin*/
signature VisionSensor__RED_ROLLER = signature (1, 7467, 9637, 8552, -1203, 115, -544, 5, 0);
signature VisionSensor__BLUE_ROLLER = signature (2, -3591, -2439, -3015, 7859, 13779, 10819, 1.3, 0);
vision VisionSensor = vision (PORT12, 50, VisionSensor__RED_ROLLER, VisionSensor__BLUE_ROLLER);
/*vex-vision-config:end*/

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