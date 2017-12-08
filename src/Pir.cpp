#include "Pir.h"

void Pir::begin(uint8_t _pin, Buzzer* buzzer, PubSubClient* client)
{
  pin = _pin;
  this->buzzer = buzzer;
  this->client = client;
  pinMode(pin, INPUT);
}

void Pir::loop(){
  val = get();
  Serial.println("val Pir: ");
  Serial.println(val);

  if(val == HIGH){
    if(pirState == LOW){
      Serial.println("PIR: Presencia detectada!");
      pirState = HIGH;
      client->publish("server/espino", "PIR Detection");
      buzzer->set(true);
    }
  }else{
    if(pirState == HIGH){
      Serial.println("PIR: Se ha detenido la presencia!");
      pirState = LOW;
    }
  }
}

bool Pir::get()
{
  return digitalRead(pin);
}
