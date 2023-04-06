#include "TechnicHub.h"
#include <ArduinoBLE.h>
#include "TechnicHubProtocol.h"
#include "debug.h"

#define GATT_SERVICE "00001623-1212-efde-1623-785feabcd123"
#define GATT_CHARACTERISTIC "00001624-1212-efde-1623-785feabcd123"

TechnicHub::TechnicHub() {
}

void TechnicHub::scanAndConnect() {
  hub = BLE.available();
  
  if (hub)
  {
    DPRINT("Device found: "); DPRINTLN(hub.address());

    DPRINTLN("Connecting ...");

    BLE.stopScan();

    connect();
  }  
}

void TechnicHub::connect() {
  if (hub.connect()) {
    DPRINTLN("Connected");    
  } else {
    DPRINTLN("Failed to connect!");
    return;
  } 

  if(hub.discoverAttributes()) {   
    DPRINTLN("Attributes discovered");
  } else {
    DPRINTLN("ERROR: Attribute discovery failed!");
    hub.disconnect();
    return;
  }

  service = hub.service(GATT_SERVICE);

  if(!service) {
    DPRINTLN("ERROR: Service not found!"); 
    hub.disconnect();
    return;        
  }

  characteristic = hub.characteristic(GATT_CHARACTERISTIC);

  if(!characteristic) {
    DPRINTLN("ERROR: Characteristic not found!");  
    hub.disconnect(); 
    return;      
  } else {
    if (characteristic.subscribe()) {
      DPRINTLN("Successfully subscribed");  
    } else {
      DPRINTLN("ERROR: Was unable to subscribe!");  
    };
  } 
}

void TechnicHub::loop() {
  if (characteristic.valueUpdated()) {
    received_length = characteristic.valueLength();
    characteristic.readValue(&received_buffer, received_length);
    
    //Switch message type
    switch(received_buffer[2]) {
      case (uint8_t)TechnicHubMessageType::portValueSingle:
        int portId = received_buffer[3];
        if (ports[portId] != nullptr) {
          int direction_position;
          memcpy(&direction_position, &received_buffer[4], 4);
          ports[portId]->position = direction_position;
        }      
        break;
    }
  } 
}

bool TechnicHub::isConnected() {
  return hub != NULL && hub.connected();
}

SimplePort* TechnicHub::initABVirtualPort() {
  sendMessage(
    TechnicHubMessageBuilder::createVirtualPortConnect(
      PORT_A,
      PORT_B
    )
  );
  return new SimplePort(*this, 16);
}

SimplePort* TechnicHub::initSimplePort(uint8_t port) {
  if (ports[port] == nullptr) {
    ports[port] = new SimplePort(*this, port);
  }
  return ports[port];
}

bool TechnicHub::sendMessage(uint8_t message) {
  int write_result = characteristic.writeValue(&TechnicHubMessageBuilder::buffer, message);    
  return write_result == 0;
}

void TechnicHub::unsubscribeCharacteristics() {
  characteristic.unsubscribe();
}

SimplePort::SimplePort(TechnicHub& hub, uint8_t portId) {
  this->hub = hub;
  this->portId = portId;
  hub.sendMessage(
    TechnicHubMessageBuilder::createPortInputFormatSetupSingle(
      portId,
      2,
      0x1,
      true
    )
  );
}

void SimplePort::setPower(int8_t power) {
  hub.sendMessage(
    TechnicHubMessageBuilder::createPortOutputStartPower(portId, power)
  );  
}

void SimplePort::setSpeed(int8_t speed, int8_t maxPower, bool useAccelerationProfile, bool useDecelerationProfile) {
  hub.sendMessage(
    TechnicHubMessageBuilder::createPortOutputStartSpeed(portId, speed, maxPower, useAccelerationProfile, useDecelerationProfile)
  );
}

void SimplePort::goToPositionAndHold(int32_t position, int8_t speed, int8_t maxPower, boolean useAccelerationProfile, boolean useDecelerationProfile) {
  hub.sendMessage(
      TechnicHubMessageBuilder::createPortOutputGoToAbsolutePosition(
        portId,
        position,
        speed,
        maxPower,
        TechnicHubPortOutputCommandEndState::_hold,
        useAccelerationProfile,
        useDecelerationProfile
      )
  );
}

void SimplePort::requestPosition() {
  hub.sendMessage(
    TechnicHubMessageBuilder::createPortInformationRequest(portId, TechnicHubPortInformationType::portValue)
  );
}

void SimplePort::unsubscribePosition() {
  hub.unsubscribeCharacteristics();
}

int32_t SimplePort::getPosition() {
  return position;
}