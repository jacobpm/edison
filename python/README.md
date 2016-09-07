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

The following software is required:

 * [Dell gateway]()
 * [Intel Edison Arduino breakout kit]()
 * [Seeedstudio base shield]()
 * [Seedstuido sensor kit]()

A computer is required to connect the Edison board to for
flashing and configuring.

## Installation & configuration

### Setting up the hardware

Assemble the Arduino Expansion Board found in the Dell Starter Kit
following
[Intel Edison guide](https://software.intel.com/en-us/node/628221).

### Setting up the firmware

To flash the operating system onto the board use the
[Intel® Edison Board Configuration Tool](https://software.intel.com/en-us/get-started-edison-osx-step2)
[where are the instructions for Linux/Windoze?]. The official
operating system is provided by the
[Yocto Project](https://www.yoctoproject.org/), which is a Linux
flavour geared towards embedded systems.

The setup wizard will guide you through flashing of the firmware, SSH
and WiFi configuration. When configured a green checkmark is shown
next to each of the setup options on the configuration wizard.

![IP Address](board-configuration-tool.png)


### Connecting your personal computer with the Intel Edison

To connect the board to your computer we'll need to use SSH in
order to login into it:

```shell
ssh root@<edison's-IP-address>
```

The Edison's IP address is found in the WiFi section of the **Intel
Edison Board Configuration Tool**. 

Alternatively we can find the IP address of the board **without** the
Intel configuration wizard by using any of the following methods:

 1. Log in your router/access point and find the IP address assigned to the Edison board.
 2. Setup [mDNS](http://www.multicastdns.org/) on your Intel Edison.
 3. Setup the board using a **static** IP.
 4. Find the IP address of the board using a scanner like [`nmap`](https://nmap.org/).

When connecting via SSH and being prompted for a password 
use the one that was set in the security settings of the Intel Edison Board
Configuration Tool. If everything worked properly we should now be
logged in as **root** on the board.

### Setting up the `libmraa` library

Once you are connected to Intel Edison and able to interact with its
Linux shell we can move on and install all the libraries and Python
packages that you'll need. The default firmware (Linux Yocto image)
comes with Python 2.7 already installed so you can start running
scripts right away. However, if you wish to interact with the GPIOs of
the Intel Edison breakout board you'll have to install
the [`libmraa`](https://github.com/intel-iot-devkit/mraa) C/C++ library
which provides bindings for Java, JavaScript and Python. As noted in
the linked github repository, you can install it by using the 
[OPKG package manager](https://wiki.openwrt.org/doc/techref/opkg) by
executing the following three commands:

```shell
echo 'src mraa-upm http://iotdk.intel.com/repos/3.5/intelgalactic/opkg/i586' > /etc/opkg/mraa-upm.conf
opkg update
opkg install mraa
```

The first line adds the source of the `libmraa` repository to the
package manager source list, the second one updates the existing
[opkg](https://en.wikipedia.org/wiki/Opkg) packages and the last one
installs the `libmraa` package.

### Python Dependencies

The next step is to install the python dependencies. We need to
send messages with the sensor readings to the [Dell gateway](). 

When dealing with Python dependencies and versions is a good practice
to create an isolated Python environment for each application we are
running. 

#### Virtualenv

`virtualenv` is a tool for creating and managing isolated Python
environments. It's installed using the `pip` Python package
manager.

```shell
pip install virtualenv
```
Once installed we can now create an isolated environment wherever we
want:

```shell
virtualenv --system-site-packages <ENV>
```

where `<ENV>` is the name of the directory where the new virtual
environment exists. The `--system-site-packages` switch is for
inheriting from all the packages in global site-packages directory (in
this case the `libmraa` package).

To activate the environment we issue:

```shell
source bin/activate
```

You will notice the `(<ENV>)` prefix appearing in your command-line
interface which indicates that you are in a isolated Python
environment called `<ENV>`.

Now we can use `pip` package manager and Python interpreter just as we
would normally do, however, when installing any additional Python
package, it will be installed **only** in the virtual environment

To deactivate the virtual environment, do:

```shell deactivate ``` If you run into any questions when using
`virtualenv` consult the
[official documentation](https://virtualenv.pypa.io/en/stable/).


####Paho MQTT

To run the Python examples provided in this repository we need to
install [`paho-mqtt`](https://pypi.python.org/pypi/paho-mqtt/1.1)
package which provides a MQTT client library, enabling the
sending/receiving of messages to/from a MQTT
broker. [MQTT](https://en.wikipedia.org/wiki/MQTT) is a lightweight
messaging protocol built on top of TCP/IP. We chose it because of
its simplicity and low overhead.

To install `paho-mqtt`  with `pip`, do:

```shell
pip install paho-mqtt
```

Once installed we can now use `paho-mqtt` classes by importing the
module into our script:

```python
import paho-mqtt
```

To learn more about the functionalities that the
`paho-mqtt` Python client see the 
[official documentation](https://pypi.python.org/pypi/paho-mqtt/1.1).

After finishing all installation and configuration steps we're now 
ready to make use of MQTT and run the code examples below.

## Code Examples

Intro for examples clone the repository


### Example 1 (Blink)

intro, what, how, congrats

### Example 2 (PIR Sensor)

intro, what, how, congrats

### Example 3 (Buzzer)

intro, what, how, congrats

# References
