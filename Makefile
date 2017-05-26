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
CHDIR=cd

PROJECT_ROOT=$(shell pwd)

WICED_SDK_BUNDLE=$(notdir $(WICED_SDK_BUNDLE_PATH))
WICED_SDK_BUNDLE_DIR=$(patsubst %.7z.zip,%,$(WICED_SDK_BUNDLE))
EVRYTHNG_WICED_APPS_DIR=evrythng_sdk

LIBS_DIR=libs
APPS_DIR=apps
COMMON_TARGETS=all wiced_sdk_unpack apps_symlinks libs_symlinks distclean clean

APP_TARGETS=$(patsubst %_run,%,$(strip $(filter-out $(COMMON_TARGETS),$(MAKECMDGOALS))))
$(foreach app,$(APP_TARGETS),$(if $(filter $(APPS_DIR)/$(app),$(wildcard $(APPS_DIR)/*)),,$(error "$(app)" was not found in $(APPS_DIR) folder)))

define all_app_names
$(notdir $(wildcard $(APPS_DIR)/*))
endef

ifeq ($(MAKECMDGOALS),)
APP_TARGETS=$(call all_app_names)
endif
ifeq ($(MAKECMDGOALS),all)
APP_TARGETS=$(call all_app_names)
endif

.PHONY: $(COMMON_TARGETS) $(APP_TARGETS)

all: $(APP_TARGETS)

gen_test_header:
	@$(PROJECT_ROOT)/$(LIBS_DIR)/evrythng/core/tests/gen_header.sh \
		$(PROJECT_ROOT)/test_config \
		$(PROJECT_ROOT)/$(LIBS_DIR)/evrythng/core/tests/evrythng_config.h

gen_evt_header:
	@$(PROJECT_ROOT)/$(LIBS_DIR)/evrythng/core/tests/gen_header.sh \
		$(PROJECT_ROOT)/evt_config \
		$(PROJECT_ROOT)/$(APPS_DIR)/demo/evt_config.h

define build_app
$(1): apps_symlinks libs_symlinks gen_test_header gen_evt_header
	$(AT)$(CHDIR) $(WICED_SDK_BUNDLE_DIR) && ./make $(EVRYTHNG_WICED_APPS_DIR).$(1)-$(TARGET_PLATFORM)-$(TARGET_OS)-$(TARGET_IPSTACK)-$(TARGET_BUILD)

$(1)_run:
	$(AT)$(CHDIR) $(WICED_SDK_BUNDLE_DIR) && ./make $(EVRYTHNG_WICED_APPS_DIR).$(1)-$(TARGET_PLATFORM)-$(TARGET_OS)-$(TARGET_IPSTACK)-$(TARGET_BUILD) download run
endef
$(foreach app,$(APP_TARGETS),$(eval $(call build_app,$(app))))

wiced_sdk_unpack: 
	$(AT)if [ ! -d $(WICED_SDK_BUNDLE_DIR) ]; \
	then \
		$(UNZIP) $(WICED_SDK_BUNDLE_PATH); \
		$(UN7ZIP) $(patsubst %.zip,%,$(WICED_SDK_BUNDLE)); \
	fi;

apps_symlinks: wiced_sdk_unpack
	$(AT)$(MKDIR) $(WICED_SDK_BUNDLE_DIR)/apps/$(EVRYTHNG_WICED_APPS_DIR); \
	for app in $(call all_app_names); \
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


distclean: 
	$(AT)$(RMRF) $(wildcard $(WICED_SDK_BUNDLE_DIR)*)

clean:
	$(AT)if [ -d $(WICED_SDK_BUNDLE_DIR) ]; then $(CHDIR) $(WICED_SDK_BUNDLE_DIR) && ./make clean; fi;

