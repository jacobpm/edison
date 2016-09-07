# Intel Edison and the Dell Starter Kit (Python edition)

## Introduction

This document will guide you through all the necessary steps required to use Intel Edison as a sensor node within the [Dell Starter Kit](). The Edison board configuration and code examples provided here are Python specific, if you wish to use Arduino IDE follow [this documentation](https://github.com/relayr/edison/tree/master/Arduino).

## Requirements

You'll need the following components of the Dell Starter kit:

* [Dell gateway]()
* [Intel Edison Arduino breakout kit]()
* [Seeedstudio base shield]()
* [Seedstuido sensor kit]()

Additionally, you'll need a laptop to connect and configure Intel Edison and the Dell gateway.

## Installation & configuration

### Setting up the hardware

To assemble the Arduino Expansion Board found in the Dell Starter Kit please follow the official [Intel Edison guide](https://software.intel.com/en-us/node/628221).

### Setting up the firmware

To run python scripts on Intel Edison board you'll first have to install an operating system. The easies way is to run [Intel® Edison Board Configuration Tool](https://software.intel.com/en-us/get-started-edison-osx-step2) and install the default firmware, which is a Linux version built using the [Yocto Project](https://www.yoctoproject.org/). The setup wizard will guide you through flashing of the firmware, SSH configuration and WiFi configuration. When your Intel Edison board is ready and connected to the internet you'll see a green checkmark icon next to each of the setup options of the Intel Edison Board Configuration wizard.

![IP Address](board-configuration-tool.png)


### Connecting your personal computer with the Intel Edison

To connect your laptop with the Intel Edison open your command-line interface and use the SSH protocol:

```shell
ssh root@<edison's-IP-address>
```

You can find the Edison's IP address in the WiFi section of the *Intel Edison Board Configuration Tool*. To find your IP address without using the Intel configuration tool you can:

1. Log in your router and find the IP address assigned to the Edison board.
2. Setup mDNS on your Intel Edison.
3. Setup a static IP.
4. Scan your network with utility such as [`nmap`](https://nmap.org/).

When connecting via SSH and being prompt for a password use the one you've set in the security settings of the Intel Edison Board Configuration Tool and press enter. If everything worked well you'll be logged in your Edison board as a root user with access to all the files and Linux commands.

### Setting up the `limraa` library

Once you are connected to Intel Edison and able to interact with its Linux shell we can move on and install all the libraries and Python packages that you'll need. The default firmware (Linux Yocto image) comes with Python 2.7 already installed so you can start running scripts right away. However, if you wish to interact with the GPIOs of the Intel Edison breakout board you'll have to install [`libmraa`](https://github.com/intel-iot-devkit/mraa) C/C++ library which provides bindings for Java, JavaScript and Python. As noted in the linked github repository, you can install it by using [OPKG package manager](https://wiki.openwrt.org/doc/techref/opkg) by executing the following three commands:

```shell
echo "src mraa-upm http://iotdk.intel.com/repos/3.5/intelgalactic/opkg/i586" > /etc/opkg/mraa-upm.conf
opkg update
opkg install mraa
```
The first line adds the source of the `libmraa` repository to the package manager, the second one updates the existing OPKG packages and the last one installs the `libmraa` package.

### Python Dependencies

The next step is to install the python dependencies you'll need to send messages of the sensor readings to the [Dell gateway](). When dealing with Python dependencies and versions a good practice is to create an isolated Python environment for each application you are running. If you wish to follow good practices and install the needed dependencies in an virtual Python environment follow the next subsection which will help you setup the `virtualenv`, otherwise skip to the *Paho MQTT* subsection.

#### Virtualenv

Virtualenv is a tool for creating and managing isolated Python environments. You can install it with `pip` Python package manager using the following command:

```shell
pip install virtualenv
```
After having `virtualenv` installed you can move (`cd`) in any project folder and create a new virtual Python environment: 

```shell
virtualenv --system-site-packages ENV
```

where `ENV` is the name of the directory to place the new virtual environment in. The `--system-site-packages` flag is used to inherit packages in global site-packages directory (in our case the `libmraa` python bindings package).

To activate an environment use:

```shell
source bin/activate
```

You will notice the `(ENV)` prefix appearing in your command-line interface which indicates that you are in a isolated Python environment called `ENV`. 

Now you can use `pip` package manager and Python interpreter just as you would normally, however, when you install any Python package it will be installed only for the virtual environment and you wont pollute the global python environment.

To deactivate the virtual environment and return into global one write:

```shell
deactivate
```
If you run into any questions when using `virtualenv` consult the [official documentation](https://virtualenv.pypa.io/en/stable/).


#### MQTT Paho

To run the Python examples provided in this repository you'll have to install [`paho-mqtt`](https://pypi.python.org/pypi/paho-mqtt/1.1) package which provides a MQTT client library enabling the sending of messages to a MQTT broker. MQTT is a lightweight messaging protocol developed specifically for constrained devices. We are using it because of it's simplicity and low overhead. If you wish to learn more about the MQTT protocol you can start at the [MQTT wikipedia entry](https://en.wikipedia.org/wiki/MQTT).

To install `paho-mqtt` package use the `pip` Python package manager:

```shell
pip install paho-mqtt
```

When done you can use the `paho-mqtt` classes by importing the module in your Python script:

```python
import paho-mqtt
```

If you wish to learn more about the functionalities that the `paho-mqtt` Python client offers check out the [official documentation](https://pypi.python.org/pypi/paho-mqtt/1.1).

After finishing all installation and configuration steps you are ready to start running the code examples below.

## Code Examples

Intro for examples clone the repository


### Example 1 (Blink)

intro, what, how, congrats

### Example 2 (PIR Sensor)

intro, what, how, congrats

### Example 3 (Buzzer)

intro, what, how, congrats

# References
