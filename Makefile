CC = gcc
CFLAGS = -Wall -Wextra -pedantic -O2

PREFIX = /usr/local
BINDIR = $(PREFIX)/bin

bench: bench.c
	$(CC) $(CFLAGS) $< -o $@

.PHONY: install uninstall clean

install: bench
	install -d $(BINDIR)
	install -m 755 $< $(BINDIR)

uninstall:
	rm -f $(BINDIR)/bench

clean:
	rm -f bench

