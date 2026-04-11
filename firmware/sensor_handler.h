#ifndef DEPTH_SENSOR_H
#define DEPTH_SENSOR_H



#define SENSOR_CONSTANT 234

#include "hardware_config.h"
#include <Wire.h>
#include <Adafruit_ADS1X15.h>

void init_sensor();
int16_t read_depth_sensor();

#endif