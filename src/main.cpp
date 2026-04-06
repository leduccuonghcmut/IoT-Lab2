#include <WiFi.h>
#include <Arduino_MQTT_Client.h>
#include <ThingsBoard.h>
#include "DHT20.h"
#include "Wire.h"

#include "config.h"
#include "globals.h"
#include "wifiTask.h"
#include "iotTask.h"
#include "sensorTask.h"
#include "attributesTask.h"
#include "loopTask.h"

// Task handles
TaskHandle_t wifiTaskHandle = nullptr;
TaskHandle_t iotTaskHandle = nullptr;
TaskHandle_t sensorTaskHandle = nullptr;
TaskHandle_t attributesTaskHandle = nullptr;
TaskHandle_t tbLoopTaskHandle = nullptr;

void setup() {
  Serial.begin(SERIAL_DEBUG_BAUD);
  Serial.println("\n\n========== RTOS-based IoT Application ==========");
  
  // Initialize GPIO
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  
  delay(1000);

  // Initialize I2C and DHT20 sensor
  Wire.begin(SDA_PIN, SCL_PIN);
  dht20.begin();
  
  Serial.println("Sensors initialized");

  // Initialize global variables and mutexes
  initGlobals();
  Serial.println("Globals initialized");

  // Create RTOS tasks
  BaseType_t xReturned;

  // WiFi Task - Monitor and reconnect wifi
  xReturned = xTaskCreatePinnedToCore(
    wifiTaskFunction,           // Task function
    "WiFiTask",                 // Task name
    WIFI_TASK_STACK_SIZE,       // Stack size
    nullptr,                    // Task parameters
    WIFI_TASK_PRIORITY,         // Task priority
    &wifiTaskHandle,            // Task handle
    1                           // Core 1
  );
  if (xReturned != pdPASS) {
    Serial.println("ERROR: Failed to create WiFi task!");
  }

  // IoT Connection Task - Handle ThingsBoard connection and subscriptions
  xReturned = xTaskCreatePinnedToCore(
    iotTaskFunction,
    "IoTTask",
    IOT_TASK_STACK_SIZE,
    nullptr,
    IOT_CONNECTION_TASK_PRIORITY,
    &iotTaskHandle,
    1
  );
  if (xReturned != pdPASS) {
    Serial.println("ERROR: Failed to create IoT task!");
  }

  // Sensor Task - Read DHT20 and send telemetry
  xReturned = xTaskCreatePinnedToCore(
    sensorTaskFunction,
    "SensorTask",
    SENSOR_TASK_STACK_SIZE,
    nullptr,
    SENSOR_TASK_PRIORITY,
    &sensorTaskHandle,
    0                           // Core 0
  );
  if (xReturned != pdPASS) {
    Serial.println("ERROR: Failed to create Sensor task!");
  }

  // Attributes Task - Send WiFi and device attributes
  xReturned = xTaskCreatePinnedToCore(
    attributesTaskFunction,
    "AttributesTask",
    ATTRIBUTES_TASK_STACK_SIZE,
    nullptr,
    ATTRIBUTES_TASK_PRIORITY,
    &attributesTaskHandle,
    0
  );
  if (xReturned != pdPASS) {
    Serial.println("ERROR: Failed to create Attributes task!");
  }

  // Loop Task - Execute tb.loop() to process incoming messages
  xReturned = xTaskCreatePinnedToCore(
    loopTaskFunction,
    "LoopTask",
    LOOP_TASK_STACK_SIZE,
    nullptr,
    LOOP_TASK_PRIORITY,
    &tbLoopTaskHandle,
    1
  );
  if (xReturned != pdPASS) {
    Serial.println("ERROR: Failed to create Loop task!");
  }

  Serial.println("All tasks created successfully!");
  Serial.println("================================================");
}

void loop() {
  // In RTOS mode, loop() is not used
  // All work is done by the RTOS tasks
  vTaskDelay(10000 / portTICK_PERIOD_MS);
}
