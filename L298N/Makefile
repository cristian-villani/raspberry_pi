.PHONY: all motor http bluetooth test clean

all: motor http bluetooth test

motor:
	$(MAKE) -C motor

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
