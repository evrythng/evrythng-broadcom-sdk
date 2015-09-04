
NAME := Lib_Evrythng

GLOBAL_INCLUDES := core/evrythng/include core/platforms

$(NAME)_INCLUDES := core/embedded-mqtt/MQTTClient-C/src \
				    core/embedded-mqtt/MQTTPacket/src

$(NAME)_SOURCES := core/evrythng/src/evrythng_core.c \
				   core/evrythng/src/evrythng_api.c \
				   core/embedded-mqtt/MQTTClient-C/src/MQTTClient.c \
				   core/embedded-mqtt/MQTTPacket/src/MQTTConnectClient.c \
				   core/embedded-mqtt/MQTTPacket/src/MQTTConnectServer.c \
				   core/embedded-mqtt/MQTTPacket/src/MQTTDeserializePublish.c \
				   core/embedded-mqtt/MQTTPacket/src/MQTTFormat.c \
				   core/embedded-mqtt/MQTTPacket/src/MQTTPacket.c \
				   core/embedded-mqtt/MQTTPacket/src/MQTTSerializePublish.c \
				   core/embedded-mqtt/MQTTPacket/src/MQTTSubscribeClient.c \
				   core/embedded-mqtt/MQTTPacket/src/MQTTSubscribeServer.c \
				   core/embedded-mqtt/MQTTPacket/src/MQTTUnsubscribeClient.c \
				   core/embedded-mqtt/MQTTPacket/src/MQTTUnsubscribeServer.c \
				   platform/WICED/wiced.c

ifdef EVRYTHNG_TESTS
GLOBAL_INCLUDES += core/tests
$(NAME)_SOURCES += core/tests/CuTest.c core/tests/tests.c
endif

$(NAME)_DEFINES  += WICED_PLATFORM

#$(NAME)_CFLAGS  := $(COMPILER_SPECIFIC_PEDANTIC_CFLAGS)
$(NAME)_CFLAGS  = $(COMPILER_SPECIFIC_STANDARD_CFLAGS)
