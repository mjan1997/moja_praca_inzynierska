// esp32 thing plus 
//czujnik w doniczce


#include <WiFi.h>
#include "Esp32MQTTClient.h"
#include <PubSubClient.h>
#include "Adafruit_seesaw.h"
#include <stdlib.h>
Adafruit_seesaw ss;
#define ssid      "MyRouter2G4"
#define password  "Zosiunia ma blue oczunia"
#define mqtt_server "a256abec4e95e73ce3a3c195b9d6fef32.asuscomm.com"
#define mqtt_port 10
#define MQTT_USER "test-user"
#define MQTT_PASSWORD "MJ_09236"



int minuty = 1; // co ile minut ma publikowac wartosci z czujnika na topic
int potwierdzenie= 0;


WiFiClient wifiClient;
PubSubClient client(wifiClient);
void setup_wifi() {
    delay(10);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
    }
    randomSeed(micros());
}
void reconnect() {
  while (!client.connected()) {
    String clientId = "ESP32Client-";
    clientId += String(random(0xffff), HEX);
    if (client.connect(clientId.c_str(),MQTT_USER,MQTT_PASSWORD)) {
    } else {
      delay(5000);
    }
  }
}

void callback(char* topic, byte* payload, unsigned int length) 
{int p =(char)payload[0]-'0';
if (strcmp(topic,"esp/confirm")==0){//0 == true
  if(p==1)
 { potwierdzenie =1;}
  
}

}
void setup() {


  setup_wifi();

  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
  reconnect(); 
   if (!ss.begin(0x36)) {
    while(1);
  } 
  client.subscribe("esp/confirm");

}

void loop() {

  client.loop();

  float tempC = ss.getTemp();
  uint16_t capread = ss.touchRead(0);
char result[8];
dtostrf(tempC, 6, 2, result);
    char buffer [sizeof(unsigned int)*8+1];
    char * s = (char *) utoa(capread,buffer,10);

  client.publish("esp/wilg1", s); 
  client.publish("esp/temp1", result);   

  if(potwierdzenie == 1)
  {client.publish("esp/confirm", "0");   potwierdzenie =0; delay(minuty * 60000);}
  
  delay(60000);
  
}
