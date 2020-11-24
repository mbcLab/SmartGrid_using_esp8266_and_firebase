#include <PZEM004Tv30.h>
#include <ESP8266WiFi.h>
#include "FirebaseESP8266.h"

/* Use software serial for the PZEM
 * Pin 11 Rx (Connects to the Tx pin on the PZEM)
 * Pin 12 Tx (Connects to the Rx pin on the PZEM)
*/
#ifndef STASSID
#define STASSID "MBC Laboratory."
#define STAPSK  "123gogoans"
#endif

const char* ssid     = STASSID;
const char* password = STAPSK;

//#define HardwareSerial Serial2(2);

FirebaseData firebaseData;
PZEM004Tv30 pzem(14,12);
const int relay=2;
int relayStat;

void setup() {
  Serial.begin(115200);
  pinMode (relay, OUTPUT);

  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  
  Firebase.begin("smart-grid-6d934.firebaseio.com", "tFOVrDOF3rkupziVBhagPNRmwouu0h9oVS3tB5U6");
  Firebase.reconnectWiFi(true);
  Firebase.setMaxRetry(firebaseData, 3);
  Firebase.setFloatDigits(2);
  
  Firebase.setInt(firebaseData, "/RELAY_STATUS", 0);
}

void loop() {
  int i;
    if (Serial.available()) {
      i = Serial.read();
      if (i == 48){
        digitalWrite(relay, HIGH);
      }else if (i == 49){
        digitalWrite(relay, LOW);
      }
    }
  
    float voltage = pzem.voltage();
    if( !isnan(voltage) ){
//        Serial.print("Voltage: "); Serial.print(voltage); Serial.println("V");
        Firebase.setFloat(firebaseData, "/Voltage", voltage);
    } else {
        Serial.println("Error reading voltage");
    }

    float current = pzem.current();
    if( !isnan(current) ){
//        Serial.print("Current: "); Serial.print(current); Serial.println("A");
        Firebase.setFloat(firebaseData, "/Current", current);
    } else {
        Serial.println("Error reading current");
    }

    float power = pzem.power();
    if( !isnan(power) ){
//        Serial.print("Power: "); Serial.print(power); Serial.println("W");
        Firebase.setFloat(firebaseData, "/Power", power);
    } else {
        Serial.println("Error reading power");
    }

    float energy = pzem.energy();
    if( !isnan(energy) ){
//        Serial.print("Energy: "); Serial.print(energy,3); Serial.println("kWh");
        Firebase.setFloat(firebaseData, "/Energy", energy);
    } else {
        Serial.println("Error reading energy");
    }

    float frequency = pzem.frequency();
    if( !isnan(frequency) ){
//        Serial.print("Frequency: "); Serial.print(frequency, 1); Serial.println("Hz");
        Firebase.setFloat(firebaseData, "/Frequency", frequency);
    } else {
        Serial.println("Error reading frequency");
    }

    float pf = pzem.pf();
    if( !isnan(pf) ){
//        Serial.print("PF: "); Serial.println(pf);
        Firebase.setFloat(firebaseData, "/PowerFactor", pf);
    } else {
        Serial.println("Error reading power factor");
    }


    relayStat = Firebase.getInt(firebaseData, "/RELAY_STATUS");
    Serial.println(relayStat);
    if (relayStat == 1) {
      Serial.print("\n Relay ON");
      digitalWrite(relay, HIGH);  
   }
   else{
      Serial.print("\n Relay OFF");
      digitalWrite(relay, LOW);  
   }
    
    Serial.println();
    delay(200);
}
