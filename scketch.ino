#include <WiFi.h>
#include <PubSubClient.h> // instalar biblioteca PubSubClient no gerenciador de bibliotecas do Arduino
#include <time.h>

const char* ssid = "Wokwi-GUEST"; // mudar senha do wifi
const char* password = "";

const char* mqtt_server = "broker.hivemq.com";

WiFiClient espClient;
PubSubClient client(espClient);

const int ledPin = 23;

bool schedule = true;

struct Timestamp {
  int hour;
  int minute;
};

void setup_wifi() {

  Serial.println();
  Serial.print("Conectando WiFi");

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {

    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi conectado");
}

void callback(char* topic,
              byte* payload,
              unsigned int length) {

  String mensagem = "";

  for (int i = 0; i < length; i++) {

    mensagem += (char)payload[i];
  }

  Serial.print("Recebido: ");
  Serial.println(mensagem);

  if (mensagem == "ON") {

    digitalWrite(ledPin, HIGH);
  }

  if (mensagem == "OFF") {

    digitalWrite(ledPin, LOW);
  }

  if (mensagem == "OFF-SCHEDULE"){

    schedule = false;
    Serial.println("Agendamento Off");
  }

  if (mensagem == "ON-SCHEDULE"){

    schedule = true;
    Serial.println("Agendamento On");
  }

  if (mensagem.indexOf("SCHEDULE") != -1){

    client.publish("web/iot-refletor-esp32-simples",
    schedule?"ON-SCHEDULE":"OFF-SCHEDULE");

    Serial.println("Status agentamento: " + schedule?"ativado":"desativado");
  }
}

void reconnect() {

  while (!client.connected()) {

    Serial.print("MQTT...");

    String clientId =
      "ESP32-" +
      String(random(0xffff), HEX);

    if (client.connect(clientId.c_str())) {

      Serial.println("conectado");

      client.subscribe("esp32/iot-refletor-esp32-simples");

    } else {

      Serial.print("erro: ");
      Serial.println(client.state());

      delay(2000);
    }
  }
}

Timestamp getTimestamp() {
  struct tm timeinfo;

  if (!getLocalTime(&timeinfo)) {
    Serial.println("Erro ao capturar tempo.");
    return {0, 0};
  }

  char hour[6];
  char minute[6];

  strftime(hour, sizeof(hour), "%H", &timeinfo);
  strftime(minute, sizeof(minute), "%M", &timeinfo);

  return {atoi(hour), atoi(minute)};
}

void handleScheduler(){
  Timestamp time_now = getTimestamp();

  if (time_now.hour >= 18 && time_now.minute >= 00){
    if (digitalRead(ledPin) == LOW){
      digitalWrite(ledPin, HIGH);
      Serial.println("ativando led no horário");
    }
  }else if (time_now.hour >= 00 && time_now.minute >= 00){
    if (digitalRead(ledPin) == HIGH){
      digitalWrite(ledPin, LOW);
      Serial.println("desativando led no horário");
    }
  }
}

void setup() {

  Serial.begin(115200);

  pinMode(ledPin, OUTPUT);

  digitalWrite(ledPin, LOW);

  setup_wifi();

  client.setServer(mqtt_server, 1883);

  client.setCallback(callback);

  configTime(-10800, 0, "pool.ntp.org");
}

void loop() {

  if (!client.connected()) {

    reconnect();
  }

  client.loop();

  if (schedule){

    handleScheduler();
  }
}