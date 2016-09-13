import json
import datetime
import paho.mqtt.client as paho
import pyupm_buzzer as upm_buzzer
import pyupm_apds9002 as upm_luminance
import pyupm_biss0001 as upm_motion


def passed_time(initial_time):
    """ Returns the time difference.

    Calculates the millisecond time difference between the input time and
    current time.

    :param initial_time: initial time
    :return: time difference between initial_time and current time (ms)
    """
    return datetime.timedelta.total_seconds(datetime.datetime.now() -
                                            initial_time) * 1000


def on_connect(client, userdata, flags, rc):
    """ Callback triggered when connected to the MQTT broker.

    :param client: the client instance for this callback
    :param userdata: the private user data as set in Client() or userdata_set()
    :param flags: response flags sent by the broker
    :param rc: the connection result
    :return: None
    """
    print("Connected to the MQTT broker: %s" % rc)
    # Subscribe to 'cmd' (command) and 'config' (configuration) MQTT topics.
    client.subscribe(mqtt_credentials['topic']+'cmd')
    client.subscribe(mqtt_credentials['topic']+'config')


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

# Message publishing frequency in milliseconds.
pub_freq_ms = 500
# Variable for storing the time of the last sent message.
last_sent = datetime.datetime.now()
# Instantiate a buzzer object with digital pin D5 as an output.
buzzer = upm_buzzer.Buzzer(5)
buzzer.stopSound()
buzzer.setVolume(0.01)
# Instantiate a luminosity sensor object with analog pin A0 as an input.
luminosity = upm_luminance.APDS9002(0)
# Instantiate a motion sensor object with digital pin D2 as an input.
motion = upm_motion.BISS0001(2)

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
# Start a network loop on a separate thread.
client.loop_start()

# Main loop.
while True:
    # Calculate if enough time has elapsed to send the next message.
    if passed_time(last_sent) > pub_freq_ms:
        # Save the current time as the time of the last sent message.
        last_sent = datetime.datetime.now()
        # Form a python dictionary payload
        data = [{'meaning': 'motion', 'value': motion.value()},
                {'meaning': 'luminosity', 'value': luminosity.value()}]
        # Publish the payload as a json message to the 'data' MQTT topic.
        client.publish(mqtt_credentials['topic'] + '/data',
                       payload=json.dumps(data),
                       qos=0, retain=False)
