#include "pid_control.h"

void pid_control(void * pvParameters){
  unsigned long current_time;
  unsigned long previous_time = millis();
  float current_error;
  float previous_error = 0;
  float error;
  bool pidstate = 1;
  int pid_eth_command[DATA_LENGTH];
  float integral_sum = 0;
  int setpoint;
  // can this pid_control function passed to  xTaskCreatePinnedToCore() to be run on each core?
  while (pidstate==1){
    current_time = millis();
    if (receive_ethernet_data(pid_eth_command)){ // passing pid_eth_command to be modified.
      setpoint = pid_eth_command[6];
    }    
    float depth = read_depth_sensor(); // the depth_sensor() returns float value
    current_error = setpoint - depth;
    float dt = (current_time - previous_time) / 1000.0f;
    if (dt <= 0) dt = 0.001f; // Prevent division by zero
    float proportional = current_error*KP;
    float integral = current_error*KI*dt;
    integral_sum += integral;
    if(integral_sum > 400) integral_sum = 400;
    if(integral_sum < -400) integral_sum = -400;
    float derivative = KP*(current_error-previous_error)/dt;

    float control_action = proportional + integral_sum + derivative;
    int final_output = (int)std::round(control_action);

    // sends the data to thruster
    int pid_thrust_cmd[NUM_THRUSTERS];
    for (int i = 0; i < NUM_THRUSTERS; i++){
      if (i<4){
        pid_thrust_cmd[i] = pid_eth_command[i]; // set the 4 original thruster with its supposed to
      }
      if (i >= 4 && i <= 6){
        pid_thrust_cmd[i] = final_output;
      }
    }
    set_thruster(pid_thrust_cmd);

    previous_error = current_error;
    previous_time = current_time;
    vTaskDelay(pdMS_TO_TICKS(10));
    
  }
}