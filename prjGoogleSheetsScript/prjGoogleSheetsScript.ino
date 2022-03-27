#include <WiFiClientSecure.h>
#include "string.h"

//#include <HTTPClient.h>

//#include <WiFi.h>

//const char* ssid     = "Carlos";     // your network SSID (name of wifi network)
//const char* password = "Qwerty2018"; // your network password

#define WIFI_SSID     "Carlos"
#define WIFI_PASSWORD "Qwerty2018"

//char *server = "script.google.com";  // Server URL

char *server = "howsmyssl.com";  // Server URL

char *GScriptId = "AKfycbyk64JRyQEeQvcOxw_S4UXKFxRn2pEk9rRXIFsSyB_gLZhXJczkGShtX2iqRVZw2qA"; //cpb1

const char* root_ca = \
"-----BEGIN CERTIFICATE-----\n" \
"MIIN1jCCDL6gAwIBAgIRANpcJKruPRmYEgAAAAAFph0wDQYJKoZIhvcNAQELBQAw\n" \
"RjELMAkGA1UEBhMCVVMxIjAgBgNVBAoTGUdvb2dsZSBUcnVzdCBTZXJ2aWNlcyBM\n" \
"TEMxEzARBgNVBAMTCkdUUyBDQSAxQzMwHhcNMjIwMzE3MTAyNjA4WhcNMjIwNjA5\n" \
"MTAyNjA3WjAXMRUwEwYDVQQDDAwqLmdvb2dsZS5jb20wWTATBgcqhkjOPQIBBggq\n" \
"hkjOPQMBBwNCAATtYBIFrigABQ4fmk2FmwNZhOsA5o5Z+V6m1npj4TjYTAxTLHoO\n" \
"Qv2wKY7YfnQD6Jb7yQhc7Jma4UdPV/jplArko4ILtzCCC7MwDgYDVR0PAQH/BAQD\n" \
"AgeAMBMGA1UdJQQMMAoGCCsGAQUFBwMBMAwGA1UdEwEB/wQCMAAwHQYDVR0OBBYE\n" \
"FI0OoPWBuG+vJ2nvFqPxQqYA+RyiMB8GA1UdIwQYMBaAFIp0f6+Fze6VzT2c0OJG\n" \
"FPNxNR0nMGoGCCsGAQUFBwEBBF4wXDAnBggrBgEFBQcwAYYbaHR0cDovL29jc3Au\n" \
"cGtpLmdvb2cvZ3RzMWMzMDEGCCsGAQUFBzAChiVodHRwOi8vcGtpLmdvb2cvcmVw\n" \
"by9jZXJ0cy9ndHMxYzMuZGVyMIIJaAYDVR0RBIIJXzCCCVuCDCouZ29vZ2xlLmNv\n" \
"bYIWKi5hcHBlbmdpbmUuZ29vZ2xlLmNvbYIJKi5iZG4uZGV2ghIqLmNsb3VkLmdv\n" \
"b2dsZS5jb22CGCouY3Jvd2Rzb3VyY2UuZ29vZ2xlLmNvbYIYKi5kYXRhY29tcHV0\n" \
"ZS5nb29nbGUuY29tggsqLmdvb2dsZS5jYYILKi5nb29nbGUuY2yCDiouZ29vZ2xl\n" \
"LmNvLmlugg4qLmdvb2dsZS5jby5qcIIOKi5nb29nbGUuY28udWuCDyouZ29vZ2xl\n" \
"LmNvbS5hcoIPKi5nb29nbGUuY29tLmF1gg8qLmdvb2dsZS5jb20uYnKCDyouZ29v\n" \
"Z2xlLmNvbS5jb4IPKi5nb29nbGUuY29tLm14gg8qLmdvb2dsZS5jb20udHKCDyou\n" \
"Z29vZ2xlLmNvbS52boILKi5nb29nbGUuZGWCCyouZ29vZ2xlLmVzggsqLmdvb2ds\n" \
"ZS5mcoILKi5nb29nbGUuaHWCCyouZ29vZ2xlLml0ggsqLmdvb2dsZS5ubIILKi5n\n" \
"b29nbGUucGyCCyouZ29vZ2xlLnB0ghIqLmdvb2dsZWFkYXBpcy5jb22CDyouZ29v\n" \
"Z2xlYXBpcy5jboIRKi5nb29nbGV2aWRlby5jb22CDCouZ3N0YXRpYy5jboIQKi5n\n" \
"c3RhdGljLWNuLmNvbYIPZ29vZ2xlY25hcHBzLmNughEqLmdvb2dsZWNuYXBwcy5j\n" \
"boIRZ29vZ2xlYXBwcy1jbi5jb22CEyouZ29vZ2xlYXBwcy1jbi5jb22CDGdrZWNu\n" \
"YXBwcy5jboIOKi5na2VjbmFwcHMuY26CEmdvb2dsZWRvd25sb2Fkcy5jboIUKi5n\n" \
"b29nbGVkb3dubG9hZHMuY26CEHJlY2FwdGNoYS5uZXQuY26CEioucmVjYXB0Y2hh\n" \
"Lm5ldC5jboIQcmVjYXB0Y2hhLWNuLm5ldIISKi5yZWNhcHRjaGEtY24ubmV0ggt3\n" \
"aWRldmluZS5jboINKi53aWRldmluZS5jboIRYW1wcHJvamVjdC5vcmcuY26CEyou\n" \
"YW1wcHJvamVjdC5vcmcuY26CEWFtcHByb2plY3QubmV0LmNughMqLmFtcHByb2pl\n" \
"Y3QubmV0LmNughdnb29nbGUtYW5hbHl0aWNzLWNuLmNvbYIZKi5nb29nbGUtYW5h\n" \
"bHl0aWNzLWNuLmNvbYIXZ29vZ2xlYWRzZXJ2aWNlcy1jbi5jb22CGSouZ29vZ2xl\n" \
"YWRzZXJ2aWNlcy1jbi5jb22CEWdvb2dsZXZhZHMtY24uY29tghMqLmdvb2dsZXZh\n" \
"ZHMtY24uY29tghFnb29nbGVhcGlzLWNuLmNvbYITKi5nb29nbGVhcGlzLWNuLmNv\n" \
"bYIVZ29vZ2xlb3B0aW1pemUtY24uY29tghcqLmdvb2dsZW9wdGltaXplLWNuLmNv\n" \
"bYISZG91YmxlY2xpY2stY24ubmV0ghQqLmRvdWJsZWNsaWNrLWNuLm5ldIIYKi5m\n" \
"bHMuZG91YmxlY2xpY2stY24ubmV0ghYqLmcuZG91YmxlY2xpY2stY24ubmV0gg5k\n" \
"b3VibGVjbGljay5jboIQKi5kb3VibGVjbGljay5jboIUKi5mbHMuZG91YmxlY2xp\n" \
"Y2suY26CEiouZy5kb3VibGVjbGljay5jboIRZGFydHNlYXJjaC1jbi5uZXSCEyou\n" \
"ZGFydHNlYXJjaC1jbi5uZXSCHWdvb2dsZXRyYXZlbGFkc2VydmljZXMtY24uY29t\n" \
"gh8qLmdvb2dsZXRyYXZlbGFkc2VydmljZXMtY24uY29tghhnb29nbGV0YWdzZXJ2\n" \
"aWNlcy1jbi5jb22CGiouZ29vZ2xldGFnc2VydmljZXMtY24uY29tghdnb29nbGV0\n" \
"YWdtYW5hZ2VyLWNuLmNvbYIZKi5nb29nbGV0YWdtYW5hZ2VyLWNuLmNvbYIYZ29v\n" \
"Z2xlc3luZGljYXRpb24tY24uY29tghoqLmdvb2dsZXN5bmRpY2F0aW9uLWNuLmNv\n" \
"bYIkKi5zYWZlZnJhbWUuZ29vZ2xlc3luZGljYXRpb24tY24uY29tghZhcHAtbWVh\n" \
"c3VyZW1lbnQtY24uY29tghgqLmFwcC1tZWFzdXJlbWVudC1jbi5jb22CC2d2dDEt\n" \
"Y24uY29tgg0qLmd2dDEtY24uY29tggtndnQyLWNuLmNvbYINKi5ndnQyLWNuLmNv\n" \
"bYILMm1kbi1jbi5uZXSCDSouMm1kbi1jbi5uZXSCFGdvb2dsZWZsaWdodHMtY24u\n" \
"bmV0ghYqLmdvb2dsZWZsaWdodHMtY24ubmV0ggxhZG1vYi1jbi5jb22CDiouYWRt\n" \
"b2ItY24uY29tgg0qLmdzdGF0aWMuY29tghQqLm1ldHJpYy5nc3RhdGljLmNvbYIK\n" \
"Ki5ndnQxLmNvbYIRKi5nY3BjZG4uZ3Z0MS5jb22CCiouZ3Z0Mi5jb22CDiouZ2Nw\n" \
"Lmd2dDIuY29tghAqLnVybC5nb29nbGUuY29tghYqLnlvdXR1YmUtbm9jb29raWUu\n" \
"Y29tggsqLnl0aW1nLmNvbYILYW5kcm9pZC5jb22CDSouYW5kcm9pZC5jb22CEyou\n" \
"Zmxhc2guYW5kcm9pZC5jb22CBGcuY26CBiouZy5jboIEZy5jb4IGKi5nLmNvggZn\n" \
"b28uZ2yCCnd3dy5nb28uZ2yCFGdvb2dsZS1hbmFseXRpY3MuY29tghYqLmdvb2ds\n" \
"ZS1hbmFseXRpY3MuY29tggpnb29nbGUuY29tghJnb29nbGVjb21tZXJjZS5jb22C\n" \
"FCouZ29vZ2xlY29tbWVyY2UuY29tgghnZ3BodC5jboIKKi5nZ3BodC5jboIKdXJj\n" \
"aGluLmNvbYIMKi51cmNoaW4uY29tggh5b3V0dS5iZYILeW91dHViZS5jb22CDSou\n" \
"eW91dHViZS5jb22CFHlvdXR1YmVlZHVjYXRpb24uY29tghYqLnlvdXR1YmVlZHVj\n" \
"YXRpb24uY29tgg95b3V0dWJla2lkcy5jb22CESoueW91dHViZWtpZHMuY29tggV5\n" \
"dC5iZYIHKi55dC5iZYIaYW5kcm9pZC5jbGllbnRzLmdvb2dsZS5jb22CG2RldmVs\n" \
"b3Blci5hbmRyb2lkLmdvb2dsZS5jboIcZGV2ZWxvcGVycy5hbmRyb2lkLmdvb2ds\n" \
"ZS5jboIYc291cmNlLmFuZHJvaWQuZ29vZ2xlLmNuMCEGA1UdIAQaMBgwCAYGZ4EM\n" \
"AQIBMAwGCisGAQQB1nkCBQMwPAYDVR0fBDUwMzAxoC+gLYYraHR0cDovL2NybHMu\n" \
"cGtpLmdvb2cvZ3RzMWMzL2ZWSnhiVi1LdG1rLmNybDCCAQMGCisGAQQB1nkCBAIE\n" \
"gfQEgfEA7wB2ACl5vvCeOTkh8FZzn2Old+W+V32cYAr4+U1dJlwlXceEAAABf5eg\n" \
"31sAAAQDAEcwRQIgKO/qyXDbVstUmyGkus1+NtSfQeBVeaah4uvJ4h5zODUCIQCr\n" \
"IAXRbx0K9/GQGOK/OCcdH04AszWgCoHyR2AZjTaTfQB1AN+lXqtogk8fbK3uuF9O\n" \
"PlrqzaISpGpejjsSwCBEXCpzAAABf5eg364AAAQDAEYwRAIgAr7Yby6/4yctoeiV\n" \
"N84JsUBOFT8H5Wm9/JxeRhLzOOcCIAXVbvK2b8fGEBcnMXaiMEB3A2NknYf3eeKV\n" \
"hkPNl1/BMA0GCSqGSIb3DQEBCwUAA4IBAQAfxzM4OzKzXj4vvS4ian65PHlV1YiY\n" \
"JOonKRWzA3LDjZ0TAF1WMEXiD46yV6HYAYdeQTGin6AaL2P4Z11rCVJPQVHW6UGv\n" \
"8AoqW4QmBer0U3dJpu28UZ7IA2KPUdAJKhukl7Y5M4fotMxydh5nmh/743GZw3g9\n" \
"krSDY8HAFqhr2R9zo2gh42IPUQI4YJcZiU4vnZlaE9NbaJmZmCm357AJ6RhHMC66\n" \
"WZ1O8+UD3EpkFWamhrPEU2GOLFjDDG/SOtXv7BBG0zCn/VRwShCj5kfivZWfWxvN\n" \
"/UrJwT49JDcuuHV7P0SwabvCQurMRd7J6ANO+esgdxGavWsnzT4U3Ac2\n" \
"-----END CERTIFICATE-----\n";



const char* root_ca2 = \
"-----BEGIN CERTIFICATE-----\n" \
"MIIEjjCCA3agAwIBAgISBFyT5YOjU+RAY9RQg69lD0/uMA0GCSqGSIb3DQEBCwUA\n" \
"MDIxCzAJBgNVBAYTAlVTMRYwFAYDVQQKEw1MZXQncyBFbmNyeXB0MQswCQYDVQQD\n" \
"EwJSMzAeFw0yMjAxMjcwMzU4MzNaFw0yMjA0MjcwMzU4MzJaMBwxGjAYBgNVBAMT\n" \
"EXd3dy5ob3dzbXlzc2wuY29tMFkwEwYHKoZIzj0CAQYIKoZIzj0DAQcDQgAEnM+d\n" \
"v8dxH2f9Ilp+ZfTB4T9dFqudvUGFtvv1O2MMbIVIOgGcli7VK+nOjDEIYwHbwgVm\n" \
"y/IHjQ77Kf+xw0Cz8aOCAn0wggJ5MA4GA1UdDwEB/wQEAwIHgDAdBgNVHSUEFjAU\n" \
"BggrBgEFBQcDAQYIKwYBBQUHAwIwDAYDVR0TAQH/BAIwADAdBgNVHQ4EFgQUZ98N\n" \
"P/DHLiIKsnXqUR0a4fCoTVAwHwYDVR0jBBgwFoAUFC6zF7dYVsuuUAlA5h+vnYsU\n" \
"wsYwVQYIKwYBBQUHAQEESTBHMCEGCCsGAQUFBzABhhVodHRwOi8vcjMuby5sZW5j\n" \
"ci5vcmcwIgYIKwYBBQUHMAKGFmh0dHA6Ly9yMy5pLmxlbmNyLm9yZy8wTQYDVR0R\n" \
"BEYwRIINaG93c215c3NsLmNvbYINaG93c215dGxzLmNvbYIRd3d3Lmhvd3NteXNz\n" \
"bC5jb22CEXd3dy5ob3dzbXl0bHMuY29tMEwGA1UdIARFMEMwCAYGZ4EMAQIBMDcG\n" \
"CysGAQQBgt8TAQEBMCgwJgYIKwYBBQUHAgEWGmh0dHA6Ly9jcHMubGV0c2VuY3J5\n" \
"cHQub3JnMIIBBAYKKwYBBAHWeQIEAgSB9QSB8gDwAHYAKXm+8J45OSHwVnOfY6V3\n" \
"5b5XfZxgCvj5TV0mXCVdx4QAAAF+meZKHAAABAMARzBFAiAdEnqlmxYV6doTwQ84\n" \
"qBxTfMOwvulILA08ftd4AakhMgIhAIbJ0jtVLBbqLSrdeXAH6H9jqb0WyGFsSbih\n" \
"0Ay+Vf1eAHYAb1N2rDHwMRnYmQCkURX/dxUcEdkCwQApBo2yCJo32RMAAAF+meZK\n" \
"aQAABAMARzBFAiBtnGnOUjmY0+Kp6a2yDbNAMchzlRgzpHuxe8Kzb+Q2hQIhAJNq\n" \
"pP++F/HhVs0KSDx+gqPAJpMfmRJlu/3gqWceKTbVMA0GCSqGSIb3DQEBCwUAA4IB\n" \
"AQAH/puk6TyKdetT97tI0W1TWADTjJs4Cp8gUulnUS+9/bOBn0KLyavhJML7zAu4\n" \
"y/sYDYxyf5Op0ft3NLa4/bhS1Cx/q+WGCim4EKgeu/oHueuUArFb856dgUlW5S9n\n" \
"ci6ZMIqAj/8na0u+OHOElKXY2yjFNSzrdmanTKH4w/+OPtEXRlFJMmwMkaJp0nxN\n" \
"KecAT9GoM7yuSOEd+o2TuryRZOn1bkZfqatazPR5j8/9vxh6GZRCg9NZzVzqWONN\n" \
"FIjetWeDmTXb93S8jIna5k16+517U9jwvYlPvASadPuM8kupnICzThAl/GMTzd+x\n" \
"mEERr8bkh5XBe/mKIVninLJa\n" \
"-----END CERTIFICATE-----\n";











const int httpsPort = 443;

//TaskHandle_t Task1;
//TaskHandle_t Task2;

WiFiClientSecure client;


//HTTPClient http;


float tensao = 0;
float corrente = 0;
float tensaoFinal = 0;
float correnteFinal = 0;
int correnteInst[1000];
int tensaoInst[1000];
long contaFinal = 0;
long tempoEnvio = 0;

void setup() {
  Serial.begin(115200); 

  connect_wifi();
  connect_host();

  //create a task that will be executed in the Task1code() function, with priority 1 and executed on core 0
//  xTaskCreatePinnedToCore(
//                    Task1code,   /* Task function. */
//                    "Task1",     /* name of task. */
//                    10000,       /* Stack size of task */
//                    NULL,        /* parameter of the task */
//                    1,           /* priority of the task */
//                    &Task1,      /* Task handle to keep track of created task */
//                    1);          /* pin task to core 0 */                  
// 
//  //create a task that will be executed in the Task2code() function, with priority 1 and executed on core 1
//  xTaskCreatePinnedToCore(
//                    Task2code,   /* Task function. */
//                    "Task2",     /* name of task. */
//                    10000,       /* Stack size of task */
//                    NULL,        /* parameter of the task */
//                    1,           /* priority of the task */
//                    &Task2,      /* Task handle to keep track of created task */
//                    0);          /* pin task to core 1 */
 
}

void loop() {
// disableCore0WDT();
// disableCore1WDT();

  //Make a HTTP request:

  client.println("GET https://www.howsmyssl.com/a/check HTTP/1.0");
  client.println("Host: www.howsmyssl.com");
  client.println("Connection: close");
  client.println();


  while (client.connected())
  {
    String line = client.readStringUntil('\n');
    if (line == "\r")
    {
      Serial.println("headers received");
      break;
    }
  }
  //if there are incoming bytes available
  //from the server, read them and print them:
  while (client.available())
  {
    char c = client.read();
    Serial.write(c);
  }
  client.stop();



  
  //send_data("");
  //tensaoFinal = random(10.0, 100.0);
  //correnteFinal = random(101.0, 200.0);
   


}
void connect_wifi()
{
//  delay(10);
//  Serial.print("Connecting to wifi: ");
//  Serial.println(ssid);
//  //Serial.flush();
//  WiFi.begin(ssid, password);
//  while (WiFi.status() != WL_CONNECTED)
//  {
//  
//    delay(500);
//    Serial.print(WiFi.status());
//    Serial.print(".");
//  }
//
//  Serial.println("");
//  Serial.println("WiFi connected");
//  Serial.println("IP address: ");
//  Serial.println(WiFi.localIP());


  delay(10);
  Serial.println("");
  Serial.print("[WIFI] Conectando-se em " + String(WIFI_SSID));

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print(F("[WIFI] SSID: "));
  Serial.println(WIFI_SSID);
  Serial.print(F("[WIFI] IP: "));
  Serial.println(WiFi.localIP());
  Serial.print(F("[WIFI] Mac: "));
  Serial.println(WiFi.macAddress());
  Serial.println("");
  delay(2000);
  

}


void connect_host(void)
{
  delay(10);
  Serial.println("\nStarting connection to server...");
  //client.setInsecure();


  
  client.setCACert(root_ca2); // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<



  
  if(!client.connect(server, httpsPort))
  {
    Serial.println("Connection failed!");
    delay(1000);
  }
  
    Serial.println("Connected to server!");

//while(!http.begin(server))
//{
//  Serial.println("Connection failed!");
//  delay(1000);
//}
//  Serial.println("Connected to server!");
}


void send_data (String Data)
{
 tempoEnvio = millis() - tempoEnvio;
  // Make a HTTP request:
  String Request = String("GET ") + "/macros/s/" + GScriptId + "/exec?" + "value1=" + String(10) + "&value2=" + String(20) + "&value3=" + String(tempoEnvio) + " HTTP/1.1\r\n" + "Host: script.google.com\r\n" + "User-Agent: ESP8266\r\n" + "Connection: close\r\n" + "\r\n\r\n";
  //String Request = "script.google.com/macros/s/AKfycbyk64JRyQEeQvcOxw_S4UXKFxRn2pEk9rRXIFsSyB_gLZhXJczkGShtX2iqRVZw2qA/exec?value1=113&value2=125&value3=55";
  
  
  
  
  
  tempoEnvio = millis();
  contaFinal = 0;
  tensaoFinal = 0;
  correnteFinal = 0;
  client.println(Request);



  
  while (client.connected())
  {
    String line = client.readStringUntil('\n');
    if (line == "\r")
    {
      Serial.println("headers received");
      break;
    }
  }
  //if there are incoming bytes available
  //from the server, read them and print them:
  while (client.available())
  {
    char c = client.read();
    Serial.write(c);
  }
  client.stop();
}
//
//void Task2code( void * pvParameters ){
// 
//   for(;;){
//   connect_host();
//   send_data("");
//    }
//}
//
//void Task1code( void * pvParameters ) {
//  tensaoFinal = random(10.0, 100.0);
//  correnteFinal = random(101.0, 200.0);
//}
