import json
import datetime
import paho.mqtt.client as paho
import pyupm_apds9002 as upm_luminosity


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

# MQTT server address.
mqtt_server = "mqtt.relayr.io"
# MQTT credentials.
mqtt_credentials = {
    "user": "ed939030-e620-4ed4-bb8d-16dc499de1ce",
    "password": "sxb7F8P1lI_C",
    "clientId": "T7ZOQMOYgTtS7jRbcSZ3hzg",
    "topic": "/v1/ed939030-e620-4ed4-bb8d-16dc499de1ce/"
}

# Message publishing frequency in milliseconds.
pub_freq_ms = 500
# Variable for storing the time of the last sent message.
last_sent = datetime.datetime.now()
# Instantiate a luminosity sensor object with analog pin A0 as an input.
luminosity = upm_luminosity.APDS9002(0)
# Initialize the MQTT client.
client = paho.Client()
# Set the MQTT username and password.
client.username_pw_set(mqtt_credentials['user'], mqtt_credentials['password'])
# Register a 'on_connect' callback.
client.on_connect = on_connect
# Connect to the MQTT broker.
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
        data = {'meaning': 'luminosity', 'value': luminosity.value()}
        # Publish the payload as a json message to the 'data' MQTT topic.
        client.publish(mqtt_credentials['topic'] + '/data',
                       payload=json.dumps(data),
                       qos=0, retain=False)
