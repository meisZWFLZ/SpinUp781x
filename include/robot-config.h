using namespace vex;

extern brain Brain;

// VEXcode devices
extern controller Controller1;
extern inertial Inertial10;
extern limit CatapultLimitSwitch;
extern digital_out ExpansionPiston;
extern motor LeftDriveA;
extern motor LeftDriveB;
extern motor LeftDriveC;
extern motor RightDriveA;
extern motor RightDriveB;
extern motor RightDriveC;
extern motor Intake;
extern motor Catapult1;
extern rotation LeftDriveR;
extern rotation RightDriveR;
extern rotation HoriR;
extern optical RollerSensor;

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 * 
 * This should be called at the start of your int main function.
 */
void  vexcodeInit( void );