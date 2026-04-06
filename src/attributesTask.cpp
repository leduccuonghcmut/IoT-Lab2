#include "attributesTask.h"
#include "globals.h"
#include "config.h"

void attributesTaskFunction(void *pvParameters) {
  (void) pvParameters;

  Serial.println("[Attributes Task] Started");

  uint32_t lastSendTime = 0;

  while (1) {
    // Send attributes at specified interval
    if (millis() - lastSendTime >= ATTRIBUTES_SEND_INTERVAL) {
      lastSendTime = millis();

      // Check if ThingsBoard is connected
      if (!tbConnected) {
        Serial.println("[Attributes Task] ThingsBoard not connected, skipping attributes send");
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        continue;
      }

      // Send WiFi signal strength
      if (tb.sendAttributeData("rssi", WiFi.RSSI())) {
        Serial.print("[Attributes Task] Sent RSSI: ");
        Serial.println(WiFi.RSSI());
      }

      // Send WiFi channel
      if (tb.sendAttributeData("channel", WiFi.channel())) {
        Serial.print("[Attributes Task] Sent channel: ");
        Serial.println(WiFi.channel());
      }

      // Send WiFi BSSID
      if (tb.sendAttributeData("bssid", WiFi.BSSIDstr().c_str())) {
        Serial.print("[Attributes Task] Sent BSSID: ");
        Serial.println(WiFi.BSSIDstr());
      }

      // Send Local IP
      if (tb.sendAttributeData("localIp", WiFi.localIP().toString().c_str())) {
        Serial.print("[Attributes Task] Sent Local IP: ");
        Serial.println(WiFi.localIP());
      }

      // Send SSID
      if (tb.sendAttributeData("ssid", WiFi.SSID().c_str())) {
        Serial.print("[Attributes Task] Sent SSID: ");
        Serial.println(WiFi.SSID());
      }

      // Send LED state if it changed
      if (attributesChanged) {
        attributesChanged = false;
        if (tb.sendAttributeData(LED_STATE_ATTR, digitalRead(LED_PIN))) {
          Serial.print("[Attributes Task] Sent LED state: ");
          Serial.println(digitalRead(LED_PIN));
        }
      }
    }

    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}
