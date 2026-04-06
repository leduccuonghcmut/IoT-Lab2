#include "iotTask.h"
#include "globals.h"
#include "config.h"

// ==================== RPC CALLBACKS ====================
RPC_Response setLedSwitchState(const RPC_Data &data) {
  Serial.println("[IoT Task] Received RPC: setLedSwitchValue");
  bool newState = data;
  Serial.print("[IoT Task] LED state change: ");
  Serial.println(newState);
  
  digitalWrite(LED_PIN, newState);
  ledState = newState;
  attributesChanged = true;
  
  return RPC_Response("setLedSwitchValue", newState);
}

// ==================== ATTRIBUTE CALLBACKS ====================
void processSharedAttributes(const Shared_Attribute_Data &data) {
  Serial.println("[IoT Task] Processing shared attributes");
  
  for (auto it = data.begin(); it != data.end(); ++it) {
    if (strcmp(it->key().c_str(), BLINKING_INTERVAL_ATTR) == 0) {
      const uint16_t new_interval = it->value().as<uint16_t>();
      if (new_interval >= BLINKING_INTERVAL_MS_MIN && 
          new_interval <= BLINKING_INTERVAL_MS_MAX) {
        blinkingInterval = new_interval;
        Serial.print("[IoT Task] Blinking interval set to: ");
        Serial.println(new_interval);
      }
    } else if (strcmp(it->key().c_str(), LED_STATE_ATTR) == 0) {
      ledState = it->value().as<bool>();
      digitalWrite(LED_PIN, ledState);
      Serial.print("[IoT Task] LED state set to: ");
      Serial.println(ledState);
    }
  }
  
  attributesChanged = true;
}

// ==================== CALLBACK REGISTRATION ====================
const std::array<RPC_Callback, 1U> callbacks = {
  RPC_Callback{ "setLedSwitchValue", setLedSwitchState }
};

constexpr std::array<const char *, 2U> SHARED_ATTRIBUTES_LIST = {
  LED_STATE_ATTR,
  BLINKING_INTERVAL_ATTR
};

const Shared_Attribute_Callback attributes_callback(
  &processSharedAttributes, 
  SHARED_ATTRIBUTES_LIST.cbegin(), 
  SHARED_ATTRIBUTES_LIST.cend()
);

const Attribute_Request_Callback attribute_shared_request_callback(
  &processSharedAttributes, 
  SHARED_ATTRIBUTES_LIST.cbegin(), 
  SHARED_ATTRIBUTES_LIST.cend()
);

// ==================== IOT TASK FUNCTION ====================
void iotTaskFunction(void *pvParameters) {
  (void) pvParameters;

  Serial.println("[IoT Task] Started");

  while (1) {
    // Check if WiFi is connected
    if (!wifiConnected) {
      vTaskDelay(1000 / portTICK_PERIOD_MS);
      continue;
    }

    // Try to connect to ThingsBoard if not connected
    if (!tb.connected()) {
      Serial.print("[IoT Task] Connecting to: ");
      Serial.print(THINGSBOARD_SERVER);
      Serial.print(" with token ");
      Serial.println(TOKEN);

      if (!tb.connect(THINGSBOARD_SERVER, TOKEN, THINGSBOARD_PORT)) {
        Serial.println("[IoT Task] Failed to connect to ThingsBoard");
        tbConnected = false;
        vTaskDelay(IOT_CHECK_INTERVAL / portTICK_PERIOD_MS);
        continue;
      }

      Serial.println("[IoT Task] Connected to ThingsBoard");
      tbConnected = true;

      // Send initial data
      if (tb.sendAttributeData("macAddress", WiFi.macAddress().c_str())) {
        Serial.println("[IoT Task] Sent MAC address");
      }

      // Subscribe for RPC
      Serial.println("[IoT Task] Subscribing for RPC...");
      if (!tb.RPC_Subscribe(callbacks.cbegin(), callbacks.cend())) {
        Serial.println("[IoT Task] Failed to subscribe for RPC");
      } else {
        Serial.println("[IoT Task] RPC subscription successful");
      }

      // Subscribe for shared attributes updates
      if (!tb.Shared_Attributes_Subscribe(attributes_callback)) {
        Serial.println("[IoT Task] Failed to subscribe for shared attribute updates");
      } else {
        Serial.println("[IoT Task] Shared attributes subscription successful");
      }

      // Request current shared attributes
      if (!tb.Shared_Attributes_Request(attribute_shared_request_callback)) {
        Serial.println("[IoT Task] Failed to request shared attributes");
      } else {
        Serial.println("[IoT Task] Shared attributes request successful");
      }
    } else {
      if (!tbConnected) {
        Serial.println("[IoT Task] Reconnected to ThingsBoard");
        tbConnected = true;
      }
    }

    vTaskDelay(IOT_CHECK_INTERVAL / portTICK_PERIOD_MS);
  }
}
