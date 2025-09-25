#pragma once
#include "esphome.h"

class UARTDistanceSensor : public PollingComponent {
 public:
  Sensor *distance_sensor = new Sensor();
  UARTDistanceSensor(UARTComponent *parent);

  void setup() override;
  void update() override;

 protected:
  UARTComponent *uart;
  uint8_t buffer[4];
  uint8_t buffer_index;
};
