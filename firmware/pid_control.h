#ifndef PID_CONTROL_H
#define PID_CONTROL_H

#define KP 1
#define KI 1
#define KD 1

#include "hardware_config.h"
#include "ethernet_handler.h"
#include "sensor_handler.h"
#include "thruster_control.h"

void pid_control();

#endif