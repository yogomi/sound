UNAME = $(shell uname)

ifeq ($(UNAME),Linux)
LDFLAGS=-L/usr/lib -L/usr/lib/x86_64-linux-gnu
endif
ifeq ($(UNAME),Darwin)
LDFLAGS=-framework OpenAL
endif

all:
	$(CXX) ${LDFLAGS} sound.cpp -o sound

.PHONY: all clean dependents
