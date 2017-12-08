#ifndef MAGNETICSENSOR_H
#define MAGNETICSENSOR_H

#include <Arduino.h>
#include <Buzzer.h>
#include <PubSubClient.h>

class MagneticSensor {
private:
  uint8_t pin;
  Buzzer* buzzer;
  PubSubClient* client;
  int val = 0;
  int sensorState = LOW;
public:
  void begin(uint8_t _pin, Buzzer* buzzer, PubSubClient* client);
  void loop();
  bool get();
};

#endif //MAGNETICSENSOR_H
