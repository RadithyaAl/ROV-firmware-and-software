#ifndef ETHERNET_HANDLER_H
#define ETHERNET_HANDLER_H

#include <Arduino.h>
#include <SPI.h>
#include <Ethernet.h>
#include <EthernetUdp.h>
#include "hardware_config.h"

// Function declarations
void init_ethernet();
bool send_ethernet_data(String data_to_be_sent);
bool receive_ethernet_data(int computer_command[]);
#endif