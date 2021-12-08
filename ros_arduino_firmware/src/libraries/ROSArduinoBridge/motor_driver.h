/***************************************************************
   马达驱动的接口定义，用不动的马达驱动板都要实现此文件定义的三个函数
   Motor driver function definitions - by James Nugen
   *************************************************************/

#ifdef L298_MOTOR_DRIVER
  // 右轮电机驱动写引脚（接L298电驱板input口）
  #define LEFT_MOTOR_BACKWARD  9
  #define LEFT_MOTOR_FORWARD   10
  
  // 右轮电机驱动写引脚（接L298电驱板input口）
  #define RIGHT_MOTOR_BACKWARD 5
  #define RIGHT_MOTOR_FORWARD  6

  // 电机开关（目前没用，可以不接）
  #define RIGHT_MOTOR_ENABLE 12
  #define LEFT_MOTOR_ENABLE 13
#endif

void initMotorController();
void setMotorSpeed(int i, int spd);
void setMotorSpeeds(int leftSpeed, int rightSpeed);
