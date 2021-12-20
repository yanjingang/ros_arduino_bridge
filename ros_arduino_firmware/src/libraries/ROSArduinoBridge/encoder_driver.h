/* *************************************************************
   编码器驱动头文件
   这里只是针对了Arduino UNO，使用了中断接口D2,D3,和模拟接口A4，A5；所以电机编码器的输出信号线需要按照此规则接线，另外要注意编码器要有两路输出
   左侧电机的编码输出接D2,D3；右侧电机的编码输出接A4,A5
   Encoder driver function definitions - by James Nugen
   ************************************************************ */
   
   
#ifdef ARDUINO_ENC_COUNTER
  //below can be changed, but should be PORTD pins; 
  //otherwise additional changes in the code are required
  // 左轮电机编码器信号读引脚（电机编码信号线直连arduino读引脚）
  #define LEFT_ENC_PIN_A PC4  //pin A4
  #define LEFT_ENC_PIN_B PC5   //pin A5
  
  //below can be changed, but should be PORTC pins
  // 右轮电机编码器信号读引脚（电机编码信号线直连arduino读引脚）
  #define RIGHT_ENC_PIN_A PD2  //pin 2
  #define RIGHT_ENC_PIN_B PD3  //pin 3
#endif
   
long readEncoder(int i);
void resetEncoder(int i);
void resetEncoders();
