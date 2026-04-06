#include "globals.h"
#include "config.h"

// ==================== SHARED OBJECTS ====================
WiFiClient wifiClient;
Arduino_MQTT_Client mqttClient(wifiClient);
ThingsBoard tb(mqttClient, MAX_MESSAGE_SIZE);
DHT20 dht20;

// ==================== WIFI STATE ====================
volatile bool wifiConnected = false;

// ==================== THINGSBOARD STATE ====================
volatile bool tbConnected = false;

// ==================== SENSOR DATA ====================
volatile float temperature = 0.0f;
volatile float humidity = 0.0f;

// ==================== LED CONTROL ====================
volatile bool ledState = false;
volatile int ledMode = 0;
volatile uint16_t blinkingInterval = 1000U;
volatile bool attributesChanged = false;

// ==================== SYNCHRONIZATION PRIMITIVES ====================
SemaphoreHandle_t wifiMutex = nullptr;
SemaphoreHandle_t tbMutex = nullptr;
SemaphoreHandle_t sensorMutex = nullptr;

void initGlobals() {
  // Create mutexes for thread-safe access
  wifiMutex = xSemaphoreCreateMutex();
  tbMutex = xSemaphoreCreateMutex();
  sensorMutex = xSemaphoreCreateMutex();
  
  if (wifiMutex == nullptr || tbMutex == nullptr || sensorMutex == nullptr) {
    Serial.println("ERROR: Failed to create semaphores!");
    while (1) {
      delay(1000);
    }
  }
}
