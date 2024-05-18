
<div align="center"> 
    <img src="../../Image_assets/Glider_v1_0_kit.png", height=300> 
</div>

</br>

# Glider v1.0

The first version of the Glider board series. A versatile, ESP32 powered, robot control board that allows one to build amazing robots. This board comes along with other components in form of a kit.

Apart from the roboboard, the other components include :

* [12 channel Line Sensor Array](#12-channel-line-sensor-array)
* [Parallel to Serial adapter for Line Sensor Array](#parallel-to-serial-adapter-for-line-sensor-array)
* [Neopixel Array & Buzzer board](#neopixel-array--buzzer-board)
* [OLED based GUI board](#oled-based-gui-board)

One of the common applications of this kit is Line Follower / Maze Solver robot. Apart from the components mentioned above, the kit also includes the necessary JST cables for enabling connectivity between the boards.

## The Roboboard 

<br/>

<p align="center">
    <img src="../../Image_assets/Roboboard_Connector_Overview.png" style="max-height:700;"/>
</p>

<br/>

<p align="center">
    <img src="../../Image_assets/roboboard_desc_p2.png" style="max-height:700;"/>
</p>

<br/>

Like most roboboards, the Glider v1.0 board too has a microcontroller and a motor driver However, this board has a lot more than just two of these components. Following are the core features of the roboboard : 

### 1. Wide range power supply 

this roboard has two buck converters : 12V and 5V. This way the board can be powered by upto 20V via the DC barrel jack.

### 2. USB Type C connectivity 

Unlike most arduino boards, like uno and nano, this has a type C USB connector for uploading the code to ESP32 and serial monitoring.

### 3. Mosfet based TB6612FNG motor driver

known for it's efficiency and decent current output of 1.2A per channel (continous & 3A peak), this board Has the widely used Tb6612fng motor driver integrated on board.

### 4. Encoder Motor Connectivity 

besides scew terminal blocks for controlling two independently running DC motors, the board has two connectors for motors with encoders. ***It's important to note that the first two pins of the connector (close to the terminal blocks) are connected to the screw terminals***.

### 5. Connectivity for MPU6050 

besides having encoders for obtaining odomentry data, the robot may need an IMU for better position estimation. The roboboard has a connectivity for an MPU6050, positioned in a way that the IMU is present at the center.

### 6. Connectivity for HC-05

ESP32 has an inbuilt WiFi and Bluetooth connectivity, however this communication is managed by the code. Such wireless connections may fail to persist after the micrcocontroller resets. To enble wireless debugging over bluetooth, the roboboard has the connectivity for an external module like the HC-05.

### 7. Neopixels (WS2812B RGB LED) array with buzzer 

most robots have to indicate the occurence of an event like detecting a checkpoint ( in case of a line follower ) or detecting a valid intersection ( in case of a maze solvinf robot). The kit comes along with a pcb strip which includes an array of 6 WS2812B leds and a buzzer for unique audio and visual indications.

### 8. Additional Connectivity for I2C based devices  

many projects may require the board to interface with external I2C based peripherals like sensors & motors controllers. Once such example could be PCA9685 for controlling multiple servo motors. Thus projects like a hexapod or a bipedal humanoid robot can also be built using this roboboard.

### 9. 3.3V to 5V transaltion  

Circuitry for conversion of logic level from 3.3v to 5v and vice-versa exists. All the pins the ESP32 board are connected to MOSFET based logic level converters / translators for safe interfaction with 5V logic level devices.

### 10.  GUI board with OLED display connectivity 

The kit includes a GUI board with 4 smd button arranged in Top, Left, Right & Bottom configuration. The board also includes the debouncing circuitry for each of the 4 buttons. Besides, the GUI board has a connectivity for a 0.96" OLED display, hence providing a GUI setup for users to make necessary configurations on the robot dynamically.   

### 11. Easy Motor Driver Control  

The board has a push to On / Off switch for enabling / disabling the motor driver. The TB6612FNG has a STBY pin that allows it to be enabled / disabled by applying +5V digital logic ( 1 or 0). This pin is connected to +5V via a push to on / off switch that haults the motors without affecting the microcontroller's operation and powering off the entire board. This feature comes handy in many scenarious specially in case of a Line Follower Robot.  

### 12. Firm Connections to ensure reliabiity 

Unline most arduino based DIY projects, the use of Jumper / dupont wires may often lead to undesirable behavior as such connecting wires are prone to wearing out over multiple connections / disconnections. Japan Solderless Terminals (JST) connectors on the other hand are widely used in the industry, specially in most electronics based products due to their reliable nature. Leveraging this reliability and firm connectivity JST connectors are used throughout to allow firm connections as jumper wires are often fragile and wear out over time.


## 12 channel Line Sensor Array

<br/>

<p align="center">
    <img src="../../Image_assets/line_sensor_desc.png" style="max-height:700;" />
</p>

<br/>

This sensor array has 12 individual sensors that output either +5V / 0V (digital 1 / 0) depending on how white / black the surface beneath the sensor is. The threshold above which the sensor outputs 1, is adjusted by the corresponding potentiometer. This way the Line sensor array outputs a 12 bit digital data which looks somethings like : 

```
0000110000000000
0000011000000000
0000001100000000
0000000110000000
0000000011000000
0000000001100000
0000000000110000
```

If we consider a white surface to be represented by 1 and a black surface to be represented by a 0, sliding the sensor above a white line drawn above the black surface  

## Parallel to Serial adapter for Line Sensor Array

<br/>

<p align="center">
    <img src="../../Image_assets/adapter_board_desc.png" style="max-height:700;" />
</p>

## Neopixel Array & Buzzer board

<br/>

<p align="center">
    <img src="../../Image_assets/indication_board_desc.png" style="max-height:700;" />
</p>

<br/>

## OLED based GUI board

<br/>

<p align="center">
    <img src="../../Image_assets/gui_board_desc.png" style="max-height:700;" />
</p>

<br/>

## Pinouts 

Following are the links to the images that describe each of the pins of the connectors on the respective boards : 

1. [Roboboard pinout](/Image_assets/Roboboard_Connector_Overview.png)
2. [Line sensor adapter pinout](/Image_assets/adapter_pinout.png)
3. [Indication pinout](/Image_assets/indication_board_pinout.png)
4. [GUI pinout](/Image_assets/gui_board_pinout.png)
