#include "Potentiometer.h"

Potentiometer::Potentiometer(
  pin_size_t pin,
  int input_min, int input_center, int input_max
) {
  this->pin = pin;
  this->input_min = input_min;
  this->input_center = input_center;
  this->input_max = input_max;
  this->output_min = -100;
  this->output_center = 0;
  this->output_max = 100;
}

Potentiometer::Potentiometer(
  pin_size_t pin,
  int input_min, int input_center, int input_max,
  int output_min, int output_center, int output_max
) {
  this->pin = pin;
  this->input_min = input_min;
  this->input_center = input_center;
  this->input_max = input_max;
  this->output_min = output_min;
  this->output_center = output_center;
  this->output_max = output_max;
}

int Potentiometer::getValue(uint8_t readCount) {
  int input = 0;
  for (int i=0; i<readCount; i++) {
    input += analogRead(pin);
  }
  input = input / readCount;

  if (input < input_center) {
    return max(
      output_min,
      map(input, input_min, input_center, output_min, output_center)      
    );
  } else {
    return min(
      output_max,
      map(input, input_center, input_max, output_center, output_max)      
    );
  }
}
