
//Have a look at the onboarding tutorial at the relayr Github:
//https://github.com/relayr/relayr-edison


//Libraries for the WiFi module 
#include <WiFi.h>
#include <signal.h>


//Library for the MQTT protocol
//Please use this fork if you experience problems with the original library:
//https://github.com/uberdriven/pubsubclient
#include <PubSubClient.h>


//WiFi credentials
//#define SSID "INTRODUCE_YOUR_SSID_HERE"
//#define PASSWORD "INTRODUCE_YOUR_PASSWORD_HERE"

#define SSID "relayr"
#define PASSWORD "!D1g1t@lTransformati0n!"

//Credentials from the developer dashboard
//#define DEVICE_ID "INTRODUCE_YOUR_DEVICE_ID_HERE"
//#define MQTT_USER "INTRODUCE_YOUR_MQTT_USER_HERE"
//#define MQTT_PASSWORD "INTRODUCE_YOUR_MQTT_PASSWORD_HERE"
//#define MQTT_CLIENTID "INTRODUCE_YOUR_MQTT_CLIENT_HERE" //It can be anything else
//#define MQTT_TOPIC "INTRODUCE_YOUR_MQTT_TOPIC_HERE"
//#define MQTT_SERVER "mqtt.relayr.io"

#define DEVICE_ID "eec7a6d3-8dc3-496d-9ec0-e678ab258aae"
#define MQTT_USER "eec7a6d3-8dc3-496d-9ec0-e678ab258aae"
#define MQTT_PASSWORD "49gi7s3kMsMl"
#define MQTT_CLIENTID "T7sem043DSW2ewOZ4qyWKrg" //can be anything else
#define MQTT_TOPIC "/v1/eec7a6d3-8dc3-496d-9ec0-e678ab258aae/"
#define MQTT_SERVER "mqtt.relayr.io"

//This creates the WiFi client and the pub-sub client instance
WiFiClient edisonClient;
PubSubClient client(edisonClient);

//This is to check if the WiFi is active
int status = WL_IDLE_STATUS;

//Some definitions, including the publishing period
const int led = 13;
int ledState = LOW;
int analog0;
int test_counter=0;
unsigned long lastPublishTime = 0;
unsigned long lastBlinkTime = 0;
int publishingPeriod = 1000; //In ms!

//for the messages
char msg[50];
char message_buff[100];

//Function prototypes
void setup_wifi();
void mqtt_connect();
void callback(char* topic, byte* payload, unsigned int length);
void handlePayload(char* payload);
void publish();


//Setup function
void setup()
{
  //Initializing pins
  pinMode(A0, INPUT);
  pinMode(led, OUTPUT);
  pinMode(1, OUTPUT);
  pinMode(2, OUTPUT);

  //Initializing serial port
  Serial.begin(9600);
  Serial.println("");
  Serial.println("Hello there, I'm your ESP8266.");
  Serial.println("Let's talk to the relayr Cloud!");
  
  system("wpa_supplicant -B -Dnl80211 -iwlan0 -c/etc/wpa_supplicant/wpa_supplicant.conf");
  system("busybox udhcpc -i wlan0");

  signal(SIGPIPE,SIG_IGN);
  
  setup_wifi();
  client.setServer(MQTT_SERVER, 1883);
  client.setCallback(callback);

  //200ms is the minimum publishing period
  publishingPeriod = publishingPeriod > 200 ? publishingPeriod : 200;
  mqtt_connect();
}




//------------------------------------------------------------------------------------//
// This function connects to the WiFi network, and prints the current IP address      //
//------------------------------------------------------------------------------------//

void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println("");
  Serial.print("Connecting to ");
  Serial.print(SSID);
  Serial.print("...");

  WiFi.begin(SSID, PASSWORD);
  WiFi.status();

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}




//------------------------------------------------------------------------------------//
// Callback function, necessary for the MQTT communication                            //
//------------------------------------------------------------------------------------//

void callback(char* topic, byte* payload, unsigned int length)
{
  //Store the received payload and convert it to string
  char p[length + 1];
  memcpy(p, payload, length);
  p[length] = NULL;
  //Print the topic and the received payload
  Serial.println("topic: " + String(topic));
  Serial.println("payload: " + String(p));
  //Call our method to parse and use the received payload
  handlePayload(p);
}




//------------------------------------------------------------------------------------//
// This processes the payload; commands and configurations should be implemented here //  
//------------------------------------------------------------------------------------//

void handlePayload(char* payload)
{
//  StaticJsonBuffer<200> jsonBuffer;
//  //Convert payload to json
//  JsonObject& json = jsonBuffer.parseObject(payload);
//  
//  if (!json.success())
//  {
//    Serial.println("json parsing failed");
//    return;
//  }
//
//  //Get the value of the key "name", aka. listen to incoming commands and configurations
//  const char* command = json["name"];
//  Serial.println("parsed command: " + String(command));
//  
//  
//  //COMMAND "d1": We can toggle the digital output 1
//  if (String(command).equals("d1"))
//  {
//    const char* d1 = json["value"];
//    Serial.println("digital output 1: " + String(d1));
//    String s(d1);
//    
//    if (s.equals("high"))
//      digitalWrite(1, HIGH);
//    
//    else if (s.equals("low"))
//      digitalWrite(1, LOW);
//  }
//  
//  //COMMAND "d2": We can toggle the digital output 2
//  if (String(command).equals("d2"))
//  {
//    const char* d2 = json["value"];
//    Serial.println("digital output 2: " + String(d2));
//    String s(d2);
//    
//    if (s.equals("high"))
//      digitalWrite(2, HIGH);
//    
//    else if (s.equals("low"))
//      digitalWrite(2, LOW);
//  }
//  
//  //CONFIGURATION "frequency": We can change the publishing period
//  if (String(command).equals("frequency"))
//  {
//    int frequency = json["value"];
//    
//    if ( (frequency>=200) && (frequency<=5000) )
//    {
//      Serial.println("Adjusting publishing period (ms): " + String(frequency));
//      publishingPeriod = frequency;
//    }
//
//    else
//      Serial.println("The requested publishing period is out of the defined range!");   
//  }
}




//------------------------------------------------------------------------------------//
// This function establishes the connection with the MQTT server                      //
//------------------------------------------------------------------------------------//

void mqtt_connect()
{
  Serial.println("");
  Serial.println("Connecting to MQTT server...");
  
  if (client.connect(MQTT_CLIENTID, MQTT_USER, MQTT_PASSWORD))
  {
    Serial.println("Connection successful! Subscribing to topic...");
    //Subscribing to the topic "cmd", so we can listen to commands
    client.subscribe("/v1/"DEVICE_ID"/cmd");
    //And to "config" as well, for the configurations
    client.subscribe("/v1/"DEVICE_ID"/config");
  }

  else
  {
      Serial.println("Connection failed! Check your credentials or the WiFi network");
      //This reports the error code
      Serial.println("rc = ");
      Serial.print(client.state());
      //And it tries again in 5 seconds
      Serial.println("Retrying in 5 seconds...");
      delay(5000);
  }
}




//------------------------------------------------------------------------------------//
// This is for the LED to blink                                                       //
//------------------------------------------------------------------------------------//

void blink(int interval)
{
  if (millis() - lastBlinkTime > interval)
  {
    //Save the last time you blinked the LED
    lastBlinkTime = millis();
    
    if (ledState == LOW)
      ledState = HIGH;
      
    else
      ledState = LOW;
      
    //Set the LED with the ledState of the variable:
    digitalWrite(led, ledState);
  }
}




//------------------------------------------------------------------------------------//
// This is the MAIN LOOP, it's repeated until the end of time! :)                     //
//------------------------------------------------------------------------------------//

void loop()
{
  //When counter reaches the maximum specified in the default model, it's reset
  if(test_counter == 32767)
    test_counter = 0;
    
  //If we're connected, we can send data...
  if (client.connected())
  {

    
    //client.loop();


    
    //Publish within the defined publishing period
        if (millis() - lastPublishTime > publishingPeriod)
        {
            //Time to publish, as defined by the publishing period
            //Let's update the value of the ADC (analog input 0) first, since it will be published...
            analog0 = analogRead(A0);
            //Then update the last publish time...
            lastPublishTime = millis();
            //And finally, publish the payload...
            publish();
        }
        //Blink LED  
        blink(publishingPeriod / 2);
  }

  //If the connection is lost, then reconnect...
  else
  {
    Serial.println("Retrying...");
    mqtt_connect();
  }      
}




//------------------------------------------------------------------------------------//
// Publish function: What we want to send to the relayr Cloud                         //
//------------------------------------------------------------------------------------//

void publish()
{    
//  //MQTT_MAX_PACKET_SIZE is defined in "PubSubClient.h", it's 128 bytes by default
//  //A modified version with 512 bytes it's available here:
//  //   https://github.com/uberdriven/pubsubclient
//  StaticJsonBuffer<MQTT_MAX_PACKET_SIZE> pubJsonBuffer;
//  //Create our JsonArray
//  JsonArray& root = pubJsonBuffer.createArray();
//
////-------------------------------------------------
//  //First object: analog input 0
//  JsonObject& leaf1 = root.createNestedObject();
//  //This is how we name what we are sending
//  leaf1["meaning"] = "analog0";
//  //This contains the readings of the port
//  leaf1["value"] = analog0;
////-------------------------------------------------
//  
////-------------------------------------------------  
//  //Second object: test counter (+1 for every sent message)
//  JsonObject& leaf2 = root.createNestedObject();
//  //This is how we name what we are sending
//  leaf2["meaning"] = "test_counter";
//  //This contains the value of the counter (increased with every iteration)
//  leaf2["value"] = test_counter++;
////-------------------------------------------------
//  
//  char message_buff[MQTT_MAX_PACKET_SIZE];
//  root.printTo(message_buff, sizeof(message_buff));
//  client.publish("/v1/"DEVICE_ID"/data", message_buff);
//  Serial.println("Publishing " + String(message_buff));

  //create our json payload
  String pubString = "{\"meaning\":\"Sound\", \"value\":";
  //read and add sensor data to payload
  pubString += analogRead(A1);
  pubString += "}";
  pubString.toCharArray(message_buff, pubString.length() + 1);
  //publish our json payload
  client.publish("/v1/"DEVICE_ID"/data", message_buff);
  Serial.println("Publishing " + String(message_buff));
    
}


