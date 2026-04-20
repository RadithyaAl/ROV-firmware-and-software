#include "hardware_config.h"
#include "thruster_control.h"
#include "ethernet_handler.h"
#include "sensor_handler.h"
#include "pid_control.h"




void TaskCore0(void * parameter) { // for pid control maybe i can pass the pid control function in pid_control.h directly to this.
  while (true) {
  }
}

void TaskCore1(void * parameter) { // for communications
  init_ethernet();
  init_thruster();  
  volatile int computer_command[DATA_LENGTH] = {1000, 1000, 1000, 1000, 1000, 1000, 0, 0};
  volatile int ver_thruster_cmd[2]; // i want this variable can be acessed in core 0
  int nav_thrust_cmd[4];
  for (int i = 0; i < NUM_THRUSTERS; i++){
    if (i <= 2){
      ver_thruster_cmd[i] = computer_command[i];
    }
    if (i > 2 && i < 6){
      nav_thrust_cmd[i] = computer_command[i];
    }
  }

  // set the value to initial value
  set_ver_thruster(ver_thruster_cmd);
  set_nav_thruster(nav_thrust_cmd); 



  while (true) {
  receive_ethernet_data(*computer_command);

  for (int i = 0; i <= NUM_THRUSTERS; i++){
    if (i <= 2){
      ver_thruster_cmd[i] = computer_command[i];
    }
    if (i > 2 && i <= 6){
      nav_thrust_cmd[i] = computer_command[i];
    }
  }
  set_ver_thruster(ver_thruster_cmd);
  // set_servo(computer_command[7]); // the function doesnt exist yet. but the command is like that.

  }
}

void setup() {
  Serial.begin(115200);

  xTaskCreatePinnedToCore(
    TaskCore0,     // Function
    "Task0",       // Name
    10000,         // Stack size
    NULL,          
    1,             // Priority
    NULL,
    0              // Core 0
  );

  xTaskCreatePinnedToCore(
    TaskCore1,
    "Task1",
    10000,
    NULL,
    1,
    NULL,
    1              // Core 1
  );
}

void loop() {
  // Can be empty or used if needed
}
