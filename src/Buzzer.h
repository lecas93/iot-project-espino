#ifndef BUZZER_H
#define BUZZER_H

#include <Arduino.h>

class Buzzer {
private:
  uint8_t pin;
public:
  void begin(uint8_t _pin);
  void set(bool on);
};

#endif //BUZZER_H
