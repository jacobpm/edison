# Intel® Edison and the IoT Acceleration Starter Kit

## Introduction

The [Intel® Edison](http://www.intel.com/content/www/us/en/do-it-yourself/edison.html) is a tiny board-computer with a Silvermont dual-core Intel Atom CPU, integrating WiFi, Bluetooth 4.0, 1 GB DDR, 4 GB eMMC flash memory and USB controllers. The 40 multiplexed GPIO pins, high computing power and connectivity capabilities allow rapid industrial IoT and fog computing prototyping.

In this repository you will find tutorials and code examples to make use of the Edison as a sensor node with the [IoT Acceleration Starter
Kit](http://www.iot-starterkit.de/), using two languages: Python and Arduino.

![](./assets/edison_intro_pic.jpg)

This document provides some resources on how to get started with the Edison, including the mechanical assembly. **If your board has already been assembled, then please navigate to one of the available tutorials to continue:**

* [Python](https://github.com/relayr/edison/tree/master/python)
* [Arduino](https://github.com/relayr/edison/tree/master/arduino)

## Requirements

The following hardware is required:

 * [Dell Edge Gateway 5100]()
 * [Intel® Edison Arduino Breakout Kit]()

Note: The additional hardware required for the code examples is specified the Arduino and Python tutorials, although everything what's necessary is already included in the [IoT Acceleration Starter Kit](http://www.iot-starterkit.de/).

## Installation & Configuration (WIP!)

### Setting Up the Hardware (WIP!)

Assemble the Arduino Expansion Board found in your Dell Starter Kit according
to the directions in the [Intel® Edison guide](https://software.intel.com/en-us/node/628221).

Before continuing, make sure that your board looks like this:

![](./assets/edison_assembled_board.jpg)

## Association with Vertex

**NOTE:** This documentation will not be necessary when using the Vertex dashboard, since this process is implemented automatically. This section describes the required manual process until the dashboard is ready fr release.

**IMPORTANT:** To associate your device with Vertex, first it's necessary to configure the device according to the programming language of your choice. We'll come back to this section later! **First, please select one of the available languages**, and follow the instructions until the end:

* [Python](https://github.com/relayr/edison/tree/master/python)
* [Arduino](https://github.com/relayr/edison/tree/master/arduino)

Once you've completed one of the tutorials (for Python or Arduino), and you are ready to try the code examples, we can associate the device with Vertex. This section explains how to do that, as well as where to find the required parameters to introduce in the code examples.

### Create a Device In the Developer Dashboard

The very first step in order to try the different examples is to create a device in the relayr platform. Navigate to the [Developer Dashboard](https://dev.relayr.io), log in with your account credentials, and follow the instructions of [this tutorial](http://docs.relayr.io/getting-started/devices-guide/#introduction).

**NOTE:** Log in to the account where Vertex has been onboarded! All devices associated with Vertex have to be on the same account as Vertex itself.

### Parameters

Once the device is created, to proceed we're going to need a few parameters in order to associate the device with Vertex.

* `Vertex Id`: UUID of your Vertex gateway. For more information, please [refer to the Vertex documentation]() **[LINK TO BE ADDED!!!]**.

* `Authorization Token`: A token that authorizes you to make API calls to your user account and devices that you manage. [Here is more info about it](http://docs.relayr.io/getting-started/account-creation/#user-id-and-authorization-token) and where to retrieve it.

* `Device Id`: UUID of the device that will be associated with Vertex. Provided by the relayr platform [when adding a device](http://docs.relayr.io/getting-started/devices-guide/).

* `User`: MQTT user, in this case the same as the `Device Id`.

* `Password`: MQTT password. Provided by the relayr platform [when adding a device](http://docs.relayr.io/getting-started/devices-guide/).

* `MQTT Server Hostname`: The IP address of your Vertex gateway. This is not specifically required in this step, but it will be added into the code examples. For more information, please [refer to the Vertex documentation]() **[LINK TO BE ADDED!!!]**.


### Using cURL

Open the terminal (or CLI), and make the following POST request. Simply complete the request shown below with your own parameters as explained in the previous section, copy it, and press enter:

```
curl -H "Content-Type: application/json" -H "Authorization: Bearer {your Authorization Token}" -X POST -d '{"deviceId": "{your Device Id}","transport": "mqtt","credentials": {"user": "{your Device Id}","password": "{your Password}"},"configuration": {}}' http://prod-vertex.relayr.io:8081/vertices/{your Vertex Id}/devices
```

### Using Postman

The POST request to associate your device with Vertex can also be done using Postman. If you are not familiar with it, you may have a look at [this tutorial](https://www.getpostman.com/docs/requests) first.

Fill in the following parameters to make the request:

**POST URL:**
`http://prod-vertex.relayr.io:8081/vertices/{your Vertex Id}/devices`

**Headers:**

* 	`Authorization: Bearer {your Authorization Token}`
*  `Content-Type: application/json`

**Body:**

```
{
  "deviceId": "{your Device Id}",
  "transport": "mqtt",
  "credentials": {
    "user": "{your Device Id}",
    "password": "{your Password}"
  },
  "configuration": {}
}

```

## References

## License

 Copyright (C) 2016 relayr GmbH, Klemen Lilija <klemen@relayr.de>, Jaime González-Arintero <jaime@relayr.io>

 Permission is hereby granted, free of charge, to any person obtaining a 
 copy of this software and associated documentation files (the "Software"), 
 to deal in the Software without restriction, including without limitation 
 the rights to use, copy, modify, merge, publish, distribute, sublicense, 
 and/or sell copies of the Software, and to permit persons to whom the 
 Software is furnished to do so, subject to the following conditions: 

 The above copyright notice and this permission notice shall be included in 
 all copies or substantial portions of the Software. 

 Except as contained in this notice, the name(s) of the above copyright 
 holders shall not be used in advertising or otherwise to promote the sale, 
 use or other dealings in this Software without prior written authorization. 

 THE SOFTWARE IS PROVIDED "AS IS," WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL 
 THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
 FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
 DEALINGS IN THE SOFTWARE. 