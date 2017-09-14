CC=gcc
STRIP=strip
CFLAGS= -Os
OBJS= read_block.o
SRCS= read_block.c $(GR_INSTALL_ROOT)/lib/libwebman.a
OUT_PUT=read_block

all:	$(OUT_PUT)

read_block:	$(OBJS)
	$(CC) $(CFLAGS) -o $(OUT_PUT) $(SRCS)

install:
	cp -f $(OUT_PUT) ${prefix}/sbin/

clean:
	rm -f *.o *~ $(OUT_PUT)

distclean: clean
	rm -f $(OUT_PUT)
