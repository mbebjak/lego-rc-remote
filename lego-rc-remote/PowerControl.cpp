#include "PowerControl.h"

PowerControl::PowerControl(SimplePort* port) {
  this->port = port;
}

void PowerControl::loop() {
  if (newPower == currentPower) {
    return;
  }

  currentPower = newPower;
  port->setPower(newPower);
}

void PowerControl::setPower(int power) {
  newPower = power;
}
