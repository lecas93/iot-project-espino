#include "MagneticSensor.h"

void MagneticSensor::begin(uint8_t _pin, Buzzer *buzzer, PubSubClient* client){
  pin = _pin;
  this->buzzer = buzzer;
  this->client = client;
  pinMode(pin, INPUT_PULLUP);
}

void MagneticSensor::loop(){
  val = get();
  Serial.println("val MS: ");
  Serial.println(val);

  if(val == HIGH){
    if(sensorState == LOW){
      Serial.println("MS: Presencia detectada!");
      sensorState = HIGH;
      client->publish("server/espino", "MS Detection");
      buzzer->set(true);
    }
  }else{
    if(sensorState == HIGH){
      Serial.println("MS: Se ha detenido la presencia!");
      sensorState = LOW;
    }
  }
}

bool MagneticSensor::get(){
  return digitalRead(pin);
}
