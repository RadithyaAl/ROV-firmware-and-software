#include "pid_control.h"
#include "sensor_handler.h"

unsigned long current_time;
unsigned long previous_time = 0;

float current_error;
float previous_error = 0;

float integral = 0;

float controller_gain = CONTROLLER_GAIN;

void pid(float depth_sensor_value, float set_point_value, float pid_thrust[]) {

  current_time = millis();
  float dt = (current_time - previous_time) / 1000.0;

  // Prevent division by zero
  if (dt <= 0) dt = 0.001;

  current_error = set_point_value - depth_sensor_value;

  float proportional = KP * current_error;
  integral += KI * current_error * dt;

if (integral > I_MAX){
  integral = I_MAX;
  }
if (integral < -I_MAX){
  integral = -I_MAX;
  }

  float derivative = KD * (current_error - previous_error) / dt;

  float action = proportional + integral + derivative;

  // Apply to thrusters
  pid_thrust[0] = *controller_gain * action;
  pid_thrust[1] = *controller_gain * action;

  for (int i = 0; i < 2; i++){
    if (pid_thrust[i] < MINTHRUST){
      pid_thrust[i] = MINTHRUST;
    } else if (pid_thrust[i] > MAXTHRUST){
      pid_thrust[i] = MAXTHRUST;
    }
  }

  // others = 0
  for(int i = 2; i < NUM_THRUSTERS; i++){
    pid_thrust[i] = 0;
  }

  previous_time = current_time;
  previous_error = current_error;
}