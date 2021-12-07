概述 Overview
--------
此分支 (indigo-devel) 适用于 ROS Indigo 及更高版本，并使用 Catkin 构建系统。它也能与 ROS Hydro 兼容。
This branch (indigo-devel) is intended for ROS Indigo and above, and uses the Catkin buildsystem. It may also be compatible with ROS Hydro.

这个 ROS 库包括一个 Arduino 库（ROSArduinoBridge）和一组 ROS 包，用于使用标准 ROS 消息和服务控制基于 Arduino 的机器人。这个库不依赖于ROS Serial。
This ROS stack includes an Arduino library (called ROSArduinoBridge) and a collection of ROS packages for controlling an Arduino-based robot using standard ROS messages and services.  The stack does **not** depend on ROS Serial.

特点包括：
Features of the stack include:

* 直接支持 Ping 声纳和夏普红外 (GP2D12) 传感器
* Direct support for Ping sonar and Sharp infrared (GP2D12) sensors

* 可以从通用模拟和数字传感器读取数据
* Can also read data from generic analog and digital sensors

* 可以控制数字输出（例如打开和关闭开关或 LED）
* Can control digital outputs (e.g. turn a switch or LED on and off)

* 支持 PWM 伺服
* Support for PWM servos
* 如果使用所需硬件，可配置Base Controller
* Configurable base controller if using the required hardware

该库包括一个用于差速驱动机器人的Base Controller，该控制器接受 ROS Twist 消息并将 Odom 里程计数据发布回 PC。Base Controller需要使用电机控制器和编码器来读取里程数据。库的当前版本支持以下基本控制器硬件：
The stack includes a base controller for a differential drive
robot that accepts ROS Twist messages and publishes odometry data back to
the PC. The base controller requires the use of a motor controller and encoders for reading odometry data.  The current version of the stack provides support for the following base controller hardware:

* Pololu VNH5019 双电机控制器屏蔽 ( http://www.pololu.com/catalog/product/2502 ) 或 Pololu MC33926 双电机屏蔽 ( http://www.pololu.com/catalog/product/2503 )。
* Pololu VNH5019 dual motor controller shield (http://www.pololu.com/catalog/product/2502) or Pololu MC33926 dual motor shield (http://www.pololu.com/catalog/product/2503).

* Robogaia Mega Encoder shield ( http://www.robogaia.com/two-axis-encoder-counter-mega-shield-version-2.html ) 或板载车轮编码器计数器。
* Robogaia Mega Encoder shield
(http://www.robogaia.com/two-axis-encoder-counter-mega-shield-version-2.html) or on-board wheel encoder counters.

**注意：** Robogaia Mega Encoder shield 只能与 Arduino Mega 一起使用。板载车轮编码器计数器目前仅受 Arduino Uno 支持。
**NOTE:** The Robogaia Mega Encoder shield can only be used with an Arduino Mega. The on-board wheel encoder counters are currently only supported by Arduino Uno.

* L298电机驱动器
* L298 motor driver

* 该库可以轻松扩展以包括对其他电机控制器和编码器硬件或库的支持。
* The library can be easily extended to include support for other motor controllers and encoder hardware or libraries.


官方 ROS 文档 Official ROS Documentation
--------------------------
可以在 ROS wiki 上找到此文档的标准 ROS 样式版本：
A standard ROS-style version of this documentation can be found on the ROS wiki at:

http://www.ros.org/wiki/ros_arduino_bridge


系统要求 System Requirements
-------------------
**Python Serial:** 在Ubuntu下安装python-serial包，使用命令：
**Python Serial:** To install the python-serial package under Ubuntu, use the command:

    $ sudo apt-get install python-serial

在非 Ubuntu 系统上，使用：
On non-Ubuntu systems, use either:

    $ sudo pip install --upgrade pyserial

or

    $ sudo easy_install -U pyserial

**Arduino IDE 1.6.6 or Higher:**
请注意，条件 #include 语句的预处理在早期版本的 Arduino IDE 中被破坏。为确保 ROS Arduino Bridge 固件正确编译，请务必安装 1.6.6 或更高版本的 Arduino IDE。您可以从https://www.arduino.cc/en/Main/Software下载 IDE 。
Note that the preprocessing of conditional #include statements is broken in earlier versions of the Arduino IDE.  To ensure that the ROS Arduino Bridge firmware compiles correctly, be sure to install version 1.6.6 or higher of the Arduino IDE.  You can download the IDE from https://www.arduino.cc/en/Main/Software.

**硬件:**
**Hardware:**
固件应与任何兼容 Arduino 的控制器配合使用，以读取传感器和控制 PWM 伺服系统。但是，要使用Base Controller，您将需要上述支持的电机控制器和编码器硬件。如果您没有这个硬件，您仍然可以尝试读取传感器和控制伺服系统的软件包。有关如何执行此操作的说明，请参阅本文档末尾的 NOTES 部分。
The firmware should work with any Arduino-compatible controller for reading sensors and controlling PWM servos.  However, to use the base controller, you will need a supported motor controller and encoder hardware as described above. If you do not have this hardware, you can still try the package for reading sensors and controlling servos.  See the NOTES section at the end of this document for instructions on how to do this.

要使用Base Controller，您还必须为电机控制器和编码器安装适当的库。对于 Pololu VNH5019 双电机屏蔽，可以在以下位置找到库：
To use the base controller you must also install the appropriate libraries for your motor controller and encoders.  For the Pololu VNH5019 Dual Motor Shield, the library can be found at:

https://github.com/pololu/Dual-VNH5019-Motor-Shield

对于 Pololu MC33926 双电机屏蔽，可以在以下位置找到库：
For the Pololu MC33926 Dual Motor Shield, the library can be found at:

https://github.com/pololu/dual-mc33926-motor-shield

Robogaia Mega Encoder 库可以在以下位置找到：
The Robogaia Mega Encoder library can be found at:

http://www.robogaia.com/uploads/6/8/0/9/6809982/__megaencodercounter-1.3.tar.gz

L298 电机驱动器不需要任何库
L298 Motor Driver doesn't require any libraries

这些库应该安装在您的标准 Arduino 素描本/库目录中。
These libraries should be installed in your standard Arduino
sketchbook/libraries directory.

最后，假设您使用的是 1.0 或更高版本的 Arduino IDE。
Finally, it is assumed you are using version 1.0 or greater of the
Arduino IDE.

在 Linux 下准备你的串口 Preparing your Serial Port under Linux
--------------------------------------
您的 Arduino 可能会作为端口 /dev/ttyACM# 或 /dev/ttyUSB# 连接到您的 Linux 计算机，其中 # 是一个数字，如 0、1、2 等，具体取决于连接的其他设备的数量。做出决定的最简单方法是拔下所有其他 USB 设备，插入您的 Arduino，然后运行命令：
Your Arduino will likely connect to your Linux computer as port /dev/ttyACM# or /dev/ttyUSB# where # is a number like 0, 1, 2, etc., depending on how many other devices are connected.  The easiest way to make the determination is to unplug all other USB devices, plug in your Arduino, then run the command:

    $ ls /dev/ttyACM*

or 

    $ ls /dev/ttyUSB*

希望这两个命令之一将返回您正在寻找的结果（例如 /dev/ttyACM0），另一个将返回错误“没有这样的文件或目录”。
Hopefully, one of these two commands will return the result you're looking for (e.g. /dev/ttyACM0) and the other will return the error "No such file or directory".

接下来，您需要确保您对端口具有读/写访问权限。假设您的 Arduino 已连接到 /dev/ttyACM0，请运行以下命令：
Next you need to make sure you have read/write access to the port.  Assuming your Arduino is connected on /dev/ttyACM0, run the command:

    $ ls -l /dev/ttyACM0

您应该会看到类似于以下内容的输出：
and you should see an output similar to the following:

    crw-rw---- 1 root work 166, 0 2013-02-24 08:31 /dev/ttyACM0

请注意，只有 root 和“work”组具有读/写访问权限。因此，您需要成为work组的成员。您只需执行一次此操作，然后它应该适用于您稍后插入的所有 USB 设备。
Note that only root and the "work" group have read/write access.  Therefore, you need to be a member of the work group.  You only have to do this once and it should then work for all USB devices you plug in later on.

要将自己添加到work组，请运行以下命令：
To add yourself to the work group, run the command:

    $ sudo usermod -a -G work your_user_name

其中 your_user_name 是您的 Linux 登录名。您可能必须退出 X-window 会话，然后重新登录，或者如果您想确定，只需重新启动计算机。
where your\_user\_name is your Linux login name.  You will likely have to log out of your X-window session then log in again, or simply reboot your machine if you want to be sure.

当您再次登录时，请尝试以下命令：
When you log back in again, try the command:

    $ groups

您应该会看到您所属的组列表，包括work。
and you should see a list of groups you belong to including work. 

ros_arduino_bridge 库的安装 Installation of the ros\_arduino\_bridge Stack
----------------------------------------------

    $ cd ~/catkin_ws/src
    $ git clone git@github.com:yanjingang/ros_arduino_bridge.git
    $ cd ~/catkin_ws
    $ catkin_make

提供的 Arduino 库称为 ROSArduinoBridge，位于 ros_arduino_firmware 包中。此示例库有特殊的硬件要求，但也可以通过关闭BaseController（如本文档末尾的 NOTES 部分所述）与其他 Arduino 类型板（例如 Uno）一起使用。
The provided Arduino library is called ROSArduinoBridge and is
located in the ros\_arduino\_firmware package.  This sketch is
specific to the hardware requirements above but it can also be used
with other Arduino-type boards (e.g. Uno) by turning off the base
controller as described in the NOTES section at the end of this
document.

要安装 ROSArduinoBridge 库，请按照下列步骤操作：
To install the ROSArduinoBridge library, follow these steps:

    $ cd SKETCHBOOK_PATH

其中 SKETCHBOOK_PATH 是 Arduino 示例库的路径。
where SKETCHBOOK_PATH is the path to your Arduino sketchbook directory.

    $ cp -rp `rospack find ros_arduino_firmware`/src/libraries/ROSArduinoBridge ROSArduinoBridge

最后一条命令将 ROSArduinoBridge 文件复制到您的 示例 文件夹中。下一节将介绍如何配置、编译和上传此库。
This last command copies the ROSArduinoBridge sketch files into your sketchbook folder.  The next section describes how to configure, compile and upload this sketch.


加载 ROSArduinoBridge 示例 Loading the ROSArduinoBridge Sketch
-----------------------------------

* 如果您使用的是base controller，请确保您已经将合适的电机控制器和编码器库安装到您的 Arduino 示例/库文件夹中。
* If you are using the base controller, make sure you have already installed the appropriate motor controller and encoder libraries into your Arduino sketchbook/librariesfolder.

* 启动 Arduino 1.0 IDE 并加载 ROSArduinoBridge 示例。您应该可以通过以下方式找到它：
* Launch the Arduino 1.0 IDE and load the ROSArduinoBridge sketch.
  You should be able to find it by going to:

    文件->示例->ROSArduinoBridge
    File->Sketchbook->ROSArduinoBridge
  
注意：如果您没有所需的基本控制器硬件但仍想尝试代码，请参阅文件末尾的注释。
NOTE: If you don't have the required base controller hardware but
still want to try the code, see the notes at the end of the file.

通过取消对#define 语句的注释并注释掉任何其他语句来选择一种受支持的电机控制器。默认情况下，选择 Pololu VNH5019 驱动程序。
Choose one of the supported motor controllers by uncommenting its #define statement and commenting out any others.  By default, the Pololu VNH5019 driver is chosen.

通过取消注释#define 语句并注释掉任何其他语句来选择支持的编码器库。默认选择 Robogaia Mega Encoder 防护罩。
Choose a supported encoder library by by uncommenting its #define statement and commenting out any others. the Robogaia Mega Encoder shield is chosen by default.

如果要控制连接到控制器的 PWM 伺服系统，请查找以下行：
If you want to control PWM servos attached to your controller, look for the line:

<pre>
#define USE_SERVOS
</pre>

并确保它没有像这样注释掉：
and make sure it is not commented out like this:

<pre>
//#define USE_SERVOS
</pre>

然后您必须编辑头文件servos.h 并更改N_SERVOS 参数以及您所连接的舵机的引脚编号。
You must then edit the include file servos.h and change the N_SERVOS
parameter as well as the pin numbers for the servos you have attached.

* 编译示例并将其上传到您的 Arduino。
* Compile and upload the sketch to your Arduino.

固件命令 Firmware Commands
-----------------
ROSArduinoLibrary 通过串行端口接受单字母命令，用于轮询传感器、控制伺服系统、驱动机器人和读取编码器。这些命令可以通过任何串行接口发送到 Arduino，包括 Arduino IDE 中的串行监视器。
The ROSArduinoLibrary accepts single-letter commands over the serial port for polling sensors, controlling servos, driving the robot, and reading encoders.  These commands can be sent to the Arduino over any serial interface, including the Serial Monitor in the Arduino IDE.

**注意：** 在尝试这些命令之前，使用串行监视器窗口右下角的两个下拉菜单将串行监视器波特率设置为 57600，并将行终止符设置为“回车”或“NL & CR”。
**NOTE:** Before trying these commands, set the Serial Monitor baudrate to 57600 and the line terminator to "Carriage return" or "Both NL & CR" using the two pulldown menus on the lower right of the Serial Monitor window.

命令列表可以在文件commands.h 中找到。目前的名单包括：
The list of commands can be found in the file commands.h.  The current list includes:

<pre>
#define ANALOG_READ    'a'
#define GET_BAUDRATE   'b'
#define PIN_MODE       'c'
#define DIGITAL_READ   'd'
#define READ_ENCODERS  'e'
#define MOTOR_SPEEDS   'm'
#define PING           'p'
#define RESET_ENCODERS 'r'
#define SERVO_WRITE    's'
#define SERVO_READ     't'
#define UPDATE_PID     'u'
#define DIGITAL_WRITE  'w'
#define ANALOG_WRITE   'x'
</pre>

例如，要获取引脚 3 上的模拟读数，请使用以下命令：
For example, to get the analog reading on pin 3, use the command:

a 3

要将数字引脚 3 的模式更改为 OUTPUT，请发送命令：
To change the mode of digital pin 3 to OUTPUT, send the command:

c 3 1

获取当前编码器计数：
To get the current encoder counts:

e

要以每秒 20 个编码器标记/秒 向前移动机器人：
To move the robot forward at 20 encoder ticks per second:

m 20 20


测试您的接线连接 Testing your Wiring Connections
-------------------------------
在差动驱动机器人上，电机连接到极性相反的电机控制器端子。类似地，来自编码器的 A/B 引线以相反的方式相互连接。但是，您仍然需要确保 
(a) 在给定正电机速度时车轮向前移动，以及 (b) 当车轮向前移动时编码器计数增加。
On a differential drive robot, the motors are connected to the motor controller terminals with opposite polarities to each other.  Similarly, the A/B leads from the encoders are connected in the reverse sense to each other.  However, you still need to make sure that (a) the wheels move forward when given a positive motor speed and (b) that the encoder counts increase when the wheels move forward.

之后将你的机器人放到地上，您可以使用 Arduino IDE 中的串行监视器来测试这两个要求。使用“m”命令激活电机，使用“e”命令获取编码器计数，使用“r”命令将编码器重置为 0。请记住，在固件级别，电机速度以每个编码器滴答数给出其次，对于编码器分​​辨率，例如每轮旋转 4000 次计数，诸如“m 20 20”之类的命令应该相当缓慢地移动轮子。（轮子只会移动 2 秒，这是 AUTO_STOP_INTERVAL 的默认设置。）还要记住，第一个参数是左电机速度，第二个参数是右电机速度。类似地，当使用“e”命令时，返回的第一个数字是左编码器计数，第二个数字是右编码器计数。
After **placing your robot on blocks**, you can use the Serial Monitor in the Arduino IDE to test both requirements.  Use the 'm' command to activate the motors, the 'e' command to get the encoder counts, and the 'r' command to reset the encoders to 0.  Remember that at the firmware level, motor speeds are given in encoder ticks per second so that for an encoder resolution of, say 4000 counts per wheel revolution, a command such as 'm 20 20' should move the wheels fairly slowly.  (The wheels will only move for 2 seconds which is the default setting for the AUTO\_STOP\_INTERVAL.)  Also remember that the first argument is the left motor speed and the second argument is the right motor speed.  Similarly, when using the 'e' command, the first number returned is the left encoder count and the second number is the right encoder count.

最后，您可以使用“r”和“e”命令通过手动旋转轮子大约一整圈并检查报告的计数来验证预期的编码器计数。
Finally, you can use the 'r' and 'e' commands to verify the expected encoder counts by rotating the wheels by hand roughly one full turn and checking the reported counts.


配置 ros_arduino_python 节点  Configuring the ros\_arduino\_python Node
-----------------------------------------
现在您的 Arduino 正在运行所需的示例，您可以在您的 PC 上配置 ROS 端。您可以通过编辑目录 ros_arduino_python/config 中的 YAML 文件来定义机器人的尺寸、PID 参数和传感器配置。所以首先进入该目录：
Now that your Arduino is running the required sketch, you can
configure the ROS side of things on your PC.  You define your robot's
dimensions, PID parameters, and sensor configuration by editing the
YAML file in the directory ros\_arduino\_python/config.  So first move
into that directory:

    $ roscd ros_arduino_python/config

现在将提供的配置文件复制到一个您可以修改的文件中：
Now copy the provided config file to one you can modify:

    $ cp arduino_params.yaml my_arduino_params.yaml

在您喜欢的文本编辑器中打开您的 params 文件副本 (my_arduino_params.yaml)。它应该开始看起来像这样：
Bring up your copy of the params file (my\_arduino\_params.yaml) in
your favorite text editor.  It should start off looking like this:

<pre>
port: /dev/ttyUSB0
baud: 57600
timeout: 0.1

rate: 50
sensorstate_rate: 10

use_base_controller: False
base_controller_rate: 10

# === 机器人传动系统参数 Robot drivetrain parameters
#wheel_diameter: 0.146
#wheel_track: 0.2969
#encoder_resolution: 8384 # from Pololu for 131:1 motors
#gear_reduction: 1.0
#motors_reversed: True

# === PID parameters
#Kp: 20
#Kd: 12
#Ki: 0
#Ko: 50
#accel_limit: 1.0

# === 传感器定义。仅示例 - 请为您的机器人单独设置 Sensor definitions.  Examples only - edit for your robot.
#     传感器类型可以是以下之一（区分大小写！） Sensor type can be one of the follow (case sensitive!):
#	  * Ping
#	  * GP2D12
#	  * Analog
#	  * Digital
#	  * PololuMotorCurrent
#	  * PhidgetsVoltage
#	  * PhidgetsCurrent (20 Amp, DC)

sensors: {
  #motor_current_left:   {pin: 0, type: PololuMotorCurrent, rate: 5},
  #motor_current_right:  {pin: 1, type: PololuMotorCurrent, rate: 5},
  #ir_front_center:      {pin: 2, type: GP2D12, rate: 10},
  #sonar_front_center:   {pin: 5, type: Ping, rate: 10},
  arduino_led:          {pin: 13, type: Digital, rate: 5, direction: output}
}
</pre>

**注意：** 不要在 .yaml 文件中使用选项卡，否则解析器会在尝试加载它时将其退回。请始终使用空格代替。 另外：定义传感器参数时，列表中的最后一个传感器在行尾没有逗号 (,)，但其余所有传感器都必须有逗号。
**NOTE**: Do not use tabs in your .yaml file or the parser will barf it back out when it tries to load it.   Always use spaces instead.  **ALSO**: When defining your sensor parameters, the last sensor in the list does **not** get a comma (,) at the end of the line but all the rest **must** have a comma.

现在让我们看看这个文件的每个部分。
Let's now look at each section of this file.

_端口设置_
 _Port Settings_

端口可能是 /dev/ttyACM0 或 /dev/ttyUSB0。相应地设置。
The port will likely be either /dev/ttyACM0 or /dev/ttyUSB0. Set accordingly.

The MegaRobogaiaPololu Arudino sketch connects at 57600 baud by default.

_轮询速率_
_Polling Rates_

主要速率参数（默认为 50 Hz）决定了外部 ROS 循环的运行速度。在大多数情况下，默认值就足够了。无论如何，它应该至少与您最快的传感器速率（定义如下）一样快。
The main *rate* parameter (50 Hz by default) determines how fast the
outside ROS loop runs.  The default should suffice in most cases.  In
any event, it should be at least as fast as your fastest sensor rate
(defined below).

该sensorstate_rate决定如何经常发布所有传感器读数的汇总清单。每个传感器还发布自己的主题和速率。
The *sensorstate\_rate* determines how often to publish an aggregated
list of all sensor readings.  Each sensor also publishes on its own
topic and rate.

该use_base_controller参数默认设置为False。将其设置为 True 以使用base control（假设您有所需的硬件。）您还必须设置后面的 PID 参数。
The *use\_base\_controller* parameter is set to False by default.  Set it to True to use base control (assuming you have the required hardware.)  You will also have to set the PID paramters that follow.

该base_controller_rate决定如何经常发布里程计读数。
The *base\_controller\_rate* determines how often to publish odometry readings.

_定义传感器_
_Defining Sensors_

传感器参数是定义传感器名称和传感器参数的字典。（您可以随意命名每个传感器，但请记住，传感器的名称也将成为该传感器的主题名称。）
The *sensors* parameter defines a dictionary of sensor names and
sensor parameters. (You can name each sensor whatever you like but
remember that the name for a sensor will also become the topic name
for that sensor.)

四个最重要的参数是引脚、类型、速率和方向。该速率定义了每秒要轮询该传感器的次数。例如，电压传感器可能每秒只轮询一次（甚至每 2 秒一次：rate=0.5），而声纳传感器可能每秒轮询 20 次。类型必须是列出的类型之一（区分大小写！）。默认方向是输入，以便定义输出引脚，将方向明确设置为输出。在上面的示例中，Arduino LED（引脚 13）将以每秒 2 次的速率打开和关闭。
The four most important parameters are *pin*, *type*, *rate* and *direction*.
The *rate* defines how many times per second you want to poll that
sensor.  For example, a voltage sensor might only be polled once a
second (or even once every 2 seconds: rate=0.5), whereas a sonar
sensor might be polled at 20 times per second.  The *type* must be one
of those listed (case sensitive!).  The default *direction* is input so
to define an output pin, set the direction explicitly to output.  In
the example above, the Arduino LED (pin 13) will be turned on and off
at a rate of 2 times per second.

_设置传动系统和 PID 参数_
_Setting Drivetrain and PID Parameters_

要使用base control，您必须取消注释并设置机器人传动系统和 PID 参数。示例传动系统参数适用于相距 11.5" 的 6" 驱动轮。请注意，ROS 使用米表示距离，因此相应地进行转换。示例编码器分辨率（每转标记）来自 Pololu 131:1 电机的规格。为您的电机/编码器组合设置适当的编号。如果您发现车轮向后转动，请将 motor_reversed 设置为 True，否则设置为 False。
To use the base controller, you will have to uncomment and set the
robot drivetrain and PID parameters.  The sample drivetrain parameters
are for 6" drive wheels that are 11.5" apart.  Note that ROS uses
meters for distance so convert accordingly.  The sample encoder
resolution (ticks per revolution) is from the specs for the Pololu
131:1 motor.  Set the appropriate number for your motor/encoder
combination.  Set the motors_reversed to True if you find your wheels
are turning backward, otherwise set to False.

PID 参数设置起来比较棘手。您可以从示例值开始，但请确保在向其发送第一个 Twist 命令之前将您的机器人放在块上。
The PID parameters are trickier to set.  You can start with the sample
values but be sure to place your robot on blocks before sending it
your first Twist command.


启动 ros_arduino_python 节点 Launching the ros\_arduino\_python Node
---------------------------------------
查看 ros_arduino_python/launch 目录中的启动文件 arduino.launch。如您所见，它指向一个名为 my_arduino_params.yaml 的配置文件。如果您将配置文件命名为不同的名称，请更改启动文件中的名称。
Take a look at the launch file arduino.launch in the
ros\_arduino\_python/launch directory.  As you can see, it points to a
config file called my\_arduino\_params.yaml.  If you named your config
file something different, change the name in the launch file.

连接 Arduino 并运行 MegaRobogaiaPololu 草图后，使用您的参数启动 ros_arduino_python 节点：
With your Arduino connected and running the MegaRobogaiaPololu sketch,
launch the ros\_arduino\_python node with your parameters:

    $ roslaunch ros_arduino_python arduino.launch

您应该会看到类似于以下输出的内容：
You should see something like the following output:

<pre>
process[arduino-1]: started with pid [6098]
Connecting to Arduino on port /dev/ttyUSB0 ...
Connected at 57600
Arduino is ready.
[INFO] [WallTime: 1355498525.954491] Connected to Arduino on port /dev/ttyUSB0 at 57600 baud
[INFO] [WallTime: 1355498525.966825] motor_current_right {'rate': 5, 'type': 'PololuMotorCurrent', 'pin': 1}
[INFO]
etc
</pre>

如果你的机器人上有任何 Ping 声纳传感器并且你在你的配置文件中定义了它们，它们应该开始闪烁以表明你已经建立了连接。
If you have any Ping sonar sensors on your robot and you defined them
in your config file, they should start flashing to indicate you have
made the connection.

查看传感器数据 Viewing Sensor Data
-------------------
要查看汇总的传感器数据，请回显传感器状态主题：
To see the aggregated sensor data, echo the sensor state topic:

    $ rostopic echo /arduino/sensor_state

要查看任何特定传感器的数据，请回显其主题名称：
To see the data on any particular sensor, echo its topic name:

    $ rostopic echo /arduino/sensor/sensor_name

例如，如果您有一个名为 ir_front_center 的传感器，您可以使用以下命令查看其数据：
For example, if you have a sensor called ir\_front\_center, you can see
its data using:

    $ rostopic echo /arduino/sensor/ir_front_center

您还可以使用 rxplot 绘制范围数据：
You can also graph the range data using rxplot:

    $ rxplot -p 60 /arduino/sensor/ir_front_center/range


发送 Twist 命令和查看里程计数据 Sending Twist Commands and Viewing Odometry Data
------------------------------------------------
将您的机器人放在块上，然后尝试发布 Twist 命令：
Place your robot on blocks, then try publishing a Twist command:

    $ rostopic pub -1 /cmd_vel geometry_msgs/Twist '{ angular: {z: 0.5} }'

车轮应按与逆时针旋转一致的方向转动（右轮向前，左轮向后）。如果它们转向相反的方向，请将配置文件中的 motor_reversed 参数设置为其当前设置的相反方向，然后终止并重新启动 arduino.launch 文件。
The wheels should turn in a direction consistent with a
counter-clockwise rotation (right wheel forward, left wheel backward).
If they turn in the opposite direction, set the motors_reversed
parameter in your config file to the opposite of its current setting,
then kill and restart the arduino.launch file.

使用以下命令停止机器人：
Stop the robot with the command:

    $ rostopic pub -1 /cmd_vel geometry_msgs/Twist '{}'

查看里程计数据：
To view odometry data:

    $ rostopic echo /odom

or

   $ rxplot -p 60 /odom/pose/pose/position/x:y, /odom/twist/twist/linear/x, /odom/twist/twist/angular/z

ROS服务 ROS Services
------------
ros_arduino_python 包还定义了一些 ROS 服务，如下所示：
The ros\_arduino\_python package also defines a few ROS services as follows:

**digital_set_direction** - 设置数字引脚的方向
**digital\_set\_direction** - set the direction of a digital pin

    $ rosservice call /arduino/digital_set_direction pin direction

其中 pin 是引脚编号，方向为 0 表示输入，1 表示输出。
where pin is the pin number and direction is 0 for input and 1 for output.

**digital_write** - 向数字引脚发送低 (0) 或高 (1) 信号
**digital\_write** - send a LOW (0) or HIGH (1) signal to a digital pin

    $ rosservice call /arduino/digital_write pin value

其中 pin 是引脚编号，值为 0 表示低电平，1 表示高电平。
where pin is the pin number and value is 0 for LOW and 1 for HIGH.

**伺服写入** - 设置伺服的位置
**servo\_write** - set the position of a servo

    $ rosservice call /arduino/servo_write id pos

其中 id 是 Arduino 草图 (servos.h) 中定义的伺服索引，pos 是弧度 (0 - 3.14) 中的位置。
where id is the index of the servo as defined in the Arduino sketch (servos.h) and pos is the position in radians (0 - 3.14).

**servo\_read** -读伺服的位置
**servo\_read** - read the position of a servo

    $ rosservice call /arduino/servo_read id

其中 id 是 Arduino 草图 (servos.h) 中定义的伺服索引
where id is the index of the servo as defined in the Arduino sketch (servos.h)

使用板载车轮编码器计数器（仅限 Arduino Uno） Using the on-board wheel encoder counters (Arduino Uno only)
------------------------------------------------------------
该固件支持 Arduino Uno 的板载车轮编码器计数器。这允许将车轮编码器直接连接到 Arduino 板，而无需任何额外的车轮编码器计数器设备（例如 RoboGaia 编码器屏蔽）。
The firmware supports on-board wheel encoder counters for Arduino Uno.
This allows connecting wheel encoders directly to the Arduino board, without the need for any additional wheel encoder counter equipment (such as a RoboGaia encoder shield).

为了速度，代码直接寻址特定的 Atmega328p 端口和中断，使此实现依赖于 Atmega328p（Arduino Uno）。（不过，它应该很容易适应其他板/AVR 芯片。）
For speed, the code is directly addressing specific Atmega328p ports and interrupts, making this implementation Atmega328p (Arduino Uno) dependent. (It should be easy to adapt for other boards/AVR chips though.)

要使用板载车轮编码器计数器，请将您的车轮编码器连接到 Arduino Uno，如下所示：
To use the on-board wheel encoder counters, connect your wheel encoders to Arduino Uno as follows:

    Left wheel encoder A output -- Arduino UNO pin 2
    Left wheel encoder B output -- Arduino UNO pin 3

    Right wheel encoder A output -- Arduino UNO pin A4
    Right wheel encoder B output -- Arduino UNO pin A5

在 ROSArduinoBridge 草图中进行以下更改以禁用 RoboGaia 编码器屏蔽，并启用板载编码器屏蔽：
Make the following changes in the ROSArduinoBridge sketch to disable the RoboGaia encoder shield, and enable the on-board one:

    /* The RoboGaia encoder shield */
    //#define ROBOGAIA
    /* Encoders directly attached to Arduino board */
    #define ARDUINO_ENC_COUNTER

编译更改并上传到您的控制器。
Compile the changes and upload to your controller.

使用 L298 电机驱动器 Using L298 Motor driver
-----------------------
L298电机驱动器和arduino板之间的接线在固件中的motor_driver.h中定义如下：
the wiring between the L298 motor driver and arduino board is defined in motor_driver.h in the firmware as follow:

    #define RIGHT_MOTOR_BACKWARD 5
    #define LEFT_MOTOR_BACKWARD  6
    #define RIGHT_MOTOR_FORWARD  9
    #define LEFT_MOTOR_FORWARD   10
    #define RIGHT_MOTOR_ENABLE 12
    #define LEFT_MOTOR_ENABLE 13

以这种方式连接它们或根据需要更改它们，并确保定义了 L298 电机驱动程序，然后编译并上传固件。
wire them this way or change them if you want, and make sure that the L298 motor driver is defined then compile and upload the firmware.

笔记 NOTES
-----
如果您没有运行base control器所需的硬件，请按照以下说明操作，以便您仍然可以使用与 Arduino 兼容的控制器读取传感器和控制 PWM 伺服系统。
If you do not have the hardware required to run the base controller,
follow the instructions below so that you can still use your
Arduino-compatible controller to read sensors and control PWM servos.

首先，您需要编辑 ROSArduinoBridge 示例。在文件的顶部注释掉这一行：
First, you need to edit the ROSArduinoBridge sketch. At the top of
the file comment out the line:

<pre>
#define USE_BASE
</pre>

所以它看起来像这样：
so that it looks like this:

<pre>
//#define USE_BASE
</pre>

**注意：** 如果您使用的是 1.6.6 之前的 Arduino IDE 版本，您还需要注释掉文件 encoder_driver.ino 中如下所示的行：
**NOTE:** If you are using a version of the Arduino IDE previous to 1.6.6, you also need to comment out the line that looks like this in the file encoder_driver.ino:

    #include "MegaEncoderCounter.h"

所以它看起来像这样：
so it looks like this:

    //#include "MegaEncoderCounter.h"

编译更改并上传到您的控制器。
Compile the changes and upload to your controller.

接下来，编辑 my_arduino_params.yaml 文件并确保 use_base_controller 参数设置为 False。这里的所有都是它的。
Next, edit your my\_arduino_params.yaml file and make sure the
use\_base\_controller parameter is set to False.  That's all there is to it.
