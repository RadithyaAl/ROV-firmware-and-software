#ifndef PID_CONTROL_H
#define PID_CONTROL_H 

#include "hardware_config.h"

#define KP 1.0;
#define KI 1.0;
#define KD 1.0;
#define CONTROLLER_GAIN 10;
#define I_MAX 100;
#define MINTHRUST 1000;
#define MAXTHRUST 2000;

void pid(float depth_sensor_value, float set_point_value, float pid_thrust[]);



#endif
