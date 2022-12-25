#define BLYNK_TEMPLATE_ID "copy from blynk"
#define BLYNK_DEVICE_NAME "Gas Detection"
#define BLYNK_AUTH_TOKEN "copy from blynk"
//firebase
#define FIREBASE_AUTH "copy from firebase>project settings>service accounts>database secrets"
#define FIREBASE_HOST "database URL"
//firebase
#define BLYNK_PRINT Serial

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <FirebaseESP32.h>
BlynkTimer timer;
FirebaseData firebaseData;
FirebaseJson json;

char auth[] =BLYNK_AUTH_TOKEN;


char ssid[] = "";
char pass[] = "";

#define MQ135 34
#define LED 13 
int sensorValue = 0;
boolean state = false;

void setup()
{
  pinMode(LED,OUTPUT);
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);
  Firebase.begin(FIREBASE_HOST,FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);
  pinMode(MQ135, INPUT);
  timer.setInterval(1000L, sendUptime);
}

void sendUptime()
{
  sensorValue = analogRead(MQ135);
  Blynk.virtualWrite(V1, sensorValue);
  json.set("/PPM",sensorValue);
  Firebase.updateNode(firebaseData,"/Air",json);
  Serial.println(sensorValue);
  if (sensorValue > 1500)
  {
    Blynk.logEvent("notify",String("Gas detektovan"));
    digitalWrite(LED,HIGH);
  }
   else{
    digitalWrite(LED,LOW);
  }
}

void loop()
{
  Blynk.run();
  timer.run();
}