#include "Buzzer.h"

void Buzzer::begin(uint8_t _pin)
{
  pin = _pin;
  pinMode(pin, OUTPUT);
  set(false);
}

void Buzzer::set(bool on)
{
  digitalWrite(pin, on);
}
