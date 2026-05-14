# Projeto IoT - ESP32 com MQTT e Interface Web

## 📋 Descrição

Projeto de Internet das Coisas (IoT) que implementa um sistema de controle remoto de LED usando um microcontrolador ESP32. O projeto utiliza o protocolo MQTT para comunicação com um broker HiveMQ e fornece uma interface web para controlar o LED de forma remota.

## 📦 Dependências

Para compilar o sketch Arduino, é necessário instalar a seguinte biblioteca:

- **PubSubClient** - Para comunicação MQTT

### Instalação da Biblioteca
1. Abra a Arduino IDE
2. Acesse: `Sketch` → `Incluir Biblioteca` → `Gerenciar Bibliotecas`
3. Procure por "PubSubClient"
4. Clique em "Instalar"

## ⚙️ Configuração

### 1. Configurar Credenciais WiFi

No arquivo `scketch.ino`, altere as linhas:

```cpp
const char* ssid = "Wokwi-GUEST";        // Seu SSID WiFi
const char* password = "";                 // Sua senha WiFi
```

### 2. Configurar Pino do LED

O LED está configurado no pino GPIO 23:

```cpp
const int ledPin = 23;  // Alterar se necessário
```

### 3. Broker MQTT

O projeto utiliza o broker público HiveMQ:

```cpp
const char* mqtt_server = "broker.hivemq.com";
```

## 📡 Tópicos MQTT

- **Tópico de Subscrição**: `esp32/iot-refletor-esp32-simples`
- **Tópico de Publicação**: `web/iot-refletor-esp32-simples`

### Mensagens Suportadas

| Mensagem | Ação |
|----------|------|
| `ON` | Liga o LED |
| `OFF` | Desliga o LED |
| `ON-SCHEDULE` | Ativa o agendamento |
| `OFF-SCHEDULE` | Desativa o agendamento |
| `SCHEDULE` | Consulta status do agendamento |

## 🌐 Interface Web

A interface web (`index.html`) fornece:

- Botões de controle (Ligar/Desligar)
- Toggle switch para agendamento
- Design responsivo com tema escuro
- Comunicação em tempo real via MQTT

## 📝 Como Usar

1. **Carregar o Sketch**: Abra `scketch.ino` na Arduino IDE e faça upload para o ESP32
2. **Abrir Interface Web**: Abra [Ver site online](https://jfelipe-santosf.github.io/iot-refletor-esp32-simples/)
3. **Controlar o LED**: Use os botões ou toggle switch para ligar/desligar o LED
4. **Gerenciar Agendamento**: Use as opções de agendamento conforme necessário

## 🔌 Conexões de Hardware

| Pino ESP32 | Componente |
|-----------|-----------|
| GPIO 23 | LED (com resistor) |
| GND | Ground do LED |

## 🐛 Troubleshooting

- **ESP32 não conecta ao WiFi**: Verifique as credenciais SSID e senha
- **MQTT não conecta**: Verifique se o broker HiveMQ está online e acessível
- **LED não responde**: Confira se o pino GPIO 23 está correto e se o LED está corretamente conectado
- **Interface Web não funciona**: Certifique-se de que o navegador suporta WebSockets e que o MQTT está conectado

## 📚 Recursos Úteis

- [Documentação ESP32](https://docs.espressif.com/projects/esp-idf/en/latest/)
- [PubSubClient Library](https://pubsubclient.knolleary.net/)
- [HiveMQ Broker](https://www.hivemq.com/)
- [Protocolo MQTT](https://mqtt.org/)

## 👤 Autor

Trabalho de IoT desenvolvido como projeto educacional

## 📄 Licença

MIT
