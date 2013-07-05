
LDFLAGS='-L/usr/lib -L/usr/lib/x86_64-linux-gnu'

all:
	g++ ${LDFLAGS} sound.cpp -o sound -lalut

.PHONY: all clean dependents
