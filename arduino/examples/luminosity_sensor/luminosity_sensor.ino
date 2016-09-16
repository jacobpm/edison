
// Have a look at the onboarding tutorial at the relayr Github:
// https://github.com/relayr/relayr-edison

// Libraries for the WiFi module 
#include <WiFi.h>
#include <signal.h>

// Library for the MQTT protocol
// Please use this fork if you experience problems with the original library:
// https://github.com/uberdriven/pubsubclient
#include <PubSubClient.h>

// WiFi credentials
#define SSID "INTRODUCE_YOUR_SSID_HERE"
#define PASSWORD "INTRODUCE_YOUR_PASSWORD_HERE"

// Credentials from the developer dashboard
#define DEVICE_ID "INTRODUCE_YOUR_DEVICE_ID_HERE"
#define MQTT_USER "INTRODUCE_YOUR_MQTT_USER_HERE"
#define MQTT_PASSWORD "INTRODUCE_YOUR_MQTT_PASSWORD_HERE"
#define MQTT_CLIENTID "INTRODUCE_YOUR_MQTT_CLIENT_HERE" //It can be anything else
#define MQTT_TOPIC "INTRODUCE_YOUR_MQTT_TOPIC_HERE"
#define MQTT_SERVER "INTRODUCE_YOUR_GATEWAY_IP_ADDRESS_HERE"

// This creates the WiFi client and the pub-sub client instance
WiFiClient edisonClient;
PubSubClient client(edisonClient);

// This is to check if the WiFi is active
int status = WL_IDLE_STATUS;

// Some definitions, including the publishing period
const int led = 13;
int ledState = LOW;
unsigned long lastPublishTime = 0;
unsigned long lastBlinkTime = 0;
int publishingPeriod = 1000; // In ms!

// This is to put the payloads together
char msg[50];
char message_buff[100];

// Function prototypes
void setup_wifi();
void mqtt_connect();
void publish();




//------------------------------------------------------------------------------------//
// Setup function: Configuration and initialization                                   //
//------------------------------------------------------------------------------------//

void setup()
{
  // Initializing pins for the LED and the luminance sensor
  pinMode(A0, INPUT);
  pinMode(led, OUTPUT);

  // Initializing serial port
  Serial.begin(9600);
  Serial.println("");
  Serial.println("Onboarding Edison onto Vertex-enabled gateway...");

  // Uncomment these two lines if you have trouble connecting to the network after booting
  // system("wpa_supplicant -B -Dnl80211 -iwlan0 -c/etc/wpa_supplicant/wpa_supplicant.conf");
  // system("busybox udhcpc -i wlan0");

  // Network & MQTT configuration
  signal(SIGPIPE,SIG_IGN);
  setup_wifi();
  client.setServer(MQTT_SERVER, 1883);

  // 200ms is the minimum publishing period
  publishingPeriod = publishingPeriod > 200 ? publishingPeriod : 200;

  mqtt_connect();
}




//------------------------------------------------------------------------------------//
// This function connects to the WiFi network, and prints the current IP address      //
//------------------------------------------------------------------------------------//

void setup_wifi()
{
  delay(10);

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
// This function establishes the MQTT connection with Vertex                          //
//------------------------------------------------------------------------------------//

void mqtt_connect()
{
  Serial.println("");
  Serial.println("Connecting to Vertex via MQTT...");
  
  if (client.connect(MQTT_CLIENTID, MQTT_USER, MQTT_PASSWORD))
  {
    Serial.println("Connection successful!");
  }

  else
  {
    Serial.println("Connection failed! Check your credentials or the WiFi network");
      
    // This reports the error code
    Serial.println("rc = ");
    Serial.print(client.state());
      
    // Re-try after 5 seconds
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
    // Save the last time that the LED blinked
    lastBlinkTime = millis();
    
    if (ledState == LOW)
      ledState = HIGH;
      
    else
      ledState = LOW;
      
    // Set the LED with the ledState of the variable
    digitalWrite(led, ledState);
  }
}




//------------------------------------------------------------------------------------//
// This is the MAIN LOOP, it's repeated until the end of time! :)                     //
//------------------------------------------------------------------------------------//

void loop()
{
  // When counter reaches the maximum specified in the default model, it's reset
  if(test_counter == 32767)
    test_counter = 0;
    
  // If we're connected, we can send data...
  if (client.connected())
  { 
    // Publish within the defined publishing period
    if (millis() - lastPublishTime > publishingPeriod)
    {      
      // Update the last time it published
      lastPublishTime = millis();
      
      // And finally publish the payload
      publish();
    }
    
    // Blink the LED
    blink(publishingPeriod / 2);
  }

  // If the connection is lost, then reconnect
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
  // Create our JSON payload
  String pubString = "{\"meaning\":\"luminosity\", \"value\":";
  
  // Read and add sensor data to the payload, and close it
  pubString += analogRead(A0);
  pubString += "}";
  pubString.toCharArray(message_buff, pubString.length() + 1);
  
  // Publish the JSON payload
  client.publish("/v1/"DEVICE_ID"/data", message_buff);
  Serial.println("Publishing " + String(message_buff));
}


