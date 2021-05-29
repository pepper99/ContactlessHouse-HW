#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char* ssid     = "Perhome_2.4G";
const char* password = "0273392230";
const char* mqtt_server = "mqtt.netpie.io";
const int mqtt_port = 1883;
const char* mqtt_Client = "fcdc8b4e-693a-42ad-bc58-f62d48562bbf";
const char* mqtt_username = "yH5N33b1xQ1igYmNKZpw72qB91BxZFBT";
const char* mqtt_password = "#kZTRVOTbcODM60_EeZYFsku!N0p4(eE";

WiFiClient espClient;
PubSubClient client(espClient);

#define LIGHT1_ON 1
#define LIGHT1_OFF 2
#define LIGHT2_ON 3
#define LIGHT2_OFF 4
#define DOOR_UNLOCK 5
#define DOOR_LOCK 6

#define ONBOARD_LED 2

long lastMsg = 0;
char msg[100];

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection…");
    if (client.connect(mqtt_Client, mqtt_username, mqtt_password)) {
      Serial.println("connected");
      client.subscribe("@msg/light1");
      client.subscribe("@msg/light2");
      client.subscribe("@msg/door");
    }
    else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println("try again in 5 seconds");
      delay(5000);
    }
  }
}

int toggle = HIGH;

void callback(char* topic, byte* payload, unsigned int length) {
//  Serial.print("Message arrivied (");
//  Serial.print(topic);
//  Serial.print(") ");
  String message;
  for (int i = 0; i < length; i++) {
    message = message + (char)payload[i];
  }
//  Serial.println(message);
  digitalWrite(ONBOARD_LED, toggle);
  toggle = !toggle;

  if(String(topic) == "@msg/light1") {
    if (message == "on") {
      Serial.print(LIGHT1_ON);
      client.publish("@shadow/data/update","{\"data\":{\"light1\":\"on\"}}");
    }
    else if (message =="off") {
      Serial.print(LIGHT1_OFF);
      client.publish("@shadow/data/update","{\"data\":{\"light1\":\"off\"}}");
    }
  }
  else if(String(topic) == "@msg/light2") {
    if (message == "on") {
      Serial.print(LIGHT2_ON);
      client.publish("@shadow/data/update","{\"data\":{\"light2\":\"on\"}}");
    }
    else if (message =="off") {
      Serial.print(LIGHT2_OFF);
      client.publish("@shadow/data/update","{\"data\":{\"light2\":\"off\"}}");
    }
  }
  else if(String(topic) == "@msg/door") {
    if (message == "lock") {
      Serial.print(DOOR_LOCK);
      client.publish("@shadow/data/update","{\"data\":{\"door\":\"lock\"}}");
    }
    else if (message =="unlock") {
      Serial.print(DOOR_UNLOCK);
      client.publish("@shadow/data/update","{\"data\":{\"door\":\"unlock\"}}");
    }
  }
  Serial.print("\r\n");
}

void setup() 
{
  Serial.begin(115200);
  Serial.println();
  pinMode(ONBOARD_LED, OUTPUT);
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}

int val = 50;

void loop() 
{
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  if (Serial.available()) {
    String s = Serial.readStringUntil('x');
//    Serial.print(s);
    String data = "{\"data\":{\"brightness\":" + String(s) + "}}";
//    Serial.println(data);
    data.toCharArray(msg, (data.length() + 1));
    client.publish("@shadow/data/update", msg);
  }
}
