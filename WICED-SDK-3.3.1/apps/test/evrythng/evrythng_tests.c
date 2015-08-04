/*
 * Copyright 2014, Broadcom Corporation
 * All Rights Reserved.
 *
 * This is UNPUBLISHED PROPRIETARY SOURCE CODE of Broadcom Corporation;
 * the contents of this file may not be disclosed to third parties, copied
 * or duplicated in any form, in whole or in part, without the prior
 * written permission of Broadcom Corporation.
 */

#include "wiced.h"
#include "evrythng.h"
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
