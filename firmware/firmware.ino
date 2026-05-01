#include "hardware_config.h"
#include "sensor_handler.h"

void setup(){
  init_sensor();
  Serial.begin(115200);
}

void loop(){
  float depth = read_depth_sensor();
  Serial.println(depth);
}