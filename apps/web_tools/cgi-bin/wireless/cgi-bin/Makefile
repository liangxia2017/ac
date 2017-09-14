include ../../Rules.elinux
INSTDIR   = $(prefix)/cgi-bin
INSTMODE  = 0755

PROGS     = wireless.cgi


SRCS      = wireless.c

OBJS      = wireless.o

CFLAGS    += -Werror

all:    $(PROGS)

#CFLAGS   += -I../../library
#LDLIBS    = -lwebman -lgcc -liw
#LDFLAGS   += -L$(INSTALL_ROOT)/lib

$(PROGS): $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ $(LDLIBS) -o $@
	$(STRIP) $@

install:        $(PROGS)
	$(INSTALL) -d $(INSTDIR)
	$(INSTALL) -m $(INSTMODE) $(PROGS) $(INSTDIR)

clean:
	rm -rf $(PROGS) *.o core

depend:
	makedepend -Y -- $(CFLAGS) -- $(SRCS) 2>/dev/null
# DO NOT DELETE

#//wireless.o: ../../library/webman.h ../../library/prop.h
#wireless.o: ../../library/webman.h 
#wireless.o: ../../library/postdata.h ../../library/parser.h
#//wireless.o: ../../library/myproc.h ../../library/print_handle.h
#wireless.o: ../../library/print_handle.h
#wireless.o: ../../library/comm_ioctl.h ../../library/as_shm.h

