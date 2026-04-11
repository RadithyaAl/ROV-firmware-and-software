// include/hardware_config.h

#ifndef HARDWARE_CONFIG_H
#define HARDWARE_CONFIG_H

// --- I2C BUS (IMU & ADS1115) ---
#define PIN_I2C_SDA         21
#define PIN_I2C_SCL         22

// --- SPI BUS (W5500 ETHERNET) ---
#define PIN_SPI_MOSI        23
#define PIN_SPI_MISO        19
#define PIN_SPI_SCK         18
#define PIN_W5500_CS        32 // Moved to 32 to avoid strapping pin 5
#define PIN_W5500_RST       4

// --- BLDC THRUSTER ESCs ---
#define NUM_THRUSTERS          6
#define PIN_THRUSTER_1         13
#define PIN_THRUSTER_2         14
#define PIN_THRUSTER_3         25
#define PIN_THRUSTER_4         26
#define PIN_THRUSTER_5         27
#define PIN_THRUSTER_6         33
#define PWM_FREQ               50

// --- ADC CHANNELS ---
// The pressure sensor is connected to the ADS1115, not the ESP32.
// We define the ADS1115 channel here for software reference.
#define ADS1115_PRESSURE_CH 0 // Connected to A0 on the ADS module

#endif // HARDWARE_CONFIG_H