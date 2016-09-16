# Intel Edison and the Dell Starter Kit (Python edition)

## Introduction

This document details all the necessary steps required
to make use of the 
[Intel Edison](http://www.intel.com/content/www/us/en/do-it-yourself/edison.html)
as a sensor node with
[Dell Starter Kit](http://www.iot-starterkit.de/). This document
refers **only** to using **Python** on the Edison. See
[here](https://github.com/relayr/edison/tree/master/Arduino) for
using Arduino instead.

## Requirements

The following hardware is required:

 * [Dell Edge Gateway 5100](https://www.iot-starterkit.de/fileadmin/media/pdf/iot/EdgeGateway_5000_5100_TechnicalGuidebook-PDF_A4__OEM___1_.pdf)
 * [Intel Edison Arduino breakout kit](https://www.iot-starterkit.de/fileadmin/media/pdf/iot/edisonarduino_hg_331191007_2.pdf)
 * [Seeedstudio Grove Base Shield](http://wiki.seeedstudio.com/wiki/Grove_-_Base_shield_v2)
 * Seeedstudio Sensor Kit

A computer is required to connect to the Edison board for
flashing and configuring.

## Installation & configuration

### Setting up the hardware

Assemble the Arduino Expansion Board found in your Dell Starter Kit
according to the directions in the
[Intel Edison guide](https://software.intel.com/en-us/node/628221).

![Intel Edison](assets/intel-edison.jpg)

### Setting up the firmware

To flash the latest firmware on the Edison board, use the
*Intel® Edison Board Configuration Tool*
([Mac OS X](https://software.intel.com/en-us/get-started-edison-osx-step2),
[Linux](https://software.intel.com/en-us/get-started-edison-linux-step2),
[Windows](https://software.intel.com/en-us/get-started-edison-windows-step2)).
The official firmware is an operating system provided by the
[Yocto Project](https://www.yoctoproject.org/), which is a Linux
flavor geared towards embedded systems.

The setup wizard will guide you through the firmware flashing process, SSH
configuration and WiFi configuration. Start with flashing the firmware and continue with SSH and WiFi configuration. When a step is completed, a green checkmark is shown next
to the setup options in the wizard.

![IP Address](./assets/board-configuration-tool.png)


### Connecting your personal computer with the Intel Edison

When done with firmware flashing and the basic configurations of the Edison board, we can SSH into Intel Edison board and do further configurations by using its Linux shell. The Edison's IP address is found in the WiFi section of the **Intel Edison Board Configuration Tool**.

Alternatively, we can find the IP address of the board **without** the
Intel configuration wizard by using any of the following methods:

 1. Log in to your router/access point and find the IP address assigned to the Edison board.
 2. Set up [mDNS](http://www.multicastdns.org/) on your Intel Edison.
 3. Set up the board using a **static** IP.
 4. Find the IP address of the board using a scanner such as [`nmap`](https://nmap.org/).

Once we know the IP of the Intel Edison board we can execute the following command which will log us in the Edison board using SSH:

```shell
ssh root@<edison's-IP-address>
```

When prompted for a password use the one that was set in the security settings of the Intel Edison Board Configuration Tool. If everything worked properly, we should now be
logged in to the board as **root**.

### Clone the git repository

Once you are logged in the Intel Edison and are able to interact with its Linux shell, you can clone the git repository by executing the following command:

```shell
git clone https://github.com/relayr/edison
```

From then on you can find the folder with the Python code examples `~/edison/python/examples` of the Edison board. However, before running any of them we first have to install all the necessary libraries and Python dependencies.

### Setting up the `libmraa` library

The default firmware (Linux Yocto image)
comes with Python 2.7 already installed, so you could start running
scripts right away. However, if you wish to interact with the GPIOs of
the Intel Edison breakout board, you'll have to install
the [`libmraa`](https://github.com/intel-iot-devkit/mraa) C/C++ library,
which provides bindings for Java, JavaScript and Python. As noted in
the linked Github repository, you can install it through the 
[OPKG package manager](https://wiki.openwrt.org/doc/techref/opkg) by
executing the following three commands in the Linux shell of the Edison board:

```shell
echo 'src mraa-upm http://iotdk.intel.com/repos/3.5/intelgalactic/opkg/i586' > /etc/opkg/mraa-upm.conf
opkg update
opkg install mraa
```

The first line adds the source of the `libmraa` repository to the
package manager source list, the second one updates the existing
[opkg](https://en.wikipedia.org/wiki/Opkg) packages, and the last one
installs the `libmraa` package.

### Python Dependencies

The next step is to install the needed Python dependencies for running the examples on the Edison board. 

**NOTE:** When dealing with Python dependencies and versions, it is good practice
to create an isolated Python environment for each application we are
running. 

#### Virtualenv

`virtualenv` is a tool for creating and managing isolated Python
environments. It's installed using the `pip` Python package
manager.

```shell
pip install virtualenv
```
Once installed, we can create an isolated environment wherever we
want. Lets move (`cd`) into our code repository and create an environment called `ENV`.

```shell
cd ~/edison/
virtualenv --system-site-packages ENV
```

The `--system-site-packages` flag is for inheriting all the packages in the global site-packages directory. As we wish to use the `libmraa` python bindings, which we previously installed globally, it is necessary that we use `----system-site-packages` flag.

To activate the environment, run:

```shell
source bin/activate
```

You will notice the `(ENV)` prefix appearing in your command-line
interface, which indicates that you are in an isolated Python
environment called `ENV`.

Now we can use `pip` package manager and the Python interpreter just as we
normally would. However, additional Python packages will be installed **only**
in the virtual environment.

To deactivate the virtual environment, run the following shell command:

```shell
deactivate
``` 

If you encounter any issues when using `virtualenv`, consult the [official
documentation](https://virtualenv.pypa.io/en/stable/).

####Paho MQTT

In order to run the Python examples provided in this repository, we need to
install the [`paho-mqtt`](https://pypi.python.org/pypi/paho-mqtt/1.1) package,
which provides a MQTT client library and enables sending/receiving of
messages to/from a MQTT broker. [MQTT](https://en.wikipedia.org/wiki/MQTT) is
a lightweight messaging protocol built on top of TCP/IP. We chose it for exchanging messages between the gateway and the Edison board because of its simplicity and low overhead.

To install `paho-mqtt`  with `pip`, run:

```shell
pip install paho-mqtt
```

Once installed, we can use `paho-mqtt` classes by importing the
module into our script, as we will see later in our code examples.

To learn more about the functionalities of the
`paho-mqtt` Python client, see the
[official documentation](https://pypi.python.org/pypi/paho-mqtt/1.1).

After finishing all the installation and configuration steps, we are now 
ready to make use of MQTT and run the code examples.

## Code Examples

The code examples below will get you started with using Intel Edison board as a sensor node.

We will need a representation of our Intel Edison board in the relayr Cloud. Through this entity we will manage sent/received data, MQTT credentials and UI interface of the relayr Dashboard. To create a device on relayr Dashboard follow the [**Devices Guide**](http://docs.relayr.io/getting-started/devices-guide/) and select the **Intel Edison (IoT acceleration starter kit)** as a *Device Model*.

After successfully creating a device on relayr Cloud we are ready to run the first example.

### Example 1 (`blink.py`)

The `blink.py` example is a *Hello world* script which will toggle the LED on the Edison board every 0.5 seconds. We will run it mostly to test if `libmraa` library has been installed properly and you can interact with the GPIOs of your Edison board

To run the Python code example execute the following command in the Intel Edison Linux shell:

```shell
python path/to/blink.py
```

If the `libmraa` version was printed in your terminal and the LED on the board started blinking then you can move on to the next example in which we will connect a motion sensor to the Edison board. If the code example failed to execute without errors then the problem is most likely related to the installation of the `libmraa` library. Review the `libmraa` installation instruction in the previous section of this document and consult the official [documentation](https://github.com/intel-iot-devkit/mraa) for troubleshooting.

### Example 2 (`motion_sensor.py`)

The `motion_sensor.py` example uses a PIR motion sensor to detect movements in its proximity. Sensor outputs a digital value (*HIGH* if there is movement and *LOW* if there is no movement) which is then sent to the Dell gateway and relayr Cloud.

First prepare the hardware by connecting the *PIR motion sensor* to the **Digital Pin 2 (D2)**.

![D2 pin](assets/d2-pin.jpg)

Next modify the `motion_sensor.py` Python script with MQTT credentials of the device you've created at relayr Dashboard. You can use the `nano path/to/motion_sensor.py` command to open the script in the nano editor. Change the following part of the code:

```python
# MQTT credentials.
mqtt_credentials = {
    "user": "<your user ID>",
    "password": "<your password>",
    "clientId": "<your client ID>",
    "topic": "<your MQTT topic>"
}
```

and save the changes by pressing `control` + `X` and confirming the command with `Y`.

Now you are ready to execute the Python script by using the following command:

```shell
python path/to/motion_sensor.py
```

If everything went well, you should see a message in your terminal when the MQTT client running on the Edison connects to the MQTT broker in the Dell gateway. If the connection is successfully established, and everything is working properly a `{'meaning': 'motion', 'value': True}` JSON message will be sent every 0.5 second to the Dell gateway and from there to the relayr Cloud. The `'value': True` will change accordingly to the motion detected by the PIR sensor. You can observe the changes on the relayr Dashboard.

If you run into troubles, make sure that you have all the Python dependencies installed (`libmraa` and `paho-mqtt`) and virtual Python environment activated (if you are using one.)

### Example 3 (`buzzer.py`)

The `buzzer.py` example shows you how to receive commands from the relayr Cloud and/or the Dell gateway. The command received by Intel Edison will remotely turn on or off a piezo buzzer.

First prepare the hardware by connecting the [grove buzzer](http://wiki.seeedstudio.com/wiki/Grove_-_Buzzer) to the **Digital pin 5 (D5)**.

![D5 pin](assets/d5-pin.jpg)

Similarly as in the `motion.py` example, copy the credentials of the device created on the relayr Dashboard in the appropriate place of the `buzzer.py`.

```python
# MQTT credentials.
mqtt_credentials = {
    "user": "<your user ID>",
    "password": "<your password>",
    "clientId": "<your client ID>",
    "topic": "<your MQTT topic>"
}
```

Run the code example by executing the following Linux shell command:

```shell
python path/to/buzzer.py
```

Now the Intel Edison is listening to the messages from the relayr Cloud. You can control the buzzer by pressing **True** or **False** in the **buzzer** widget on the relayr Dashboard and if you did everything right you'll hear a buzzing sound.


### Example 4 (`multi_sensor.py`)

The `multi_sensor.py` example uses the following sensors and actuators: PIR motion sensor, luminosity sensor, water sensor and a buzzer.

For the code example to work properly you must first connect all the above mentioned sensors and actuators. Here is a list of pins you should use:

* PIR motion sensor - **D2**
* Buzzer - **D5**
* Luminosity sensor - **A0**
* Water sensor - **D3**

As in previous examples you must add your MQTT credentials in the appropriate place in the `multi_sensor.py` code example and then run the script with executing the following shell command:

```shell
python path/to/multi_sensor.py
```

From then on your device will be publishing sensor data every 0.5 seconds to the relayr Cloud. You can see the incoming data on the relayr Dashboard and similarly as in the `buzzer.py` example, you can trigger a sound by pressing *True* in the *buzzer* widget.

### Starting scripts automatically at system start-up

In most cases you'll wish to start sending data automatically when you power on your Intel Edison board. This section will guide you through configuration of Edison for automatically triggering a Python scrip on boot.

We will use [**`systemd`**](https://www.freedesktop.org/software/systemd/man/systemd.html#), a system and service manager for Linux operating system, while creating a new service responsible for monitoring and sending sensor data. You can find a `mote.service` service configuration file in `edison/python/autostart/` folder.

If you look into it you'll see a simple `systemd` unit definition triggering a `mote` shell script.

```text
[Unit]
Description=Sensor Monitoring Service
Wants=network-online.target
After=network-online.target

[Service]
ExecStart=/home/root/edison/python/autostart/mote
Restart=on-failure
RestartSec=10

[Install]
WantedBy=multi-user.target
```

Mote shell script is also in the `edison/python/autostart/` and contains only two commands:

```shell
source /home/root/edison/ENV/bin/activate
python /home/root/edison/python/examples/multi_sensor.py
```

The first one activates `ENV` python virtual environment in the code repository folder, while the second one runs the `multi_sensor.py` script in the examples folder. Adjust the paths to the virtual environment (or delete the line if you don't use one) and to the Python script you wish to run at system start-up.

Next make the `mote` shell script executable by utilizing the following command:

```shell
chmod +x edison/python/autostart/mote
```

You can test if the shell script has right permissions and works properly by executing:

```shell
./edison/python/autostart/mote
```

If everything is fine and the Python script is being triggered you can kill the process and move on to copying `mote.service` to the `/lib/systemd/system/` folder:

```shell
cp edison/python/autostart/mote.service /lib/systemd/system/
```

Once the service configuration file is in the proper `systemd` folder we have to change its permissions:

```shell
chmod 644 /lib/systemd/system/mote.service
```

Now everything is ready and you can reload all the `systemd` configurations

```shell
systemctl daemon-reload
```

and enable the service with:

```shell
systemctl enable mote.service
```

From now on, when restarting the Edison board the `multi_sensor.py` script should start automatically. You can use `systemctl stop mote.service` to stop the `mote.service` and `systemctl disable mote.service` to remove it from the list of services that start on system start-up. For troubleshooting and/or more advance service configurations consult the [following documentation](https://www.freedesktop.org/software/systemd/man/systemd.html#).


# Further steps

To explore further and write your own Python code for sensors of your choice look into the code examples and its comments. Furthermore, you can browse the [UPM Python library](http://iotdk.intel.com/docs/master/upm/python/) and its sensor [code example list](https://github.com/intel-iot-devkit/upm/tree/master/examples/python).
