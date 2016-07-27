#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>

byte mac[]    = {  0xDE, 0xED, 0xBA, 0xFE, 0xFE, 0xED };
IPAddress ip(192, 168, 1, 234);
IPAddress server(192, 168, 1, 107);

EthernetClient ethClient;
PubSubClient client(ethClient);


const int OPEN = 6; // Pin connected to reed switch
const int CLOSED = 7; 
int DOOR_LAST_STATE;
int DOOR_STATE;

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i=0;i<length;i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("arduinoClient")) {
      Serial.println("connected");
      client.publish("/home/garage/porte","hello world");
        if (digitalRead(OPEN) == HIGH && digitalRead(CLOSED) == LOW)
        {
          client.publish("/home/garage/porte","0",true);
        }
        else if (digitalRead(OPEN) == LOW && digitalRead(CLOSED) == HIGH)
        {
          client.publish("/home/garage/porte","1",true);
        }
        else if (digitalRead(OPEN) == HIGH && digitalRead(CLOSED) == HIGH)
        {
          client.publish("/home/garage/porte","2",true);
        }
        else
        {
          client.publish("/home/garage/porte","3",true);
        }
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}


void setup() 
{
  Serial.begin(9600);
  pinMode(OPEN, INPUT_PULLUP);
  pinMode(CLOSED, INPUT_PULLUP);
  client.setServer(server, 1883);
  client.setCallback(callback);
  Ethernet.begin(mac, ip);
  delay(1500);
}

void loop() 
{
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
    
  int is_open = digitalRead(OPEN);
  int is_closed = digitalRead(CLOSED);
  if (is_open == HIGH && is_closed == LOW)
  {
    DOOR_STATE = 0; // Ferme
  }
  else if (is_open == LOW && is_closed == HIGH)
  {
    DOOR_STATE = 1; // Ouvert
  }
  else if (is_open == HIGH && is_closed == HIGH)
  {
      DOOR_STATE = 2; // Mouvement Ouverture
  }
  else
  {
    DOOR_STATE = 3; // Erreur
  }

  if(DOOR_STATE != DOOR_LAST_STATE) {
    //Publish new state to the broker.
    String val = String(DOOR_STATE);
    char valBuff[2];
    val.toCharArray(valBuff, 2);
    Serial.println("changement");
    client.publish("/home/garage/porte",valBuff,true);
    Serial.println(valBuff);
    DOOR_LAST_STATE = DOOR_STATE;
  }

}
