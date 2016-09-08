# Intel® Edison and the IoT Acceleration Starter Kit (Arduino Edition)

## Introduction

This document details all the necessary steps required
to make use of the [Intel® Edison](http://www.intel.com/content/www/us/en/do-it-yourself/edison.html)
as a sensor node with the [IoT Acceleration Starter Kit](http://www.iot-starterkit.de/). This document refers **only** to using **Arduino** on the Edison. See [here](https://github.com/relayr/edison/tree/master/python) for using Python instead.


## Requirements

The following hardware is required:

 * [Dell Edge Gateway 5100]()
 * [Intel® Edison Arduino breakout kit]()
 * [Seedstudio Grove Base Shield v2](http://wiki.seeedstudio.com/wiki/Grove_-_Base_shield_v2)
 * [Seedstudio sensor kit]()

A computer is required to connect the Edison board to for flashing and configuring.

## Installation & Configuration

### Setting Up the Hardware

Assemble the Arduino Expansion Board found in your Dell Starter Kit according to the directions in the
[Intel® Edison guide](https://software.intel.com/en-us/node/628221).

Before continuing, make sure that your board looks like this:

![](https://software.intel.com/sites/default/files/did_feeds_images/ede08869-dd67-4ac5-a530-3078328837d4/ede08869-dd67-4ac5-a530-3078328837d4-imageId=850cc2a6-6c4d-4181-bd77-098bb6ec97f8.jpg)

### Setting Up the Firmware 

Unfortunately, there are some known issues with the latest versions of the Edison firmware that may cause the board to crash when running Arduino sketches after booting. Thus, we will need to **downgrade the Edison's firmware**.

1. First, download a previous version. These code examples have been tested using the **firmware image 146 (ww18-15)**, which you can find [here](https://downloadmirror.intel.com/24910/eng/edison-image-ww18-15.zip).

2. Once the download is complete, unzip the file.

3. Now let's download Intel® Flash Tool Lite, which will help you flashing the firmware on the board. The procedure for the different operating systems is clearly described in [this guide](https://software.intel.com/en-us/using-flash-tool-lite).

4. Open Flash Tool Lite, and follow the procedure to flash the firmware image you just downloaded, as explained in [this tutorial](https://software.intel.com/en-us/flashing-firmware-with-flash-tool-lite).

It may take over 5 minutes until the process is complete, so be patient!

### Verifying the Current Firmware Version

Now we'll make sure that the firmware version of your board has been properly downgraded. To do that, as well as to optionally adjust the network configuration, we will be using the **Intel® Edison Board Configuration Tool**. Follow the setup instructions according to your operating system:

* [Windows 64-bit](https://software.intel.com/en-us/get-started-edison-windows-step2)
* [Windows 32-bit](https://software.intel.com/en-us/get-started-edison-windows-32-step2)
* [Mac OS X](https://software.intel.com/en-us/get-started-edison-osx-step2)
* [Linux](https://software.intel.com/en-us/get-started-edison-linux-step2)

Once the tool is installed, connect the board to your computer as shown below:
	
![](https://software.intel.com/sites/default/files/did_feeds_images/ede08869-dd67-4ac5-a530-3078328837d4/ede08869-dd67-4ac5-a530-3078328837d4-imageId=af5fd740-016c-4634-b944-4226600c31b8.jpg)

Now, open the Intel® Edison Board Configuration Tool, and you will see something like this, showing that the firmware version installed corresponds to the 146.

![](./assets/edison_arduino_config_tool_fw_verification.png)

**IMPORTANT!**  
Do not click on "Flash Firmware"! That will update the firmware again to the latest version, overwritting the one what we just installed.

### Optional: Enable SSH and Configure the WiFi

As you can see, the Intel® Edison Board Configuration Tool also allows you to enable the SSH on the board and configure the WiFi. These options are not required, but are very handy since they will provide remote access to your Edison board.

Simply follow the instructions provided by the tool, and you'll be ready to go. From then on, the board can be accessed remotely, for instance through the CLI. Although this is not required to work with Arduino sketches, if you're curious you may look at [this step](https://github.com/relayr/edison/tree/master/python#connecting-your-personal-computer-with-the-intel-edison) of the Python tutorial which describes the procedure.

### Install and Configure the Arduino IDE

Now the Edison board is ready to run Arduino sketches, so we're almost ready to work with the provided examples. Another tool is required to flash sketches on the board, the Arduino IDE (Integrated Development Environment). In [this link](https://www.arduino.cc/en/Main/Software) you may find different downloads according to your operating system.

Once ready, open the IDE and navigate to **Tools → Board → Boards Manager**. On the search bar (with the label "filter your search"), type "Edison." Only one result should be displayed now, including the family of Intel® i686 boards. Select it, and click on "Install." The boards manager will now download and install the new package.

To make sure that everything works as expected, you may connect the board to your computer using both USB ports, and try to run the "blink" example. The procedure is pretty straight forward, and it's documented in this [tutorial](https://www.arduino.cc/en/Guide/IntelEdison#toc3).

We are now ready to run the code examples below!

## Code Examples

Intro for examples clone the repository

### Example 1 (Blink)

intro, what, how, congrats

### Example 2 (PIR Sensor)

intro, what, how, congrats

### Example 3 (Buzzer)

intro, what, how, congrats

## References