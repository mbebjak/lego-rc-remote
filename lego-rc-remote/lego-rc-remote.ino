#include <Arduino_LSM9DS1.h>
#include <ArduinoBLE.h>
#include "debug.h"

#include "TechnicHubProtocol.h"
#include "TechnicHub.h"

#include "Potentiometer.h"

#include "SteeringControl.h"
#include "PowerControl.h"

Potentiometer steeringPot(
  A0,
  65, 395, 700
);

Potentiometer powerPot(
  A7,
  210, 405, 670
);

const uint8_t steeringReadCount = 10;
const uint8_t powerReadCount = 5;

void setup() {
#ifdef DEBUG
  Serial.begin(115200);
  while (!Serial);
#endif
  
  if (!BLE.begin()) {
    DPRINT("Starting BLE failed!");
    while (1);
  }

  DPRINTLN("Scanning for Technic Hub");

  // start scanning for peripheral
  BLE.scanForName("Technic Hub");

}

void loop() {
  TechnicHub hub;

  hub.scanAndConnect();

  
  if (!hub.isConnected()) {
    return;
  }  

  // DPRINTLN("Initializing virtual port");
  // hub.initVirtualPort(PORT_A, PORT_B);

  DPRINTLN("Initializing ...");
  SteeringControl steeringControl(
    hub.initSimplePort(PORT_C)
  );
  PowerControl powerControl(
    hub.initSimplePort(PORT_D)
  );

  while (hub.isConnected()) {
    if (steeringControl.isReady()) {
      int steeringInput = steeringPot.getValue(steeringReadCount);
      int powerInput = powerPot.getValue(powerReadCount);
      DPRINT("INPUT steering: ");DPRINT(steeringInput);DPRINT(" power: ");DPRINTLN(powerInput);

      steeringControl.setPosition(steeringInput);
      powerControl.setPower(powerInput);
    }

    hub.loop();
    steeringControl.loop();
    powerControl.loop();
    
    delay(100);
  }

  BLE.scanForName("Technic Hub");
}
