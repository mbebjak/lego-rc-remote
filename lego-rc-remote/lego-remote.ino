/*  This sketch was made to remote control a LEGO Technic vehicle
    Copyright (C) 2020  hoharald 
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include <Arduino_LSM9DS1.h>
#include <ArduinoBLE.h>
#include <math.h>
#include "TechnicHubProtocol.h"
#include "Potentiometer.h"
#include "debug.h"
#include "TechnicHub.h"
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

const uint8_t steeringReadCount = 20;

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
  // PowerControl powerControl(
  //   hub.initSimplePort(PORT_D)
  // );

  while (hub.isConnected()) {
    if (steeringControl.isReady()) {
      int steeringInput = steeringPot.getValue(steeringReadCount);
      //int powerInput = powerPot.getValue();
      steeringControl.setPosition(steeringInput);
      //powerControl.setPower(powerInput);
    }

    hub.loop();
    steeringControl.loop();
    //powerControl.loop();
    
    delay(100);
  }

  BLE.scanForName("Technic Hub");
}