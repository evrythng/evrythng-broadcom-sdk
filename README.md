# Step by step installation

## Clone

`git clone --recursive git@github.com:evrythng/evrythng-broadcom-sdk.git`

## WiFi Setup

* go to `/path/to/repo/evrythng-broadcom-sdk/WICED-SDK-3.3.1`
* edit file `apps/demo/evrythng/wifi_config_dct.h`, find lines:

```
#define CLIENT_AP_SSID       "<wifi ssid>"
#define CLIENT_AP_PASSPHRASE "<wifi passphrase>"
#define CLIENT_AP_CHANNEL    "<wifi channel>"
```

and change it to the settings of your WiFi.

## EVRYTHNG Setup

Change the EVRYTHNG settings in `WICED-SDK-3.3.1/apps/demo/evrythng/evrythng.c`
```
#define DEVICE_API_KEY "<EVRYTHNG Device Api Key>"
#define EVRYTHNG_BROADCOM_THNG "<EVRYTHNG Thng Id>"
```

## Build and flash

* connect the evaluation board with usb cable to your computer
* type/paste the next command in terminal in the above directory:

`./make demo.evrythng-BCM943341WCD1-debug download run`

Pressing buttons on eval board will update appropriate properties, changing green/red_led properties to true/false will turn on/off the eval board leds. You can delete all of these properties before the demo and it will be created upon board connect (app sends initial values to the cloud on startup).
