# Getting Started Using Arduino

## Prerequisites

This tutorial will help you configuring the Edison so it can run [Arduino](https://www.arduino.cc/en/Guide/Introduction) sketches, and thus, interfacing with the relayr Cloud using the existing examples written in Wiring. But first, please make sure that your board looks like this:

![](https://software.intel.com/sites/default/files/did_feeds_images/ede08869-dd67-4ac5-a530-3078328837d4/ede08869-dd67-4ac5-a530-3078328837d4-imageId=850cc2a6-6c4d-4181-bd77-098bb6ec97f8.jpg)

Otherwise, you will have to assemble the expansion board according to the [official tutorial](https://software.intel.com/en-us/node/628232).

## Downgrade the Edison's Firmware

Unfortunately, there are some known issues with the latest versions of the Edison firmware that may cause the board to crash when running Arduino sketches after booting.

* First, download a previous version. These code examples have been tested using the firmware image 146 (ww18-15), which you can find [here](https://downloadmirror.intel.com/24910/eng/edison-image-ww18-15.zip). Unzip the file once the download is complete.

* Flash the firmware image you just download using Intel's Flash Tool Lite, as explained in [this tutorial](https://software.intel.com/en-us/flashing-firmware-with-flash-tool-lite). If you need help installing and using Flash Tool Lite on the different operating systems, you may check [this guide](https://software.intel.com/en-us/using-flash-tool-lite).

* Once ready, check the current firmware version of your board, to make sure it was properly downgraded. For that 

* 



--

* Install the Edison board on the Arduino IDE


## Serial Terminal

* Access to the serial terminal (once connected to the laptop): `screen -L /dev/cu.usbserial-A903ULPA 115200 -L`
* Hit enter twice!
* First you will be prompted to introduce a user login, simply type `root`
* As for the password, the one you set when the board was configured
* You may check the version of the current firmware with the command `configure_edison --version`


## How to start 

6XcncxrqamHI

https://forum.relayr.io/uploads/default/logo-white.png


