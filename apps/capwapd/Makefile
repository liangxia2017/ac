CC=gcc
STRIP=strip
CFLAGS= -Os -I$(GR_INSTALL_ROOT)/sqlite/include -lsqlite3 -lm -L$(GR_INSTALL_ROOT)/sqlite/lib
OBJS= eloop.o os_unix.o capwapd.o capwap_protocal.o capwapd_db.o debug_info.o
SRCS= eloop.c os_unix.c capwapd.c capwap_protocal.c capwapd_db.c debug_info.c
OUT_PUT=capwapd



all:	$(OUT_PUT)

capwapd:	$(OBJS)
	$(CC) $(CFLAGS) -o $(OUT_PUT) $(SRCS)

install:
	cp -f $(OUT_PUT) ${prefix}/sbin/

clean:
	rm -f *.o *~ $(OUT_PUT)

distclean: clean
	rm -f $(OUT_PUT)
