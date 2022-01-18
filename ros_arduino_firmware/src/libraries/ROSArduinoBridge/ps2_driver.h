/**
 * @file ps2.h
 * @author yanjingang@mail.com
 * @brief ps2蓝牙手柄控制初始化
 * @version 0.1
 * @date 2022-01-16
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <PS2X_lib.h>     //ps2驱动

// 初始化手柄控制对象
PS2X ps2x;

int ps2_error = 0;
byte ps2_type = 0;
byte ps2_vibrate = 0;


//设置引脚 setup pins and settings:  GamePad(clock, command, attention,  data, Pressures?, Rumble?) 
#define PS2_CLOCK_PIN   13
#define PS2_COMMAND_PIN  11
#define PS2_ATTENTION_PIN  10
#define PS2_DATA_PIN 12
#define PS2_PRESSURES true
#define PS2_RUMBLE true
