CC=gcc
STRIP=strip
CFLAGS= -Os
OBJS= license_tool.o
SRCS= license_tool.c $(GR_INSTALL_ROOT)/lib/libwebman.a
OUT_PUT=license_tool

all:	$(OUT_PUT)

license_tool:	$(OBJS)
	$(CC) $(CFLAGS) -o $(OUT_PUT) $(SRCS)

install:
	cp -f $(OUT_PUT) ${prefix}/sbin/

clean:
	rm -f *.o *~ $(OUT_PUT)

distclean: clean
	rm -f $(OUT_PUT)
