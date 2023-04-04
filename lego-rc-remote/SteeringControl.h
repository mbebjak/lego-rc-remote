#ifndef _STEERING_CONTROL_H_
#define _STEERING_CONTROL_H_

#include "TechnicHub.h"

class SteeringControl {
  private:
    SimplePort* port;
    unsigned long waitTill = 0;

    int currentPosition = 0;
    int newPosition = 0;

    enum class State {
      idle = 0,
      left = 1,
      wait_for_left = 2,
      wait_for_left_read = 3,
      right = 4,
      wait_for_right = 5,
      wait_for_right_read = 6,
      center = 7,
      done = 8
    } state = State::idle;
  
    int32_t left, center, right;

    void steer();

  public:
    SteeringControl(SimplePort* port);

    bool isReady();

    void setPosition(int position);

    void loop();
};

#endif
