BUILDIR = build
DEBUGDIR = $(BUILDIR)/debug
RELEASEDIR = $(BUILDIR)/release
SRCDIR = ../../dev 

all: debug release

debug:
	mkdir -p $(DEBUGDIR)
	cd $(DEBUGDIR); cmake $(SRCDIR)
	cd $(DEBUGDIR); cmake --build .

release:
	mkdir -p $(RELEASEDIR)
	cd $(RELEASEDIR); cmake $(SRCDIR)
	cd $(RELEASEDIR); cmake --build .

test: test-debug test-release

test-debug:
	cd $(DEBUGDIR) && ctest

test-release:
	cd $(RELEASEDIR) && ctest

clean:
	rm -rf build