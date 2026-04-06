#include "sensorTask.h"
#include "globals.h"
#include "config.h"

void sensorTaskFunction(void *pvParameters) {
  (void) pvParameters;

  Serial.println("[Sensor Task] Started");

  uint32_t lastSendTime = 0;

  while (1) {
    // Send telemetry data at specified interval
    if (millis() - lastSendTime >= TELEMETRY_SEND_INTERVAL) {
      lastSendTime = millis();

      // Check if ThingsBoard is connected
      if (!tbConnected) {
        Serial.println("[Sensor Task] ThingsBoard not connected, skipping telemetry send");
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        continue;
      }

      // Read sensor data
      if (dht20.read()) {
        float temp = dht20.getTemperature();
        float hum = dht20.getHumidity();

        if (!isnan(temp) && !isnan(hum)) {
          // Store sensor data in global variables (thread-safe)
          if (xSemaphoreTake(sensorMutex, pdMS_TO_TICKS(100)) == pdTRUE) {
            temperature = temp;
            humidity = hum;
            xSemaphoreGive(sensorMutex);
          }

          Serial.print("[Sensor Task] Temperature: ");
          Serial.print(temp);
          Serial.print(" °C, Humidity: ");
          Serial.print(hum);
          Serial.println(" %");

          // Send telemetry data to ThingsBoard
          if (tb.sendTelemetryData("temperature", temp)) {
            Serial.println("[Sensor Task] Sent temperature");
          }

          if (tb.sendTelemetryData("humidity", hum)) {
            Serial.println("[Sensor Task] Sent humidity");
          }
        } else {
          Serial.println("[Sensor Task] Failed to read valid data from DHT20!");
        }
      } else {
        Serial.println("[Sensor Task] Failed to read from DHT20!");
      }
    }

    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}
