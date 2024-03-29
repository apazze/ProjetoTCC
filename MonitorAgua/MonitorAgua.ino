//BIBLIOTECAS E VARIAVEIS

//WIFI E MQTT
#include <WiFi.h>

extern "C" {
  #include "freertos/FreeRTOS.h"
  #include "freertos/timers.h"
}

#include <AsyncMqttClient.h>

#define WIFI_SSID "ALISSON-PC" // FIBER-LVT-VANESSA // Adelia ACESSONET // ASP // ALISSON
#define WIFI_PASSWORD "12345678" // samirinha01 //02645674 // 12345678 // Qwerty2018

#define MQTT_HOST IPAddress(192, 168, 137, 1) //104
#define MQTT_PORT 5011
#define topic "monitorAgua/vazao"
#define topicAdvertencia "monitorAgua/advertencia"

AsyncMqttClient mqttClient;
TimerHandle_t mqttReconnectTimer;
TimerHandle_t wifiReconnectTimer;

//RELOGIO
//Pinos SDA na GPIO 21 e SCL na GPIO 22 (ESP32)

#include <Wire.h>
#include "RTClib.h"
RTC_DS1307 rtc;
char diasDaSemana[7][12] = {"Domingo", "Segunda", "Terca", "Quarta", "Quinta", "Sexta", "Sabado"};

//VAZAO
#define portaVazao 13

volatile int pulsos_vazao = 0;
unsigned long ultimo_valor_salvo = 0;
float vazao = 0;
float vazao_somando = 0;
float MiliLitros = 0;
long lastMsg = 0;
bool gastandoAgora = false;
bool masGastou = false;

//RELE

#define RELE 27

//MONTAR PAYLOAD JSON

#include <ArduinoJson.h>


// =============================================================================


//RELOGIO FUNCTIONS
void InicializaRTC()
{
  if (! rtc.begin())
  {
    Serial.println("RTC NAO INICIALIZADO");
    while(1);
  }
    Serial.print("\n\nRTC INICIALIZADO COM SUCESSO ");
    ImprimeData();
  
  //Necessita ajustar o relogio manualmente na primeira utilização:
  //rtc.adjust(DateTime(2022, 4, 3, 19, 00, 00));
  
  delay(100);
}
void ImprimeData()
{
  DateTime now = rtc.now();
  String data = String(now.day()) + "/" + String(now.month()) + "/" + String(now.year());
  String hora = String(now.hour()) + ":" + String(now.minute()) + ":" + String(now.second());
  String dataEHora = "[ " + String(diasDaSemana[now.dayOfTheWeek()]) + ", " + data + " " + hora + " ]";
  Serial.print(dataEHora);
}


//VAZAO FUNCTIONS

void InicializaVazao()
{
  Configurar_interrupcao((gpio_num_t) portaVazao);
}

// funçao chamada cada vez que o sensor de fluxo mandar um pulso
void IRAM_ATTR Interrupcao(void* arg)
{ 
  //soma a variavel de contagem de pulsos do sensor de fluxo de agua
  pulsos_vazao++;
  
  portYIELD_FROM_ISR();
}
void Configurar_interrupcao(gpio_num_t Port) 
{
  //configura pino como entrada
  pinMode(portaVazao, INPUT_PULLUP);
  
  //tipo de interrupção
  gpio_set_intr_type(Port, GPIO_INTR_NEGEDGE);

  //ativa a porta
  gpio_intr_enable(Port);    
                    
  //instala a interrupção
  gpio_install_isr_service(0);
  
  //o que fazer ao detectar a interrupção
  gpio_isr_handler_add(Port, Interrupcao, (void*) Port);
}

//WIFI AND MQTT FUNCTIONS

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
  uint16_t packetIdSub = mqttClient.subscribe(topicAdvertencia, 2);
  Serial.print("Subscribing at QoS 2, packetId: ");
  Serial.println(packetIdSub);

  //String msgParaEnvio = String(random(3.0, 10.0));
  //EnviaMsg(msgParaEnvio);
  
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

  DeserializaPayloadJson(payload);
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
  //mqttClient.onSubscribe(onMqttSubscribe);
  //mqttClient.onUnsubscribe(onMqttUnsubscribe);
  mqttClient.onMessage(onMqttMessage);
  //mqttClient.onPublish(onMqttPublish);
  mqttClient.setServer(MQTT_HOST, MQTT_PORT);

  connectToWifi();
  Serial.print("Conected #########");

}


//MONTAR PAYLOAD

String getPayload(String nameSensor, String type, String value, String unit)
{
  StaticJsonDocument<256> doc;

  doc["NameSensor"] = nameSensor;
  doc["Type"] = type;
  doc["Value"] = value;
  doc["Unit"] = unit;

  String payload;

  serializeJson(doc, payload);

  return payload;
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

// ADVERTENCIA RELE FUNCTION

void InicializaRele()
{
  pinMode(RELE, OUTPUT);
  digitalWrite(RELE, HIGH);
  delay(500);
  digitalWrite(RELE, LOW);
  delay(500);
}

void AcionaSirene()
{
  unsigned short quantidadeBeeps = 2;
  unsigned short intervalo = 200;
  Serial.println("\n\nRecebido msg para advertência!\n\n");
  while(quantidadeBeeps > 0)
  {
    digitalWrite(RELE, HIGH);
    delay(intervalo);
    digitalWrite(RELE, LOW);
    delay(intervalo);
    quantidadeBeeps--;
  }
}

void DeserializaPayloadJson(char* payload)
{
  StaticJsonDocument<200> doc;

  DeserializationError error = deserializeJson(doc, payload);
  if(error)
  {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  }
  
  bool acionarSirene = doc["acionarSirene"];
  String dataEnvio = doc["dataEnvio"];

  Serial.print("Lido => "); 
  Serial.print(acionarSirene); Serial.print(" | ");
  Serial.print(dataEnvio);

  if(acionarSirene)
    AcionaSirene();

  
}

//CORE FUNCTION

void MonitorVazaoAgua()
{
  long agora = millis();
  // caso ja tenha passado 1 segundo do ultimo dado aferido e salvo
  if ((millis() - ultimo_valor_salvo) > 1000)
  { 
    ultimo_valor_salvo = millis();
    //guarda as informaçoes de data e tempo no objeto now
    DateTime now = rtc.now();
 
    //converte a quantia de pulsos que o sensor mandou para a vazao da agua em litros por minuto
    vazao = pulsos_vazao / 5.5;
    pulsos_vazao = 0;
    MiliLitros = vazao / 60;

    // calcula a vazao total do dia
    vazao_somando = vazao_somando + MiliLitros;

    //As 23:59:59 se tiver consumo, envia valor
    //if((now.hour() == 23 && now.minute() == 59 && now.second() == 59))
    //if(now.second() % 10 == 0)

    //verifica a cada 10s se gastou e nao esta gastando no momento pra enviar
    if(now.second() % 10 == 0 && masGastou && !gastandoAgora)
    {
       masGastou = false;
       lastMsg = agora;
       Serial.println(" ---------------------------------- ");
       Serial.println(vazao_somando);
       Serial.println(" ---------------------------------- ");
       
       //double val = random(0,99)/100.0;
       //val+=random(2.0, 50.0);
       //String msgParaEnvio = String(val);
       
       String msgParaEnvio = String(vazao_somando);
       EnviaMsg(msgParaEnvio);
       
       //reinicia a contagem da vazao de agua diario
       vazao_somando = 0;
    }
    
    // Esta consumindo agua agora? Entao mostra o valor da leitura do sensor
    if(MiliLitros > 0)
    {
      Serial.print(" Sensor de Vazao esta registrando "); Serial.print(MiliLitros); Serial.println(" litros/Segundo");
      gastandoAgora = true;
      masGastou = true;
    }else{
      gastandoAgora = false;
    }
    
  }
}

void EnviaMsg(String msgParaEnvio)
{
  String msg = getPayload("Vazao", "Encoder", msgParaEnvio, "L");
  mqttClient.publish(topic, 0, true, msg.c_str());
}


void setup()
{
  Serial.begin(115200);
  InicializaRTC();
  InicializaVazao();
  InicializaWifiMqtt();
  InicializaRele();
}

void loop()
{
  MonitorVazaoAgua();  
}
