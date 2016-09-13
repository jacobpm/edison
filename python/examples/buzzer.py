import json
import paho.mqtt.client as paho
import pyupm_buzzer as upm_buzzer


def on_connect(client, userdata, flags, rc):
    """ Callback triggered when connected to the MQTT broker.

    :param client: the client instance for this callback
    :param userdata: the private user data as set in Client() or userdata_set()
    :param flags: response flags sent by the broker
    :param rc: the connection result
    :return: None
    """
    print("Connected to the MQTT broker: %s" % rc)
    # Subscribe to the 'cmd' (commands) MQTT topic.
    client.subscribe(mqtt_credentials['topic']+'cmd')


def on_message(client, userdata, msg):
    """ Callback triggered when an MQTT message is received.

    :param client: the client instance for this callback
    :param userdata: the private user data as set in Client() or userdata_set()
    :param msg: an instance of MQTTMessage. This is a class with members topic,
    payload, qos, retain.
    :return: None
    """
    # Decode the received JSON message.
    message = json.loads(msg.payload)
    # Handle the received messages with 'name':'buzzer'.
    if message['name'] == 'buzzer':
        if message['value']:
            # Trigger the buzzer in 'DO' note.
            buzzer.playSound(upm_buzzer.DO, 0)
        else:
            # Stop the buzzer.
            buzzer.stopSound()

# MQTT server address.
mqtt_server = "mqtt.relayr.io"
# MQTT credentials.
mqtt_credentials = {
    "user": "<your user ID>",
    "password": "<your password>",
    "clientId": "<your client ID>",
    "topic": "<your MQTT topic>"
}

# Instantiate a buzzer object with digital pin D5 as an output.
buzzer = upm_buzzer.Buzzer(5)
buzzer.stopSound()
buzzer.setVolume(0.01)
# Initialize the MQTT client.
client = paho.Client()
# Set the MQTT username and password.
client.username_pw_set(mqtt_credentials['user'], mqtt_credentials['password'])
# Register a 'on_connect' callback.
client.on_connect = on_connect
# Register a callback for received messages.
client.on_message = on_message
# Connect to the MQTT broker through port 1883 (no SSL) and timeout of  60s.
client.connect(mqtt_server, 1883, 60)
# Start a blocking network loop.
client.loop_forever()
