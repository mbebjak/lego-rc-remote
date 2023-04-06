#ifndef _TECHNIC_HUB_H_
#define _TECHNIC_HUB_H_

#include "BLEDevice.h"
#include <ArduinoBLE.h>
#include "debug.h"

const uint8_t PORT_A = 0;
const uint8_t PORT_B = 1;
const uint8_t PORT_C = 2;
const uint8_t PORT_D = 3;


class SimplePort;

class TechnicHub {

  private:
    BLEDevice hub;
    BLEService service;
    BLECharacteristic characteristic;

    uint8_t received_length;
    byte received_buffer[64];

    SimplePort* ports[4] = {nullptr,nullptr,nullptr,nullptr};

    void connect();

  public:
    TechnicHub();

    void scanAndConnect();

    bool isConnected();

    void loop();

    void unsubscribeCharacteristics();

    bool sendMessage(uint8_t message);

    SimplePort* initABVirtualPort();

    SimplePort* initSimplePort(uint8_t port);
};

class SimplePort {
  private:
    TechnicHub hub;
    uint8_t portId;

    int32_t position;
  
  public:
    SimplePort();
    
    SimplePort(TechnicHub& hub, uint8_t portId);

    void setPower(int8_t power);

    void setSpeed(int8_t speed, int8_t maxPower, bool useAccelerationProfile, bool useDecelerationProfile);

    void goToPositionAndHold(int32_t position, int8_t speed, int8_t maxPower, boolean useAccelerationProfile, boolean useDecelerationProfile);

    void requestPosition();

    void unsubscribePosition();

    int32_t getPosition();

    friend class TechnicHub;

};

#endif