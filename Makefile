PROJECT_DIR :=$(shell pwd)
CPP_STD = c++20

BUIL_DIR = $(PROJECT_DIR)/build
SRC_DIR = $(PROJECT_DIR)/dev
BASH_SCRIPT_DIR = $(PROJECT_DIR)/scripts/bash

DEBUG_DIR = $(BUIL_DIR)/debug
RELEASE_DIR = $(BUIL_DIR)/release
DOC_DIR = $(BUIL_DIR)/docs

LIB_SRC_DIR = $(SRC_DIR)/libs
APP_SRC_DIR = $(SRC_DIR)/app
DOC_SRC_DIR = $(SRC_DIR)/docs

BASHCMD = bash
REPLACE_CMD = $(BASHCMD) $(BASH_SCRIPT_DIR)/replace.sh
CLANG_TIDY_CMD = $(BASHCMD) $(BASH_SCRIPT_DIR)/clang-tidy.sh
CLANG_FORMAT_CMD = $(BASHCMD) $(BASH_SCRIPT_DIR)/clang-format.sh


CLANG_TIDY_CONFIG = $(SRC_DIR)/linter/.clang-tidy
CLANG_FORMAT_STYLE = $(SRC_DIR)/linter/.clang-format

# COMPILE_COMMANDS_FILE = compile_commands.json
# ifeq ($(wildcard $(DEBUG_DIR)/$(COMPILE_COMMANDS_FILE)),)
# 	ifeq ($(wildcard $(RELEASE_DIR)/$(COMPILE_COMMANDS_FILE)),)
# 		$(info Neither $(DEBUG_DIR)/$(COMPILE_COMMANDS_FILE) nor $(RELEASE_DIR)/$(COMPILE_COMMANDS_FILE) exist. \
# 		Run make debug or make release first.)
# 	else
# 		COMPILE_COMMANDS := $(RELEASE_DIR)/$(COMPILE_COMMANDS_FILE)  
# 	endif
# else
# 	COMPILE_COMMANDS := $(DEBUG_DIR)/$(COMPILE_COMMANDS_FILE)
# endif

all: debug release documentation

debug:
	mkdir -p $(DEBUG_DIR)
	cd $(DEBUG_DIR); cmake -DCMAKE_BUILD_TYPE=Debug $(SRC_DIR)
	cd $(DEBUG_DIR); cmake --build  .

release:
	mkdir -p $(RELEASE_DIR)
	cd $(RELEASE_DIR); cmake -DCMAKE_BUILD_TYPE=Release $(SRC_DIR)
	cd $(RELEASE_DIR); cmake --build  .

test: test-debug test-release

test-debug:
	cd $(DEBUG_DIR) && ctest --rerun-failed --output-on-failure

test-release:
	cd $(RELEASE_DIR) && ctest --rerun-failed --output-on-failure

documentation:
	mkdir -p $(DOC_DIR)
	cp  $(DOC_SRC_DIR)/Doxyfile $(DOC_DIR)
	$(REPLACE_CMD) SRC_DIR $(SRC_DIR) $(DOC_DIR)/Doxyfile
	$(REPLACE_CMD) DOC_DIR $(DOC_DIR) $(DOC_DIR)/Doxyfile
	doxygen $(DOC_DIR)/Doxyfile

linter: clang-tidy clang-format

clang-tidy:
	$(CLANG_TIDY_CMD) $(PROJECT_DIR) $(COMPILE_COMMANDS) $(CLANG_TIDY_CONFIG) $(SRC_DIR) $(CPP_STD)

clang-format:
	$(CLANG_FORMAT_CMD) $(CLANG_FORMAT_STYLE) $(SRC_DIR)


clean:
	rm -rf build