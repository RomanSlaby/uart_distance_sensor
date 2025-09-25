#include "uart_distance_sensor.h"

UARTDistanceSensor::UARTDistanceSensor(UARTComponent *parent)
    : PollingComponent(2000), uart(parent), buffer_index(0) {}

void UARTDistanceSensor::setup() {
  // Inicializace
}

void UARTDistanceSensor::update() {
  while (this->uart->available()) {
    uint8_t byte_read = this->uart->read();
    buffer[buffer_index++] = byte_read;

    if (buffer_index >= 4) {
      if (buffer[0] == 0xFF) {
        uint16_t dist = ((uint16_t)buffer[1] << 8) | buffer[2];
        uint8_t checksum = buffer[0] + buffer[1] + buffer[2];
        if (checksum == buffer[3]) {
          float dist_m = dist / 100.0;
          distance_sensor->publish_state(dist_m);
          ESP_LOGD("custom.uart_sensor", "Distance: %.2f m", dist_m);
        } else {
          ESP_LOGW("custom.uart_sensor", "Checksum error");
        }
      } else {
        ESP_LOGW("custom.uart_sensor", "Invalid frame start: %02X", buffer[0]);
      }
      buffer_index = 0;
    }
  }
}
