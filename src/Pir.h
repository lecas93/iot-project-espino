#ifndef PIR_H
#define PIR_H

#include <Arduino.h>
#include <Buzzer.h>
#include <PubSubClient.h>

class Pir {
private:
  uint8_t pin;
  Buzzer* buzzer;
  PubSubClient* client;
  int val = 0;
  int pirState = LOW;
public:
  void begin(uint8_t _pin, Buzzer* buzzer, PubSubClient* client);
  void loop();
  bool get();
};

#endif //PIR_H
