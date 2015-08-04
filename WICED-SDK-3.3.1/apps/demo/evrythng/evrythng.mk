#
# Copyright 2014, Broadcom Corporation
# All Rights Reserved.
#
# This is UNPUBLISHED PROPRIETARY SOURCE CODE of Broadcom Corporation;
# the contents of this file may not be disclosed to third parties, copied
# or duplicated in any form, in whole or in part, without the prior
# written permission of Broadcom Corporation.
#

NAME := App_Evrythng

$(NAME)_SOURCES := evrythng.c

$(NAME)_COMPONENTS := evrythng utilities/JSON_parser

$(NAME)_DEFINES = WICED_PLATFORM

WIFI_CONFIG_DCT_H := wifi_config_dct.h

APPLICATION_DCT   := 
