#include "api/Common.h"
#include "SteeringControl.h"
#include "debug.h"

const long directionLimitDelay = 2000;
const long positionReadDelay = 400;

SteeringControl::SteeringControl(SimplePort* port) {
  this->port = port;
}

void SteeringControl::loop() {
  unsigned long currentMillis = millis();
  if (waitTill > 0 && waitTill > currentMillis) {
    return;
  }

  switch (state) {
    case State::idle:
      state = State::left;
      break;
    case State::left:
      port->setSpeed(-40, 40, false, false);
      waitTill = currentMillis + directionLimitDelay;
      state = State::wait_for_left;
      break;
    case State::wait_for_left:
      port->requestPosition();
      waitTill = currentMillis + positionReadDelay;
      state = State::wait_for_left_read;
      break;
    case State::wait_for_left_read:
      left = port->getPosition();
      DPRINT("Steering left: ");DPRINTLN(left);
      state = State::right;
      break;
    case State::right:
      port->setSpeed(+40, 40, false, false);
      waitTill = currentMillis + directionLimitDelay;
      state = State::wait_for_right;
      break;
    case State::wait_for_right:
      port->requestPosition();
      waitTill = currentMillis + positionReadDelay;
      state = State::wait_for_right_read;
      break;
    case State::wait_for_right_read:
      right = port->getPosition();
      DPRINT("Steering right: ");DPRINTLN(right);
      center = left + abs(left - right) / 2;
      DPRINT("Steering center: ");DPRINTLN(center);
      state = State::center;
      break;
    case State::center:
      port->goToPositionAndHold(center, 40, 80, false, false);
      waitTill = currentMillis + 400;
      state = State::done;
      break;
    case State::done:
      steer();
      break;
  }
}

void SteeringControl::steer() {
  if (newPosition == currentPosition) {
    return;
  }        
  currentPosition = newPosition;
  
  int32_t portPosition = center;
  if (abs(newPosition) > 5) {
    if (newPosition < 0) {
      portPosition = map(newPosition, -100,    0, left,    center);
    } else {
      portPosition = map(newPosition,    0,  100, center,  right);
    }
  }
  
  //DPRINT("Steering position: ");DPRINT(newPosition);DPRINT(" - ");DPRINTLN(portPosition);
  port->goToPositionAndHold(portPosition, 80, 80, false, false);
}

bool SteeringControl::isReady() {
  return state == State::done;
}

void SteeringControl::setPosition(int position) {
  newPosition = position;
}
