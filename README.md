# Smart-Fan-IoT-Using-ESP32
A smart fan that can work in automatic and manual (smartphone controlled) mode. ESP32 Devkit V4 is the board used for this project.

## How It Works
### Inputs
1. Temperature (DHT11)
2. Person Count (Image Processing: ESP32 Cam and Laptop)
3. Smartphone (Blynk Control)
### Logic:
1. If Automatic
  1. If Temperature is higher than the specified threshold, and
  2. If Person Count is not 0
  3. Then turn on the Fan
  4. Else turn the Fan off.
2. If Manual
  1. If On/Off toggle is On (Blynk Control)
  2. Then turn the Fan on.
  3. Else turn the Fan off.
3. We control the swing of the fan from our smartphone
4. We control the Temperature Threshold from our smartphone 

## Devices Used
1. DHT11 (Temperature and Humidity Sensor)
2. ESP32 Camera Module
3. ESP32 Devkit V4
4. Single Channel SPDT Relay Module
5. Servo
6. A Small Fan
7. A laptop (for image processing)

## Our Team Personnels
1. Fernanda Risqy Septian (Funding)
2. M. Tamim Habibi (Electrical)
3. Regas Ryandhi Aditama (Electrical)
4. Yudhistira (Programmer)
