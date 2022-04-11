#define rele 27
#include <WiFi.h>

extern "C" {
  #include "freertos/FreeRTOS.h"
  #include "freertos/timers.h"
}
#include <AsyncMqttClient.h>
#include <ArduinoJson.h>

#define WIFI_SSID "Carlos" // FIBER-LVT-VANESSA
#define WIFI_PASSWORD "Qwerty2018" // samirinha01

#define MQTT_HOST IPAddress(192, 168, 0, 104)
#define MQTT_PORT 5011
#define topic "monitorAgua/vazao"
#define topic2 "dev.to/topic/json"

AsyncMqttClient mqttClient;
TimerHandle_t mqttReconnectTimer;
TimerHandle_t wifiReconnectTimer;


void connectToWifi()
{
  Serial.println("Connecting to Wi-Fi...");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
}

void connectToMqtt()
{
  Serial.println("Connecting to MQTT...");
  mqttClient.connect();
}

void WiFiEvent(WiFiEvent_t event) 
{
    Serial.printf("[WiFi-event] event: %d\n", event);
    switch(event) {
    case SYSTEM_EVENT_STA_GOT_IP:
        Serial.println("WiFi connected");
        Serial.println("IP address: ");
        Serial.println(WiFi.localIP());
        connectToMqtt();
        break;
    case SYSTEM_EVENT_STA_DISCONNECTED:
        Serial.println("WiFi lost connection");
        xTimerStop(mqttReconnectTimer, 0); // ensure we don't reconnect to MQTT while reconnecting to Wi-Fi
        xTimerStart(wifiReconnectTimer, 0);
        break;
    }
}

void onMqttConnect(bool sessionPresent) 
{
  Serial.println("Connected to MQTT.");
  Serial.print("Session present: ");
  Serial.println(sessionPresent);
  uint16_t packetIdSub = mqttClient.subscribe(topic2, 2);
  Serial.print("Subscribing at QoS 2, packetId: ");
  Serial.println(packetIdSub);
//  mqttClient.publish(topic, 0, true, getPayload2().c_str());
//  Serial.println("Publishing at QoS 0");
//  uint16_t packetIdPub1 = mqttClient.publish(topic, 1, true, getPayload2().c_str());
//  Serial.print("Publishing at QoS 1, packetId: ");
//  Serial.println(packetIdPub1);
//  uint16_t packetIdPub2 = mqttClient.publish(topic, 2, true, getPayload2().c_str());
//  Serial.print("Publishing at QoS 2, packetId: ");
//  Serial.println(packetIdPub2);
}

void onMqttDisconnect(AsyncMqttClientDisconnectReason reason) 
{
  Serial.println("Disconnected from MQTT.");

  if (WiFi.isConnected()) {
    xTimerStart(mqttReconnectTimer, 0);
  }
}

void onMqttSubscribe(uint16_t packetId, uint8_t qos) 
{
  Serial.println("Subscribe acknowledged.");
  Serial.print("  packetId: ");
  Serial.println(packetId);
  Serial.print("  qos: ");
  Serial.println(qos);
}

void onMqttUnsubscribe(uint16_t packetId) 
{
  Serial.println("Unsubscribe acknowledged.");
  Serial.print("  packetId: ");
  Serial.println(packetId);
}

void onMqttMessage(char* topicMsg, char* payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total) 
{
  Serial.println("Publish received.");
  Serial.print("  topic: ");
  Serial.println(topicMsg);
  Serial.print("  qos: ");
  Serial.println(properties.qos);
  Serial.print("  dup: ");
  Serial.println(properties.dup);
  Serial.print("  retain: ");
  Serial.println(properties.retain);
  Serial.print("  len: ");
  Serial.println(len);
  Serial.print("  index: ");
  Serial.println(index);
  Serial.print("  total: ");
  Serial.println(total);

  Serial.println("\n\n\n*******************PAYLOAD*****************");
  Serial.println(payload);

  Serial.println("*******************END*****************");

  StaticJsonDocument<200> doc;

  DeserializationError error = deserializeJson(doc, payload);
  if (error) {
      Serial.print(F("deserializeJson() failed: "));
      Serial.println(error.f_str());
      return;
   }
  
  String valor = doc["sent"];

  Serial.print("Valor => "); Serial.print(valor);
  
  
  
}

void onMqttPublish(uint16_t packetId) 
{
  Serial.println("Publish acknowledged.");
  Serial.print("  packetId: ");
  Serial.println(packetId);
}


void InicializaWifiMqtt() 
{

  mqttReconnectTimer = xTimerCreate("mqttTimer", pdMS_TO_TICKS(2000), pdFALSE, (void*)0, reinterpret_cast<TimerCallbackFunction_t>(connectToMqtt));
  wifiReconnectTimer = xTimerCreate("wifiTimer", pdMS_TO_TICKS(2000), pdFALSE, (void*)0, reinterpret_cast<TimerCallbackFunction_t>(connectToWifi));

  WiFi.onEvent(WiFiEvent);

  mqttClient.onConnect(onMqttConnect);
  //mqttClient.onDisconnect(onMqttDisconnect);
  mqttClient.onSubscribe(onMqttSubscribe);
//  mqttClient.onUnsubscribe(onMqttUnsubscribe);
  mqttClient.onMessage(onMqttMessage);
//  mqttClient.onPublish(onMqttPublish);
  mqttClient.setServer(MQTT_HOST, MQTT_PORT);

  connectToWifi();
  Serial.print("Conected #########");

}

String getPayload2() {
 
  StaticJsonDocument<256> doc;

  doc["NameSensor"] = "Temperatura";
  doc["Type"] = "DHT11";
  doc["Value"] = String(random(1.0, 80.0));
  doc["Unit"] = "C";

  String payload;

  serializeJson(doc, payload);

  return payload;
}
void setup() {
  Serial.begin(115200);
  InicializaWifiMqtt();

  
  pinMode(rele, OUTPUT);
  digitalWrite(rele, HIGH);
  delay(500);
  digitalWrite(rele, LOW);
  delay(500);

}

void loop() {

}
