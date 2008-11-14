EXECUTABLE=audioDB
SOVERSION=0
MINORVERSION=0
LIBRARY_FULL=lib$(EXECUTABLE).so.$(SOVERSION).$(MINORVERSION)
LIBRARY_VERS=lib$(EXECUTABLE).so.$(SOVERSION)
LIBRARY=lib$(EXECUTABLE).so
ARCH_FLAGS=


ifeq ($(shell uname),Darwin)
override LIBRARY_FULL=lib$(EXECUTABLE).$(SOVERSION).$(MINORVERSION).dylib
override LIBRARY_VERS=lib$(EXECUTABLE).$(SOVERSION).dylib
override LIBRARY=lib$(EXECUTABLE).dylib
ifeq ($(shell sysctl -n hw.optional.x86_64),1)
override ARCH_FLAGS+=-arch x86_64
endif
endif

all: $(LIBRARY_FULL) $(LIBRARY_VERS) $(LIBRARY) test1

$(LIBRARY_FULL):
	-ln -s ../../$(LIBRARY_FULL) $@

$(LIBRARY_VERS):
	-ln -s ../../$(LIBRARY_FULL) $@

$(LIBRARY): $(LIBRARY_VERS)
	-ln -s $< $@

test1: prog1.c
	gcc -Wall $(ARCH_FLAGS) -laudioDB -L. -Wl,-rpath,. -o $@ $<

clean:
	-rm $(LIBRARY_FULL) $(LIBRARY_VERS) $(LIBRARY)