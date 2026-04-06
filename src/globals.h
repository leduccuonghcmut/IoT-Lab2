#ifndef GLOBALS_H
#define GLOBALS_H

#include <WiFi.h>
#include <Arduino_MQTT_Client.h>
#include <ThingsBoard.h>
#include "DHT20.h"

// ==================== SHARED OBJECTS ====================
extern WiFiClient wifiClient;
extern Arduino_MQTT_Client mqttClient;
extern ThingsBoard tb;
extern DHT20 dht20;

// ==================== WIFI STATE ====================
extern volatile bool wifiConnected;

// ==================== THINGSBOARD STATE ====================
extern volatile bool tbConnected;

// ==================== SENSOR DATA ====================
extern volatile float temperature;
extern volatile float humidity;

// ==================== LED CONTROL ====================
extern volatile bool ledState;
extern volatile int ledMode;
extern volatile uint16_t blinkingInterval;
extern volatile bool attributesChanged;

// ==================== SYNCHRONIZATION PRIMITIVES ====================
extern SemaphoreHandle_t wifiMutex;
extern SemaphoreHandle_t tbMutex;
extern SemaphoreHandle_t sensorMutex;

// ==================== HELPER FUNCTIONS ====================
void initGlobals();

#endif // GLOBALS_H
