#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WebServer.h>
#include "credentials.h"
#include <PubSubClient.h>
const char* MQTT_BROKER = "192.168.178.200";
WiFiClient espClient;
PubSubClient client(espClient);

IRsend irsend(4); // D2

uint16_t PWON[95] = {282, 786,  252, 1820,  252, 810,  280, 756,  250, 810,  250, 1794,  280, 766,  270, 808,  252, 784,  250, 810,  226, 1818,  280, 1800,  272, 1818,  278, 786,  250, 812,  222, 43890,  296, 784,  250, 1822,  276, 788,  222, 816,  246, 790,  246, 840,  220, 1826,  246, 1828,  244, 1854,  244, 1826,  246, 816,  244, 792,  244, 792,  244, 1854,  244, 1826,  246, 43908,  276, 764,  246, 1848,  248, 790,  246, 816,  220, 840,  220, 1826,  246, 816,  244, 792,  246, 790,  246, 816,  246, 1826,  246, 1854,  242, 1830,  246, 790,  244, 818,  244};  // DENON 221C
uint16_t PWOFF[95] = {282, 784,  252, 1800,  272, 810,  252, 786,  250, 810,  250, 786,  252, 1800,  272, 808,  252, 784,  252, 808,  226, 1818,  280, 1800,  298, 1794,  278, 786,  274, 788,  222, 43890,  296, 786,  250, 1822,  250, 814,  222, 840,  222, 790,  246, 1852,  244, 792,  246, 1828,  244, 1852,  246, 1826,  246, 818,  242, 794,  244, 790,  246, 1854,  244, 1826,  246, 43910,  276, 764,  246, 1850,  248, 790,  246, 814,  220, 818,  244, 790,  246, 1854,  244, 792,  246, 792,  244, 818,  244, 1826,  246, 1854,  242, 1830,  244, 790,  246, 816,  244};  // DENON 211C
uint16_t VOLUP[95] = {258, 810,  280, 1770,  272, 810,  278, 756,  250, 810,  226, 1816,  282, 786,  250, 808,  254, 784,  252, 1818,  254, 1818,  306, 1796,  276, 1820,  250, 786,  274, 764,  246, 43910,  276, 786,  248, 1822,  274, 766,  244, 840,  222, 790,  246, 840,  220, 1828,  244, 1828,  244, 1878,  220, 790,  246, 840,  218, 816,  222, 790,  244, 1878,  220, 1828,  246, 43908,  276, 762,  248, 1848,  248, 790,  246, 814,  220, 840,  222, 1826,  246, 840,  220, 816,  222, 792,  244, 1876,  222, 1826,  246, 1878,  218, 1854,  220, 790,  246, 840,  220};  // DENON 223C
uint16_t VOLDOWN[95] = {260, 810,  280, 1770,  274, 810,  280, 754,  280, 780,  226, 810,  278, 1774,  274, 808,  252, 784,  278, 1790,  256, 1816,  308, 1774,  298, 1820,  250, 788,  274, 764,  246, 43888,  298, 788,  246, 1822,  250, 788,  246, 840,  220, 790,  246, 1876,  220, 816,  220, 1828,  244, 1878,  220, 790,  246, 840,  218, 818,  220, 790,  246, 1876,  220, 1828,  246, 43908,  276, 764,  246, 1850,  246, 814,  222, 814,  220, 840,  220, 790,  246, 1876,  220, 816,  222, 790,  244, 1878,  220, 1828,  246, 1876,  218, 1854,  222, 790,  244, 840,  220};  // DENON 213C
uint16_t DVD[95] = {276, 786,  250, 1822,  250, 810,  250, 788,  246, 814,  244, 1828,  246, 1826,  246, 814,  246, 790,  246, 816,  220, 1852,  246, 1828,  246, 1852,  244, 792,  246, 814,  222, 43910,  250, 812,  246, 1826,  248, 814,  222, 816,  246, 790,  244, 816,  244, 790,  246, 1826,  246, 1852,  246, 1828,  246, 814,  222, 814,  248, 790,  244, 1852,  244, 1828,  246, 43886,  274, 788,  248, 1848,  246, 790,  246, 814,  220, 816,  246, 1826,  246, 1852,  244, 790,  248, 790,  246, 814,  246, 1826,  246, 1852,  220, 1852,  246, 790,  246, 816,  244};  // DENON 231C
uint16_t TV[95] = {248, 786,  274, 1822,  250, 810,  250, 788,  248, 814,  244, 1828,  248, 790,  244, 816,  246, 1826,  246, 814,  220, 816,  246, 1826,  246, 1852,  244, 792,  246, 792,  244, 43890,  270, 812,  246, 1804,  270, 790,  246, 814,  246, 790,  244, 816,  244, 1828,  246, 1828,  244, 816,  246, 1828,  246, 1852,  220, 816,  246, 790,  246, 1850,  244, 1828,  246, 43888,  272, 788,  246, 1828,  268, 790,  246, 792,  244, 814,  246, 1828,  246, 814,  244, 792,  246, 1826,  246, 814,  246, 792,  246, 1852,  220, 1852,  246, 790,  244, 816,  244};  // DENON 224C
uint16_t VCR[95] = {252, 786,  252, 1822,  252, 808,  252, 786,  248, 812,  246, 1826,  250, 790,  246, 1850,  246, 1826,  246, 816,  220, 816,  246, 1826,  246, 1854,  244, 790,  246, 792,  244, 43910,  250, 812,  246, 1824,  248, 814,  222, 816,  246, 790,  244, 816,  244, 1828,  246, 790,  246, 816,  246, 1826,  246, 1852,  220, 814,  246, 790,  244, 1852,  244, 1828,  244, 43908,  252, 788,  246, 1848,  248, 790,  246, 814,  220, 816,  246, 1826,  246, 814,  244, 1828,  246, 1826,  246, 816,  246, 790,  246, 1852,  242, 1830,  246, 790,  246, 816,  246};  // DENON 22CC
uint16_t MUTE[95] = {250, 788,  272, 1822,  250, 810,  250, 788,  248, 814,  220, 814,  246, 790,  246, 814,  246, 790,  246, 1852,  220, 1852,  246, 1828,  246, 1852,  244, 792,  246, 792,  244, 43910,  250, 812,  246, 1802,  270, 814,  220, 816,  246, 792,  244, 1852,  244, 1828,  246, 1828,  244, 1852,  246, 790,  246, 814,  220, 816,  246, 790,  244, 1852,  244, 1828,  246, 43886,  274, 788,  246, 1850,  246, 790,  246, 792,  244, 814,  246, 790,  246, 814,  244, 792,  246, 790,  244, 1852,  246, 1826,  246, 1850,  220, 1852,  246, 790,  246, 816,  244};  // DENON 203C
uint16_t NIGHT[95] = {278, 786,  252, 786,  250, 1816,  280, 786,  250, 812,  222, 838,  224, 790,  246, 840,  220, 1826,  246, 1854,  220, 840,  222, 790,  246, 1854,  242, 790,  246, 814,  220, 43888,  270, 814,  244, 790,  248, 1826,  246, 840,  222, 790,  246, 1854,  242, 1830,  244, 1826,  246, 816,  244, 790,  246, 1876,  196, 1876,  222, 790,  246, 1852,  244, 1828,  246, 43878,  280, 788,  248, 838,  220, 1826,  248, 814,  222, 840,  222, 790,  244, 840,  220, 792,  246, 1826,  246, 1876,  220, 790,  246, 840,  196, 1854,  244, 790,  246, 840,  220};  // DENON 1064

ESP8266WebServer server(80);

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <title>Denon</title>
  <script src="http://code.jquery.com/jquery-1.11.0.min.js"></script>
  <style>
    button{font-size:4em;margin:11px;width:45%;height: 256px}
    .btnRed{background-color:#4C0000;color:white;}
    .btnGreen{background-color:#004C00;color:white;}
    .btnBlue{background-color:#00004C;color:white;}
    .btnViolet {background-color:#4C004C;color:white;;width:30%}
  </style>
  <script type="text/javascript">
      function sendVal(newVal){
        $.ajax({
          url: "?btnAction="+newVal,
          type: "GET"
      });
      }
  </script>
</head>
<body>
<button name="btnAction" onclick="sendVal(this.value)" value="PWON" class="btnRed">PWON</button>
<button name="btnAction" onclick="sendVal(this.value)" value="PWOFF" class="btnGreen">PWOFF</button>
<br>
<button name="btnAction" onclick="sendVal(this.value)" value="VOLDOWN">VOLDOWN</button>
<button name="btnAction" onclick="sendVal(this.value)" value="VOLUP">VOLUP</button>
<br>
<button name="btnAction" onclick="sendVal(this.value)" value="DVD" class="btnViolet">DVD</button>
<button name="btnAction" onclick="sendVal(this.value)" value="TV" class="btnViolet">TV</button>
<button name="btnAction" onclick="sendVal(this.value)" value="VCR" class="btnViolet">VCR</button>
<br>
<button name="btnAction" onclick="sendVal(this.value)" value="MUTE">MUTE</button>
<button name="btnAction" onclick="sendVal(this.value)" value="NIGHT">NIGHT</button>
</body>
</html>
)rawliteral";

void send(uint16_t buf[]) {
  digitalWrite(LED_BUILTIN,LOW);
  irsend.sendRaw(buf,95,38);
  digitalWrite(LED_BUILTIN,HIGH);
}

void handleRoot() {
  server.send(200,"text/html",index_html);  
  String btn = server.arg("btnAction");
  Serial.print(":");
  Serial.println(btn);
  if (btn=="PWON") send(PWON);
  if (btn=="PWOFF") send(PWOFF);
  if (btn=="VOLUP") for(int i=0;i<4;i++) {send(VOLUP);delay(200);}
  if (btn=="VOLDOWN") for(int i=0;i<4;i++) {send(VOLDOWN);delay(200);}
  if (btn=="DVD") send(DVD);
  if (btn=="TV") send(TV);
  if (btn=="VCR") send(VCR);
  if (btn=="MUTE") send(MUTE);
  if (btn=="NIGHT") send(NIGHT);
}

void mqtt(char* topic, byte* payload, unsigned int length) {
    Serial.print("Received message [");
    Serial.print(topic);
    Serial.print("] ");
    char msg[length+1];
    for (int i = 0; i < length; i++) {
        Serial.print((char)payload[i]);
        msg[i] = (char)payload[i];
    }
    Serial.println();
 
    msg[length] = '\0';
    Serial.println(msg);
 
    if(strcmp(msg,"on")==0){
        send(PWON);
    }
    else if(strcmp(msg,"off")==0){
        send(PWOFF);
    }
}

void connectWifi() {
  WiFi.begin(STASSID,STAPSK);
  WiFi.mode(WIFI_STA);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.print("Connected to ");
  Serial.println(STASSID);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  client.setServer(MQTT_BROKER, 1883);
  client.setCallback(mqtt);
  Serial.print("mqtt broker: ");
  Serial.println(MQTT_BROKER);
}

void setup() {
  pinMode(LED_BUILTIN,OUTPUT);
  digitalWrite(LED_BUILTIN,HIGH);
  Serial.begin(115200);
  irsend.begin();
  connectWifi();
  server.on("/",handleRoot);
  server.begin();
}

void reconnect() {
    while (!client.connected()) {
        Serial.println("Reconnecting MQTT...");
        if (!client.connect("ESP8266Client")) {
            Serial.print("failed, rc=");
            Serial.print(client.state());
            Serial.println(" retrying in 5 seconds");
            delay(5000);
        }
    }
    client.subscribe("/home/denon");
    Serial.println("MQTT Connected...");
}

void loop() {
  server.handleClient();
    if (!client.connected()) {
        reconnect();
    }
    client.loop();
}
