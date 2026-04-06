#ifndef CONFIG_H
#define CONFIG_H

// ==================== PIN CONFIGURATION ====================
#define LED_PIN 48
#define SDA_PIN GPIO_NUM_11
#define SCL_PIN GPIO_NUM_12

// ==================== WIFI CONFIGURATION ====================
constexpr char WIFI_SSID[] = "ACLAB";
constexpr char WIFI_PASSWORD[] = "ACLAB2023";

// ==================== THINGSBOARD CONFIGURATION ====================
constexpr char TOKEN[] = "y8m225l6zv297aarday2";
constexpr char THINGSBOARD_SERVER[] = "app.coreiot.io";
constexpr uint16_t THINGSBOARD_PORT = 1883U;

// ==================== MESSAGE CONFIGURATION ====================
constexpr uint32_t MAX_MESSAGE_SIZE = 1024U;
constexpr uint32_t SERIAL_DEBUG_BAUD = 115200U;

// ==================== ATTRIBUTE CONFIGURATION ====================
constexpr char BLINKING_INTERVAL_ATTR[] = "blinkingInterval";
constexpr char LED_MODE_ATTR[] = "ledMode";
constexpr char LED_STATE_ATTR[] = "ledState";

constexpr uint16_t BLINKING_INTERVAL_MS_MIN = 10U;
constexpr uint16_t BLINKING_INTERVAL_MS_MAX = 60000U;

// ==================== DATA SEND INTERVALS (ms) ====================
constexpr uint32_t TELEMETRY_SEND_INTERVAL = 10000U;   // 10 seconds
constexpr uint32_t ATTRIBUTES_SEND_INTERVAL = 30000U;  // 30 seconds
constexpr uint32_t WIFI_CHECK_INTERVAL = 5000U;        // 5 seconds
constexpr uint32_t IOT_CHECK_INTERVAL = 5000U;         // 5 seconds

// ==================== STACK SIZE CONFIGURATION ====================
constexpr uint32_t WIFI_TASK_STACK_SIZE = 4096;
constexpr uint32_t IOT_TASK_STACK_SIZE = 8192;
constexpr uint32_t SENSOR_TASK_STACK_SIZE = 4096;
constexpr uint32_t ATTRIBUTES_TASK_STACK_SIZE = 4096;
constexpr uint32_t LOOP_TASK_STACK_SIZE = 4096;

// ==================== TASK PRIORITY ====================
constexpr UBaseType_t WIFI_TASK_PRIORITY = 3;
constexpr UBaseType_t IOT_CONNECTION_TASK_PRIORITY = 4;
constexpr UBaseType_t SENSOR_TASK_PRIORITY = 2;
constexpr UBaseType_t ATTRIBUTES_TASK_PRIORITY = 2;
constexpr UBaseType_t LOOP_TASK_PRIORITY = 3;

#endif // CONFIG_H
