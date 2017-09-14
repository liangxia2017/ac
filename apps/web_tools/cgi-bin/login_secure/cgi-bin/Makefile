include ../../Rules.elinux
INSTDIR   = $(prefix)/cgi-bin
INSTMODE  = 0755

PROGS     = login.cgi

SRCS      = login.c

OBJS      = login.o

CFLAGS    += -Werror 

all:    $(PROGS)

#CFLAGS   += -I../../library
#LDLIBS    = -lwebman 
#LDFLAGS   = -L$(INSTALL_ROOT)/lib
 
 
$(PROGS): $(OBJS)
	$(CC)  $(CFLAGS) $(LDFLAGS) $^ $(LDLIBS) -o $@

	$(STRIP) $@

install:        $(PROGS)
	$(INSTALL) -d $(INSTDIR)
	$(INSTALL) -m $(INSTMODE) $(PROGS) $(INSTDIR)

clean:
	rm -rf $(PROGS) *.o core

depend:
	makedepend -Y -- $(CFLAGS) -- $(SRCS) 2>/dev/null
