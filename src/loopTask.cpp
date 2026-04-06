#include "loopTask.h"
#include "globals.h"
#include "config.h"

void loopTaskFunction(void *pvParameters) {
  (void) pvParameters;

  Serial.println("[Loop Task] Started");

  while (1) {
    // Call tb.loop() to process messages and keep connection alive
    if (tbConnected) {
      tb.loop();
    }

    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}
