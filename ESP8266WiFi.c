#include <PubSubClient.h>
#include <ESP8266WiFi.h>


#define wifi_ssid "LAKA2"
#define wifi_password "0602626292"

#define mqtt_server "m20.cloudmqtt.com"
#define mqtt_user "napengcd"
#define mqtt_password "X17PdC9LRURL"
#define Light_topic "Lampa/Stan/"
#define MODE_OFF    0 
#define MODE_ON     1  

void callback(char* topic, byte* payload, unsigned int length);

int senseMode = 0;
char message_buff[100];
unsigned long Time;

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(115200);
  while (!Serial);
  setup_wifi();
  client.setServer(mqtt_server,19928);
  client.setCallback(callback); 
 
  // Start sensor
 
}
void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(wifi_ssid);

  WiFi.begin(wifi_ssid, wifi_password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    // If you do not want to use a username and password, change next line to
    // if (client.connect("ESP8266Client")) {
    if (client.connect("ESP8266Client", mqtt_user, mqtt_password)) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
void loop() {
  if (!client.connected()) {
  reconnect();
  client.subscribe(Light_topic);
  }
  client.loop();
  


  switch (senseMode) {
    case MODE_OFF:
        Serial.write(MODE_OFF);
      break;
    case MODE_ON:
        Serial.write(MODE_ON);
      break;
  }

   if (millis() > (Time + 5000)) {
        Time = millis();
        String msgString;
        if(senseMode==0) msgString = "Off"; else msgString = "On";
        client.publish("/Lampa/Stan/Raport", String(msgString).c_str(), true); 
      }
}

void callback(char* topic, byte* payload, unsigned int length) {

  int i = 0;
  
  for(i=0; i<length; i++) {
    message_buff[i] = payload[i];
  }
  message_buff[i] = '\0';
  
  String msgString = String(message_buff);
 
  if (msgString.equals("OFF")) {
     senseMode = MODE_OFF;
  } else if (msgString.equals("ON")) {
    senseMode = MODE_ON;
  } 
}
