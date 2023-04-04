#ifndef _POWER_CONTROL_H_
#define _POWER_CONTROL_H_

#include "TechnicHub.h"

class PowerControl {
  private:
    SimplePort* port;

    int currentPower = 0;
    int newPower = 0;
  public:
    PowerControl(SimplePort* port);

    void setPower(int power);

    void loop();
};

#endif
