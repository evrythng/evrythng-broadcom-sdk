
NAME := Lib_Evrythng

GLOBAL_INCLUDES := evrythng-c-library/evrythng/include \
				   evrythng-c-library/platforms

$(NAME)_INCLUDES := evrythng-c-library/embedded-mqtt/MQTTClient-C/src \
				    evrythng-c-library/embedded-mqtt/MQTTPacket/src

$(NAME)_SOURCES := evrythng-c-library/evrythng/src/evrythng.c \
				   evrythng-c-library/platforms/WICED/wiced.c \
				   evrythng-c-library/embedded-mqtt/MQTTClient-C/src/MQTTClient.c \
				   evrythng-c-library/embedded-mqtt/MQTTPacket/src/MQTTConnectClient.c \
				   evrythng-c-library/embedded-mqtt/MQTTPacket/src/MQTTConnectServer.c \
				   evrythng-c-library/embedded-mqtt/MQTTPacket/src/MQTTDeserializePublish.c \
				   evrythng-c-library/embedded-mqtt/MQTTPacket/src/MQTTFormat.c \
				   evrythng-c-library/embedded-mqtt/MQTTPacket/src/MQTTPacket.c \
				   evrythng-c-library/embedded-mqtt/MQTTPacket/src/MQTTSerializePublish.c \
				   evrythng-c-library/embedded-mqtt/MQTTPacket/src/MQTTSubscribeClient.c \
				   evrythng-c-library/embedded-mqtt/MQTTPacket/src/MQTTSubscribeServer.c \
				   evrythng-c-library/embedded-mqtt/MQTTPacket/src/MQTTUnsubscribeClient.c \
				   evrythng-c-library/embedded-mqtt/MQTTPacket/src/MQTTUnsubscribeServer.c

ifdef EVRYTHNG_TESTS
GLOBAL_INCLUDES += evrythng-c-library/tests
$(NAME)_SOURCES += evrythng-c-library/tests/CuTest.c evrythng-c-library/tests/tests.c
endif

$(NAME)_DEFINES  += WICED_PLATFORM

#$(NAME)_CFLAGS  := $(COMPILER_SPECIFIC_PEDANTIC_CFLAGS)
$(NAME)_CFLAGS  = $(COMPILER_SPECIFIC_STANDARD_CFLAGS)
