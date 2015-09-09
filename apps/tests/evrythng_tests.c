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
 * Evrythng library unit tests  application
 *
 * This application connects to Evrythng's IoT cloud and launches unit tests.
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
 *    1. Go to WICED-SDK-.../libraries/evrythng/evrythng-c-library,
 *       copy Config_example to Config and open for editing
 *    2. Go to dashboard.evrythng.com and create an account
 *    3. Create a product with 2 properties and 2 actions
 *    4. Create a thing with 2 properties and 2 actions (use the same names as for product)
 *    5. Create device api key (see docs in the dashboard)
 *    6. Fill in the Config file with appropriate values
 *    7. Compile and run the evrythng test application, watch the output for
 *       a number of passed/failed tetsts.
 */


#include "wiced.h"
#include "evrythng/evrythng.h"
#include "evrythng/platform.h"
#include "tests.h"

/******************************************************
 *                      Macros
 ******************************************************/

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

static void thread_tests(uint32_t arg);

/******************************************************
 *               Variable Definitions
 ******************************************************/

/******************************************************
 *                 Global Variables
 ******************************************************/

/******************************************************
 *               Function Declarations
 ******************************************************/

void application_start(void)
{
    /* Initialise the device and WICED framework */
    wiced_init();

    /* Bring up the network interface */
    wiced_network_up( WICED_STA_INTERFACE, WICED_USE_EXTERNAL_DHCP_SERVER, NULL );

    platform_printf("Starting EVRYTHNG test suite\n");

    wiced_thread_t t;
    wiced_rtos_create_thread(&t, 0, "evt_tests", thread_tests, 16384, 0);
    wiced_rtos_thread_join(&t);
    wiced_rtos_delete_thread(&t);

    wiced_deinit();
}

void thread_tests(uint32_t arg)
{
    RunAllTests();
}
