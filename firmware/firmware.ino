#include "hardware_config.h"
#include "thruster_control.h"
#include "ethernet_handler.h"
#include "sensor_handler.h"
#include "pid_control.h"

// Create an array to hold the incoming values. 
// Initialize them to 1500 (stopped) for safety!
int current_thruster_commands[NUM_THRUSTERS] = {1500, 1500, 1500, 1500, 1500, 1500};

void setup() {
  Serial.begin(115200);
  
  thruster_init(); 
  init_ethernet();
  init_sensor();
}

void loop() {
  // If we receive a valid new command over Ethernet...
}