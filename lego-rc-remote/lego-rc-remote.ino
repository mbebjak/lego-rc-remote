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
  120, 384, 580
);

//const String hubName = "Technic Hub";
const String hubName = "Zetros Hub";

const int steeringInversion = -1;
const int powerInversion = 1;

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

  scanForHub();
}

void scanForHub() {
  DPRINT("Scanning for ");DPRINTLN(hubName);
  BLE.scanForName(hubName);
}

void loop() {
  TechnicHub hub;

  hub.scanAndConnect();

  
  if (!hub.isConnected()) {
    return;
  }  

  DPRINTLN("Initializing ...");
  SteeringControl steeringControl(
    hub.initSimplePort(PORT_D)
  );
  PowerControl powerControl(
    hub.initABVirtualPort()
  );

  while (hub.isConnected()) {
    if (steeringControl.isReady()) {
      int steeringInput = steeringPot.getValue(steeringReadCount);
      int powerInput = powerPot.getValue(powerReadCount);
      //DPRINT("INPUT steering: ");DPRINT(steeringInput);DPRINT(" power: ");DPRINTLN(powerInput);

      steeringControl.setPosition(steeringInput * steeringInversion);
      powerControl.setPower(powerInput * powerInversion);
    }

    hub.loop();
    steeringControl.loop();
    powerControl.loop();
    
    delay(100);
  }

  scanForHub();
}
