using namespace vex;

extern brain Brain;

// VEXcode devices
extern controller Controller1;
extern motor DiscLoader;
extern motor PTOLeft;
extern motor PTORight;
extern digital_out PTOPiston;
extern motor LeftDriveA;
extern motor LeftDriveB;
extern motor RightDriveA;
extern motor RightDriveB;
extern motor FlyWheel;

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 * 
 * This should be called at the start of your int main function.
 */
void  vexcodeInit( void );