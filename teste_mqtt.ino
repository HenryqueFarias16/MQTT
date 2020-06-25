//Bibliotecas
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>
#include <ArduinoOTA.h>
#include <WebSocketsClient.h>
#include <DNSServer.h>
#include <WiFiUdp.h>
#include <PubSubClient.h>
// ------------------------------------------------------------------------
//Define a pinagem do ESP8266
#define D1    16  //D0
WiFiClient espClient;
PubSubClient client(espClient);
WiFiManager wifiManager;
//------------------------------------------------------------------------
// Configuração do MQTT
const char* mqttServer = "soldier.cloudmqtt.com";                         // O endereço do seu servidor MQTT;
const int   mqttPort = 17614;                                             // A porta MQTT;
const char* mqttUser = "oomwckwg";                                        // O nome de usuario MQTT;
const char* mqttPassword = "1b4hfdhKfdjj";  // A senha MQTT;
char msgMQTT[10];
//------------------------------------------------------------------------
// Declaração das Funções
void mqtt_callback(char* topic, byte* payload, unsigned int length);
void EnvioPacote();
void EnvioPacote1();
void EnvioPacote2();
void OTA();
void WIFImanager();
void MQTT();
//------------------------------------------------------------------------
//Setup
void setup() {
  pinMode(2, OUTPUT);
  digitalWrite(2, LOW);
  Serial.begin(9600);
  Serial.println("Booting");
  //iniciar WifiManager
  WIFImanager();
  //iniciar OTA
  OTA ();
  //Inicialização MQTT
  MQTT();
  EnvioPacote();
  delay(500);
  client.publish("ESP", "fim");
  client.subscribe("ESP01",3);
  /*EnvioPacote1();
  delay(500);
  client.publish("ESP", "fim");
  EnvioPacote2();
  delay(500);*/
}

//------------------------------------------------------------------------
void EnvioPacote() {                                                    // MONITORA O SISTEMA, INFORMANDO AO USUARIO SEU STATUS;
  for (int g = 1; g <= 5; g++) {
    for (int i = 1; i <= 10; i++) {
      dtostrf(i, 5, 0, msgMQTT);
      client.publish("ESP01", msgMQTT, 3);
      Serial.println(i);
    }

  }

  Serial.println("--------");
}
void EnvioPacote1() {                                                    // MONITORA O SISTEMA, INFORMANDO AO USUARIO SEU STATUS;
  for (int h = 1; h <= 5; h++) {
    for (int j = 1; j <= 20; j++) {
      dtostrf(j, 5, 0, msgMQTT);
      client.publish("ESP01", msgMQTT, 1);
      Serial.println(j);
    }
  }
  Serial.println("--------");
}
void EnvioPacote2() {                                                    // MONITORA O SISTEMA, INFORMANDO AO USUARIO SEU STATUS;
  for (int z = 1; z <= 5; z++) {
    for (int x = 1; x <= 30; x++) {
      dtostrf(x, 5, 0, msgMQTT);
      client.publish("ESP01", msgMQTT, 1);
      Serial.println(x);
    }

  }
  Serial.println("--------");
}
//------------------------------------------------------------------------
void loop() {

  ArduinoOTA.handle();                                                    // STARTA O OTA

}
void callback(char* topic, byte* payload, unsigned int length) {
  client.subscribe("ESP01");
  Serial.print("A mensagem chegou no tópico: ");
  Serial.println(topic);
  Serial.print("Messagem:");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    String msg;
    //obtem a string do payload recebido
    for (int i = 0; i < length; i++)    {
      char c = (char)payload[i];
      msg += c;
    }
    if (msg.equals("B1"))    {
      // Disponibiliza acesso a função de controle;
    }
  }
  Serial.println();
  Serial.println("------------------------------------------------------");

}
void OTA () {
  //Inicialização OTA
  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH) {
      type = "sketch";
    } else { // U_FS
      type = "filesystem";
    }
    Serial.println("Iniciar atualização " + type);
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    //Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) {
      Serial.println("Falha na autenticação");
    } else if (error == OTA_BEGIN_ERROR) {
      Serial.println("Falha no início");
    } else if (error == OTA_CONNECT_ERROR) {
      Serial.println("Falha na conexão");
    } else if (error == OTA_RECEIVE_ERROR) {
      Serial.println("Falha no recebimento");
    } else if (error == OTA_END_ERROR) {
      Serial.println("Fim com falha");
    }
  });
  ArduinoOTA.begin();
  Serial.print("Pronto:  ");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}
//------------------------------------------------------------------------
void WIFImanager() {
  //wifiManager.resetSettings();                                  //utilizando esse comando, as configurações são apagadas da memória
  wifiManager.setConfigPortalTimeout(180);
  wifiManager.autoConnect("ESPWemos", "12345678");
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Falha na conexão! Reiniciando ...");
    delay(5000);
    ESP.restart();
  }
}
//------------------------------------------------------------------------
void MQTT() {
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);
  while (!client.connected()) {
    Serial.println("Conectando ao MQTT...");
    if (client.connect("ESP8266Client", mqttUser, mqttPassword )) {
      Serial.println("connectado ao MQTT");
    } else {
      Serial.print("Falhou com o estado ");
      Serial.print(client.state());
      client.publish("Esp", "Erro");
      delay(2000);
    }
  }
}
