CC=gcc
STRIP=strip
CFLAGS= -Os -I$(GR_INSTALL_ROOT)/sqlite/include -lsqlite3 -lm -L$(GR_INSTALL_ROOT)/sqlite/lib
OBJS= eloop.o os_unix.o locating.o calc_coordinate.o locate_db.o debug_info.o
SRCS= eloop.c os_unix.c locating.c calc_coordinate.c locate_db.c debug_info.c
OUT_PUT=locating



all:	$(OUT_PUT)

locating:	$(OBJS)
	$(CC) $(CFLAGS) -o $(OUT_PUT) $(SRCS)

install:
	cp -f $(OUT_PUT) ${prefix}/sbin/

clean:
	rm -f *.o *~ $(OUT_PUT)

distclean: clean
	rm -f $(OUT_PUT)
