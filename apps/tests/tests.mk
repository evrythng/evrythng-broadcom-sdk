#
# Copyright 2014, Broadcom Corporation
# All Rights Reserved.
#
# This is UNPUBLISHED PROPRIETARY SOURCE CODE of Broadcom Corporation;
# the contents of this file may not be disclosed to third parties, copied
# or duplicated in any form, in whole or in part, without the prior
# written permission of Broadcom Corporation.
#

NAME := App_Evrythng_Tests

$(NAME)_SOURCES := evrythng_tests.c

$(NAME)_COMPONENTS := evrythng

$(NAME)_DEFINES = WICED_PLATFORM

EVRYTHNG_TESTS = 1

WIFI_CONFIG_DCT_H := wifi_config_dct.h

APPLICATION_DCT   := 

$(shell cd $(SOURCE_ROOT)libraries/evrythng/evrythng-c-library && tests/gen_header.sh)
