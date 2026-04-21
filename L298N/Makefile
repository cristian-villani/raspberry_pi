.PHONY: all motor http bluetooth test clean

all: motor sensor http bluetooth test

motor:
	$(MAKE) -C motor

sensor:
	$(MAKE) -C sensor

http:
	$(MAKE) -C http

bluetooth:
	$(MAKE) -C bluetooth

test:
	$(MAKE) -C test

clean:
	$(MAKE) -C motor clean
	$(MAKE) -C http clean
	$(MAKE) -C bluetooth clean
	$(MAKE) -C test clean
