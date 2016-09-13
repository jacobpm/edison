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

 * [Dell gateway]()
 * [Intel Edison Arduino breakout kit]()
 * [Seedstudio base shield]()
 * [Seedstudio sensor kit]()

A computer is required to connect the Edison board to for
flashing and configuring.

## Installation & configuration

### Setting up the hardware

Assemble the Arduino Expansion Board found in your Dell Starter Kit
according to the directions in the
[Intel Edison guide](https://software.intel.com/en-us/node/628221).

### Setting up the firmware

To flash the latest firmware on the Edison board, use the
*Intel® Edison Board Configuration Tool*
([Mac OS X](https://software.intel.com/en-us/get-started-edison-osx-step2),
[Linux](https://software.intel.com/en-us/get-started-edison-linux-step2),
[Windows](https://software.intel.com/en-us/get-started-edison-windows-step2)).
The official operating system is provided by the
[Yocto Project](https://www.yoctoproject.org/), which is a Linux
flavor geared towards embedded systems.

The setup wizard will guide you through the firmware flashing process, SSH
configuration and WiFi configuration. When a step is completed, a green
checkmark is shown next to each of the setup options in the wizard.

![IP Address](./assets/board-configuration-tool.png)


### Connecting your personal computer with the Intel Edison

To connect the board to your computer, log in using SSH:

```shell
ssh root@<edison's-IP-address>
```

The Edison's IP address is found in the WiFi section of the **Intel
Edison Board Configuration Tool**. 

Alternatively, we can find the IP address of the board **without** the
Intel configuration wizard by using any of the following methods:

 1. Log in to your router/access point and find the IP address assigned to the Edison board.
 2. Set up [mDNS](http://www.multicastdns.org/) on your Intel Edison.
 3. Set up the board using a **static** IP.
 4. Find the IP address of the board using a scanner such as [`nmap`](https://nmap.org/).

When prompted for a password upon connecting via SSH, 
use the one that was set in the security settings of the Intel Edison Board
Configuration Tool. If everything worked properly, we should now be
logged in to the board as **root**.

### Setting up the `libmraa` library

Once you are connected to Intel Edison and able to interact with its
Linux shell, we can move on and install all of the libraries and Python
packages that you'll need. 

The default firmware (Linux Yocto image)
comes with Python 2.7 already installed, so you can start running
scripts right away. However, if you wish to interact with the GPIOs of
the Intel Edison breakout board, you'll have to install
the [`libmraa`](https://github.com/intel-iot-devkit/mraa) C/C++ library,
which provides bindings for Java, JavaScript and Python. As noted in
the linked Github repository, you can install it through the 
[OPKG package manager](https://wiki.openwrt.org/doc/techref/opkg) by
executing the following three commands:

```shell
echo 'src mraa-upm http://iotdk.intel.com/repos/3.5/intelgalactic/opkg/i586' > /etc/opkg/mraa-upm.conf
opkg update
opkg install mraa
```

The first line adds the source of the `libmraa` repository to the
package manager source list, and then the second one updates the existing
[opkg](https://en.wikipedia.org/wiki/Opkg) packages, and then the last one
installs the `libmraa` package.

### Python Dependencies

The next step is to install the Python dependencies. We need to
send messages with the sensor readings to the [Dell gateway](). 

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
Once it's installed, we can now create an isolated environment wherever we
want:

```shell
virtualenv --system-site-packages <ENV>
```

where `<ENV>` is the name of the directory where the new virtual
environment exists. The `--system-site-packages` switch is for
inheriting all the packages in the global site-packages directory (in
this case the `libmraa` package).

To activate the environment, run:

```shell
source bin/activate
```

You will notice the `(<ENV>)` prefix appearing in your command-line
interface, which indicates that you are in an isolated Python
environment called `<ENV>`.

Now we can use `pip` package manager and the Python interpreter just as we
normally would. However, additional Python packages will be installed **only**
in the virtual environment

To deactivate the virtual environment, run:

```
shell deactivate
``` 

If you encounter any issues when using `virtualenv`, consult the [official
documentation](https://virtualenv.pypa.io/en/stable/).

####Paho MQTT

In order to run the Python examples provided in this repository, we need to
install the [`paho-mqtt`](https://pypi.python.org/pypi/paho-mqtt/1.1) package,
which provides an MQTT client library and enables sending/receiving of
messages to/from an MQTT broker. [MQTT](https://en.wikipedia.org/wiki/MQTT) is
a lightweight messaging protocol built on top of TCP/IP. We chose it because
of its simplicity and low overhead.

To install `paho-mqtt`  with `pip`, run:

```shell
pip install paho-mqtt
```

Once it's installed, we can now use `paho-mqtt` classes by importing the
module into our script:

```python
import paho-mqtt
```

To learn more about the functionalities of the
`paho-mqtt` Python client, see the
[official documentation](https://pypi.python.org/pypi/paho-mqtt/1.1).

After finishing all installation and configuration steps, we are now 
ready to make use of MQTT and run the code examples below.

## Code Examples

The following code examples will get you started with using Intel Edison board as a sensor node.
Clone this git repository into a folder of your choice and move into the `python/examples` folder.

```shell
git clone https://github.com/relayr/edison
cd python/examples
```

Now we can run the first example and see if you can interact with your board GPIOs through Python commands.


### Example 1 (blink.py)

The `blink.py` example is a *Hello world* script that test if the `libmraa` library has been installed properly enabling you to interact with Intel Edison board GPIOs.

This script prints the `libmraa` version and toggles the GPIO 13 output every 0.5 seconds. You'll notice a blinking LED on the Arduino breakout kit as the GPIO 13 is connected to it. Run the Python script by executing the following command:

```shell
python path/to/blink.py
```

If everything is working properly you can move on to the next example in which we will connect a motion sensor to the Edison board. If you run into problems related to `libmraa` library, consult the official [documentation](https://github.com/intel-iot-devkit/mraa).

### Example 2 (motion.py)

The `motion.py` example uses a PIR motion sensor which detects the movements in its proximity and outputs a digital value (*HIGH* if there is movement and *LOW* if there is no movement). The captured value is then sent to the Dell gateway and relayr Cloud.

First prepare the hardware by connecting the *PIR motion sensor* to the **Digital Pin 2 (D2)**. Next you'll have to create a device on the relayr Cloud which will serve as an online representation of your physical Intel Edison board. Follow the [**Devices Guide**](http://docs.relayr.io/getting-started/devices-guide/) to create a device and get acquainted with the relayr Dashboard.

Once you have a device created on the relayr Dashboard, copy its MQTT credentials in the appropriate place in the `motion.py` Python script.

```python
# MQTT credentials.
mqtt_credentials = {
    "user": "<your user ID>",
    "password": "<your password>",
    "clientId": "<your client ID>",
    "topic": "<your MQTT topic>"
}
```

Execute the python script with:

```shell
python path/to/motion.py
```

You should see a message when the MQTT client running on the Edison connects to the MQTT broker. If the connection is successfully established, and everything is working properly a `{'meaning': 'motion', 'value': True}` JSON message will be sent every 0.5 second. The `'value': True` will change accordingly to the motion detected by the PIR sensor. 

If you run into troubles, make sure that you have all the Python dependencies installed (`libmraa` and `paho-mqtt`). Also if you are using a virtual Python environment make sure that it is activated and initiated in the folder of the local repository.


### Example 3 (buzzer.py)

The `buzzer.py` example shows you how to receive commands from the relayr Cloud and/or the Dell gateway. The command Intel Edison will receive will remotely trigger or turn off a piezzo buzzer.

Prepare the hardware by connecting the [grove buzzer](http://wiki.seeedstudio.com/wiki/Grove_-_Buzzer) to the **Digital pin 5 (D5)**. You can create a new de..


### Example 4 (multi_sensor.py)

An example with motion, buzzer, luminosity and water sensor.


# References
