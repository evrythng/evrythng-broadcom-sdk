# Evrythng Broadcom SDK

An Evrythng SDK based on Broadcom WICED SDK.

## Overview

The Evrythng Broadcom SDK contains Evrythng API to work with Evrythng cloud, sample application which shows how to use this API and unit tests.

## EVRYTHNG API

You can find a programming guide in the following file:
```
libs/evrythng/core/README.md
```

## Clone

`git clone -b external-sdk --recursive git@github.com:evrythng/evrythng-broadcom-sdk.git`

## Requirements

1. Go to **community.broadcom.com** and download the latest WICED SDK. This SDK was tested with WICED SDK version 3.3.1
2. Install on your local machine: `sudo apt-get install p7zip-full` 

## Configuring

Copy file **config.mk.example** to **config.mk** and set all the variable values according to your needs following the instructions inside.

## Building

Building a library, demo application and tests is as easy as typing
```
make
```
To build demo application
```
make demo
```
To build tests application
```
make tests
```
Additionally you can set NOISY to 1 to see more output (0 by default) 
```
make NOISY=1
```
To clean the build files
```
make clean
```
## Running and flashing demo and tests applications

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
3. You can now start building/flashing and running your application by calling:
```
make your_app_name
```
to build an application
```
make your_app_name_run
```
to flash the board
