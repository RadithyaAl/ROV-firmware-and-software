#include "hardware_config.h"
#include "servo_handler.h"
#include "thruster_control.h"
#include "ethernet_handler.h"
#include "sensor_handler.h"

// Shared global arrays
int thruster_cmd[NUM_THRUSTERS]; 
int ethernet_data[ETH_DATA_SIZE]; 

// FreeRTOS Mutex Handle to guard the shared arrays
SemaphoreHandle_t xDataMutex;

// Task Prototypes
void vPIDControlTask(void *pvParameters);
void vCommunicationsTask(void *pvParameters);

void setup() {
  Serial.begin(115200);
  
  init_sensor();
  init_ethernet();
  init_servo();
  init_thruster();

  // Create the Mutex before starting tasks
  xDataMutex = xSemaphoreCreateMutex();

  if (xDataMutex != NULL) {
    // Create PID Control Task (Higher priority for time-critical stability loops)
    xTaskCreatePinnedToCore(
      vPIDControlTask,     // Task function
      "PID Control",       // Task name
      4096,                // Stack size (bytes/words depending on platform)
      NULL,                // Parameter passed
      2,                   // Priority (Higher)
      NULL,                // Task handle
      0                    // Core 0
    );

    // Create Communications Task
    xTaskCreatePinnedToCore(
      vCommunicationsTask,
      "Communications",
      4096,
      NULL,
      1,                   // Priority (Lower)
      NULL,                // Task handle
      1                    // Core 1 (Assuming dual-core like ESP32 to split load)
    );
  } else {
    Serial.println("Failed to create Mutex!");
  }
}

void loop() {
  // Empty. FreeRTOS handles task scheduling.
}

void vPIDControlTask(void *pvParameters) {
  TickType_t xLastWakeTime = xTaskGetTickCount();
  const TickType_t xDelay20ms = pdMS_TO_TICKS(20); 

  float previous_error = 0.0f;
  float integral = 0.0f;
  const float dt = 0.02f; 

  while (1) {
    int local_setpoint = 0;
    float current_depth = read_depth_sensor();

    // 1. Fetch setpoint quickly
    if (xSemaphoreTake(xDataMutex, pdMS_TO_TICKS(5)) == pdTRUE) {
      local_setpoint = ethernet_data[6]; 
      xSemaphoreGive(xDataMutex);
    }

    // 2. Perform PID calculations
    float current_error = local_setpoint - current_depth;
    integral += current_error * dt;
    integral = constrain(integral, -INTEGRAL_MAX, INTEGRAL_MAX); 
    
    float derivative = (current_error - previous_error) / dt;
    float pid_results = (PID_KP * current_error) + (PID_KI * integral) + (PID_KD * derivative);
    previous_error = current_error;

    // Convert raw controller trends directly into explicit PWM microseconds
    float raw_pid_out = constrain(pid_results, -500.0f, 500.0f);
    int motor_pwm_us = 1500 + raw_pid_out;
    // 3. Stage the vertical thruster instructions quickly
    if (xSemaphoreTake(xDataMutex, pdMS_TO_TICKS(5)) == pdTRUE) {
      thruster_cmd[4] = motor_pwm_us;
      thruster_cmd[5] = motor_pwm_us;
      xSemaphoreGive(xDataMutex);
    }

    vTaskDelayUntil(&xLastWakeTime, xDelay20ms);
  }
}

void vCommunicationsTask(void *pvParameters) {
  while (1) {
    // receive_ethernet_data needs the actual array pointer
    // We pass it directly; ensure this function doesn't block indefinitely without yielding
    receive_ethernet_data(ethernet_data); 

    if (xSemaphoreTake(xDataMutex, portMAX_DELAY) == pdTRUE) {
      // Update horizontal thrusters (0 to 3) from fresh ethernet data
      for (int i = 0; i < 4; i++) {
        thruster_cmd[i] = ethernet_data[i];
      }

      // Write ALL thruster outputs to hardware inside the safe zone
      for (int i = 0; i < NUM_THRUSTERS; i++) {
        set_thruster(i, thruster_cmd[i]); // Added index 'i' so hardware knows which motor to spin
      }

      // Handle servo assignment
      set_servo(ethernet_data[7]);
      
      xSemaphoreGive(xDataMutex);
    }

    // shows the received commands. the servos, or thruster notifications are on each cpp files. it calls serial there.
      for(int i = 0; i < ETH_DATA_SIZE; i++){
        Serial.print(ethernet_data[i]);
        Serial.print(" ");
      }
      Serial.println();


    // Yield control briefly to allow network stack processing (e.g., 10ms)
    vTaskDelay(pdMS_TO_TICKS(10));
  }
}
