
all: crypt

crypt: crypt.c
	gcc -D_LARGEFILE_SOURCE -D_FILE_OFFSET_BITS=64 -o crypt \
		crypt.c
	strip crypt

install:
	cp crypt $(DESTDIR)/usr/bin/

clean:
	rm -f *.o *~ crypt

distclean: clean
	rm -f crypt
