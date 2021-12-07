/* 
    串口命令预定义
    Define single-letter commands that will be sent by the PC over the serial link.
*/

#ifndef COMMANDS_H
#define COMMANDS_H

#define ANALOG_READ    'a'  // 获取模拟引脚上的读数
#define GET_BAUDRATE   'b'  // 获取固件波特率
#define PIN_MODE       'c'  // 设置引脚模式（0 input, 1 output）
#define DIGITAL_READ   'd'  // 获取数字引脚上的读数
#define READ_ENCODERS  'e'  // 获取编码器计数
#define MOTOR_SPEEDS   'm'  // 以指定的速度移动机器人
#define PING           'p'  // 
#define RESET_ENCODERS 'r'
#define SERVO_WRITE    's'  // 设置伺服的位置 -弧度(0-3.14)
#define SERVO_READ     't'  // 读伺服的位置
#define UPDATE_PID     'u'  
#define DIGITAL_WRITE  'w'  // 向数字引脚发送低(0)或高(1)信号
#define ANALOG_WRITE   'x'  // 向模拟引脚发送模拟信号
#define LEFT            0
#define RIGHT           1

#endif
