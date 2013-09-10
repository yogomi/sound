UNAME = $(shell uname)

ifeq ($(UNAME),Linux)
LDFLAGS=-L/usr/lib -L/usr/lib/x86_64-linux-gnu -lopenal
endif
ifeq ($(UNAME),Darwin)
LDFLAGS=-framework OpenAL
endif

all:
	$(CXX) sound.cpp -o sound ${LDFLAGS}

.PHONY: all clean dependents
