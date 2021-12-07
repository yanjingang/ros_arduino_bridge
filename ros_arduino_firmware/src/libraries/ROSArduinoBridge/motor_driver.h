/***************************************************************
   马达驱动的接口定义，用不动的马达驱动板都要实现此文件定义的三个函数
   Motor driver function definitions - by James Nugen
   *************************************************************/

#ifdef L298_MOTOR_DRIVER
  #define RIGHT_MOTOR_BACKWARD 5  //
  #define RIGHT_MOTOR_FORWARD  6  // 
  #define LEFT_MOTOR_BACKWARD  9  //
  #define LEFT_MOTOR_FORWARD   10 //
  #define RIGHT_MOTOR_ENABLE 12   //
  #define LEFT_MOTOR_ENABLE 13    //
#endif

void initMotorController();
void setMotorSpeed(int i, int spd);
void setMotorSpeeds(int leftSpeed, int rightSpeed);
