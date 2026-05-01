#include "thruster_control.h"

// Define the global array here (this creates the actual objects in memory)
Servo thruster[NUM_THRUSTERS];

void init_thruster() {
  int thruster_pins[NUM_THRUSTERS] = {PIN_THRUSTER_1, PIN_THRUSTER_2, PIN_THRUSTER_3, PIN_THRUSTER_4, PIN_THRUSTER_5, PIN_THRUSTER_6};

  for (int i = 0; i < NUM_THRUSTERS; i++) {
    thruster[i].setPeriodHertz(PWM_FREQ);
    thruster[i].attach(thruster_pins[i], 1000, 2000); 
  }
}

void set_thruster(int thruster_command[NUM_THRUSTERS]){
  for (int i = 0; i < NUM_THRUSTERS; i++){
    if (thruster_command[i] > 2000){
      thruster_command[i] = 2000;
    } else if (thruster_command[i] < 1000){
      thruster_command[i] = 1000;
    }
    
    thruster[i].writeMicroseconds(thruster_command[i]);
  }
}
