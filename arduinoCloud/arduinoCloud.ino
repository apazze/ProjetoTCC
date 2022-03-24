/**********************************************************************************
 ** TÍTULO: Arduino IoT Cloud + Alexa + IR + Controle manual 4 relés com DHT11 e feedback em tempo real usando ESP32
  * Clique nos links a seguir para saber mais.
  * Preferências -> URLs do gerenciador de placas adicionais:
  * https://dl.espressif.com/dl/package_esp32_index.json, http://arduino.esp8266.com/stable/package_esp8266com_index.json
  *
  * Baixe o Board ESP32: https://github.com/espressif/arduino-esp32
  * Baixe as bibliotecas
  * Biblioteca ArduinoIoTCloud com todas as dependências: https://github.com/arduino-libraries/ArduinoIoTCloud
  * Biblioteca IRremote: https://github.com/Arduino-IRremote/Arduino-IRremote
 **********************************************************************************/

#include <ArduinoIoTCloud.h>
#include <Arduino_ConnectionHandler.h>
#include <DHT.h>
#include <IRremote.h>

const char THING_ID[]           = "b34a5b19-6924-4619-873a-bd726ef0521c"; //Entre com THING ID
const char DEVICE_LOGIN_NAME[]  = "Esp32"; //Entre com DEVICE ID

const char SSID[]               = "Carlos";    //Insira WiFi SSID (nome)
const char PASS[]               = "Qwerty2018";    //Digite a senha do WiFi
const char DEVICE_KEY[]         = "O4DQCQXTCNBBVM8JZJPB";    //Digite a senha secreta do dispositivo (chave secreta)

#define DHTPIN              4 //RX2 pino conectado com DHT
//#define IR_RECV_PIN         35 //D35 pino conectado com IC receptor IR

// define o GPIO conectado com relés e interruptores
//#define RelayPin1 23  //D23
//#define RelayPin2 22  //D22
//#define RelayPin3 21  //D21
//#define RelayPin4 19  //D19
//
//#define SwitchPin1 13  //D13
//#define SwitchPin2 12  //D12
//#define SwitchPin3 14  //D14
//#define SwitchPin4 27  //D27

#define wifiLed    2   //D2

// Remova o comentário de qualquer tipo que você esteja usando!
#define DHTTYPE DHT11     // DHT 11
//#define DHTTYPE DHT22   // DHT 22, AM2302, AM2321
//#define DHTTYPE DHT21   // DHT 21, AM2301


DHT dht(DHTPIN, DHTTYPE);
//IRrecv irrecv(IR_RECV_PIN);
decode_results results;
//
//int toggleState_1 = 0; //Defina o número inteiro para lembrar o estado de alternância para o relé 1
//int toggleState_2 = 0; //Defina o número inteiro para lembrar o estado de alternância para o relé 2
//int toggleState_3 = 0; //Defina o número inteiro para lembrar o estado de alternância para o relé 3
//int toggleState_4 = 0; //Defina o número inteiro para lembrar o estado de alternância para o relé 4

float temperature1 = 0;
//float humidity1   = 0;
int   reconnectFlag = 0;

//void onSwitch1Change();
//void onSwitch2Change();
//void onSwitch3Change();
//void onSwitch4Change();
//
//CloudSwitch switch1;
//CloudSwitch switch2;
//CloudSwitch switch3;
//CloudSwitch switch4;
CloudTemperatureSensor temperature;

void initProperties(){

  ArduinoCloud.setBoardId(DEVICE_LOGIN_NAME);
  ArduinoCloud.setSecretDeviceKey(DEVICE_KEY);
  ArduinoCloud.setThingId(THING_ID);
//  ArduinoCloud.addProperty(switch1, READWRITE, ON_CHANGE, onSwitch1Change);
//  ArduinoCloud.addProperty(switch2, READWRITE, ON_CHANGE, onSwitch2Change);
//  ArduinoCloud.addProperty(switch3, READWRITE, ON_CHANGE, onSwitch3Change);
//  ArduinoCloud.addProperty(switch4, READWRITE, ON_CHANGE, onSwitch4Change);
  ArduinoCloud.addProperty(temperature, READ, 8 * SECONDS, NULL); //Update temperature value after every 8 seconds
}

WiFiConnectionHandler ArduinoIoTPreferredConnection(SSID, PASS);

void readSensor(){
     
//  float h = dht.readHumidity();
  float t = dht.readTemperature(); // ou dht.readTemperature(true) para Fahrenheit
  
//  if (isnan(h) || isnan(t)) {
  if (isnan(t)) {
    Serial.println("Falha ao ler do sensor DHT!");
    return;
  }
  else {
//    humidity1 = h;
    temperature = t;
    Serial.println(temperature);
  }  
}

void sendSensor()
{
  readSensor();
}

//void ir_remote_control(){
//  if (irrecv.decode(&results)) {
//    
//      switch(results.value){
//          case 0x80BF49B6:  relayOnOff(1); switch1 = toggleState_1; break; //atualize o código HEX
//          case 0x80BFC936:  relayOnOff(2); switch2 = toggleState_2; break; //atualize o código HEX
//          case 0x80BF33CC:  relayOnOff(3); switch3 = toggleState_3; break; //atualize o código HEX
//          case 0x80BF718E:  relayOnOff(4); switch4 = toggleState_4; break; //atualize o código HEX
//          default : break;         
//        }   
//        //Serial.println(results.value, HEX);    
//        irrecv.resume();   
//  } 
//}

//void relayOnOff(int relay) {
//
//  switch (relay) {
//    case 1:
//      if (toggleState_1 == 0) {
//        digitalWrite(RelayPin1, LOW); // ligar o relé 1
//        toggleState_1 = 1;
//        Serial.println("Device1 ON");
//      }
//      else {
//        digitalWrite(RelayPin1, HIGH); // desligue o relé 1
//        toggleState_1 = 0;
//        Serial.println("Device1 OFF");
//      }
//      delay(100);
//      break;
//    case 2:
//      if (toggleState_2 == 0) {
//        digitalWrite(RelayPin2, LOW); // ligar o relé 2
//        toggleState_2 = 1;
//        Serial.println("Device2 ON");
//      }
//      else {
//        digitalWrite(RelayPin2, HIGH); // desligue o relé 2
//        toggleState_2 = 0;
//        Serial.println("Device2 OFF");
//      }
//      delay(100);
//      break;
//    case 3:
//      if (toggleState_3 == 0) {
//        digitalWrite(RelayPin3, LOW); // ligar o relé 3
//        toggleState_3 = 1;
//        Serial.println("Device3 ON");
//      } else {
//        digitalWrite(RelayPin3, HIGH); // desligue o relé 3
//        toggleState_3 = 0;
//        Serial.println("Device3 OFF");
//      }
//      delay(100);
//      break;
//    case 4:
//      if (toggleState_4 == 0) {
//        digitalWrite(RelayPin4, LOW); // ligar o relé 4
//        toggleState_4 = 1;
//        Serial.println("Device4 ON");
//      }
//      else {
//        digitalWrite(RelayPin4, HIGH); // desligue o relé 4
//        toggleState_4 = 0;
//        Serial.println("Device4 OFF");
//      }
//      delay(100);
//      break;
//    default : break;
//  }
//}

//void manual_control() {
//  //Manual Switch Control
//  if (digitalRead(SwitchPin1) == LOW) {
//    delay(200);
//    relayOnOff(1);
//    switch1 = toggleState_1;
//  }
//  else if (digitalRead(SwitchPin2) == LOW) {
//    delay(200);
//    relayOnOff(2);
//    switch2 = toggleState_2;
//  }
//  else if (digitalRead(SwitchPin3) == LOW) {
//    delay(200);
//    relayOnOff(3);
//    switch3 = toggleState_3;
//  }
//  else if (digitalRead(SwitchPin4) == LOW) {
//    delay(200);
//    relayOnOff(4);
//    switch4 = toggleState_4;
//  }
//}

void doThisOnConnect(){
  /* adicione seu código personalizado aqui */
  Serial.println("Placa conectada com sucesso ao Arduino IoT Cloud");
  digitalWrite(wifiLed, HIGH); //Desligue o LED WiFi
}
void doThisOnSync(){
  /* adicione seu código personalizado aqui */
  Serial.println("Propriedades da Coisa sincronizadas");
}

void doThisOnDisconnect(){
  /* add your custom code here */
  Serial.println("Placa desconectada do Arduino IoT Cloud");
  digitalWrite(wifiLed, LOW); //Desligue o LED WiFi
}

void setup() {
  // Inicialize o serial e espere a porta abrir:
  Serial.begin(9600);
  // Este atraso dá a chance de esperar por um Monitor Serial sem bloquear se nenhum for encontrado
  delay(1500);

  // Definido em thingProperties.h
  initProperties();
  dht.begin();
  //irrecv.enableIRIn(); // Start the receiver
  // Conecte-se ao Arduino IoT Cloud
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);

  ArduinoCloud.addCallback(ArduinoIoTCloudEvent::CONNECT, doThisOnConnect);
  ArduinoCloud.addCallback(ArduinoIoTCloudEvent::SYNC, doThisOnSync);
  ArduinoCloud.addCallback(ArduinoIoTCloudEvent::DISCONNECT, doThisOnDisconnect);

  setDebugMessageLevel(2);
  ArduinoCloud.printDebugInfo();

//  pinMode(RelayPin1, OUTPUT);
//  pinMode(RelayPin2, OUTPUT);
//  pinMode(RelayPin3, OUTPUT);
//  pinMode(RelayPin4, OUTPUT);

  pinMode(wifiLed, OUTPUT);
//
//  pinMode(SwitchPin1, INPUT_PULLUP);
//  pinMode(SwitchPin2, INPUT_PULLUP);
//  pinMode(SwitchPin3, INPUT_PULLUP);
//  pinMode(SwitchPin4, INPUT_PULLUP);

  //Durante a partida, todos os relés devem DESLIGAR
//  digitalWrite(RelayPin1, HIGH);
//  digitalWrite(RelayPin2, HIGH);
//  digitalWrite(RelayPin3, HIGH);
//  digitalWrite(RelayPin4, HIGH);
}

void loop() {

  ArduinoCloud.update();
  
  //manual_control();     //Manual Control
  //ir_remote_control();  //Controle Remoto IR
  sendSensor();         //Obter dados do sensor
}

//void onSwitch1Change() {
//  //Controlar o dispositivo
//  if (switch1 == 1)
//  {
//    digitalWrite(RelayPin1, LOW);
//    Serial.println("Device1 ON");
//    toggleState_1 = 1;
//  }
//  else
//  {
//    digitalWrite(RelayPin1, HIGH);
//    Serial.println("Device1 OFF");
//    toggleState_1 = 0;
//  }
//}
//
//void onSwitch2Change() {
//  if (switch2 == 1)
//  {
//    digitalWrite(RelayPin2, LOW);
//    Serial.println("Device2 ON");
//    toggleState_2 = 1;
//  }
//  else
//  {
//    digitalWrite(RelayPin2, HIGH);
//    Serial.println("Device2 OFF");
//    toggleState_2 = 0;
//  }
//}
//
//void onSwitch3Change() {
//  if (switch3 == 1)
//  {
//    digitalWrite(RelayPin3, LOW);
//    Serial.println("Device3 ON");
//    toggleState_3 = 1;
//  }
//  else
//  {
//    digitalWrite(RelayPin3, HIGH);
//    Serial.println("Device3 OFF");
//    toggleState_3 = 0;
//  }
//}
//
//void onSwitch4Change() {
//  if (switch4 == 1)
//  {
//    digitalWrite(RelayPin4, LOW);
//    Serial.println("Device4 ON");
//    toggleState_4 = 1;
//  }
//  else
//  {
//    digitalWrite(RelayPin4, HIGH);
//    Serial.println("Device4 OFF");
//    toggleState_4 = 0;
//  }
//}
