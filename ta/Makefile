CFG_TEE_TA_LOG_LEVEL ?= 2
CPPFLAGS += -DCFG_TEE_TA_LOG_LEVEL=$(CFG_TEE_TA_LOG_LEVEL)
BINARY=ee7a77e9-75bd-4cc3-a5eaee4235ac2afd

include $(TA_DEV_KIT_DIR)/mk/ta_dev_kit.mk

all: $(BINARY).ta

clean: clean_ta_file

.PHONY: clean_ta_file
clean_ta_file:
	rm -f $(BINARY).ta
