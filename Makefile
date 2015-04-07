# Makefile
#

SH = bash
CFLAGS = -g
LDFLAGS = -lm

all: candycrush

candycrush: candycrush.o functions.o
	gcc $(LDFLAGS) $^ -o $@ `pkg-config --cflags gtk+-2.0` `pkg-config --libs gtk+-2.0`

candycrush.o: candycrush.c functions.h
	gcc -Wall -c $(CFLAGS) $< -o $@ `pkg-config --cflags gtk+-2.0` `pkg-config --libs gtk+-2.0`
	
functions.o: functions.c functions.h
	gcc -c $(CFLAGS) $< -o $@ `pkg-config --cflags gtk+-2.0` `pkg-config --libs gtk+-2.0`

.PHONY: clean lint test

clean:
	rm -rf functions.o
	rm -rf candycrush.o
	
lint:
	for f in *.c; do echo $$f; splint -standard $$f; done;

test: all
	$(SH) tests/run_tests.sh
