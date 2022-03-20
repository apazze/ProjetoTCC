// BIBLIOTECAS E VARIAVEIS GLOBAIS
//RTC
#include <Wire.h>
#include "RTClib.h"
//SD Card
#include "FS.h"
#include "SD.h"
#include "SPI.h"
//Temperatura
#include <Adafruit_Sensor.h>
#include <DHT.h>
#define DHTPIN 4
#define DHTTYPE DHT11
DHT dht = DHT(DHTPIN, DHTTYPE);
//Leds
#define ledG 14
#define ledR 15
#define ledY 26
//Vazão
#define portaVazao 13

volatile int pulsos_vazao = 0;
unsigned long ultimo_valor_salvo = 0;
float vazao = 0;
float vazao_somando = 0;
float MiliLitros = 0;
 
void IRAM_ATTR Interrupcao(void* arg) { // funçao chamada cada vez que o sensor de fluxo mandar um pulso
  pulsos_vazao++;                  //soma a variavel de contagem de pulsos do sensor de fluxo de agua
  portYIELD_FROM_ISR();
}
 
void Configurar_interrupcao(gpio_num_t Port) {
  pinMode(portaVazao, INPUT_PULLUP);            //configura pino como entrada
  gpio_set_intr_type(Port, GPIO_INTR_NEGEDGE);  //tipo de interrupçao
  gpio_intr_enable(Port);                       //ativa a porta
  gpio_install_isr_service(0);                  //instala a interrupçao
  gpio_isr_handler_add(Port, Interrupcao, (void*) Port); // oque fazer ao detectar a interupçao
}

RTC_DS1307 rtc;
char diasDaSemana[7][12] = {"Domingo", "Segunda", "Terca", "Quarta", "Quinta", "Sexta", "Sabado"};

String nomeDoArquivo = "registra_temperatura";

void setup() {
  //Inicializa comunicação serial
  Serial.begin(9600);
  
  InicializaRTC();
  
  InicializaSDCard();
  
  PreparaSD();
  
  dht.begin();

  //configura pinos dos leds como saída
  pinMode(ledG, OUTPUT);
  pinMode(ledR, OUTPUT);
  pinMode(ledY, OUTPUT);
  
  //chama a funçao que ira configurar a interrupçao
 Configurar_interrupcao((gpio_num_t) portaVazao); 

}




void loop() {
  
  //delay(8000);
  //Serial.println("############# LOOP #############");
  //LeSensorTemperatura();
  //AcionaLeds();

  //LeArquivo();

  //appendFile(SD, "/t.txt", "mensagem de teste1");

  //delay(1000);
  //Serial.print("Conteudo do t.txt: \n\n");
  //readFile(SD, "/t.txt");
  //deleteFile(SD, "/t.txt");

  //Serial.println("VAZAO SOMA => [ ");
  //Serial.print(vazao_somando);
  //Serial.print(" ]\n\n");

  salvar_dados();
   
}

void salvar_dados() {
  if ((millis() - ultimo_valor_salvo) > 1000) { // caso ja tenha passado 1 segundo do ultimo dado aferido e salvo
    ultimo_valor_salvo = millis();
 
    DateTime now = rtc.now(); //guarda as informaçoes de data e tempo no objeto now
 
    //converte a quantia de pulsos que o sensor mandou para a vazao da agua em litros por minuto
    vazao = pulsos_vazao / 5.5;
    pulsos_vazao = 0;
    MiliLitros = vazao / 60;
    vazao_somando = vazao_somando + MiliLitros; // calcula a vazao total do dia
    if(now.hour() == 16 && now.minute() == 56 && now.second() == 59){ // caso ja seja 23:59:59 vamos salvar os dados no cartao sd
       Serial.println(" ---------------------------------- ");
       Serial.println(vazao_somando);
       Serial.println(" ---------------------------------- ");
 
       String dataMessage = String(now.day()) + "/" + String(now.month()) + "/" + String(now.year()) + " ---  " + String(vazao_somando) + " L/dia <br> \n "; // cria a string que sera salva no cartao sd
       Serial.println(dataMessage);
       EditarArquivo(SD, "/Fluxo_de_agua_dia.txt", dataMessage.c_str());
 
      vazao_somando = 0; // reinicia a contagem de vazao de agua diarios
    }
 
    // mostra o valor da leitura do sensor
    if(MiliLitros > 0)
    {
      Serial.print(" Sensor de Vazao esta registrando "); Serial.print(MiliLitros); Serial.println(" litros/Segundo");
    }
      
 
  }
}

//======== SETUP ==========

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
  //rtc.adjust(DateTime(2022, 3, 20, 11, 26, 00));
  
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
void InicializaSDCard()
{
  if(!SD.begin())
  {
    Serial.println("SD NAO INICIALIZADO");
    while(1);
  }
  Serial.print("\n\nSD INICIALIZADO COM SUCESSO ");
  
  uint8_t cardType = SD.cardType();

  if(cardType == CARD_NONE)
  {
    Serial.println("No SD card attached");
    return;
  }
  
  switch(cardType)
  {
    case CARD_MMC:
      Serial.print("[ MMC ]\n");
      break;
    case CARD_SD:
      Serial.print("[ SDSC ]\n\n");
      break;
    case CARD_SDHC:
      Serial.print("[ SDHC ]\n");
      break;
    default:
      Serial.print("[ UNKNOWN ]\n");
      break;
  }
  
  delay(100);
}
void PreparaSD()
{
  
  listDir(SD, "/", 0);
  
  /*removeDir(SD, "/MonitorAgua");
  deleteFile(SD, "/test.txt");
  renameFile(SD, "/hello.txt", "/foo.txt");
  readFile(SD, "/foo.txt");
  testFileIO(SD, "/test.txt");
  createDir(SD, "/MonitorTemperatura");
  testFileIO(SD, "/test.txt");
  */
 
}

// ============================

void AcionaLeds()
{
  
  digitalWrite(ledG, HIGH);
  delay(1000);
  digitalWrite(ledG, LOW);
  delay(1000);
  digitalWrite(ledR, HIGH);
  delay(1000);
  digitalWrite(ledR, LOW);
  delay(1000);
  digitalWrite(ledY, HIGH);
  delay(1000);
  digitalWrite(ledY, LOW);
  delay(1000);
  
}

void LeSensorTemperatura()
{
  
  float temperatura = dht.readTemperature();
  
  if(isnan(temperatura))
  {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  Serial.println("Temperatura: ");
  Serial.print(temperatura);
  Serial.print(" \xC2\xB0");
  Serial.print("C\n");
  
}

void LeArquivo()
{
  String path = "/" + nomeDoArquivo + ".txt";
  Serial.printf("Lendo o arquivo: %s\n", path);
  File file = SD.open(path); // abre o arquivo para a leitura
  if (!file)
  { //caso o arquivo nao tenha sido iniciado ou aberto
    Serial.println("Erro ao abrir arquivo para a leitura");
  }

  while (file.available()) 
  { //se tiver dados pra ler...
    Serial.write(file.read()); //mostre ao navegador do cliente
  }
}

void EditarArquivo(fs::FS &fs, const char * local, const char * mensagem) {
  Serial.printf("editando o arquivo: %s\n", local);
 
  File file = fs.open(local, FILE_APPEND);
  if (!file) {
    Serial.println("Falha ao abrir o arquivo para editar");
    return;
  }
  file.print(mensagem);
  file.close();
}

void EscreveArquivo()
{
  
  String dataMessage = "Bem Vindo...";
  
  EditarArquivo(SD, "/Temperatura.txt", dataMessage.c_str());
  
}



// ========= SD Functions =================
void createDir(fs::FS &fs, const char * path)
{
  
  Serial.printf("Creating Dir: %s\n", path);
  if (fs.mkdir(path))
  {
    Serial.println("Dir created");
  }
  else
  {
    Serial.println("mkdir failed");
  }
  
}

void listDir(fs::FS &fs, const char * dirname, uint8_t levels){
    Serial.printf("Listing directory: %s\n", dirname);

    File root = fs.open(dirname);
    if(!root){
        Serial.println("Failed to open directory");
        return;
    }
    if(!root.isDirectory()){
        Serial.println("Not a directory");
        return;
    }

    File file = root.openNextFile();
    while(file){
        if(file.isDirectory()){
            Serial.print("  DIR : ");
            Serial.println(file.name());
            if(levels){
                listDir(fs, file.name(), levels -1);
            }
        } else {
            Serial.print("  FILE: ");
            Serial.print(file.name());
            Serial.print("  SIZE: ");
            Serial.println(file.size());
        }
        file = root.openNextFile();
    }
}

void removeDir(fs::FS &fs, const char * path){
    Serial.printf("Removing Dir: %s\n", path);
    if(fs.rmdir(path)){
        Serial.println("Dir removed");
    } else {
        Serial.println("rmdir failed");
    }
}

void writeFile(fs::FS &fs, const char * path, const char * message){
    Serial.printf("Writing file: %s\n", path);

    File file = fs.open(path, FILE_WRITE);
    if(!file){
        Serial.println("Failed to open file for writing");
        return;
    }
    if(file.print(message)){
        Serial.println("File written");
    } else {
        Serial.println("Write failed");
    }
    file.close();
}

void appendFile(fs::FS &fs, const char * path, const char * message){
    Serial.printf("Appending to file: %s\n", path);

    File file = fs.open(path, FILE_APPEND);
    if(!file){
        Serial.println("Failed to open file for appending");
        return;
    }
    if(file.print(message)){
        Serial.println("Message appended");
    } else {
        Serial.println("Append failed");
    }
    file.close();
}

void readFile(fs::FS &fs, const char * path){
    Serial.printf("Reading file: %s\n", path);

    File file = fs.open(path);
    if(!file){
        Serial.println("Failed to open file for reading");
        return;
    }

    Serial.print("Read from file: ");
    while(file.available()){
        Serial.write(file.read());
    }
    file.close();
}

void renameFile(fs::FS &fs, const char * path1, const char * path2){
    Serial.printf("Renaming file %s to %s\n", path1, path2);
    if (fs.rename(path1, path2)) {
        Serial.println("File renamed");
    } else {
        Serial.println("Rename failed");
    }
}

void testFileIO(fs::FS &fs, const char * path){
    File file = fs.open(path);
    static uint8_t buf[512];
    size_t len = 0;
    uint32_t start = millis();
    uint32_t end = start;
    if(file){
        len = file.size();
        size_t flen = len;
        start = millis();
        while(len){
            size_t toRead = len;
            if(toRead > 512){
                toRead = 512;
            }
            file.read(buf, toRead);
            len -= toRead;
        }
        end = millis() - start;
        Serial.printf("%u bytes read for %u ms\n", flen, end);
        file.close();
    } else {
        Serial.println("Failed to open file for reading");
    }


    file = fs.open(path, FILE_WRITE);
    if(!file){
        Serial.println("Failed to open file for writing");
        return;
    }

    size_t i;
    start = millis();
    for(i=0; i<2048; i++){
        file.write(buf, 512);
    }
    end = millis() - start;
    Serial.printf("%u bytes written for %u ms\n", 2048 * 512, end);
    file.close();
}

void deleteFile(fs::FS &fs, const char * path){
    Serial.printf("Deleting file: %s\n", path);
    if(fs.remove(path)){
        Serial.println("File deleted");
    } else {
        Serial.println("Delete failed");
    }
}
