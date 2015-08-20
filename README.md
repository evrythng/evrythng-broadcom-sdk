# Step by step installation

1. clone the evrythng-broadcom-sdk

`git clone --recursive git@github.com:evrythng/evrythng-broadcom-sdk.git`

2. go to `/path/to/repo/evrythng-broadcom-sdk/WICED-SDK-3.3.1`
3. edit file `apps/demo/evrythng/wifi_config_dct.h` :
Find lines
`#define CLIENT_AP_SSID       "<wifi ssid>"`
`#define CLIENT_AP_PASSPHRASE "<wifi passphrase>"`
`#define CLIENT_AP_CHANNEL    <wifi channel>`
and change it to the settings of your WiFi

Change the EVT settings in `WICED-SDK-3.3.1/apps/demo/evrythng/evrythng.c`
```
#define DEVICE_API_KEY "<<EVT Device Api Key>>"

#define EVRYTHNG_BROADCOM_THNG "<<EVT Thng Id>>"
```

5. connect the evaluation board with usb cable to your mac
6. type/paste the next command in terminal in the above directory:

`./make demo.evrythng-BCM943341WCD1-debug download run`

Pressing buttons on eval board will update appropriate properties, changing green/red_led properties to true/false will turn on/off the eval board leds. You can delete all of these properties before the demo and it will be created upon board connect (app sends initial values to the cloud on startup).
