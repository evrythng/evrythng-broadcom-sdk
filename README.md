# EVRYTHNG Broadcom SDK

An EVRYTHNG SDK for Broadcom IoT SoC boards (based on WICED SDK).

## Overview

The EVRYTHNG Broadcom SDK contains an MQTT SDK to work with the EVRYTHNG cloud as well as sample applications and unit tests.

## EVRYTHNG API

You can find a programming guide in the following file:
```
libs/evrythng/core/README.md
```

## Get the latest release

See [our release page](https://github.com/evrythng/evrythng-broadcom-sdk/releases).


## Clone

Ensure you use the `--recursive` option as this project depends on the [EVRYTHNG C library](https://github.com/evrythng/evrythng-c-library):

`git clone --recursive git@github.com:evrythng/evrythng-broadcom-sdk.git`

## Dependencies

1. In order to compile the library and run the tests you should install the following dependencies on your local machine:
2. Go to **community.broadcom.com** and download the latest WICED SDK. This SDK was tested with WICED SDK version 3.3.1
3. Install on your local machine: `sudo apt-get install p7zip-full` 

## Configuring

Copy the `config.mk.example` file to `config.mk` and configure it for your environment.

## Building

To build the library, demo application and tests just run:
```
make
```
To build the demo application only run:
```
make demo
```
To build the tests only run:
```
make tests
```
Additionally you can set NOISY to 1 to see more output (0 by default) 
```
make NOISY=1
```
To clean the build files run:
```
make clean
```
## Running and flashing the demo and tests applications

### WiFi Setup

* go to `apps/demo`
* edit file `wifi_config_dct.h`, find lines:
```
#define CLIENT_AP_SSID       "<wifi ssid>"
#define CLIENT_AP_PASSPHRASE "<wifi passphrase>"
```
and change it to the settings of your WiFi.

### EVRYTHNG Setup

Change the EVRYTHNG settings in `apps/demo/evrythng.c`
```
#define DEVICE_API_KEY "<EVRYTHNG Device Api Key>"
#define EVRYTHNG_BROADCOM_THNG "<EVRYTHNG Thng Id>"
```

### Build and flash

* connect the evaluation board with usb cable to your computer
* the following command will flash the board and run application:

`make demo_run`

Pressing buttons on eval board will update appropriate properties, changing green/red_led properties to true/false will turn on/off the eval board leds. You can delete all of these properties before the demo and it will be created upon board connect (app sends initial values to the cloud on startup).


## Creating your own application

1. Go to apps folder and copy-rename demo application
2. To build/flash an application run:
```
make your_app_name
```
and to flash run:
```
make your_app_name_run
```
