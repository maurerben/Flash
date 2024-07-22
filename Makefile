BUILDIR = build
DEBUGDIR = $(BUILDIR)/debug
RELEASEDIR = $(BUILDIR)/release
SRCDIR = ../../dev 

all: debug release

debug:
	mkdir -p $(DEBUGDIR)
	cd $(DEBUGDIR); cmake -DCMAKE_BUILD_TYPE=Debug $(SRCDIR)
	cd $(DEBUGDIR); cmake --build  .

release:
	mkdir -p $(RELEASEDIR)
	cd $(RELEASEDIR); cmake -DCMAKE_BUILD_TYPE=Release $(SRCDIR)
	cd $(RELEASEDIR); cmake --build  .

test: test-debug test-release

test-debug:
	cd $(DEBUGDIR) && ctest --rerun-failed --output-on-failure

test-release:
	cd $(RELEASEDIR) && ctest --rerun-failed --output-on-failure

clean:
	rm -rf build