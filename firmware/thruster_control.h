#ifndef THRUSTER_CONTROL_H
#define THRUSTER_CONTROL_H

#include <ESP32Servo.h>
#include "hardware_config.h" // Ensures NUM_THRUSTERS and PWM_FREQ are known

// Declare the servo array as 'extern' so main.ino can see and control it later
extern Servo thruster[NUM_THRUSTERS];

// #define MAXTHRUST 2000;
// #define MINTHRUST 1000;

// Function declaration
void init_thruster();
void set_nav_thruster(int thruster_command[NUM_THRUSTERS-2]);
void set_ver_thruster(int thruster_command[2]);

#endif