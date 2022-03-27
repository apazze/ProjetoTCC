#include <WiFi.h>
#include <HTTPClient.h>

//Things to change
const char * ssid = "Carlos";
const char * password = "Qwerty2018";

String GOOGLE_SCRIPT_ID = "AKfycbwhcnmC6cSE_P5MKice-T6GX8pfe4rFFNiKUnY9-vuwE9hwzCA"; 
//https://script.google.com/macros/s/AKfycbwhcnmC6cSE_P5MKice-T6GX8pfe4rFFNiKUnY9-vuwE9hwzCA/exec?tag=adc_A0&value=123

const int sendInterval = 5000; 

const char * root_ca=\
"-----BEGIN CERTIFICATE-----\n" \
"MIIRDzCCD/egAwIBAgIRAOp5jK0daD5SCgAAAAE7B48wDQYJKoZIhvcNAQELBQAw\n" \
"RjELMAkGA1UEBhMCVVMxIjAgBgNVBAoTGUdvb2dsZSBUcnVzdCBTZXJ2aWNlcyBM\n" \
"TEMxEzARBgNVBAMTCkdUUyBDQSAxQzMwHhcNMjIwMzE3MTExNDM5WhcNMjIwNjA5\n" \
"MTExNDM4WjAiMSAwHgYDVQQDDBcqLmdvb2dsZXVzZXJjb250ZW50LmNvbTBZMBMG\n" \
"ByqGSM49AgEGCCqGSM49AwEHA0IABLtBNbbUomDu74y57+Yx1+6RC4NcarA6Y85R\n" \
"qctX1D+koX7pXiloyZVSIX4TRVPSXtjSAq7OvjUu5cgYwYZ+Mgmjgg7lMIIO4TAO\n" \
"BgNVHQ8BAf8EBAMCB4AwEwYDVR0lBAwwCgYIKwYBBQUHAwEwDAYDVR0TAQH/BAIw\n" \
"ADAdBgNVHQ4EFgQUqYOW30Ote791LhNApJ3mw+6YeUMwHwYDVR0jBBgwFoAUinR/\n" \
"r4XN7pXNPZzQ4kYU83E1HScwagYIKwYBBQUHAQEEXjBcMCcGCCsGAQUFBzABhhto\n" \
"dHRwOi8vb2NzcC5wa2kuZ29vZy9ndHMxYzMwMQYIKwYBBQUHMAKGJWh0dHA6Ly9w\n" \
"a2kuZ29vZy9yZXBvL2NlcnRzL2d0czFjMy5kZXIwggyTBgNVHREEggyKMIIMhoIX\n" \
"Ki5nb29nbGV1c2VyY29udGVudC5jb22CIGNvbW1vbmRhdGFzdG9yYWdlLmdvb2ds\n" \
"ZWFwaXMuY29tgiIqLmNvbW1vbmRhdGFzdG9yYWdlLmdvb2dsZWFwaXMuY29tghZz\n" \
"dG9yYWdlLmdvb2dsZWFwaXMuY29tghgqLnN0b3JhZ2UuZ29vZ2xlYXBpcy5jb22C\n" \
"G3N0b3JhZ2UubXRscy5nb29nbGVhcGlzLmNvbYIkKi5hcHBzcG90LmNvbS5zdG9y\n" \
"YWdlLmdvb2dsZWFwaXMuY29tgiAqLmNvbnRlbnQtc3RvcmFnZS5nb29nbGVhcGlz\n" \
"LmNvbYIjKi5jb250ZW50LXN0b3JhZ2UtcDIuZ29vZ2xlYXBpcy5jb22CJyouY29u\n" \
"dGVudC1zdG9yYWdlLXVwbG9hZC5nb29nbGVhcGlzLmNvbYIpKi5jb250ZW50LXN0\n" \
"b3JhZ2UtZG93bmxvYWQuZ29vZ2xlYXBpcy5jb22CHyouc3RvcmFnZS11cGxvYWQu\n" \
"Z29vZ2xlYXBpcy5jb22CISouc3RvcmFnZS1kb3dubG9hZC5nb29nbGVhcGlzLmNv\n" \
"bYIMYmxvZ3Nwb3QuY29tgg4qLmJsb2dzcG90LmNvbYIPYnAuYmxvZ3Nwb3QuY29t\n" \
"ghEqLmJwLmJsb2dzcG90LmNvbYIaZG91YmxlY2xpY2t1c2VyY29udGVudC5jb22C\n" \
"HCouZG91YmxlY2xpY2t1c2VyY29udGVudC5jb22CCWdncGh0LmNvbYILKi5nZ3Bo\n" \
"dC5jb22CD2dvb2dsZWRyaXZlLmNvbYIRKi5nb29nbGVkcml2ZS5jb22CFyouZ29v\n" \
"Z2xlc3luZGljYXRpb24uY29tgiEqLnNhZmVmcmFtZS5nb29nbGVzeW5kaWNhdGlv\n" \
"bi5jb22CFWdvb2dsZXVzZXJjb250ZW50LmNvbYIQdXNlcmNvbnRlbnQuZ29vZ4IS\n" \
"Ki51c2VyY29udGVudC5nb29nghoqLnNhbmRib3gudXNlcmNvbnRlbnQuZ29vZ4Il\n" \
"bWFuaWZlc3QuYy5tYWlsLmdvb2dsZXVzZXJjb250ZW50LmNvbYIlbWFuaWZlc3Qu\n" \
"bGgzLWRhLmdvb2dsZXVzZXJjb250ZW50LmNvbYIlbWFuaWZlc3QubGgzLWRiLmdv\n" \
"b2dsZXVzZXJjb250ZW50LmNvbYIlbWFuaWZlc3QubGgzLWRjLmdvb2dsZXVzZXJj\n" \
"b250ZW50LmNvbYIlbWFuaWZlc3QubGgzLWRkLmdvb2dsZXVzZXJjb250ZW50LmNv\n" \
"bYIlbWFuaWZlc3QubGgzLWRlLmdvb2dsZXVzZXJjb250ZW50LmNvbYIlbWFuaWZl\n" \
"c3QubGgzLWRmLmdvb2dsZXVzZXJjb250ZW50LmNvbYIlbWFuaWZlc3QubGgzLWRn\n" \
"Lmdvb2dsZXVzZXJjb250ZW50LmNvbYIlbWFuaWZlc3QubGgzLWR6Lmdvb2dsZXVz\n" \
"ZXJjb250ZW50LmNvbYIibWFuaWZlc3QubGgzLmdvb2dsZXVzZXJjb250ZW50LmNv\n" \
"bYIebWFuaWZlc3QubGgzLnBob3Rvcy5nb29nbGUuY29tghJnb29nbGV3ZWJsaWdo\n" \
"dC5jb22CFCouZ29vZ2xld2VibGlnaHQuY29tgg50cmFuc2xhdGUuZ29vZ4IQKi50\n" \
"cmFuc2xhdGUuZ29vZ4IkKi5kZXYuYW1wNG1haWwuZ29vZ2xldXNlcmNvbnRlbnQu\n" \
"Y29tgiUqLnByb2QuYW1wNG1haWwuZ29vZ2xldXNlcmNvbnRlbnQuY29tgisqLnBs\n" \
"YXlncm91bmQuYW1wNG1haWwuZ29vZ2xldXNlcmNvbnRlbnQuY29tgjQqLnBsYXln\n" \
"cm91bmQtaW50ZXJuYWwuYW1wNG1haWwuZ29vZ2xldXNlcmNvbnRlbnQuY29tgisq\n" \
"LmFpcGxhdGZvcm0tdHJhaW5pbmcuZ29vZ2xldXNlcmNvbnRlbnQuY29tgj0qLmF1\n" \
"ZGlvYm9vay1hZGRpdGlvbmFsLW1hdGVyaWFsLXN0YWdpbmcuZ29vZ2xldXNlcmNv\n" \
"bnRlbnQuY29tgjUqLmF1ZGlvYm9vay1hZGRpdGlvbmFsLW1hdGVyaWFsLmdvb2ds\n" \
"ZXVzZXJjb250ZW50LmNvbYIcKi5hcHBzLmdvb2dsZXVzZXJjb250ZW50LmNvbYIf\n" \
"Ki5zYWZlbnVwLmdvb2dsZXVzZXJjb250ZW50LmNvbYIfKi5zYW5kYm94Lmdvb2ds\n" \
"ZXVzZXJjb250ZW50LmNvbYIgKi5jb21wb3Nlci5nb29nbGV1c2VyY29udGVudC5j\n" \
"b22CKCouY29tcG9zZXItc3RhZ2luZy5nb29nbGV1c2VyY29udGVudC5jb22CIyou\n" \
"Y29tcG9zZXItcWEuZ29vZ2xldXNlcmNvbnRlbnQuY29tgiQqLmNvbXBvc2VyLWRl\n" \
"di5nb29nbGV1c2VyY29udGVudC5jb22CICouZGF0YXBsZXguZ29vZ2xldXNlcmNv\n" \
"bnRlbnQuY29tgigqLmRhdGFwbGV4LXN0YWdpbmcuZ29vZ2xldXNlcmNvbnRlbnQu\n" \
"Y29tgiQqLmRhdGFwbGV4LWRldi5nb29nbGV1c2VyY29udGVudC5jb22CICouZGF0\n" \
"YXByb2MuZ29vZ2xldXNlcmNvbnRlbnQuY29tgi4qLmRhdGFwcm9jLWltYWdlLXN0\n" \
"YWdpbmcuZ29vZ2xldXNlcmNvbnRlbnQuY29tgigqLmRhdGFwcm9jLXN0YWdpbmcu\n" \
"Z29vZ2xldXNlcmNvbnRlbnQuY29tgiUqLmRhdGFwcm9jLXRlc3QuZ29vZ2xldXNl\n" \
"cmNvbnRlbnQuY29tgiIqLmRhdGFmdXNpb24uZ29vZ2xldXNlcmNvbnRlbnQuY29t\n" \
"gioqLmRhdGFmdXNpb24tc3RhZ2luZy5nb29nbGV1c2VyY29udGVudC5jb22CJiou\n" \
"ZGF0YWZ1c2lvbi1kZXYuZ29vZ2xldXNlcmNvbnRlbnQuY29tgiYqLmRhdGFmdXNp\n" \
"b24tYXBpLmdvb2dsZXVzZXJjb250ZW50LmNvbYIuKi5kYXRhZnVzaW9uLWFwaS1z\n" \
"dGFnaW5nLmdvb2dsZXVzZXJjb250ZW50LmNvbYIqKi5kYXRhZnVzaW9uLWFwaS1k\n" \
"ZXYuZ29vZ2xldXNlcmNvbnRlbnQuY29tghsqLmdzYy5nb29nbGV1c2VyY29udGVu\n" \
"dC5jb22CGyouZ2NjLmdvb2dsZXVzZXJjb250ZW50LmNvbYIbKi50dWYuZ29vZ2xl\n" \
"dXNlcmNvbnRlbnQuY29tgiQqLnR1Zi1hdXRvcHVzaC5nb29nbGV1c2VyY29udGVu\n" \
"dC5jb22CHyoudHVmLWRldi5nb29nbGV1c2VyY29udGVudC5jb22CIyoudHVmLXN0\n" \
"YWdpbmcuZ29vZ2xldXNlcmNvbnRlbnQuY29tgicqLmZ1Y2hzaWEtdXBkYXRlcy5n\n" \
"b29nbGV1c2VyY29udGVudC5jb22CMCouZnVjaHNpYS11cGRhdGVzLWF1dG9wdXNo\n" \
"Lmdvb2dsZXVzZXJjb250ZW50LmNvbYI1Ki5mdWNoc2lhLXVwZGF0ZXMtYXV0b3B1\n" \
"c2gtcXVhbC5nb29nbGV1c2VyY29udGVudC5jb22CKyouZnVjaHNpYS11cGRhdGVz\n" \
"LWRldi5nb29nbGV1c2VyY29udGVudC5jb22CLyouZnVjaHNpYS11cGRhdGVzLXN0\n" \
"YWdpbmcuZ29vZ2xldXNlcmNvbnRlbnQuY29tgiEqLm5vdGVib29rcy5nb29nbGV1\n" \
"c2VyY29udGVudC5jb22CISoucGlwZWxpbmVzLmdvb2dsZXVzZXJjb250ZW50LmNv\n" \
"bYIjKi50ZW5zb3Jib2FyZC5nb29nbGV1c2VyY29udGVudC5jb22CLCoudGVuc29y\n" \
"Ym9hcmQtYXV0b3B1c2guZ29vZ2xldXNlcmNvbnRlbnQuY29tgicqLnRlbnNvcmJv\n" \
"YXJkLWRldi5nb29nbGV1c2VyY29udGVudC5jb22CKyoudGVuc29yYm9hcmQtc3Rh\n" \
"Z2luZy5nb29nbGV1c2VyY29udGVudC5jb22CKCoudGVuc29yYm9hcmQtdGVzdC5n\n" \
"b29nbGV1c2VyY29udGVudC5jb22CHyoua2VybmVscy5nb29nbGV1c2VyY29udGVu\n" \
"dC5jb22CJyoua2VybmVscy1zdGFnaW5nLmdvb2dsZXVzZXJjb250ZW50LmNvbYIk\n" \
"Ki5rZXJuZWxzLXRlc3QuZ29vZ2xldXNlcmNvbnRlbnQuY29tMCEGA1UdIAQaMBgw\n" \
"CAYGZ4EMAQIBMAwGCisGAQQB1nkCBQMwPAYDVR0fBDUwMzAxoC+gLYYraHR0cDov\n" \
"L2NybHMucGtpLmdvb2cvZ3RzMWMzL1FxRnhiaTlNNDhjLmNybDCCAQYGCisGAQQB\n" \
"1nkCBAIEgfcEgfQA8gB3AMNl+bNlTzKDx52pjpPXQY9bq3vjJSyY4dLwS7nrQn0j\n" \
"AAABf5fNVYIAAAQDAEgwRgIhAMVB9UsN8H3byDDf9nWaam8BqiO0OJw/D0ZnkUAf\n" \
"Q6R6AiEAz9Pj7ZKpuLUGfEk3EmPORrhhD6vXB5v5mdZrQpgQyvYAdwBGpVXrdfqR\n" \
"IDC1oolp9PN9ESxBdL79SbiFq/L8cP5tRwAAAX+XzVPeAAAEAwBIMEYCIQCHrBf1\n" \
"Pthd4n6zVmoOmWZgYfH+AakgdFE/SVqgCfEPWgIhAKRThS/XjXPWKkBa6bEZqydj\n" \
"kVFAKU4IPCuhcYE2u31QMA0GCSqGSIb3DQEBCwUAA4IBAQCCQVYIknaqymSIdM4N\n" \
"b/mJHUsW8q1K/BJIbET4RqUj9nTKPUZiWF1SjQSi+6U9XVMp/13nXTZrWtM2bqCp\n" \
"x2bd8nd9qb2AulJebjMp4/pxWKR3+agAG5HhTWYQVBX7FD1Wab27+fLsCj0WYYfQ\n" \
"4zNNU/tEmhnbig3WPNS0dxEP7HXPCrGE4FqWFu7mexTSg/QrX5mjXvgT34TAgQLw\n" \
"WrwZfl/RWSQu0CCulFNZhX62zZnwGhPdFER1TAGqTzdCTJLLR2Y1RPVKRBNoKCsH\n" \
"OCPIhRcF0usWk3SaQsRsT8/i2Bly2xGSOBw1tDUao41y7D0l4yEeGM3y473uQOd3\n" \
"uBtN\n" \
"-----END CERTIFICATE-----\n";



WiFiClientSecure client;

void setup() {
  Serial.begin(115200);
  delay(10);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

Serial.println("Started");
Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

Serial.println("Ready to go");
}

void loop() {
  float lightpercentage = getLightPercentage();
  float temp = getTemperature();
  String temp_s(temp);
  String lightPer_s(10);

  sendData("tag=adc_A0&value="+lightPer_s);
  delay(sendInterval);
}

void sendData(String params) {
   HTTPClient http;
   String url="https://script.google.com/macros/s/"+GOOGLE_SCRIPT_ID+"/exec?"+params;
   Serial.print(url);
    Serial.print("Making a request");
    http.begin(url, root_ca); //Specify the URL and certificate
    int httpCode = http.GET();  
    http.end();
    Serial.println(": done "+httpCode);
}
