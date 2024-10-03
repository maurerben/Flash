PROJECT_DIR :=$(shell pwd)

BUIL_DIR = $(PROJECT_DIR)/build
SRC_DIR = $(PROJECT_DIR)/dev
SCRIPT_DIR = $(PROJECT_DIR)/scripts

DEBUG_DIR = $(BUIL_DIR)/debug
RELEASE_DIR = $(BUIL_DIR)/release
DOC_DIR = $(BUIL_DIR)/docs

LIB_SRC_DIR = $(SRC_DIR)/libs
APP_SRC_DIR = $(SRC_DIR)/app
DOC_SRC_DIR = $(SRC_DIR)/docs


BASHCMD = bash
REPLACE_CMD = $(BASHCMD) $(SCRIPT_DIR)/bash/replace.sh


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

clean:
	rm -rf build