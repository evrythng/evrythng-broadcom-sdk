include config.mk

ifeq ($(NOISY),1)
AT=
else
AT=@
endif

UNZIP=unzip -o $(if $(AT),-q,)
UN7ZIP=7z -y x
RMRF=rm -rf
MAKE=make
MKDIR=mkdir -p
LINK=ln -s

PROJECT_ROOT=$(shell pwd)

WICED_SDK_BUNDLE=$(notdir $(WICED_SDK_BUNDLE_PATH))
WICED_SDK_BUNDLE_DIR=$(patsubst %.7z.zip,%,$(WICED_SDK_BUNDLE))
EVRYTHNG_WICED_APPS_DIR=evrythng_sdk

LIBS_DIR=libs
APPS_DIR=apps

export PROJECT_ROOT WICED_SDK_BUNDLE_DIR 

.PHONY: all wiced_sdk_unpack demo demo_clean tests lib lib_clean clean 


all: demo tests

wiced_sdk_unpack: 
	$(AT)if [ ! -d $(WICED_SDK_BUNDLE_DIR) ]; \
	then \
		$(UNZIP) $(WICED_SDK_BUNDLE_PATH); \
		$(UN7ZIP) $(patsubst %.zip,%,$(WICED_SDK_BUNDLE)); \
	fi;

apps_symlinks: wiced_sdk_unpack
	$(AT)$(MKDIR) $(WICED_SDK_BUNDLE_DIR)/apps/$(EVRYTHNG_WICED_APPS_DIR); \
	for app in $(notdir $(wildcard $(APPS_DIR)/*)); \
	do \
		if [ ! -h $(WICED_SDK_BUNDLE_DIR)/apps/$(EVRYTHNG_WICED_APPS_DIR)/$$app ]; then \
			$(LINK) $(PROJECT_ROOT)/$(APPS_DIR)/$$app $(WICED_SDK_BUNDLE_DIR)/apps/$(EVRYTHNG_WICED_APPS_DIR); \
		fi \
	done \

libs_symlinks: wiced_sdk_unpack
	$(AT)for lib in $(notdir $(wildcard $(LIBS_DIR)/*)); \
	do \
		if [ ! -h $(WICED_SDK_BUNDLE_DIR)/libraries/$$lib ]; then \
			$(LINK) $(PROJECT_ROOT)/$(LIBS_DIR)/$$lib $(WICED_SDK_BUNDLE_DIR)/libraries; \
		fi \
	done \



demo: apps_symlinks libs_symlinks
	$(AT)cd $(WICED_SDK_BUNDLE_DIR) && ./make $(EVRYTHNG_WICED_APPS_DIR).demo-$(TARGET_PLATFORM)-$(TARGET_OS)-$(TARGET_IPSTACK)-$(TARGET_BUILD)

demo_run: demo
	$(AT)cd $(WICED_SDK_BUNDLE_DIR) && ./make $(EVRYTHNG_WICED_APPS_DIR).demo-$(TARGET_PLATFORM)-$(TARGET_OS)-$(TARGET_IPSTACK)-$(TARGET_BUILD) download run



gen_config:
	$(MAKE) -C $(PROJECT_ROOT)/lib/core gen_config

tests: apps_symlinks libs_symlinks
	$(AT)cd $(WICED_SDK_BUNDLE_DIR) && ./make $(EVRYTHNG_WICED_APPS_DIR).tests-$(TARGET_PLATFORM)-$(TARGET_OS)-$(TARGET_IPSTACK)-$(TARGET_BUILD)

tests_run: tests
	$(AT)cd $(WICED_SDK_BUNDLE_DIR) && ./make $(EVRYTHNG_WICED_APPS_DIR).tests-$(TARGET_PLATFORM)-$(TARGET_OS)-$(TARGET_IPSTACK)-$(TARGET_BUILD) download run


distclean: 
	$(AT)$(RMRF) $(wildcard $(WICED_SDK_BUNDLE_DIR)*)

clean:
	$(AT)if [ -d $(WICED_SDK_BUNDLE_DIR) ]; then cd $(WICED_SDK_BUNDLE_DIR) && ./make clean; fi;

