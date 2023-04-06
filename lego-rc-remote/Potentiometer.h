#ifndef _POTENTIOMETER_H_
#define _POTENTIOMETER_H_

#include "Arduino.h"

class Potentiometer {
  private:
    pin_size_t pin;
    int input_min;
    int input_center;
    int input_max;
    int output_min;
    int output_center;
    int output_max;

  public:
    Potentiometer(
      pin_size_t pin,
      int input_min, int input_center, int input_max
    );

    Potentiometer(
      pin_size_t pin,
      int input_min, int input_center, int input_max,
      int output_min, int output_center, int output_max
    );

    int getValue(uint8_t readCount = 5);

};

#endif