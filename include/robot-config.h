using namespace vex;

extern brain Brain;

using signature = vision::signature;

// VEXcode devices
extern controller Controller1;
extern motor PTOLeft;
extern motor PTORight;
extern digital_out PTOPiston;
extern motor LeftDriveA;
extern motor LeftDriveB;
extern motor RightDriveA;
extern motor RightDriveB;
extern motor_group FlyWheel;
extern encoder HoriEncoder;
extern encoder VertEncoder;
extern inertial Inertial10;
extern signature VisionSensor__RED_ROLLER;
extern signature VisionSensor__BLUE_ROLLER;
extern signature VisionSensor__SIG_3;
extern signature VisionSensor__SIG_4;
extern signature VisionSensor__SIG_5;
extern signature VisionSensor__SIG_6;
extern signature VisionSensor__SIG_7;
extern vision VisionSensor;
extern motor_group Catapult1;
extern limit CatapultLimitSwitch;
extern digital_out ExpansionPiston;

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 * 
 * This should be called at the start of your int main function.
 */
void  vexcodeInit( void );