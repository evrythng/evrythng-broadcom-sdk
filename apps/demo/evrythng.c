/*
 * Copyright 2014, Broadcom Corporation
 * All Rights Reserved.
 *
 * This is UNPUBLISHED PROPRIETARY SOURCE CODE of Broadcom Corporation;
 * the contents of this file may not be disclosed to third parties, copied
 * or duplicated in any form, in whole or in part, without the prior
 * written permission of Broadcom Corporation.
 */

/** @file
 *
 * Evrythng cloud demo application
 *
 * This application connects to Evrythng's IoT cloud, subscribes to LED properties' changes
 * and publish buttons' status.
 *
 * The application demonstrates the following features
 *  - Wi-Fi client mode
 *  - GPIO interface to capture button inputs
 *  - LED control via GPIO
 *  - Evrythng API
 *
 * Device Configuration
 *    The application is configured to use the Wi-Fi configuration
 *    from the local wifi_config_dct.h file. 
 *
 * Evrythng Setup (must be completed before compiling)
 *    1. Go to dashboard.evrythng.com and create an account
 *    2. Create a thing, change EVRYTHNG_BROADCOM_THNG definition (see below) with your thing id
 *    3. Create device api key (see docs in the dashboard) and change 
 *       DEVICE_API_KEY definition (see below) with your device api key
 *    4. Compile and run the evrythng demo application
 *    5. Press buttons and update leds' properties on the dashboard
 */

#include "wiced.h"
#include "JSON.h"

#include "evrythng/evrythng.h"
#include "evrythng/platform.h"

/******************************************************
 *                      Macros
 ******************************************************/

#define MQTT_URL "ssl://mqtt.evrythng.com:443"

#define DEVICE_API_KEY "iaaRIcy8WGvDSzgA4xX6IUac4jsjnvAtmMunXw0cigtlbxCvgIsJii6LrhEfPJifxeyRvvjyiqdmWXNg"

#define EVRYTHNG_BROADCOM_THNG "UC5tXtqQPVpRPHKUHs7Ggkrr"

#define BUTTON_1_PROPERTY "button_1"
#define BUTTON_2_PROPERTY "button_2"
#define RED_LED_PROPERTY   "red_led"
#define GREEN_LED_PROPERTY "green_led"

/******************************************************
 *                    Constants
 ******************************************************/

/******************************************************
 *                   Enumerations
 ******************************************************/

/******************************************************
 *                 Type Definitions
 ******************************************************/

/******************************************************
 *                    Structures
 ******************************************************/

/******************************************************
 *               Static Function Declarations
 ******************************************************/

static void log_callback(evrythng_log_level_t level, const char* fmt, va_list vl);
static void evrythng_cloud_connect(evrythng_handle_t h);
static void property_callback(const char* str_json, size_t len);

static wiced_result_t json_callback(wiced_json_object_t* json_object);

static void evrythng_pub(uint32_t arg);

static void on_connection_lost();
static void on_connection_restored();

/******************************************************
 *               Variable Definitions
 ******************************************************/

/******************************************************
 *                 Global Variables
 ******************************************************/

static evrythng_handle_t evt_handle;

/******************************************************
 *               Function Declarations
 ******************************************************/

void application_start(void)
{
    /* Initialise the device and WICED framework */
    wiced_init( );

    /* Bring up the network interface */
    wiced_network_up( WICED_STA_INTERFACE, WICED_USE_EXTERNAL_DHCP_SERVER, NULL );

    EvrythngInitHandle(&evt_handle);
    EvrythngSetLogCallback(evt_handle, log_callback);
    EvrythngSetConnectionCallbacks(evt_handle, on_connection_lost, on_connection_restored);
    EvrythngSetUrl(evt_handle, MQTT_URL);
    EvrythngSetKey(evt_handle, DEVICE_API_KEY);

    wiced_JSON_parser_register_callback(json_callback);

    evrythng_cloud_connect(evt_handle);

    wiced_thread_t pub_thread;
    if (wiced_rtos_create_thread(&pub_thread, 0, "evrythng_publish", evrythng_pub, WICED_DEFAULT_APPLICATION_STACK_SIZE, 0) != WICED_SUCCESS)
    {
        platform_printf("failed to create publish thread\n");
    }

    wiced_rtos_thread_join(&pub_thread);
    wiced_rtos_delete_thread(&pub_thread);
}


void evrythng_pub(uint32_t arg)
{
    char msg[128];

    wiced_bool_t previous_button1_state;
    wiced_bool_t previous_button2_state;
    wiced_bool_t button1_pressed;
    wiced_bool_t button2_pressed;

    previous_button1_state = button1_pressed = WICED_FALSE;
    previous_button2_state = button2_pressed = WICED_FALSE;

    while (1)
    {
        /* Read the state of Button 1 */
        button1_pressed = wiced_gpio_input_get( WICED_BUTTON1 ) ? WICED_FALSE : WICED_TRUE;  /* The button has inverse logic */

        if (button1_pressed != previous_button1_state)
        {
            button1_pressed == WICED_TRUE ? sprintf(msg, "[{\"value\": true}]") : sprintf(msg, "[{\"value\": false}]");
            previous_button1_state = button1_pressed;
            EvrythngPubThngProperty(evt_handle, EVRYTHNG_BROADCOM_THNG, BUTTON_1_PROPERTY, msg);
        }

        /* Read the state of Button 2 */
        button2_pressed = wiced_gpio_input_get( WICED_BUTTON2 ) ? WICED_FALSE : WICED_TRUE;  /* The button has inverse logic */

        if (button2_pressed != previous_button2_state)
        {
            button2_pressed == WICED_TRUE ? sprintf(msg, "[{\"value\": true}]") : sprintf(msg, "[{\"value\": false}]");
            previous_button2_state = button2_pressed;
            EvrythngPubThngProperty(evt_handle, EVRYTHNG_BROADCOM_THNG, BUTTON_2_PROPERTY, msg);
        }

        wiced_rtos_delay_milliseconds(100);
    }
}

void on_connection_lost()
{
    wiced_gpio_output_high(WICED_LED1);
    wiced_gpio_output_low(WICED_LED2);
}

void on_connection_restored()
{
    wiced_gpio_output_high(WICED_LED2);
    wiced_gpio_output_low(WICED_LED1);
}

void log_callback(evrythng_log_level_t level, const char* fmt, va_list vl)
{
    char msg[512];

    unsigned n = vsnprintf(msg, sizeof msg, fmt, vl);
    if (n >= sizeof msg)
        msg[sizeof msg - 1] = '\0';

    switch (level)
    {
        case EVRYTHNG_LOG_ERROR:
            platform_printf("ERROR: ");
            break;
        case EVRYTHNG_LOG_WARNING:
            platform_printf("WARNING: ");
            break;
        default:
        case EVRYTHNG_LOG_DEBUG:
            platform_printf("DEBUG: ");
            break;
    }
    platform_printf("%s\n", msg);
}

void evrythng_cloud_connect(evrythng_handle_t h)
{
    on_connection_lost();

    while (EvrythngConnect(evt_handle) != EVRYTHNG_SUCCESS) 
    {
        platform_printf("Retrying\n");
        wiced_rtos_delay_milliseconds(2000);
    }

    on_connection_restored();

    /* send initial "false" values to the cloud */
    char msg[128];
    sprintf(msg, "[{\"value\": false}]");
    EvrythngPubThngProperty(evt_handle, EVRYTHNG_BROADCOM_THNG, RED_LED_PROPERTY, msg);
    EvrythngPubThngProperty(evt_handle, EVRYTHNG_BROADCOM_THNG, GREEN_LED_PROPERTY, msg);

    EvrythngPubThngProperty(evt_handle, EVRYTHNG_BROADCOM_THNG, BUTTON_1_PROPERTY, msg);
    EvrythngPubThngProperty(evt_handle, EVRYTHNG_BROADCOM_THNG, BUTTON_2_PROPERTY, msg);

    EvrythngSubThngProperty(evt_handle, EVRYTHNG_BROADCOM_THNG, RED_LED_PROPERTY, 1, property_callback);
    EvrythngSubThngProperty(evt_handle, EVRYTHNG_BROADCOM_THNG, GREEN_LED_PROPERTY, 1, property_callback);
}

static int led_to_swtich = -1;
static int led_value = -1;

void property_callback(const char* str_json, size_t len)
{
    char msg[len+1]; snprintf(msg, sizeof msg, "%s", str_json);
    platform_printf("Received message: %s\n", msg);

    wiced_JSON_parser(str_json, len);

    if (led_value >= 0 && led_to_swtich >= 0)
    {
        if (led_value) wiced_gpio_output_high(led_to_swtich);
        else wiced_gpio_output_low(led_to_swtich);
    }

    led_to_swtich = -1;
    led_value = -1;
}

wiced_result_t json_callback(wiced_json_object_t* json_object)
{
    if (!json_object)
        return WICED_SUCCESS;

    switch(json_object->value_type)
    {
        case JSON_STRING_TYPE:
            if (!strncmp(json_object->object_string, "key", json_object->object_string_length))
            {
                if (!strncmp(json_object->value, RED_LED_PROPERTY, json_object->value_length))
                    led_to_swtich = WICED_LED1;
                else if (!strncmp(json_object->value, GREEN_LED_PROPERTY, json_object->value_length))
                    led_to_swtich = WICED_LED2;
            }
            break;
        case JSON_BOOLEAN_TYPE:
            if (!strncmp(json_object->object_string, "value", json_object->object_string_length))
            {
                if (!strncmp(json_object->value, "false", json_object->value_length))
                    led_value = 0;
                else if (!strncmp(json_object->value, "true", json_object->value_length))
                    led_value = 1;
            }
            break;
        default:
            break;
    }
    
    return WICED_SUCCESS;
}
