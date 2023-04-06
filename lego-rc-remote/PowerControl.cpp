#include "PowerControl.h"
#include "debug.h"

PowerControl::PowerControl(SimplePort* port) {
  this->port = port;
}

void PowerControl::loop() {
  int powerToSet;
  if (abs(newPower) < 5) {
    powerToSet = 0;
  } else if (newPower < -95) {
    powerToSet = -100;
  } else if (newPower > 95) {
    powerToSet = 100;
  } else if (abs(newPower-currentPower) < 5) {
    return;
  } else {
    powerToSet = newPower;
  }
  if (currentPower == powerToSet) {
    return;
  }

  currentPower = powerToSet;
  DPRINT("Power: ");DPRINTLN(powerToSet);
  port->setPower(powerToSet);
}

void PowerControl::setPower(int power) {
  newPower = power;
}
