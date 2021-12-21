/*********************************************************************
 *  ROSArduinoBridge 主程序
    核心功能实现，程序入口
 
    可以通过一组简单的串口命令来控制差分机器人并接收回传的传感器与里程计
    数据，默认使用的是 Arduino Mega + Pololu电机驱动模块，如果使用其他的
    编码器或电机驱动需要重写readEncoder()与setMotorSpeed()函数

    Created for the Pi Robot Project: http://www.pirobot.org
    and the Home Brew Robotics Club (HBRC): http://hbrobotics.org
    
    Authors: Patrick Goebel, James Nugen

    Inspired and modeled after the ArbotiX driver by Michael Ferguson
    
    Software License Agreement (BSD License)

    Copyright (c) 2012, Patrick Goebel.
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions
    are met:

     * Redistributions of source code must retain the above copyright
       notice, this list of conditions and the following disclaimer.
     * Redistributions in binary form must reproduce the above
       copyright notice, this list of conditions and the following
       disclaimer in the documentation and/or other materials provided
       with the distribution.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
    "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
    LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
    FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
    COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
    INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
    BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
    CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
    LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
    ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 *********************************************************************/

/* 0.常量/变量初始化 */

//是否启用底盘控制器
#define USE_BASE      // Enable the base controller code
//#undef USE_BASE     // Disable the base controller code

/* Define the motor controller and encoder library you are using */
//启用基座控制器需要设置的电机驱动以及编码器驱动
#ifdef USE_BASE
   /* The Pololu VNH5019 dual motor driver shield */
   //#define POLOLU_VNH5019

   /* The Pololu MC33926 dual motor driver shield */
   //#define POLOLU_MC33926

   /* The RoboGaia encoder shield */
   //#define ROBOGAIA
   
   /* 电机编码器信号处理 Encoders directly attached to Arduino board */
   #define ARDUINO_ENC_COUNTER

   /* L298N电机驱动版 Motor driver*/
   //#define L298_MOTOR_DRIVER
   
   /* L298P电机驱动版 Motor driver*/
   #define L298P_MOTOR_DRIVER
#endif

//是否启用舵机
//#define USE_SERVOS  // Enable use of PWM servos as defined in servos.h
#undef USE_SERVOS     // Disable use of PWM servos

/* Serial port baud rate */
//波特率
#define BAUDRATE     57600

/* Maximum PWM signal */
//最大PWM值
#define MAX_PWM        255

//根据Arduino型号来包含对应的头文件
#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

/* Include definition of serial commands */
//串口命令
#include "commands.h"

/* Sensor functions */
//传感器文件
#include "sensors.h"

/* Include servo support if required */
//如果启用舵机，需要包含的头文件
#ifdef USE_SERVOS
   #include <Servo.h>
   #include "servos.h"
#endif

//如果启用基座控制器需要包含的头文件
#ifdef USE_BASE
  /* Motor driver function definitions */
  #include "motor_driver.h"     //电机驱动

  /* Encoder driver function definitions */
  #include "encoder_driver.h"   //编码器驱动

  /* PID parameters and functions */
  #include "diff_controller.h"  //PID调速

  /* Run the PID loop at 30 times per second */
  #define PID_RATE           30     // Hz 调速频率

  /* Convert the rate into an interval */
  const int PID_INTERVAL = 1000 / PID_RATE; //调速周期
  
  /* Track the next time we make a PID calculation */
  unsigned long nextPID = PID_INTERVAL;

  /* Stop the robot if it hasn't received a movement command
   in this number of milliseconds */
  #define AUTO_STOP_INTERVAL 200   //2000 move命令持续执行时间(可按需修改)
  long lastMotorCommand = AUTO_STOP_INTERVAL;
#endif

// A pair of varibles to help parse serial commands (thanks Fergs)
int arg = 0;
int index = 0;

// Variable to hold an input character
char chr;

// Variable to hold the current single-character command
char cmd;

// Character arrays to hold the first and second arguments
char argv1[16];
char argv2[16];

// The arguments converted to integers
long arg1;
long arg2;




/* Setup function--runs once at startup. */
void setup() {
  Serial.begin(BAUDRATE);

  // Initialize the motor controller if used */
  #ifdef USE_BASE
    #ifdef ARDUINO_ENC_COUNTER
        //set as inputs
        DDRD &= ~(1<<LEFT_ENC_PIN_A);
        DDRD &= ~(1<<LEFT_ENC_PIN_B);
        DDRC &= ~(1<<RIGHT_ENC_PIN_A);
        DDRC &= ~(1<<RIGHT_ENC_PIN_B);
        
        //enable pull up resistors
        PORTD |= (1<<LEFT_ENC_PIN_A);
        PORTD |= (1<<LEFT_ENC_PIN_B);
        PORTC |= (1<<RIGHT_ENC_PIN_A);
        PORTC |= (1<<RIGHT_ENC_PIN_B);
        
        // tell pin change mask to listen to left encoder pins
        PCMSK2 |= (1 << LEFT_ENC_PIN_A)|(1 << LEFT_ENC_PIN_B);
        // tell pin change mask to listen to right encoder pins
        PCMSK1 |= (1 << RIGHT_ENC_PIN_A)|(1 << RIGHT_ENC_PIN_B);
        
        // enable PCINT1 and PCINT2 interrupt in the general interrupt mask
        PCICR |= (1 << PCIE1) | (1 << PCIE2);
    #endif

    //初始化电机控制
    initMotorController();
    //重置 PID
    resetPID();
  #endif

  /* Attach servos if used */
  #ifdef USE_SERVOS
    int i;
    for (i = 0; i < N_SERVOS; i++) {
        servos[i].initServo(
            servoPins[i],
            stepDelay[i],
            servoInitPosition[i]);
    }
  #endif
}


/* Enter the main loop.  Read and parse input from the serial port
   and run any valid commands. Run a PID calculation at the target
   interval and check for auto-stop conditions.
*/
void loop() {
  //读取串口命令
  while (Serial.available() > 0) {
    // Read the next character
    chr = Serial.read();

    // Terminate a command with a CR
    if (chr == 13) {
      if (arg == 1) argv1[index] = NULL;
      else if (arg == 2) argv2[index] = NULL;
      runCommand();
      resetCommand();
    }else if (chr == ' ') {  // Use spaces to delimit parts of the command
      // Step through the arguments
      if (arg == 0) arg = 1;
      else if (arg == 1)  {
        argv1[index] = NULL;
        arg = 2;
        index = 0;
      }
      continue;
    }else {
      if (arg == 0) {
        // The first arg is the single-letter command
        cmd = chr;
      }else if (arg == 1) {
        // Subsequent arguments can be more than one character
        argv1[index] = chr;
        index++;
      }else if (arg == 2) {
        argv2[index] = chr;
        index++;
      }
    }
  }

  // If we are using base control, run a PID calculation at the appropriate intervals
  // 启动底盘控制时，执行PID控制
  #ifdef USE_BASE
    if (millis() > nextPID) {
        // 读取编码器值->计算PID->设置电机PWM
        updatePID();
        nextPID += PID_INTERVAL;
    }
    
    // Check to see if we have exceeded the auto-stop interval
    // 电机指令自动停止
    if ((millis() - lastMotorCommand) > AUTO_STOP_INTERVAL) {
        setMotorSpeeds(0, 0);
        moving = 0;
    }
  #endif

  // Sweep servos
  // 扫描伺候
  #ifdef USE_SERVOS
    int i;
    for (i = 0; i < N_SERVOS; i++) {
        servos[i].doSweep();
    }
  #endif
}




/* Clear the current command parameters */
// 重置命令
void resetCommand() {
  cmd = NULL;
  memset(argv1, 0, sizeof(argv1));
  memset(argv2, 0, sizeof(argv2));
  arg1 = 0;
  arg2 = 0;
  arg = 0;
  index = 0;
}

/* Run a command.  Commands are defined in commands.h */
// 执行串口命令
int runCommand() {
  int i = 0;
  char *p = argv1;
  char *str;
  int pid_args[4];
  arg1 = atoi(argv1);
  arg2 = atoi(argv2);
  
  switch(cmd) {
    case GET_BAUDRATE:
        Serial.println(BAUDRATE);
        break;
    case ANALOG_READ:
        Serial.println(analogRead(arg1));
        break;
    case DIGITAL_READ:
        Serial.println(digitalRead(arg1));
        break;
    case ANALOG_WRITE:
        analogWrite(arg1, arg2);
        Serial.println("OK"); 
        break;
    case DIGITAL_WRITE:
        if (arg2 == 0) digitalWrite(arg1, LOW);
        else if (arg2 == 1) digitalWrite(arg1, HIGH);
        Serial.println("OK"); 
        break;
    case PIN_MODE:
        if (arg2 == 0) pinMode(arg1, INPUT);
        else if (arg2 == 1) pinMode(arg1, OUTPUT);
        Serial.println("OK");
        break;
    case PING:
        Serial.println(Ping(arg1));
        break;

    #ifdef USE_SERVOS
    case SERVO_WRITE:
        servos[arg1].setTargetPosition(arg2);
        Serial.println("OK");
        break;
    case SERVO_READ:
        Serial.println(servos[arg1].getServo().read());
        break;
    #endif
        
    #ifdef USE_BASE
    case READ_ENCODERS:
        Serial.print(readEncoder(LEFT));
        Serial.print(" ");
        Serial.println(readEncoder(RIGHT));
        break;
    case RESET_ENCODERS:
        resetEncoders();
        resetPID();
        Serial.println("OK");
        break;
    case MOTOR_SPEEDS:
        /* Reset the auto stop timer */
        lastMotorCommand = millis();
        if (arg1 == 0 && arg2 == 0) {
            setMotorSpeeds(0, 0);
            resetPID();
            moving = 0;
        }
        else moving = 1;
        leftPID.TargetTicksPerFrame = arg1;
        rightPID.TargetTicksPerFrame = arg2;
        Serial.println("OK"); 
        break;
    case UPDATE_PID:
        while ((str = strtok_r(p, ":", &p)) != '\0') {
            pid_args[i] = atoi(str);
            i++;
        }
        Kp = pid_args[0];
        Kd = pid_args[1];
        Ki = pid_args[2];
        Ko = pid_args[3];
        Serial.println("OK");
        break;
    #endif

    default:
        Serial.println("Invalid Command");
        break;
  }
}
