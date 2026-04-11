#include  "sensor_handler.h"
#include "ethernet_handler.h"
Adafruit_ADS1X15 ads;

void init_sensor(){
  Wire.begin(PIN_I2C_SDA, PIN_I2C_SCL);
  if (!ads.begin()) {
    String message = "Failed to Initialize ADC";
    send_ethernet_data(message);
    while (1);
  }
}

int16_t read_depth_sensor(){
  int16_t raw = ads.readADC_SingleEnded(ADS1115_PRESSURE_CH);
  float voltage = ads.computeVolts(raw);
  return voltage;
}
