#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#include <Button.h>
#include <Led.h>
#include <Buzzer.h>
#include <Pir.h>
#include <MagneticSensor.h>

#define BTN 0
#define LEDR 2
#define LEDG 4
#define LEDB 5

#define BUZZER 16
#define PIR 14
#define MAGNETICSENSOR 12

Button btn;
Led ledR;
Led ledG;
Led ledB;

Buzzer buzzer;
Pir pir;
MagneticSensor magneticSensor;

const char* ssid = "nombre-de-la-red";
const char* password = "password-de-la-red";
const char* mqtt_server = "ip-del-broker";

char id[50];

WiFiClient espClient;
PubSubClient client(espClient);

long lastMsg = 0;
char msg[50];
int value = 0;

bool isActive = false;

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // Switch on the LED if an 1 was received as first character
  if((char)payload[0] == 'R'){
    if((char)payload[1] == '1'){
      ledR.set(true);
    }else{
      ledR.set(false);
    }
  }

  if((char)payload[0] == 'G'){
    if((char)payload[1] == '1'){
      ledG.set(true);
    }else{
      ledG.set(false);
    }
  }

  if((char)payload[0] == 'B'){
    if((char)payload[1] == '1'){
      ledB.set(true);
    }else{
      ledB.set(false);
    }
  }

  //Aqui procesamos los comandos recibidos
  if((char)payload[0] == 'C'){
    if((char)payload[1] == '0'){
      buzzer.set(false);
      client.publish("server/espino", "Bocina apagada!");
    }

    if((char)payload[1] == '1'){
      if(isActive){
        client.publish("server/espino", "La alarma esta encendida!");
      }else{
        client.publish("server/espino", "La alarma esta apagada!");
      }
    }
  }

  if((char)payload[0] == 'A'){
    if((char)payload[1] == '1'){
      isActive = true;
      client.publish("server/espino", "Alarma encendida!");
    }else{
      isActive = false;
      buzzer.set(false);
      client.publish("server/espino", "Alarma apagada!");
    }
  }
}

void setup() {
  //Setup de nuestros botones y leds
  btn.begin(BTN);
  ledR.begin(LEDR);
  ledG.begin(LEDG);
  ledB.begin(LEDB);

  //Setup de nuestros sensores y el buzzer
  buzzer.begin(BUZZER);
  pir.begin(PIR, &buzzer, &client);
  magneticSensor.begin(MAGNETICSENSOR, &buzzer, &client);
  //
  pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect(WiFi.localIP().toString().c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("server/clients", WiFi.localIP().toString().c_str());
      // ... and resubscribe
      snprintf (id, 75, "Espino/%s", WiFi.localIP().toString().c_str());
      client.subscribe(id); //Para llamadas especificas
      client.subscribe("Espino/commands"); //Para llamadas generales
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void loop() {

  if (!client.connected()) {
    reconnect();
  }

  client.loop();

  if(isActive){
    pir.loop();
    magneticSensor.loop();
  }

//Apagar el buzzer con el boton | Solo para propositos de DEBUG
  if(btn.pressed()){
    buzzer.set(false);
    client.publish("server/espino", "Bocina apagada!");
    Serial.println("Bocina apagada!");
  }

  delay(1000);
  /*
  long now = millis();
  if (now - lastMsg > 2000) {
    lastMsg = now;
    ++value;
    snprintf (msg, 75, "hello world #%ld", value);
    Serial.print("Publish message: ");
    Serial.println(msg);
    client.publish("outTopic", msg);
  }
  */
}
