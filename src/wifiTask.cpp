#include "wifiTask.h"
#include "globals.h"
#include "config.h"

void wifiTaskFunction(void *pvParameters) {
  (void) pvParameters;

  // Initial WiFi connection
  Serial.println("[WiFi Task] Starting WiFi connection...");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  uint32_t connectionStartTime = millis();
  const uint32_t CONNECTION_TIMEOUT = 20000; // 20 seconds timeout

  // Wait for initial connection with timeout
  while (WiFi.status() != WL_CONNECTED && 
         (millis() - connectionStartTime) < CONNECTION_TIMEOUT) {
    delay(500);
    Serial.print(".");
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\n[WiFi Task] Connected to WiFi!");
    Serial.print("[WiFi Task] IP: ");
    Serial.println(WiFi.localIP());
    wifiConnected = true;
  } else {
    Serial.println("\n[WiFi Task] Failed to connect to WiFi!");
  }

  // Main loop - check and reconnect if needed
  while (1) {
    // Check WiFi status every WIFI_CHECK_INTERVAL
    vTaskDelay(WIFI_CHECK_INTERVAL / portTICK_PERIOD_MS);

    if (WiFi.status() == WL_CONNECTED) {
      if (!wifiConnected) {
        Serial.println("[WiFi Task] WiFi reconnected!");
        wifiConnected = true;
      }
    } else {
      // WiFi disconnected, attempt to reconnect
      if (wifiConnected) {
        Serial.println("[WiFi Task] WiFi disconnected! Attempting to reconnect...");
        wifiConnected = false;
      }

      Serial.print("[WiFi Task] Reconnecting to WiFi");
      WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

      // Wait for reconnection with timeout
      uint32_t reconnectStartTime = millis();
      while (WiFi.status() != WL_CONNECTED && 
             (millis() - reconnectStartTime) < CONNECTION_TIMEOUT) {
        delay(500);
        Serial.print(".");
      }

      if (WiFi.status() == WL_CONNECTED) {
        Serial.println(" [SUCCESS]");
        Serial.print("[WiFi Task] IP: ");
        Serial.println(WiFi.localIP());
        wifiConnected = true;
      } else {
        Serial.println(" [FAILED]");
      }
    }
  }
}
