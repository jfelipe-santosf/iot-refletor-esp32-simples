#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h> // instalar biblioteca PubSubClient no gerenciador de bibliotecas do Arduino
#include <time.h>

const char* ssid = "iPFelipe"; // mudar senha do wifi
const char* password = "feijaoarroz"; // mudar senha do wifi

const char* mqtt_server = "2201cadbf6a44205ab10d1d828b056f1.s1.eu.hivemq.cloud";
const char* mqtt_user = "esp32-reflector";
const char* mqtt_password = "Reflector@123"; // mudar senha do mqtt

WiFiClientSecure espClient;
PubSubClient client(espClient);

const int ledPin = 23;
bool schedule_state = false; //false manhã | true noite

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
    digitalWrite(ledPin, LOW);

    delay(500);
    Serial.print(".");

  }

  Serial.println("");
  Serial.println("WiFi conectado");
  digitalWrite(ledPin, HIGH);
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

    Serial.println(String("Status agendamento: ") + (schedule?"ativado":"desativado"));
  }
}

String mqttStateToString(int s){
  switch(s){
    case -4: return "MQTT_CONNECTION_TIMEOUT";
    case -3: return "MQTT_CONNECTION_LOST";
    case -2: return "MQTT_CONNECT_FAILED";
    case -1: return "MQTT_DISCONNECTED";
    case  0: return "MQTT_CONNECTED";
    case  1: return "MQTT_CONNECT_BAD_PROTOCOL";
    case  2: return "MQTT_CONNECT_BAD_CLIENT_ID";
    case  3: return "MQTT_CONNECT_UNAVAILABLE";
    case  4: return "MQTT_CONNECT_BAD_CREDENTIALS";
    case  5: return "MQTT_CONNECT_UNAUTHORIZED";
    default: return String(s);
  }
}

void reconnect() {

  while (!client.connected()) {

    Serial.print("MQTT...");

    String clientId =
      "ESP32-" +
      String(random(0xffff), HEX);

    if (client.connect(clientId.c_str(),
    mqtt_user,
    mqtt_password)) {

      Serial.println("conectado");

      client.subscribe("esp32/iot-refletor-esp32-simples");

    } else {

      int state = client.state();
      Serial.print("erro: ");
      Serial.print(state);
      Serial.print(" (");
      Serial.print(mqttStateToString(state));
      Serial.println(")");

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
    if (digitalRead(ledPin) == LOW && schedule_state == false){
      schedule_state = true;
      digitalWrite(ledPin, HIGH);
      Serial.println("ativando led no horário");
    }
  }else if (time_now.hour >= 00 && time_now.minute >= 00){
    if (digitalRead(ledPin) == HIGH && schedule_state == true){
      schedule_state = false;
      digitalWrite(ledPin, LOW);
      Serial.println("desativando led no horário");
    }
  }
}

void setup() {

  Serial.begin(115200);

  pinMode(ledPin, OUTPUT);

  digitalWrite(ledPin, HIGH);

  setup_wifi();

  espClient.setInsecure(); // para testes; em produção use certificado de servidor
  client.setServer(mqtt_server, 8883);

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